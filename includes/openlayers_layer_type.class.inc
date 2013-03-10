<?php

/**
 * We define base classes in the core module.
 * All other parent classes can be autoloaded through ctools.
 */
class openlayers_layer_type {

  /**
   * Stores the options for this layer.
   * @var array
   */
  public $data = array();

  /**
   * Stores the current map.
   * @var array
   */
  public $map;

  /**
   * Set configuration and store map.
   *
   * @param $layer
   *   Configuration object with the options for the layer.
   * @param $map
   *   Array with the current map.
   */
  function __construct($layer = array(), $map = array()) {
    foreach (array('name', 'title', 'description', 'data', 'export_type') as $k) {
      if (isset($layer->{$k})) {
        $this->{$k} = $layer->{$k};
      }
    }
    // Extend options with the defaults.
    $this->data += $this->options_init();

    $this->map = $map;
  }

  function attached() {
    return array();
  }

  /**
   * @return array<openlayers_projection>
   *   List of all projections that are supported by the layer.
   */
  public function getProjections() {
    $projections = array();
    // TODO Ignore incomplete data until cause is fixed (projection for every layer set during migration)
    if (isset($this->data['projection'])) {
      foreach ($this->data['projection'] as $projection) {
        $projections[] = openlayers_get_projection_by_identifier($projection);
      }
    }
    return $projections;
  }

  /**
   * Provides the default options for the layer.
   *
   * @return
   *   An associative array with the default options.
   */
  function options_init() {
    return array(
      'layer_type' => get_class($this),
      'isBaseLayer' => TRUE,
      'projection' => array('EPSG:900913'),
      'serverResolutions' => openlayers_get_resolutions('EPSG:900913'),
      'maxExtent' => openlayers_get_extent('EPSG', '900913'),
      'resolutions' => openlayers_get_resolutions('EPSG:900913'),
      'base_url' => NULL,
      'transitionEffect' => 'resize',
      'weight' => 0
    );
  }

  /**
   * Options form to configure layer instance options.
   *
   * @return
   *   Array with form elements.
   */
  function options_form($default = array()) {
    $allProjectionOptions = array();
    foreach (openlayers_get_all_projections() as $projection) {
      $allProjectionOptions[$projection->identifier] = $projection->getLocalizedMessage();
    }
    return array(
      'projection' => array(
        '#type' => 'select',
        '#title' => t('Projection'),
        '#multiple' => TRUE,
        '#options' => $allProjectionOptions,
        '#default_value' => isset($default->data['projection']) ?
          $default->data['projection'] :
          openlayers_get_projection('EPSG', '3857')->identifier
      ),
      'isBaseLayer' => array(
        '#type' => 'checkbox',
        '#title' => t('Base Layer'),
        '#description' => t('Uncheck to make this map an overlay'),
        '#default_value' => !empty($default->data['isBaseLayer']) ?
          $default->data['isBaseLayer'] : FALSE
      ),
    );
  }

  /**
   * Validate the options_form().
   *
   * @param array $default
   */
  function options_form_validate($form, &$form_state) {}

  /**
   * Submit the options_form().
   *
   * @param array $default
   */
  function options_form_submit($form, &$form_state) {

  }

  /**
   * Options form to configure layer-type-wide options.
   *
   * @return
   *   Array with form elements.
   */
  function settings_form() {
    return array();
  }

  /**
   * Render the layer and return the layer options.
   *
   * Has no return value.
   *
   * @param $map
   */
  function render(&$map) {}

}
