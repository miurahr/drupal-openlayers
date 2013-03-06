<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_touch_navigation_openlayers_behaviors() {
  return array(
    'title' => t('Touch Navigation'),
    'description' => t('Provides specific navigation controls for touch enabled devices only.  <strong>This is included with the Navigation behavior</strong>.  Only enable this if you are targeting only touch enabled devices.'),
    'type' => 'map',
    'class' => 'openlayers_behavior_touch_navigation',
    'files' => array(
      'js' => array(
        'openlayers_behavior_touch_navigation.js'
      )
    )
  );
}

/**
 * Touch Navigation Behavior
 */
class openlayers_behavior_touch_navigation extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'documentDrag' => FALSE,
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_touch_navigation.js'
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.TouchNavigation');
  }

  function options_form($defaults = array()) {
    return array(
      'documentDrag' => array(
        '#type' => 'checkbox',
        '#title' => t('Document Drag'),
        '#description' => t('Allow panning of the map by dragging outside map viewport.'),
        '#default_value' => isset($defaults['documentDrag']) ? $defaults['documentDrag'] : FALSE
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
