<?php

/**
 * @file
 * This file contains map implementations
 *
 * @ingroup openlayers
 */

require_once 'openlayers.projections.inc';

/**
 * Get maps from DB or code, via cache.
 *
 * @ingroup openlayers_api
 *
 * @param $reset
 *   Boolean whether to reset or not.
 * @return
 *   Return array of maps.
 */
function openlayers_maps($reset = FALSE) {
  ctools_include('export');
  if ($reset) {
    ctools_export_load_object_reset('openlayers_maps');
  }

  $maps = ctools_export_load_object(
    'openlayers_maps', 'all', array());
  return $maps;
}

/**
 * Given a map name, get full map object.
 *
 * This function can also be used as a
 * menu loader for a style.
 *
 * @ingroup openlayers_api
 *
 * @param $name
 *   String identifier of the map.
 * @param $reset
 *   Boolean whether to reset cache.
 * @return
 *   map object or FALSE if not found.
 */
function openlayers_map_load($name = '', $reset = FALSE) {
  ctools_include('export');
  if ($reset) {
    ctools_export_load_object_reset('openlayers_maps');
  }

  $maps = ctools_export_load_object('openlayers_maps', 'names', array($name));

  if (empty($maps[$name])) {
    return FALSE;
  } else {
    $map = $maps[$name];
    $map->data['map_name'] = $name;
    return clone $map;
  }
}

/**
 * Get map options in an array suitable for a FormAPI element.
 *
 * @ingroup openlayers_api
 *
 * @param $reset
 *   Boolean whether to reset or not.
 * @return
 *   Return array of formatted data.
 */
function openlayers_map_options($reset = FALSE) {
  $maps = openlayers_maps($reset);
  $options = array();
  foreach ($maps as $map) {
    $options[$map->name] = $map->title;
  }
  return $options;
}

/**
 * Checks map array for incompatibilities or errors.
 *
 * @ingroup openlayers_api
 *
 * @param $map
 *   Map array
 * @param $log_errors
 *   Boolean whether to log errors.
 * @return
 *   FALSE if passed. Array of descriptive errors if fail.
 */
function openlayers_error_check_map($map, $log_errors = TRUE) {
  $errors = array();

  // Check for layers
  if (!is_array($map['layers'])) {
    $errors[] = t('Map contains no renderable layers.');
  }
  else {
    // Check layer projections
    foreach ($map['layers'] as $layer) {
      openlayers_layer_sanity_check(
        array('data' => $layer),
        $map['projection'],
        TRUE);
    }
  }

  // Check if any errors found to log
  if (count($errors) > 0 && $log_errors) {
    // Log the Error(s)
    watchdog('openlayers', implode(', ', $errors), array(), WATCHDOG_ERROR);
  }

  // Check if errors and return
  return (count($errors) > 0) ? $errors : FALSE;
}

/**
 * Prepare a map for rendering.
 *
 * Takes a map array and builds up the data given the
 * reference to objects like styles, layers, and behaviors.
 *
 * @ingroup openlayers_api
 *
 * @param $map
 *   Array of map settings
 * @return
 *   Filled in map array.
 */
function openlayers_build_map($map = array()) {
  module_load_include('inc', 'openlayers', 'includes/openlayers.render');

  // If no map is specified, use the default map.
  if (empty($map)) {
    if ($loaded_map = openlayers_map_load(
      variable_get('openlayers_default_map', 'default'))) {
      $map = $loaded_map->data;
    }
  }

  // Create ID for map as this will help with alters.
  $map['id'] = !isset($map['id']) ?
    _openlayers_create_map_id() : $map['id'];

  // Hook to alter map before main processing.  Styles, behaviors,
  // layers may all be added here.
  // hook_openlayers_map_preprocess_alter($map)
  drupal_alter('openlayers_map_preprocess', $map);

  // Styles and layer styles are not required parameters
  $map['styles'] = isset($map['styles']) ? $map['styles'] : array();
  $map['layer_styles'] = isset($map['layer_styles']) ? $map['layer_styles'] : array();
  $map['layer_styles_select'] = isset($map['layer_styles_select']) ? $map['layer_styles_select'] : array();

  // Process map parts.
  $map['layers'] = _openlayers_layers_process($map['layers'], $map);
  $map['behaviors'] = _openlayers_behaviors_render($map['behaviors'], $map);
  $map['styles'] = _openlayers_styles_process($map['styles'], $map['layer_styles'], $map['layer_styles_select'], $map);

  // Hook to alter map one last time.  Final modification to existing
  // styles, behaviors, layers can happen here, but adding new styles,
  // behaviors will not get rendered.
  // hook_openlayers_map_alter($map)
  drupal_alter('openlayers_map', $map);

  // Check map for errors
  $map['errors'] = openlayers_error_check_map($map);
  return $map;
}

