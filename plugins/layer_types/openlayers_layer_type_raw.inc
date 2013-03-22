<?php
/**
 * @file
 * Raw Layer Type
 */

/**
 * Define the Ctools plugin options.
 */
function openlayers_openlayers_layer_type_raw_openlayers_layer_types() {
  return array(
    'title' => t('Raw'),
    'description' => t('Layer type for raw data input -
    not meant to be added with the UI.'
    ),
    'class' => 'openlayers_layer_type_raw',
  );
}

/**
 * @file
 * OpenLayers Raw Layer Type
 */
class openlayers_layer_type_raw extends openlayers_layer_type {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layer_handler' => 'openlayers_raw',
      'vector' => TRUE,
      'isBaseLayer' => FALSE,
    ) + parent::options_init();
  }

  /**
   * Options form which generates layers
   */
  function options_form($defaults = array()) {
    return array();
  }

  function options_form_validate($form, &$form_state) {
    parent::options_form_validate($form, $form_state);
  }

  function options_form_submit($form, &$form_state) {
    parent::options_form_submit($form, $form_state);
  }

  /**
   * Render.
   */
  function render(&$map) {
    // $features = ;
    // $this->data['features'] = $features;
  }
}

