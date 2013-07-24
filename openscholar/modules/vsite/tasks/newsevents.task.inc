<?php

/**
 * @file
 * File for including the task class creating the 'news & events' section on a new site.
 */
class vsiteTaskNewsEventsSection extends vsite_task {
  function __construct($entity_type, $entity) {
    $title = 'Create the news & events section for a site.';
    $description = 'The site will get an menu entry in the main menu linking to a news & events section.';

    parent::__construct($entity_type, $entity, $title, $description);
  }

  function execute() {
    // Sets up environment.
    $this->activate_vsite();

    // Adds a page node with menu item in the primary menu and a page sub-menu.
    $news_events_page = array(
      'type' => 'page',
      'title' => 'News & Events',
      // Creates book sub-menu.
      'book' => array(
        'original_bid' => 0,
        'menu_name' => '',
        'nid' => 'new',
        'bid' => 0,
        'router_path' => 'node/%',
        'plid' => 0,
        'mlid' => 0,
        'has_children' => 0,
        'weight' => 0,
        'module' => 'book',
        'options' => array(),
        'parent_depth_limit' => 8,
      ),
      // Add the node to the primary menu.
      'os_menu' => array(
        'enabled' => 1,
        'mlid' => 0,
        'module' => 'menu',
        'hidden' => 0,
        'has_children' => 0,
        'customized' => 0,
        'options' => array(),
        'expanded' => 0,
        'parent_depth_limit' => 8,
        'link_title' => 'News & Events',
        'description' => '',
        'parent' => 'primary-menu',
        'weight' => 0,
        'new_window' => '',
        'dirty' => 1,
      ),
    );
    $node = $this->add_node($news_events_page);

    // Creates new child menu links:
    // First, finds the menu link ID of the vsite node.
    ctools_include('menu', 'os');
    $node_path = "node/$node->nid";
    $mlid = vsite_menu_get_link_path('primary-menu', $node_path);
    $items = array(
      0 => array(
        'link_path' => 'news',
        'menu_name' => 'primary-menu',
        'link_title' => 'News',
        'options' => array(),
        'plid' => $mlid,
      ),
      1 => array(
        'link_path' => 'calendar/upcoming',
        'menu_name' => 'primary-menu',
        'link_title' => 'Upcoming Events',
        'options' => array(),
        'plid' => $mlid,
      ),
      2 => array(
        'link_path' => 'calendar',
        'menu_name' => 'primary-menu',
        'link_title' => 'Calendar',
        'options' => array(),
        'plid' => $mlid,
      ),
    );
    foreach ($items as $item) {
      os_menu_link_save($item);
    }

    // Tears down enviroment.
    $this->deactivate_vsite();
  }
}

