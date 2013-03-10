<?php

/**
 * Include necessary CSS and JS for rendering maps
 *
 * @ingroup openlayers_api
 */
function openlayers_include() {
  // Use a static variable to prevent running URL check code repeatedly.
  static $once;
  if (!isset($once)) {
    $once = TRUE;

    $path = check_plain(variable_get('openlayers_source', OPENLAYERS_DEFAULT_LIBRARY));

    // Correctly handle URLs beginning with a double backslash, see RFC 1808 Section 4
    if (substr($path, 0, 2) == '//') {
      $http_protocol = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
      $path = $http_protocol .':'. $path;
    }

    // Check for full URL and include it manually if external.
    if (valid_url($path, TRUE)) {
      drupal_add_js($path, 'external');
    }
    else {
      drupal_add_js($path);
    }

    drupal_add_css(drupal_get_path('module', 'openlayers') .
      '/css/openlayers.css', 'file');
    drupal_add_js(drupal_get_path('module', 'openlayers') .
      '/js/openlayers.js', 'file');
  }
}

/**
 * Get extent given projection
 *
 * Returns standard world-max-extents for common projections.
 * Layers implementing other projections and subsets of the
 * world should define their maxExtent in the layer array.
 *
 * @ingroup openlayers_api
 *
 * @param $authority
 *   String Organization code, such as EPSG.
 * @param $projection
 *   String of the projection value, such as 4326.
 * @return
 *   Array of maxExtent in OpenLayers toArray() format.
 */
function openlayers_get_extent($authority, $projection) {
  // Use projected extend from openlayers_projection instead but keep this function for now as it's marked as API
  return openlayers_get_projection($authority, $projection)->getProjectedExtent();
}


/**
 * Get resolution given projection
 *
 * Returns a default set of resolutions for standard
 * TMS, WMS, etc servers serving up common projections.
 * Layers supporting less common projections and resolutions
 * can easily define custom resolutions arrays.
 *
 * @ingroup openlayers_api
 *
 * @param $projection
 *   String specifying which projection this should take, like 900913.
 * @param $zoom_start
 *   Integer of first zoom level, default 0.
 * @param $zoom_end
 *   Integer of last zoom level, default FALSE.
 * @return
 *   Array of resolutions.
 */
function openlayers_get_resolutions($projection, $zoom_start = 0, $zoom_end = FALSE) {
  switch ($projection) {
    case 'EPSG:900913':
      // 16 zoom levels, taken from
      // openlayers.org TMS map
      $res = array(
        156543.0339,
        78271.51695,
        39135.758475,
        19567.8792375,
        9783.93961875,
        4891.969809375,
        2445.9849046875,
        1222.99245234375,
        611.496226171875,
        305.7481130859375,
        152.87405654296876,
        76.43702827148438,
        38.21851413574219,
        19.109257067871095,
        9.554628533935547,
        4.777314266967774,
        2.388657133483887,
        1.1943285667419434,
        0.5971642833709717,
        0.29858214169740677,
        0.14929107084870338,
        0.07464553542435169
      );
      break;
    case 'EPSG:4326':
      // 16 zoom levels, taken from
      // openlayers.org default WMS map
      $res = array(
        0.703125,
        0.3515625,
        0.17578125,
        0.087890625,
        0.0439453125,
        0.02197265625,
        0.010986328125,
        0.0054931640625,
        0.00274658203125,
        0.001373291015625,
        0.0006866455078125,
        0.00034332275390625,
        0.000171661376953125,
        0.0000858306884765625,
        0.00004291534423828125,
        0.000021457672119140625,
        0.000010728836059570312,
      );
      break;
    default:
      $res = array();
      break;
  }
  $length = ($zoom_end == FALSE) ? NULL : $zoom_end - $zoom_start;
  // By default this will not actually clip the array
  $resolutions = array_slice($res, $zoom_start, $length);
  return $resolutions;
}
