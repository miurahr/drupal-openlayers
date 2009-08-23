<?php
// $Id$

/**
 * @file
 * This file holds the hook implementations for the Module Builder module.
 */

/**
 * Implementation of hook_module_builder_info().
 */
function openlayers_module_builder_info() {
  $data = array(
    'openlayers' => array(
      'url' => 'http://cvs.drupal.org/viewvc.py/drupal/contributions/modules/module_builder/docs/%file?view=co&pathrev=%branch',
      'branch' => 'DRUPAL-6--1',
      'hook_files' => array(
        'openlayers.api.php' => '%module.module',
      ),
    ),    
  );
  
  return $data;
}
