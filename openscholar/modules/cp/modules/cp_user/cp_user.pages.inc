<?php

/**
 * Returns an array of links for this user in the Manage Members table.
 *
 * @param int $uid
 *   The specified user object's User ID.
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 *
 * @return array $columns
 */
function cp_user_check_role($uid, $gid) {
  $columns = array();

  // First checks to see if the user is the admin of this group.
  $space = spaces_get_space();

  // Checks if the current user is this vsite's author (owner).
  $is_owner = (bool) ($space->group->nid == $gid && $space->group->uid == $uid);
  // If the user is the owner, return a special row with no edit links.
  if ($is_owner) {
    $group_member = t('Manager');
    $admin_link = '';
    $remove_link = '';
  }
  // Otherwise, checks the current user's OG roles.
  // The member's role could be one of: Vsite admin, Content Editor, or Member.
  // @fixme determine better name for non-admin, non-content-editor
  else {
    // Loads the user's roles with this group node.
    $user_roles = og_get_user_roles('node', $space->group->nid, $uid);
    // Handles vsite admins...
    if (in_array('vsite admin', $user_roles)) {
      $group_member = t('Administrator');
      $admin_link = l('Remove Admin', "cp/users/remove_admin/{$uid}");
    }
    // Handles non-admins...
    else {
      $group_member = t('Member');
      $admin_link = l('Make Admin', "cp/users/create_admin/{$uid}");
    }
    $remove_link = l('Remove Membership', "cp/users/delete_membership/{$uid}");
  }

  $columns['membership'] = $group_member;
  $columns['admin_link'] = $admin_link;
  $columns['remove_link'] = $remove_link;

  return $columns;
}

/**
 * Returns an array containing the user's first, last, and username.
 *
 * @param int $uid
 *   The given User ID
 *
 * @return array $name
 *   An indexed array containing:
 *   - username
 *   - first_name
 *   - last_name
 */
function cp_user_get_user_names($uid) {
  $name = array();

  $user = user_load($uid);

  // Get the first name field's safe value.
  $items = field_get_items('user', $user, 'field_first_name');
  if (isset($items[0]['safe_value'])) {
    $first_name = $items[0]['safe_value'];
  }
  else {
    $first_name = '';
  }
  // Get the last name field's safe value.
  $items = field_get_items('user', $user, 'field_last_name');
  if (isset($items[0]['safe_value'])) {
    $last_name = $items[0]['safe_value'];
  }
  else {
    $last_name = '';
  }
  $name['username'] = $user->name;
  $name['first_name'] = $first_name;
  $name['last_name'] = $last_name;

  return $name;
}

/**
 * Returns an array containing the given vsite's members names and roles.
 *
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 *
 * @return array $members
 *   The list of members in the specified vsite.
 */
function cp_user_load_group_users($gid) {
  $query = db_select("og_membership", "ogm");
  $query->condition("ogm.gid", $gid, "=");
  $query->condition("entity_type", "user", "=");
  $query->fields("ogm", array("entity_type", "etid"));
  $result = $query->execute()->fetchAll();

  $members = array();
  foreach ($result as $entity => $value) {
    $members[] = array('User' => cp_user_get_user_names($value->etid), 'Member' => cp_user_check_role($value->etid, $gid));
  }
  return $members;
}

/**
 * Page callback; returns content for the CP > People page (Manage Members).
 */
function cp_user_user_management_page($vsite_id) {
  // Create the table.
  $rows = array();
  $header = array('Username', 'First Name', 'Last Name', 'Role', 'Admin', 'Manage');
  $data_load = cp_user_load_group_users($vsite_id);
  foreach ($data_load as $data => $load) {
    $rows[] = array($load['User']['username'],
      $load['User']['first_name'],
      $load['User']['last_name'],
      $load['Member']['membership'],
      $load['Member']['admin_link'],
      $load['Member']['remove_link'],
    );
  }
  $add_new_link = l('+ Add a user to your website', 'cp/users/add');

  $output = '<div class="cp-manage-users-wrapper"><div class="cp-add-new-user"><ul><li>' . $add_new_link . '</li></ul></div><div class="cp-manager-user-content">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '</div></div>';

  return $output;
}

/**
 * Form builder; provides confirmation for "Remove Member" action.
 */
