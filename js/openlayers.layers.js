// $Id$
/*jslint white: false */
/*global OpenLayers Drupal */

/**
 * @file
 * JS functions to handle different types of layers for core OpenLayers modules
 *
 * @ingroup openlayers
 */

/**
 * Default OpenLayers layer handlers
 * Other handlers are added via ctools plugins
 *
 * */
Drupal.openlayers.layer = {
  /*
   * Creates an OpenLayers.Layer.WMS object
   * http://dev.openlayers.org/releases/OpenLayers-2.8/doc/apidocs/files/OpenLayers/Layer/WMS-js.html
   * @param name: name (a key in the map['layers'] array)
   * @param map: the map object
   * @param options: a hash of options, currently with object values like bounds as arrays
   * @return layer object
   */
  'WMS': function (name, map, options) {
    // TODO: do not permit empty format or type parameters
    if (options.params.format === undefined) {
      options.params.format = "image/png";
    }
    // TODO: move Bounds and other instantiations into func
    options.options.maxExtent = new OpenLayers.Bounds.fromArray(options.options.maxExtent);
    return new OpenLayers.Layer.WMS(name, options.url, options.params, options.options);
  },
  'TMS': function (name, map, options) {
    var styleMap = Drupal.openlayers.getStyleMap(map, options.name);
    if (options.options.maxExtent !== undefined) {
      options.options.maxExtent = new OpenLayers.Bounds.fromArray(options.options.maxExtent);
    }
    if (options.options.type === undefined){
      options.options.type = "png";
    }
    var layer = new OpenLayers.Layer.TMS(name, options.url, options.options);
    layer.styleMap = styleMap;
    return layer;
  },
  'OSM': function (name, map, options) {
    if (typeof(options.options.maxExtent) !== 'undefined') {
      options.options.maxExtent = new OpenLayers.Bounds.fromArray(options.options.maxExtent);
    }
    if (options.options.type === undefined){
      options.options.type = "png";
    }
    var layer = new OpenLayers.Layer.OSM(name, options.url, options.options);  
    layer.attribution = "(c) OpenStreetMap (and) contributors, CC-BY-SA";
    return layer;
  },
  'Vector': function (name, map, options) {
    var styleMap = Drupal.openlayers.getStyleMap(map, options.name);
    var layer = new OpenLayers.Layer.Vector(name, {'styleMap': styleMap});
    if (options.features) {
      Drupal.openlayers.addFeatures(map, layer, options.features);
    }
    layer.title = options.title;
    return layer;
  }
};
