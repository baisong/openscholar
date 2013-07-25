<?php

/**
 * @file
 * File for including the task class creating the 'About' section on a new site.
 */
class vsiteTaskAboutSection extends vsite_task {

  function __construct($entity_type, $entity){
    $title = 'Create the about section for a site.';
    $description = 'The site will get an menu entry in the main menu linking to an about section.';

    parent::__construct($entity_type, $entity, $title, $description);
  }

  function execute(){

    // Setup Enviroment.
    $this->activate_vsite();

    //Create the main about page with menu item in the primary menu and page submenu.
    $about_node = $this->add_node(array(
      'type' => 'page',
      'title' => 'About',
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
        'weight' => 95,
        'module' => 'book',
        'options' => array(),
        'parent_depth_limit' => 8
      ),
      //Add 'About' to the main menu.
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
        'link_title' => 'About',
        'description' => '',
        'parent' => 'primary-menu',
        'weight' => 0,
        'new_window' => '',
        'dirty' => 1
      ),
      //Default body text.
      'body' => array(
        'und' => array(
          '0' => array(
            'summary' => '',
            'value' => '<p>Check back soon.</p>',
            'format' => 'filtered_html'
          )
        )
      )
    ));

   $this->add_node(array(
      'type' => 'page',
      'title' => 'Contact',
      'book' => Array(
        'original_bid' => 0,
        'menu_name' => $about_node->book['menu_name'],
        'nid' => 'new',
        'bid' => $about_node->book['bid'],
        'router_path' => 'node/%',
        'plid' => $about_node->nid,
        'mlid' => 0,
        'has_children' => 0,
        'weight' => 0,
        'module' => 'book',
        'options' => Array(),
        'parent_depth_limit' => 8
      ),
      'body' => array(
        'und' => array(
          '0' => array(
            'summary' => '',
            'value' => '<h2>Address</h2><p>Coming Soon&nbsp;<br>The University&nbsp;<br>1800 Location Street<br>Cambridge, MA 02138</p>
                        <p>Main Phone: xxx-xxx-xxxx&nbsp;<br>Main Fax: xxx-xxx-xxxx</p>',
            'format' => 'filtered_html'
          )
        )
      )
    ));

    // Tear Down Enviroment.
    $this->deactivate_vsite();
  }
}
