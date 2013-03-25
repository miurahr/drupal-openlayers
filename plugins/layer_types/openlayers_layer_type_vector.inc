<?php
/**
 * @file
 * Vector Layer Type
 */

/**
 * OpenLayers Vector Layer Type class
 */
class openlayers_layer_type_vector extends openlayers_layer_type {

  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layer_handler' => 'vector',
      'projection' => array('EPSG:4326'),
      'baselayer' => FALSE,
      'vector' => TRUE,
      'url' => NULL,
      'file' => '',
      'format' => NULL,
      'formatOptions' => array(
        'extractWaypoints' => TRUE,
        'extractTracks' => TRUE,
        'extractRoutes' => TRUE,
        'extractAttributes' => TRUE
      ),
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form($defaults = array()) {

    $views = views_get_all_views();

    $options = array('' => '<none>');
    foreach ($views as $view) {
      foreach ($view->display as $display => $data) {
        if ($data->display_plugin == 'openlayers') {
          $options[$view->name . '::' . $display] = $view->human_name . ': ' . $display;
        }
      }
    }

    $default = NULL;
    if (isset($this->data['views']['view']) && isset($this->data['views']['display'])) {
      $default = $this->data['views']['view'] . '::' . $this->data['views']['display'];
    }

    $views_element = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => 'View name',
      '#default_value' => isset($this->data['views']) ?
        $this->data['views'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="openlayers_layer_type_vector[method]"]' => array('value' => 'views'),
        )
      )
    );

    $form = array(
      'method' => array(
        '#type' => 'select',
        '#options' => array(
          '' => 'Choose the method',
          'url' => 'Provides an URL',
          'file' => 'Upload a file',
          'views' => 'From a view',
          'raw' => 'Write'
        ),
        '#default_value' => isset($this->data['method']) ?
          $this->data['method'] : '',
      ),
      'url' => array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#description' => t('The URL of the file.'),
        '#maxlength' => 2083,
        '#default_value' => isset($this->data['url']) ?
          $this->data['url'] : '',
        '#states' => array(
          'visible' => array(
            ':input[name="openlayers_layer_type_vector[method]"]' => array('value' => 'url'),
          )
        )
      ),
      'file' => array(
        '#name' => 'files[imagelayer]',
        '#type' => 'managed_file',
        '#title' => t('File'),
        '#default_value' => isset($this->data['file']) ? $this->data['file'] : '',
        '#upload_location' =>  'public://',
        '#upload_validators' => array(
          'file_validate_extensions' => array('gpx kml json')
        ),
        '#states' => array(
          'visible' => array(
            ':input[name="openlayers_layer_type_vector[method]"]' => array('value' => 'file'),
          )
        )
      ),
      'views' => $views_element,
      'raw' => array(
        '#type' => 'textarea',
        '#title' => t('Raw'),
        '#description' => t('Copy your raw vectors informations in this textarea. Don\'t forget that this is not intented to have a big length.'),
        '#default_value' => isset($this->data['raw']) ?
          $this->data['raw'] : '',
        '#states' => array(
          'visible' => array(
            ':input[name="openlayers_layer_type_vector[method]"]' => array('value' => 'raw'),
          )
        )
      ),
      // TODO: To be refactored and provide all the format
      // and format options from
      // http://dev.openlayers.org/releases/OpenLayers-2.12/doc/apidocs/files/OpenLayers/Format-js.html
      'format' => array(
        '#type' => 'select',
        '#options' => array(
          'GPX' => 'GPX',
          'KML' => 'KML',
          'GeoJSON' => 'GeoJSON',
          'features' => 'Raw features',
        ),
        '#title' => t('Format'),
        '#description' => t('Format of the uploaded file.'),
        '#default_value' => isset($this->data['format']) ?
          $this->data['format'] : ''
      ),
      'formatOptions' => array(
        'extractWaypoints' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Waypoints'),
          '#description' => t('Extract waypoints from GPX.'),
          '#default_value' => isset(
          $this->data['formatOptions']['extractWaypoints']) ?
            $this->data['formatOptions']['extractWaypoints'] : TRUE,
          '#states' => array(
            'visible' => array(
              ':input[name="openlayers_layer_type_vector[format]"]' => array('value' => 'GPX'),
            )
          )
        ),
        'extractTracks' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Tracks'),
          '#description' => t('Extract tracks.'),
          '#default_value' => isset(
          $this->data['formatOptions']['extractTracks']) ?
            $this->data['formatOptions']['extractTracks'] : TRUE,
          '#states' => array(
            'visible' => array(
              ':input[name="openlayers_layer_type_vector[format]"]' => array(array('value' => 'KML'), array('value' => 'GPX')),
            )
          )
        ),
        'extractRoutes' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Routes'),
          '#description' => t('Extract routes from GPX.'),
          '#default_value' => isset(
          $this->data['formatOptions']['extractRoutes']) ?
            $this->data['formatOptions']['extractRoutes'] : TRUE,
          '#states' => array(
            'visible' => array(
              ':input[name="openlayers_layer_type_vector[format]"]' => array('value' => 'GPX'),
            )
          )
        ),
        'extractAttributes' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Attributes'),
          '#description' => t('Extract attributes from GPX.'),
          '#default_value' => isset(
          $this->data['formatOptions']['extractAttributes']) ?
            $this->data['formatOptions']['extractAttributes'] : TRUE,
          '#states' => array(
            'visible' => array(
              ':input[name="openlayers_layer_type_vector[format]"]' => array(array('value' => 'KML'), array('value' => 'GPX')),
            )
          )
        ),
        'extractStyles' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Styles'),
          '#description' => t('Extract styles from KML.'),
          '#default_value' => isset(
          $this->data['formatOptions']['extractStyles']) ?
            $this->data['formatOptions']['extractStyles'] : TRUE,
          '#states' => array(
            'visible' => array(
              ':input[name="openlayers_layer_type_vector[format]"]' => array('value' => 'KML'),
            )
          )
        ),
      ),
    );
    return $form;
  }

  /**
   * The file is mandatory.
   */
  function options_form_validate($form, &$form_state) {
    parent::options_form_validate($form, $form_state);
    $method = $form_state['data']['method'];

    if (empty($form_state['data'][$method])) {
      form_set_error($form_state['data']['layer_type'].']['.$method, 'The field cannot be empty');
    }

    if ($method == 'file') {
      if ($file = file_load($form_state['data']['file'])) {
      } else {
        form_set_error($form_state['data']['layer_type'].']['.$method, 'Cannot access the file.');
      }
    }

    $form_state['data']['formatOptions']['extractStyles'] = $form_state['data']['formatOptions']['extractStyles']!=0?TRUE:FALSE;
    $form_state['data']['formatOptions']['extractAttributes'] = $form_state['data']['formatOptions']['extractAttributes']!=0?TRUE:FALSE;
    $form_state['data']['formatOptions']['extractTracks'] = $form_state['data']['formatOptions']['extractTracks']!=0?TRUE:FALSE;
  }

  /**
   * hook_submit() of the form.
   */
  function options_form_submit($form, &$form_state) {
    parent::options_form_submit($form, $form_state);
    global $user;
    $item = $form_state['item'];
    if (isset($item->data['file']) && $file = file_load($item->data['file'])) {
      file_delete($file);
    }

    if (isset($form_state['values']['data']['file']) && $file = file_load($form_state['values']['data']['file'])) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'openlayers', 'layer_type', $user->uid);
      $form_state['values']['data']['url'] = file_create_url($file->uri);
    }
  }

  /**
   * What to do when we delete the layer: delete the file.
   */
  function delete($item) {
    if (is_numeric($item->data['file']) && $item->data['file'] > 0) {
      $file = file_load($item->data['file']);
      file_delete($file);
    }
  }

  /**
   * Render.
   */
  function render(&$map) {
    if ($this->data['method'] == 'views') {
      list($view, $display) = explode('::', $this->data['views']);

      $args = array();
      $view = views_get_view($view);
      if (isset($view->arguments)) {
        $args = $view->arguments;
      }

      $this->data['features'] = $view->execute_display($display, $args);
    }
  }
}
