<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_mapformvalues_openlayers_behaviors() {
  return array(
    'title' => t('Map Form Values'),
    'description' => t('Provides a way of updating form elements with zoom level and centering from a map.'),
    'type' => 'map',
    'ui_visibility' => FALSE,
    'behavior' => array(
      'file' => 'openlayers_behavior_mapformvalues.inc',
      'class' => 'openlayers_behavior_mapformvalues',
      'parent' => 'openlayers_behavior',
    ),
  );
}

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
