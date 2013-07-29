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
  function __construct($entity_type, $entity, $title = NULL, $description = NULL, $options = array()) {
    if (empty($options)) {
      return FALSE;
    }

    parent::__construct();

    if (is_null($title)) {
      $title = 'Untitled';
    }
    $replacements = array('@title' => $title);
    $this->title = t('Create a Post with title "@title"', $replacements);

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
  public function add_node($type, $title, $uid = NULL, $body = '', $options = array()) {

    // Verify that the title and type of the node are specified.
    if (!isset($type, $title)) {
      return FALSE;
    }

    // Defaults to vsite owner if no author UID is given.
    $uid = !is_null($uid) ? $uid : $this->entity->uid;

    // Creates and stores the new node.
    $node = os_create_node($type, $title, $uid, $body, $options);

    // Creates and stores the new OG Membership entity.
    $og_membership = vsite_add_entity($node, 'node', $this->entity);

    // Ensures node was created and added successfully.
    if (empty($node) || empty($og_membership)) {
      return FALSE;
    }

    // Returns the newly created and added node object.
    return $node;
  }
}

