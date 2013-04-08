<?php
/**
 * @file
 * WMS Layer Type
 */

/**
 * OpenLayers WMS Layer Type class
 */
class openlayers_layer_type_wms extends openlayers_layer_type {

  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layer_handler' => 'wms',
    ) + parent::options_init();

    // Discard hard-codes resolution because the client can calculate them usually
    unset($options['serverResolutions']);
    unset($options['maxExtent']);
    unset($options['resolutions']);

    return $options;
  }

  /**
   * Options form which generates layers
   */
  function options_form($defaults = array()) {
    return array(
      'base_url' => array(
        '#type' => 'textfield',
        '#title' => t('Base URL'),
        '#maxlength' => 2083,
        '#default_value' => isset($this->data['base_url']) ?
          $this->data['base_url'] : ''
      ),

      'options' => array(
        // TODO: validate the field, only positive integers shall be allowed
        'buffer' => array(
          '#type' => 'textfield',
          '#default_value' => isset($this->data['params']['buffer']) ?
            $this->data['params']['buffer'] : 2,
          '#title' => t('Buffer'),
          '#description' => t('Used only when not in single-tile mode, this specifies umber of extra rows and colums of tiles on each side which will surround the minimum grid tiles to cover the map')
        ),
        // TODO: validate the field, only positive numbers shall be allowed
        //       numbers below 1 might also not make much sense
        'ratio' => array(
          '#type' => 'textfield',
          '#default_value' => isset($this->data['params']['ratio']) ?
            $this->data['params']['ratio'] : 1.5,
          '#title' => t('Ratio'),
          '#description' => t('Used only when in single-tile mode, this specifies the ratio of the size of the single tile to the size of the map')
        ),
        'singleTile' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($this->data['params']['singleTile']) ?
            $this->data['params']['singleTile'] : FALSE,
          '#title' => t('Single tile'),
          '#description' => t('Check to make this layer untiled')
        )
      ),
      'params' => array(
        'TRANSPARENT' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($this->data['options']['TRANSPARENT']) ?
            $this->data['options']['TRANSPARENT'] : FALSE,
          '#return_value' => 'true',
          '#title' => t('Transparent'),
          '#description' => t('When a PNG, make the background color transparent')
        ),
         'exceptions' => array(
          '#type' => 'select',
          '#title' => t('Exceptions'),
          '#options' => array(
            'application/vnd.ogc.se_xml' => 'application/vnd.ogc.se_xml',
            'application/vnd.ogc.se_inimage' => 'application/vnd.ogc.se_inimage'),
          '#default_value' => isset($this->data['options']['exceptions']) ?
            $this->data['options']['exceptions'] : 'application/vnd.ogc.se_inimage',
          '#description' => t('Select the exception handler')
        ),
        'format' => array(
          '#type' => 'select',
          '#title' => t('File Format'),
          '#options' => array(
            'image/png' => 'image/png',
            'image/gif' => 'image/gif',
            'image/jpeg' => 'image/jpeg'),
          '#default_value' => isset($this->data['options']['format']) ?
            $this->data['options']['format'] : 'image/png'
        ),
        'layers' => array(
          '#type' => 'textarea',
          '#title' => t('Layers'),
          '#default_value' => !empty($this->data['options']['layers']) ?
            implode("\r\n", $this->data['options']['layers']) : '',
          '#description' => t('Specifies which layers to show. One per line.')
        ),
        'styles' => array(
          '#type' => 'textfield',
          '#title' => t('Styles'),
          '#default_value' => isset($this->data['options']['styles']) ?
            $this->data['options']['styles'] : ''
        ),
        'maxExtent' => array(
          '#type' => 'textfield',
          '#title' => t('Max Extent'),
          '#default_value' => isset($this->data['options']['maxExtent']) ? implode(', ', $this->data['options']['maxExtent']) : '',
          '#description' => t('Should consist of four values (left, bottom, right, top). The maximum extent for the layer. Image is stretched to cover whole map if left empty.'),
        )
      ),
    );
  }

  /**
   * hook_validate()
   */
  function options_form_validate($form, &$form_state) {
    $form_state['data']['options']['layers'] = explode("\r\n", $form_state['data']['options']['layers']);
  }

  /**
   * Hook_submit().
   */
  function options_form_submit($form, &$form_state) {
    $form_state['values']['data']['options']['maxExtent'] = array_map('floatval', explode(",", $form_state['values']['data']['options']['maxExtent']));
    if(count($form_state['values']['data']['options']['maxExtent'])!==4){
      // Ignore invalid input.
      $form_state['values']['data']['options']['maxExtent'] = NULL;
    }
  }

  /**
   * Render.
   */
  function render(&$map) {
  }
}
