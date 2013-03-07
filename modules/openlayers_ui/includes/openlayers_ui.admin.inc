<?php

/**
 * @file
 * This file holds the functions for the main openlayers Admin settings.
 *
 * @ingroup openlayers
 */

/**
 * Menu callback; displays the openlayers module settings page.
 *
 * @see system_settings_form()
 */
function openlayers_ui_admin_settings() {
  $source_description =
    t('<p>By default the your site will point to a hosted version of the
      OpenLayers library at %ol_api_url.</p>
      <p>For better performance and reliability, you should use an internal version of the library.
      <a href="!ol_url">Download the OpenLayers library</a>, and add it to your Drupal installation.
      Use the new drush command \'<em>dl-openlayers</em>\' to download it and place it at the right place automatically.
      You can also use a double backslash before the domain name
      (eg. //www.example.com/OpenLayers.js) which then respects the use of both HTTPS and HTTP protocols.</p>
      <ul>
        <li>The default suggested, compatible version: <strong>%suggested_api</strong></li>
        <li>The default suggested, compatible hosted URL:  <strong>%ol_api_url</strong></li>
      </ul>',
      array(
        '%ol_api_url' => OPENLAYERS_DEFAULT_LIBRARY,
        '%suggested_api' => OPENLAYERS_SUGGESTED_LIBRARY,
        '!ol_url' => 'http://openlayers.org/'
      )
    );

  $options = array('external' => 'External');
  if ( ($library = libraries_detect('openlayers')) && !empty($library['installed'])) {
    $options += array('internal' => 'Internal');
  }

  $form['openlayers_source_type'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#title' => t('OpenLayers source type'),
    '#description' => t('
      <ul>
        <li>Select internal to load the OpenLayers library from your drupal installation.
          <br/>You can use the drush command <em>dl-openlayers</em> to download the library and place it at the right place automatically.</li>
        <li>Select external to load it from Internet.</li>
      </ul>'),
    '#default_value' => variable_get('openlayers_source_type', 'external')
  );

  $form['openlayers_source_external'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenLayers external source'),
    '#description' => $source_description,
    '#default_value' => variable_get('openlayers_source_external', OPENLAYERS_DEFAULT_LIBRARY),
    '#states' => array(
      'visible' => array(
        ':input[name="openlayers_source_type"]' => array('value' => 'external'),
      ),
    ),
  );

  $library = libraries_info('openlayers');
  $variants = array('original' => 'original') + $library['variants'];

  $form['openlayers_source_internal_variant'] = array(
    '#type' => 'radios',
    '#options' => array_combine(array_keys($variants), array_map('ucfirst', array_keys($variants))),
    '#title' => t('OpenLayers internal source variant'),
    '#description' => t('The OpenLayers library (<em>version >= 2.12</em>) has different variants. Select the one you prefer for your installation. <b>Warning: OpenLayers is design to only work for original variants.</b> Hopefully more variants will be supported soon.'),
    '#default_value' => variable_get('openlayers_source_internal_variant', 'original'),
    '#states' => array(
      'visible' => array(
        ':input[name="openlayers_source_type"]' => array('value' => 'internal'),
      ),
    ),
  );

  $form['openlayers_default_map'] = array(
    '#type' => 'select',
    '#title' => t('OpenLayers Default Map'),
    '#description' => t('This is the default map that will be used
      in cases where one is not defined.  It will also be used as
      the default when creating new maps.'),
    '#options' => openlayers_map_options(),
    '#default_value' => variable_get('openlayers_default_map', 'default'),
    '#ajax' => array(
      'callback' => '_ajax_reload_default_map',
      'method' => 'replace',
      'wrapper' => 'default_map_ajax',
      'effect' => 'fade'
    ),
  );

  $form['default_map'] = array(
    '#prefix' => '<div id="default_map_ajax">',
    '#suffix' => '</div>',
    '#type' => 'openlayers',
    '#description' => t('This is the map set as default.'),
    '#map' => variable_get('openlayers_default_map', 'default')
  );

  $form['openlayers_ui_preview_map'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preview Map'),
    '#description' => t('Turning this on will show you a map preview when editing it through the !link.',
      array('!link' => l(t('Map Interface'), 'admin/structure/openlayers/maps'))),
    '#default_value' => variable_get('openlayers_ui_preview_map', FALSE),
  );

  $form['#submit'][] = 'openlayers_ui_admin_settings_submit';

  // Make a system setting form and return
  return system_settings_form($form);
}

/**
 * Submit handler of the form.
 */
function openlayers_ui_admin_settings_submit() {
  drupal_flush_all_caches();
}

/**
 * Ajax callback for the default map element in the form.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function _ajax_reload_default_map($form, &$form_state) {
  $output = $form['default_map'];
  $output['#map'] = $form_state['values']['openlayers_default_map'];
  return $output;
}
