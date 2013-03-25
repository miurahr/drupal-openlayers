<?php
/**
 * @file
 * Implementation of OpenLayers behavior for tooltips.
 */

/**
 * Attribution Behavior
 */
class openlayers_behavior_tooltip extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'attribution' => '',
      'layers' => array(),
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_tooltip.js'
    );
  }

  /**
   * Form defintion for per map customizations.
   */
  function options_form($defaults = array()) {
    // Only prompt for vector layers
    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
        $vector_layers[$id] = $name;
      }
    }

    return array(
      'layers' => array(
        '#title' => t('Layers'),
        '#type' => 'checkboxes',
        '#options' => $vector_layers,
        '#description' => t('Select layer to apply tooltips to.'),
        '#default_value' => isset($defaults['layers']) ?
          $defaults['layers'] : array(),
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