function cp_user_remove_membership_confirm($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['account'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->uid);
  $form['username'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->name);
  $form['site_name'] = array('#type' => 'value', '#value' => $space->group->title);
  // Return hook_confirm_form();
  return confirm_form($form,
    t('Are you sure you want to remove !name from the website: %title?', array('!name' => $user_load['build_info']['args'][0]->name, '%title' => $space->group->title)),
    "cp/users",
    ' ',
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit callback; removes the specified member after user confirms action.
 */
function cp_user_remove_membership_confirm_submit($form, &$form_state) {
  // Ungroup the user from the node
  og_ungroup('node', $form_state['values']['gid'], 'user', $form_state['values']['account']);
  // Set the message and redirect back to the cp/users page
  drupal_set_message(t('!user has been removed from !website', array('!user' => $form_state['values']['username'], '!website' => $form_state['values']['site_name'])));
  drupal_goto('cp/users');
}

/**
 * Form builder; provides confirmation for "Make Admin" action.
 */
function cp_user_make_admin_confirm($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['group_type'] = array('#type' => 'value', '#value' => $space->group->type);
  $form['account'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->uid);
  $form['username'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->name);
  $form['site_name'] = array('#type' => 'value', '#value' => $space->group->title);
  // Return hook_confirm_form();
  return confirm_form($form,
    t('Are you sure you want to make !name a group administrator for the website: %title?', array('!name' => $user_load['build_info']['args'][0]->name, '%title' => $space->group->title)),
    "cp/users",
    ' ',
    t('Confirm'),
    t('Cancel')
  );
}

/**
 * Submit callback; makes specified user an admin after user confirms action.
 */
function cp_user_make_admin_confirm_submit($form, &$form_state) {
  // Get the vsite admin key.
  $og_roles = og_roles('node', $form_state['values']['group_type']);
  $key = array_search('vsite admin', $og_roles);
  $vsite_admin_role = $key;

  // Grant them the admin role
  og_role_grant('node', $form_state['values']['gid'], $form_state['values']['account'], $vsite_admin_role);
  // Set the message and redirect back to the cp/users page
  drupal_set_message(t('!user has been promoted to administrator for !website', array('!user' => $form_state['values']['username'], '!website' => $form_state['values']['site_name'])));
  drupal_goto('cp/users');
}

/**
 * Form builder; provides confirmation for "Remove Admin" action.
 */
function cp_user_remove_admin_confirm($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['group_type'] = array('#type' => 'value', '#value' => $space->group->type);
  $form['account'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->uid);
  $form['username'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->name);
  $form['site_name'] = array('#type' => 'value', '#value' => $space->group->title);
  // Return hook_confirm_form();
  return confirm_form($form,
    t('Are you sure you want to remove !name as a group administrator for the website: %title?', array('!name' => $user_load['build_info']['args'][0]->name, '%title' => $space->group->title)),
    "cp/users",
    ' ',
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit callback; removes admin from specified user after user confirms action.
 */
function cp_user_remove_admin_confirm_submit($form, &$form_state) {
  // Get the vsite admin key.
  $og_roles = og_roles('node', $form_state['values']['group_type']);
  $key = array_search('vsite admin', $og_roles);
  $vsite_admin_role = $key;

  // Revoke the admin role
  og_role_revoke('node', $form_state['values']['gid'], $form_state['values']['account'], $vsite_admin_role);
  // Set the message and redirect back to the cp/users page
  drupal_set_message(t('!user has been removed as administrator for !website', array('!user' => $form_state['values']['username'], '!website' => $form_state['values']['site_name'])));
  drupal_goto('cp/users');
}

/**
 * Form builder; provides the options for the "Edit membership" form.
 */
function cp_user_edit_membership_form($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['group_type'] = array('#type' => 'value', '#value' => $space->group->type);
  $form['account'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->uid);
  $form['username'] = array('#type' => 'value', '#value' => $user_load['build_info']['args'][0]->name);
  $form['site_name'] = array('#type' => 'value', '#value' => $space->group->title);
  // Return hook_confirm_form();
  return confirm_form($form,
    t('Are you sure you want to make !name a group administrator for the website: %title?', array('!name' => $user_load['build_info']['args'][0]->name, '%title' => $space->group->title)),
    "cp/users",
    ' ',
    t('Confirm'),
    t('Cancel')
  );
}