/**
 * Render map array
 *
 * Given a map array, render into HTML to display
 * a map.
 *
 * @ingroup openlayers_api
 *
 * @param $map
 *   Associative array of map paramters.
 * @return
 *   Map HTML.
 */
function openlayers_render_map_data($map = array()) {

  // Run map through build process
  $map = openlayers_build_map($map);

  // Restrict map to its projection extent (data outwith cannot be represented).
  // Layer can additionally specfiy their maxExtent in case they use
  // non-default grids.
  $projection = openlayers_get_projection_by_identifier($map['projection']);
  $map['maxExtent'] = $projection->getProjectedExtent();

  if (!empty($map['errors'])) {
    $output = array(
      '#markup' => 'Error while building the map.'
    );

    return $output;
  }

  // In case the layer offers the same projection as the map, use this and do not provide
  // projection definition to client. Otherwise rely on the client to reproject on the fly.
  foreach ($map['layers'] as $layer_name => $layer) {
    if(in_array($map['projection'], $map['layers'][$layer_name]['projection'])){
      $map['layers'][$layer_name]['projection'] = $map['projection'];
    } else {
      // Client is able to reproject any possible projection because their definitions need to be
      // known to be able to set up a layer with a certain projection. Thus choice does not matter.
      $layerProjectionIdentifier = $map['layers'][$layer_name]['projection'][0];
      $map['layers'][$layer_name]['projection'] = $layerProjectionIdentifier;
      // Provide client with projection definition so that it can reproject
      openlayers_add_js_projection_definition(
        openlayers_get_projection_by_identifier($layerProjectionIdentifier)
      );
    }

    // Ensure JavaScript gets proper type.
    $map['layers'][$layer_name]['isBaseLayer'] = (boolean)($map['layers'][$layer_name]['isBaseLayer']);
  }

  // Ensure projections in use are known to the client (loads Proj4js if required)
  openlayers_add_js_projection_definition($projection);
  $display_projection = openlayers_get_projection_by_identifier($map['displayProjection']);
  openlayers_add_js_projection_definition($display_projection);

  // Given hide_empty_map flag, check if the map has any features
  // defined. If not, assume it is an empty map and shouldn't be displayed.
  if (isset($map['hide_empty_map']) && $map['hide_empty_map'] == TRUE) {
    $empty = TRUE;
    foreach ($map['layers'] as $layer) {
      if (isset($layer['features']) && count($layer['features'])) {
        $empty = FALSE;
      }
    }
    if ($empty) {
      return '';
    }
  }

  if (variable_get('openlayers_source_type', 'external') == 'external') {
    openlayers_include();
  }

  $settings = array(
    'type' => 'setting',
    'data' => array(
      'openlayers' => array(
        'maps' => array(
          $map['id'] => $map
        )
      )
    )
  );

  $libs = array_keys($map['behaviors']);

  // Get class from layer name.
  foreach($map['layers'] as $name => $data) {
    $libs[] = $data['handler']['class'];
  }

  $attached = array(
    'js' => array(),
    'css' => array(),
    'libraries_load' => array()
  );

  foreach ($libs as $lib) {
    $layer_type_object = new $lib();
    $layer_attached = $layer_type_object->attached() + array(
      'js' => array(),
      'css' => array(),
      'libraries_load' => array()
    );
    $attached['libraries_load'][] = array($lib);
    if (!empty($layer_attached['libraries_load'])) {
      $attached['libraries_load'][] = $layer_attached['libraries_load'];
    }
    $attached['js'] += $layer_attached['js'];
    $attached['css'] += $layer_attached['css'];
    unset($layer_type_object);
  }
  $attached['js'] += array($settings);
  $attached['libraries_load'] = array_values($attached['libraries_load']);

  $output = array(
    '#theme' => 'openlayers_map',
    '#attached' => $attached,
    'map' => $map
  );

  return $output;
}

/**
 * Render a map by name
 *
 * Given a map name render it into a full map object.
 *
 * @ingroup openlayers_api
 *
 * @param $map
 *   Name of the map
 * @return
 *   Map HTML.
 */
