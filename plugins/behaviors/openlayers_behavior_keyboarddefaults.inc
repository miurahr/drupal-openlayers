<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
function openlayers_openlayers_behavior_keyboarddefaults_openlayers_behaviors() {
  return array(
    'title' => t('Keyboard Controls'),
    'description' => t('Provides keyboard shortcuts to pan and zoom the map, such as the up, down, left, and right arrows.  This is included with the Navigation control.  This should not be enabled unless you want to have finer control on how the user interacts with the map.'),
    'type' => 'layer',
    'class' => 'openlayers_behavior_keyboarddefaults',
  );
}

/**
 * Keyboard Defaults Behavior
 */
class openlayers_behavior_keyboarddefaults extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'keyboarddefaults' => '',
    );
  }

  /**
   * Returns an array of JS files.
   * @return array
   */
  function js() {
    return array(
      drupal_get_path('module', 'openlayers') .
        '/plugins/behaviors/openlayers_behavior_keyboarddefaults.js'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
