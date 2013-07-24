<?php

/**
 * @file
 * File for including the task class creating the 'About' section on a new site.
 */
class vsiteTaskAboutSection extends vsite_task {
  function __construct($entity_type, $entity) {
    $title = 'Create the about section for a site.';
    $description = 'The site will get an menu entry in the main menu linking to an about section.';

    parent::__construct($entity_type, $entity, $title, $description);
  }

  function execute() {

    // Setup Enviroment.
    $this->activate_vsite();

    // Adds the "About" page.
    // Creates a page node with menu item in the primary menu and a sub-menu.
    $title = t('About');
    $body = '<p>' . t('Check back soon.') . '</p>';
    $about_page = array(
      'type' => 'page',
      'title' => $title,
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
      // Adds 'About' page to the primary menu.
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
        'dirty' => 1,
      ),
      // Adds default body text.
      'body' => array(
        'und' => array(
          '0' => array(
            'summary' => '',
            'value' => $body,
            'format' => 'filtered_html',
          ),
        ),
      ),
    );
    $node = $this->add_node($about_page);

    // Adds the "Contact" page.
    // Creates a page node with menu item in the primary menu and a sub-menu.
    $title = t('Contact');
    $body = '<h2>' . t('Address') . '</h2>' . '<p>' . implode('&nbsp;<br/>', array(t('Coming Soon'), t('The University'), t('1800 Location Street'), t('Cambridge, MA 02138'))) . '</p>'; . '<p>' . implode('&nbsp;<br/>', array(t('Main Phone: xxx-xxx-xxxx'), t('Main Fax: xxx-xxx-xxxx'))) . '</p>';
    $contact_page = array(
      'type' => 'page',
      'title' => $title,
      // Creates book sub-menu.
      'book' => array(
        'original_bid' => 0,
        'menu_name' => $node->book['menu_name'],
        'nid' => 'new',
        'bid' => $node->book['bid'],
        'router_path' => 'node/%',
        'plid' => $node->nid,
        'mlid' => 0,
        'has_children' => 0,
        'weight' => 0,
        'module' => 'book',
        'options' => array(),
        'parent_depth_limit' => 8,
      ),
      // Adds default body text.
      'body' => array(
        'und' => array(
          '0' => array(
            'summary' => '',
            'value' => $body,
            'format' => 'filtered_html',
          ),
        ),
      ),
    );
    $this->add_node($contact_page);

    // Tears down enviroment.
    $this->deactivate_vsite();
  }
}

