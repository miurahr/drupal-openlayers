<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_zoombox_openlayers_behaviors() {
  return array(
    'title' => t('Zoom Box Clicking'),
    'description' => t('Provides a zoom box that can be drawn on SHIFT + click.  Please note that by default the Navigation control behavior includes this, so there is no need to have both.'),
    'type' => 'map',
    'class' => 'openlayers_behavior_zoombox',
    'files' => array(
      'js' => array(
        'openlayers_behavior_zoombox.js'
      )
    )
  );
}

/**
 * Zoom Box Behavior
 */
class openlayers_behavior_zoombox extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'zoombox' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_zoombox.js'
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.ZoomBox');
  }

  function options_form($defaults = array()) {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