function openlayers_render_map($map = '') {

  // If it's an array, then we have been passed the map data array
  if (is_array($map)) {
    return openlayers_render_map_data($map);
  }

  // If it's a string, then we are passing a map name instead of the whole map object
  // so we need to load the object
  if (!$map || is_string($map)) {
    $map_name = $map;
    if (!$map_name) {
      $map_name = variable_get('openlayers_default_map', 'default');
    }
    $map = openlayers_map_load($map_name);
  }

  return openlayers_render_map_data($map->data);
}

/**
 * Implements hook_openlayers_maps().
 */
function openlayers_openlayers_maps() {
  $items = array();

  // Default map with MapQuest
  $default = new stdClass();
  $default->api_version = 1;
  $default->name = 'default';
  $default->title = t('Default Map');
  $default->description = t('This is the default map that will be the basis for all maps, unless you have changed the !settings.  This is also a good example of a basic map.', array('!settings' => l(t('OpenLayers main settings'), 'admin/structure/openlayers')));
  $default->data = array(
    'projection' => 'EPSG:3857',
    'width' => 'auto',
    'height' => '400px',
    'default_layer' => 'mapquest_osm',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '3'
      )
    ),
    'image_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/img/',
    'css_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/style.css',
    'displayProjection' => 'EPSG:4326',
    'behaviors' => array(
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_layerswitcher' => array(),
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_navigation' => array(),
    ),
    'layers' => array(
      'mapquest_osm' => 'mapquest_osm',
      'mapquest_openaerial' => 'mapquest_openaerial',
    ),
    'styles' => array(
      'default' => 'default',
      'select' => 'default_select',
      'temporary' => 'default',
    ),
  );
  $items['default'] = $default;

  // An example Google map
  $openlayers_maps = new stdClass;
  $openlayers_maps->disabled = FALSE;
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'example_google';
  $openlayers_maps->title = t('Example Google Map');
  $openlayers_maps->description = t('An example map using Google Maps API layers.');
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/img/',
    'css_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/style.css',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0, 0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(
        'separator' => '',
      ),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'roundedCorner' => 1,
        'roundedCornerColor' => '#222222',
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_permalink' => array(
        'anchor' => 1,
      ),
      'openlayers_behavior_zoompanel' => array(),
    ),
    'default_layer' => 'google_physical',
    'layers' => array(
      'google_satellite' => 'google_satellite',
      'google_hybrid' => 'google_hybrid',
      'google_normal' => 'google_normal',
      'google_physical' => 'google_physical',
    ),
    'projection' => 'EPSG:3857',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default_select',
      'temporary' => 'default',
    ),
  );
  $items['default_google'] = $openlayers_maps;

  // Example map with MapQuest and GeoJSON
  $openlayers_maps = new stdClass();
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'example_geojson';
  $openlayers_maps->title = 'Example GeoJSON Map';
  $openlayers_maps->description = 'A simple map with a custom GeoJSON layer with direct data. Also an example of zooming into a layer.';
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/img/',
    'css_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'sortBaseLayer' => '0',
        'roundedCorner' => 1,
        'roundedCornerColor' => '#222222',
        'maximizeDefault' => 0,
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(
        'zoomWorldIcon' => 0,
        'panIcons' => 1,
      ),
      'openlayers_behavior_popup' => array(
        'layers' => array(
          'openlayers_geojson_picture_this' => 'openlayers_geojson_picture_this',
        ),
        'panMapIfOutOfView' => 0,
        'keepInMap' => 1,
      ),
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => array(
          'openlayers_geojson_picture_this' => 'openlayers_geojson_picture_this',
        ),
        'point_zoom_level' => '5',
        'zoomtolayer_scale' => '1',
      ),
    ),
    'default_layer' => 'mapquest_openaerial',
    'layers' => array(
      'mapquest_osm' => 'mapquest_osm',
      'mapquest_openaerial' => 'mapquest_openaerial',
      'openlayers_geojson_picture_this' => 'openlayers_geojson_picture_this',
    ),
    'layer_activated' => array(
      'openlayers_geojson_picture_this' => 'openlayers_geojson_picture_this',
    ),
    'projection' => 'EPSG:3857',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default_select',
      'temporary' => 'default',
    ),
  );
  $items['example_geojson'] = $openlayers_maps;

  // Example map with MapQuest and GeoJSON
  $openlayers_maps = new stdClass();
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'example_vector';
  $openlayers_maps->title = 'Example vector Map';
  $openlayers_maps->description = 'A simple map with vectors layer.';
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/img/',
    'css_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-122.0822035425683, 37.42228990140251',
        'zoom' => '6',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_mouseposition' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'sortBaseLayer' => '0',
        'roundedCorner' => 1,
        'roundedCornerColor' => '#222222',
        'maximizeDefault' => 0,
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(
        'zoomWorldIcon' => 0,
        'panIcons' => 1,
      ),
      'openlayers_behavior_popup' => array(
        'layers' => array(
          'openlayers_vector_example' => 'openlayers_vector_example',
        ),
        'panMapIfOutOfView' => 0,
        'keepInMap' => 1,
      ),
      /*
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => array(
          'openlayers_kml_example' => 'openlayers_kml_example',
        ),
        'point_zoom_level' => '5',
        'zoomtolayer_scale' => '1',
      ),
      */
    ),
    'default_layer' => 'mapquest_osm',
    'layers' => array(
      'mapquest_osm' => 'mapquest_osm',
      'openlayers_vector_example' => 'openlayers_vector_example',
    ),
    'layer_activated' => array(
      'openlayers_vector_example' => 'openlayers_vector_example',
    ),
    'projection' => 'EPSG:3857',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default_select',
      'temporary' => 'default',
    ),
  );
  $items['example_vector'] = $openlayers_maps;

  // Example Bing map
  $openlayers_maps = new stdClass;
  $openlayers_maps->disabled = FALSE;
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'example_bing';
  $openlayers_maps->title = t('Example Microsoft Bing Map');
  $openlayers_maps->description = t('This map uses the Microsoft Bing map API.');
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/img/',
    'css_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-63.281250000007, -20.46819494553',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(
        'separator' => '',
      ),
      'openlayers_behavior_fullscreen' => array(
        'activated' => 0,
      ),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'roundedCorner' => 1,
        'roundedCornerColor' => '#222222',
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoom' => array(),
    ),
    'default_layer' => 'bing_road',
    'layers' => array(
      'bing_road' => 'bing_road',
      'bing_hybrid' => 'bing_hybrid',
      'bing_aerial' => 'bing_aerial',
    ),
    'projection' => 'EPSG:3857',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default_select',
      'temporary' => 'default',
    ),
  );
  $items['example_bing'] = $openlayers_maps;

  // Example OpenStreetMap
  $openlayers_maps = new stdClass;
  $openlayers_maps->disabled = FALSE;
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'example_osm';
  $openlayers_maps->title = 'Example OpenStreetMap Map';
  $openlayers_maps->description = 'Map to show off the OpenStreetMap layers.';
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/img/',
    'css_path' => drupal_get_path('module', 'openlayers') . '/themes/default_dark/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-0.093009923589588, 51.500381463117',
        'zoom' => '12',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(
        'separator' => '',
      ),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'roundedCorner' => 1,
        'roundedCornerColor' => '#222222',
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_zoompanel' => array(),

      'openlayers_behavior_scaleline' => array(),
    ),
    'default_layer' => 'osm_mapnik',
    'layers' => array(
      'osm_mapnik' => 'osm_mapnik',
      'osm_cycle' => 'osm_cycle',
    ),
    'projection' => 'EPSG:3857',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default_select',
      'temporary' => 'default',
    ),
  );
  $items["example_osm"] = $openlayers_maps;

  // Backwards compatibilty for features presets.  Since features
  // no longers cares about the preset hooks, we need to manually include
  // the preset file that stores the old hooks.
  $found_features = FALSE;
  if (module_exists('features')) {
    $features = features_get_features();

    foreach ($features as $feature) {
      // Only utilize enabled features and look for presets
      if ($feature->status > 0 && !empty($feature->info['features']['openlayers_map_presets'])) {
        $found_features = TRUE;
        // Include the file we need.
        $presets = $feature->info['features']['openlayers_map_presets'];
        $module = $feature->name;
        $inc = module_load_include('inc', $module, $module . '.openlayers_presets');
      }
    }
  }

  // If features found, we have to explicilty reset the implement
  // cache.  This could be a significant performance hit on a site
  // that has a high number of modules installed.
  if ($found_features) {
    module_implements('openlayers_presets', FALSE, TRUE);
  }

  // Backwards compatibilty for normal presets
  $data = module_invoke_all('openlayers_presets');
  if (count($data) > 0) {
    $items = $items + $data;
  }


  return $items;
}
