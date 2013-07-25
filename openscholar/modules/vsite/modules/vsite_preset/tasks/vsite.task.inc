<?php

/**
 * @file
 * Vsite base class.
 */

class vsite_task extends og_task {
  public $entity;
  public $entity_type;

  protected $active_space;

  function __construct($entity_type, $entity, $title = NULL, $description = NULL) {

    $this->entity = $entity;
    $this->entity_type = $entity_type;

    if ($title) {
      $this->title = t($title);
    }

    if ($description) {
      $this->description = t($description);
    }

    $this->required = FALSE;
    if (!$entity->spaces_preset_og) {
      return;
    }

    $preset = spaces_preset_load($entity->spaces_preset_og,'og');
    if ($preset && isset($preset->value['og_tasks']) && in_array(get_class($this), $preset->value['og_tasks'])) {
      //The preset lists this task, set required to true
      $this->required = TRUE;
    }
  }

  /**
   * Add a post to the group {$this->entity}
   *
   *
   * @param array $node
   * @return boolean
   */
  protected function add_node($node){

    // Verify that the title and type of the node are specified.
    if (!isset($node['title'], $node['type'])) {
      return FALSE;
    }

    $defaults = array(
      'uid' => $this->entity->uid,
      'language' => 'und',
      'status' => 1,
      'promote' => 0,
      'sticky' => 0,
      'created' => time(),
      'revision' => 0,
      'comment' => 0,
      'path' => array(
        'language' => 'und',
        'pathauto' => 1
      ),
      'changed' => time(),
      'validated' => 1,
      'os_menu'=> array(
        'enabled' => 0,
        'mlid' => 0,
        'module' => 'menu',
        'hidden' => 0,
        'has_children' => 0,
        'customized' => 0,
        'options' => array(),
      ),
    );

    if ($this->entity_type == 'node' && og_is_group_type($this->entity_type, $this->entity->type)) {
      $defaults[OG_AUDIENCE_FIELD][LANGUAGE_NONE]['0'] = array('target_id' => $this->entity->nid);
    }

    //Add in the defaults
    $node = (object) array_merge($defaults, $node);
    node_save($node);

    return $node;
  }

  /**
   * Setup the active space so that drupal functions that look at the active space will work correctly.
   * This should be run before any complicated vsite CRUD funcitons.
   */
  protected function activate_vsite(){
    // Activate the current space.
    if ($this->entity_type == 'node' && og_is_group_type($this->entity_type, $this->entity->type)) {
      $this->active_space = vsite_get_vsite();
      if (!$this->active_space || $this->active_space->id != $this->entity->nid) {
        spaces_set_space(spaces_load('og', $this->entity->nid));
      }
    }
  }

  /**
   * Restore the previous active space
   */
  protected function deactivate_vsite(){
    // Tear down this task.
    if (!$this->active_space || $this->active_space != $this->entity->nid) {
      spaces_set_space($this->active_space);
    }
  }
}