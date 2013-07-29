<?php

/**
 * @file
 * Provides the vsite task base class which extends the class og_task.
 */

/**
 * Class vsite_task
 */
class vsite_add_node_task extends og_task {
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
  function __construct($entity_type, $entity, $title = NULL, $description = NULL, $weight = 100, $body = '', $options = array()) {
    if (empty($options)) {
      return FALSE;
    }

    parent::__construct();

    if (is_null($title)) {
      $title = 'Untitled';
    }
    $replacements = array('@title' => $title);
    $this->title = t('Create a Post with title "@title"', $replacements);
    $this->node_title = $title;
    $this->menu_weight = $weight;
    $this->body = $body;
    $this->options = $options;

    $this->description = t($description);
  }

  /**
   * Creates a new node and adds it to this vsite.
   *
   * @param string $type
   *   The new node's node-type; i.e. "Page". Required.
   * @param string $title
   *   The new node's title. Required.
   * @param int $uid
   *   The author's user ID. Optional, defaults to vsite author's user ID.
   * @param string $body
   *   The new node's body text. Optional, defaults to ''.
   * @param array $options
   *   Any additional node settings to be merged into os_save_node()'s defaults.
   *
   * @return $node
   *   The newly created node object, or FALSE if unable to create and add node.
   */
  protected function add_section($title = NULL, $weight = NULL, $body = NULL, $options = NULL) {
    // Uses contstructor values as defaults.
    $title = !empty($title) ? $title : $this->node_title;
    $weight =  !empty($weight) ? $weight : $this->menu_weight;
    $body = !empty($body) ? $body : $this->body;
    $options = !empty($options) ? $options : $this->options;

    // Verify that the title and type of the node are specified.
    if (!isset($type)) {
      return FALSE;
    }

    // Creates the new page node with a menu link
    $type = 'page';
    $uid = $this->entity->uid;
    $options += array(
      'os_menu' => array(
        'parent' => 'primary-menu',
        'weight' => $weight,
      ),
    );
    $node = $this->add_node($type, $title, $uid, $body, $options);

    // Ensures node was created and added successfully.
    if ($node === FALSE) {
      return FALSE;
    }

    // Returns the newly created and added node object.
    return $node;
  }
}

