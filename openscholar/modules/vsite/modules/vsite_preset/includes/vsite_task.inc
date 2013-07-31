<?php

/**
 * @file
 * Provides the vsite task base class which extends the class og_task.
 */

/**
 * Class vsite_task
 */
class vsite_task extends og_task {
  public $entity;
  public $entity_type;
  protected $active_space;

  /**
   * Constructor method.
   *
   * @param string $entity_type
   *   the entity type machine name, i.e. 'node'
   * @param object $entity
   *   the entity object instance, i.e. a vsite node object
   * @param string $title
   *   (Optional) the name of the task that displays in the checklist report
   * @param string $description
   *   (Optional) the description of the task for the checklist report
   */
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

    $preset = spaces_preset_load($entity->spaces_preset_og, 'og');
    if ($preset && isset($preset->value['og_tasks']) && in_array(get_class($this), $preset->value['og_tasks'])) {
      //The preset lists this task, set required to true
      $this->required = TRUE;
    }
  }

  /**
   * Returns TRUE if this entity is a node and an organic groups (og) group.
   */
  protected function is_group_node() {
    if ($this->entity_type != 'node') {
      return FALSE;
    }
    if (!og_is_group_type('node', $this->entity->type)) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Returns TRUE if the conditions require spaces_set_space() for tasks.
   */
  protected function requires_set_space() {
    // Returns TRUE if there was no active space before this task was run.
    if (!$this->active_space) {
      return TRUE;
    }

    // Returns TRUE if there was an active space before, but it was different
    // from the vsite node which this task was run on.
    if ($this->active_space->id != $this->entity->nid) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Sets the correct active space.
   *
   * This should be run before any complicated vsite CRUD functions, since
   * otherwise Drupal API functions that look at the active space may fail.
   */
  protected function activate_vsite() {
    // Only continues if this looks like a vsite: it's a node and a group type.
    if ($this->is_group_node()) {
      // Stores the currently loaded vsite space for deactivate_vsite().
      $this->active_space = vsite_get_vsite();

      // Sets the active space to the vsite if required for tasks to be run.
      if ($this->requires_set_space()) {
        spaces_set_space(spaces_load('og', $this->entity->nid));
      }
    }
  }

  /**
   * Restores the previous active space.
   *
   * This should be called at the end of any task that uses activate_vsite().
   */
  protected function deactivate_vsite() {
    // Sets active space to the previous space if required for tasks to be run.
    if (!$this->requires_set_space()) {
      spaces_set_space($this->active_space);
    }
  }

  /**
   * Creates and adds a new node to this vsite.
   */
  public function add_node($title, $type, $uid, $body = NULL, $options = array()) {
    $node = os_create_node($title, $type, $uid, $body, $options);
    return $node;
  }
}
