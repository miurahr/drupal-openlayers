<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Attribution Behavior
 */
class openlayers_behavior_attribution extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'separator' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_attribution.js'
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.Attribution');
  }

  function options_form($defaults = array()) {
    return array(
      'separator' => array(
        '#type' => 'textfield',
        '#title' => t('Separator'),
        '#description' => t('For multiple layers that need attribution, provide a separation string.'),
        '#default_value' => (isset($defaults['separator'])) ? $defaults['separator'] : FALSE,
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
