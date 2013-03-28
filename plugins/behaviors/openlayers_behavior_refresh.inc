<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Zoom to Max Extent Behavior
 */
class openlayers_behavior_refresh extends openlayers_behavior {

  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layers' => array()
    ) + parent::options_init();
  }

  /**
   * Form defintion for per map customizations.
   */
  function options_form($defaults = array()) {
    $form = parent::options_form(array());

    // Only prompt for vector layers.
    $layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layers[$id] = openlayers_layer_load($id);
    }

    foreach ($layers as $id => $data) {
      $form['layers'][$id] = array(
        '#type' => 'fieldset',
        '#title' => t('Options for layer @layer', array('@layer' => $data->title)),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['layers'][$id]['layer'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable the Refresh ?'),
        '#default_value' => !empty($defaults['layers'][$id]['layer']) ? 1 : FALSE,
      );
      $form['layers'][$id]['interval'] = array(
        '#title' => t('Interval'),
        '#type' => 'textfield',
        '#description' => t(''),
        '#default_value' => !empty($defaults['layers'][$id]['interval']) ? $defaults['layers'][$id]['interval'] : 0,
      );
    }

    return $form;
  }

  /**
   * Render.
   */
  function render(&$map) {
    return $this->options;
  }
}
