
/**
 * @file
 * Layer handler for WMS layers
 */

/**
 * Openlayer layer handler for WMS layer
 */
Drupal.openlayers.layer.wms = function(title, map, options) {
  if (OpenLayers.Util.isArray(options.options.maxExtent)) {
    options.options.maxExtent = OpenLayers.Bounds.fromArray(options.options.maxExtent);
  } else {
    options.options.maxExtent = OpenLayers.Bounds.fromArray(map.maxExtent);
  }

  options.options.drupalID = options.drupalID;

  // Set isBaseLayer explicitly so that OpenLayers does not guess from transparency
  options.options.isBaseLayer = Boolean(options.isBaseLayer);
  options.options.singleTile = Boolean(options.options.singleTile);

  // Convert to representation that match with WMS specification
  var paramsClone = jQuery.extend(true, {}, options.params);
  if(paramsClone.hasOwnProperty("transparent") && paramsClone.transparent===0){
    paramsClone.transparent = false;
  }
  if(paramsClone.hasOwnProperty("transparent") && paramsClone.transparent===1){
    paramsClone.transparent = false;
  }

  var optionsClone = jQuery.extend(true, {}, options.options);
  // OpenLayers can calculate the resolutions usually if provided with the number of zoom levels and tile sizes
  optionsClone.numZoomLevels=18;

  var layer = new OpenLayers.Layer.WMS(title, options.base_url, paramsClone, optionsClone);
  return layer;
};
