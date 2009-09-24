<?php
// $Id$

/**
 * @file
 * This file holds the theme functions for openlayers module
 *
 * @ingroup openlayers
 */

/**
 * Theme function for openlayers_map
 */
function theme_openlayers_map($map = array(), $preset_name = '') {
  $class = $preset_name ? "openlayers-preset-{$preset_name}" : '';
  return "<div style='width: 500px;height:500px;' id='{$map['id']}' class='openlayers-map {$class}'></div>";
}

/**
 * Theme function to be able to override styles
 */
function theme_openlayers_styles($styles = array(), $map = array()) {
  // Default is to just send the processed styles back
  return $styles;
}
