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
   * Constructor function.
   *
   * @param string $entity_type
   * @param object $entity
   * @param string $title
   * @param string $description
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
   * Sets the correct active space.
   *
   * This should be run before any complicated vsite CRUD functions, since
   * otherwise Drupal API functions that look at the active space may fail.
   */
  protected function activate_vsite() {
    // Activate the current space.
    if ($this->entity_type == 'node' && og_is_group_type($this->entity_type, $this->entity->type)) {
      $this->active_space = vsite_get_vsite();
      if (!$this->active_space || $this->active_space->id != $this->entity->nid) {
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
    // Tear down this task.
    if (!$this->active_space || $this->active_space != $this->entity->nid) {
      spaces_set_space($this->active_space);
    }
  }
}

