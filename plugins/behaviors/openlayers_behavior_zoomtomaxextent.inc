<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Zoom to Max Extent Behavior
 */
class openlayers_behavior_zoomtomaxextent extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'zoomtomaxextent' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_zoomtomaxextent.js'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
