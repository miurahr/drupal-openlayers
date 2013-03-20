<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_scaleline_openlayers_behaviors() {
  return array(
    'title' => t('Scale Line'),
    'description' => t('Provides a line of scale in the map interface.'),
    'type' => 'map',
    'class' => 'openlayers_behavior_scaleline',
  );
}

/**
 * Scaleline Behavior
 */
class openlayers_behavior_scaleline extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'scaleline' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_scaleline.js'
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.ScaleLine');
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
