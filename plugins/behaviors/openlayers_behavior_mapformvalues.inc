<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Map Form Values Behavior
 */
class openlayers_behavior_mapformvalues extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'center_form' => '#edit-center-initial-centerpoint',
      'zoom_form' => '#edit-center-initial-zoom'
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_mapformvalues.js'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
