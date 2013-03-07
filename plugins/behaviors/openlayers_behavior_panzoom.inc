<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_panzoom_openlayers_behaviors() {
  return array(
    'title' => t('Pan and Zoom Control'),
    'description' => t('Provides controls with the ability to pan and zoom in the map interface.  Do not use with the Pan Zoom Bar Control or the Zoom Panel Control.'),
    'type' => 'map',
    'class' => 'openlayers_behavior_panzoom',
  );
}

/**
 * Pan Zoom Bar Behavior
 */
class openlayers_behavior_panzoom extends openlayers_behavior {
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
        '/plugins/behaviors/openlayers_behavior_panzoom.js'
    );
  }

  function options_form($defaults = array()) {
    return array(
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
