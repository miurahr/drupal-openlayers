/**
 * @file
 * Layer handler for Vector layers
 */

/**
 * Openlayer layer handler for Vector layers
 */
Drupal.openlayers.layer.vector = function(title, map, options) {

  var layer = new OpenLayers.Layer.Vector(title, {
    drupalID: options.drupalID,
    layer_handler: options.layer_handler,
    styleMap: Drupal.openlayers.getStyleMap(map, options.drupalID)
  });

  options.formatOptions.internalProjection = new OpenLayers.Projection(map.projection);
  options.formatOptions.externalProjection = new OpenLayers.Projection(options.projection);

  if (options.method == 'file' || options.method == 'url') {
    return new OpenLayers.Layer.Vector(title, {
      drupalID: options.drupalID,
      layer_handler: options.layer_handler,
      styleMap: Drupal.openlayers.getStyleMap(map, options.drupalID),
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
    layer.events.triggerEvent('featuresadded');
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
        return new OpenLayers.Format.GeoJSON(options.formatOptions);
    }
  }

  function parseFeatures(vector, options) {
    switch(options.format) {
      case 'GPX':
        break;
      case 'KML':
        break;
      case 'GeoJSON':
        break;
      case 'features':
        // Create a method who extracts features properly
        return Drupal.openlayers.getFeatures(map, layer, options.features);
        break;
    }

    return get_format(options).read(vector);
  }
};
