/**
 * @file
 * Layer handler for Vector layers
 */

/**
 * Openlayer layer handler for Vector layers
 */
Drupal.openlayers.layer.vector = function(title, map, options) {

  var layer = new OpenLayers.Layer.Vector(title, {
    projection: new OpenLayers.Projection(options.projection),
    drupalID: options.drupalID,
    layer_handler: options.layer_handler,
    styleMap: Drupal.openlayers.getStyleMap(map, options.drupalID)
  });

  if (options.method == 'file' || options.method == 'url') {
    return new OpenLayers.Layer.Vector(title, {
      drupalID: options.drupalID,
      layer_handler: options.layer_handler,
      styleMap: Drupal.openlayers.getStyleMap(map, options.drupalID),
      projection: new OpenLayers.Projection(options.projection),
      strategies: [new OpenLayers.Strategy.Fixed()],
      protocol: new OpenLayers.Protocol.HTTP({
        url: options.url,
        format: get_format(options)
      })
    });
  }

  if (options.method == 'raw') {
    var features = parseFeatures(options.raw, options);
  }

  if (options.method == 'views') {
    var features = parseFeatures(options.features, options);
  }

  // Add features, if needed
  if (features) {
    layer.addFeatures(features);
  }

  layer.events.triggerEvent('loadend');

  return layer;

  function get_format(options) {
    switch (options.format) {
      case 'GPX':
        return new OpenLayers.Format.GPX(options.formatOptions);
      case 'KML':
        return new OpenLayers.Format.KML(options.formatOptions);
      case 'GeoJSON':
        options.formatOptions.internalProjection = new OpenLayers.Projection(map.projection);
        options.formatOptions.externalProjection = new OpenLayers.Projection(options.projection);
        return new OpenLayers.Format.GeoJSON(options.formatOptions);
    }
  }

  function parseFeatures(vector, options) {
    var features = [];
    switch(options.format) {
      case 'GPX':
        var format = new OpenLayers.Format.GPX(options.formatOptions);
        var features = format.read(vector);
        break;
      case 'KML':
        var format = new OpenLayers.Format.KML(options.formatOptions);
        var features = format.read(vector);
        break;
      case 'GeoJSON':
        options.formatOptions.internalProjection = new OpenLayers.Projection(map.projection);
        options.formatOptions.externalProjection = new OpenLayers.Projection(options.projection);
        var format = new OpenLayers.Format.GeoJSON(options.formatOptions);
        var features = format.read(vector);
        break;
      case 'features':
        // Create a method who extracts features properly
        Drupal.openlayers.addFeatures(map, layer, options.features);
        break;
    }

    return features;
  }
};
