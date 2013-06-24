<?php

/**
 * Searches through textareas and file fields for references to this file
 * We need the full file for its fid and uri
 *
 * @param $file   Fully loaded file entity
 * @return array  All entities that use this file, grouped by entity type
 */
function _os_files_install_find_nodes($file) {
  $fields = &drupal_static(__FUNCTION__, array());
  $file_field_types = array(
    'file',
    'imagefield_crop',
  );
  $text_field_types = array(
    'text_with_summary',
    'text_long',
  );

  if (empty($fields)) {
    $q = db_select('field_config', 'fc')
      ->fields('fc', array('field_name', 'type'))
      ->condition('active', 1)
      ->condition('deleted', 0)
      ->condition('type', array_merge($file_field_types, $text_field_types))
      ->execute();

    foreach ($q as $r) {
      $fields[$r->field_name] = $r->type;
    }
  }

  $eids = array();
  foreach ($fields as $f => $t) {
    $table = 'field_data_'.$f;
    $module = 'file';

    $q = db_select($table, 'ft')
      ->fields('ft', array('entity_type', 'entity_id'));
    if (in_array($t, $file_field_types)) {
      $q->condition($f.'_fid', $file->fid);
    }
    elseif (in_array($t, $text_field_types)) {
      $q->condition($f.'_format', 'filtered_html');
      $filename = basename($file->uri);
      $or = db_or()
        ->condition($f.'_value', '%"fid":"'.$file->fid.'"%', 'LIKE')
        ->condition($f.'_value', '%data-fid="'.$file->fid.'"%', 'LIKE')
        ->condition($f.'_value', '%'.$filename.'%', 'LIKE');
      $q->condition($or);
      $module = 'media';
    }
    $q = $q->execute();

    foreach ($q as $r) {
      $eids[$r->entity_type][] = array(
        'id' => $r->entity_id,
        'module' => $module
      );
    }
  }

  return $eids;
}

/**
 * Determines if a file needs its space changed.
 * Returns the new space if it does
 *
 * @param $file
 * @param $eids
 * @return int
 */
function _os_files_install_find_new_space($file, $eids) {
  $current_space = null;
  if (isset($file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'])) {
    $current_space = $file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'];
  }

  // no nodes use this file. We can only assume it's in the correct space already
  if (empty($eids['node'])) {
    return 0;
  }

  $node = node_load(reset($eids['node']));
  if (isset($node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id']) && $current_space != $node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id']) {
    return $node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'];
  }

  // return nothing if the node is in the correct space already
  return 0;
}