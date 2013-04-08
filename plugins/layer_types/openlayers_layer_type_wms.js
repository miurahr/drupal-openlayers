
/**
 * @file
 * Layer handler for WMS layers
 */

/**
 * Openlayer layer handler for WMS layer
 */
Drupal.openlayers.layer.wms = function(title, map, options) {
  /* TODO: have PHP take care of the casts here, not JS! */
  if (options.params.buffer) {
    options.params.buffer = parseInt(options.params.buffer, 10);
  }
  if (options.params.ratio) {
    options.params.ratio = parseFloat(options.params.ratio);
  }

  if (OpenLayers.Util.isArray(options.options.maxExtent)) {
    options.options.maxExtent = OpenLayers.Bounds.fromArray(options.options.maxExtent);
  } else {
    options.options.maxExtent = OpenLayers.Bounds.fromArray(map.maxExtent);
  }

  options.options.drupalID = options.drupalID;

  // Set isBaseLayer explicitly so that OpenLayers does not guess from transparency
  options.options.isBaseLayer = Boolean(options.isBaseLayer);

  // Convert to representation that match with WMS specification
  var paramsClone = jQuery.extend(true, {}, options.params);
  if(paramsClone.hasOwnProperty("TRANSPARENT") && paramsClone.TRANSPARENT===0){
    paramsClone.TRANSPARENT = "FALSE";
  }
  if(paramsClone.hasOwnProperty("TRANSPARENT") && paramsClone.TRANSPARENT===1){
    paramsClone.TRANSPARENT = "FALSE";
  }

  var optionsClone = jQuery.extend(true, {}, options.options);
  // OpenLayers can calculate the resolutions usually if provided with the number of zoom levels and tile sizes
  optionsClone.numZoomLevels=18;

  return new OpenLayers.Layer.WMS(title, options.base_url, paramsClone, optionsClone);
};
