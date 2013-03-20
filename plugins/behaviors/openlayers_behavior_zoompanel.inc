<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_zoompanel_openlayers_behaviors() {
  return array(
    'title' => t('Zoom Control'),
    'description' => t('Provides a control that can Zoom In, Zoom Out, and Zoom to Max Extent Button.  Do not use with Pan Zoom Bar Control or Pan Zoom Control.'),
    'type' => 'map',
    'class' => 'openlayers_behavior_zoompanel',
  );
}

/**
 * Zoom Panel Behavior
 */
class openlayers_behavior_zoompanel extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'panzoom' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_zoompanel.js'
    );
  }

  function js_dependency() {
    return array(
      'OpenLayers.Control.ZoomPanel'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
