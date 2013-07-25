<?php

/**
 * @file
 * File for including the task class creating the 'news & events' section on a new site.
 */
class vsiteTaskNewsEventsSection extends vsite_task {

  function __construct($entity_type, $entity){
    $title = 'Create the news & events section for a site.';
    $description = 'The site will get an menu entry in the main menu linking to a news & events section.';

    parent::__construct($entity_type, $entity, $title, $description);
  }

  function execute(){
    // Setup Enviroment.
    $this->activate_vsite();

    //Create the main about page with menu item in the primary menu and page submenu.
    $ne_node = $this->add_node(array(
      'type' => 'page',
      'title' => 'News & Events',
      // Create book submenu.
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
        'parent_depth_limit' => 8
      ),
      //Add 'news & events' to the main menu.
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
        'weight' => 90,
        'new_window' => '',
        'dirty' => 1
      ),
    ));

    ctools_include('menu','os');
    $ne_mlid = vsite_menu_get_link_path('primary-menu', 'node/'.$ne_node->nid);

    $old_news_mlid = vsite_menu_get_link_path('primary-menu', 'news');
    os_menu_link_delete_link('primary-menu', $old_news_mlid);

    $news_item = array(
      'link_path' => 'news',
      'menu_name' => 'primary-menu',
      'link_title' => 'News',
      'options' => array(),
      'plid' => $ne_mlid,
    );
    os_menu_link_save($news_item);

    $upcoming_events_item = array(
      'link_path' => 'calendar/upcoming',
      'menu_name' => 'primary-menu',
      'link_title' => 'Upcoming Events',
      'options' => array(),
      'plid' => $ne_mlid,
    );
    os_menu_link_save($upcoming_events_item);

    $upcoming_events_item = array(
      'link_path' => 'calendar',
      'menu_name' => 'primary-menu',
      'link_title' => 'Calendar',
      'options' => array(),
      'plid' => $ne_mlid,
    );
    os_menu_link_save($upcoming_events_item);

    // Tear Down Enviroment.
    $this->deactivate_vsite();
  }
}
