<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

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
