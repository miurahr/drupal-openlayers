<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Attribution Behavior
 */
class openlayers_behavior_argparser extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'anchor' => FALSE,
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.ArgParser');
  }

  function options_form($defaults = array()) {
    return array(
      'anchor' => array(
        '#type' => 'checkbox',
        '#title' => t('Anchor'),
        '#description' => t('Permalink is in the form of an anchor (#) instead of a query (?).'),
        '#default_value' => (isset($defaults['anchor'])) ? $defaults['anchor'] : FALSE,
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
