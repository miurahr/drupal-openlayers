<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * BoxSelect Behavior
 */
class openlayers_behavior_boxselect extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'input_fields' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_boxselect.js'
    );
  }

  function js_dependency() {
    return array(
      'OpenLayers.Control.DrawFeature',
      'OpenLayers.Layer.Vector',
      'OpenLayers.Handler.RegularPolygon'
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
