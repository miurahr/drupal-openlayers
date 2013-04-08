<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Compass Behavior
 */
class openlayers_behavior_compass extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'style' => 'compass'
    );
  }

  function options_form($defaults = array()) {
    return array(
      'style' => array(
        '#title' => 'Style',
        '#type' => 'select',
        '#options' => openlayers_ui_get_style_options(),
        '#default_value' => isset($defaults['style']) ?
          $defaults['style'] : 'compass',
        '#description' => 'Select the style that will be used for displaying the compass.'
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    $style = openlayers_style_load($this->options['style']);
    $this->options['style'] = $style->data;
    return $this->options;
  }
}
