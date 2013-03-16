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

  options.formatOptions.internalProjection = new OpenLayers.Projection(map.projection);
  options.formatOptions.externalProjection = new OpenLayers.Projection(options.projection);

  if (options.method == 'file' || options.method == 'url') {
    var uri = options.url;
    // Use an AJAX like call to get data from URL
    OpenLayers.Request.GET({
      url: uri,
      callback: function (response) {
        parseFeatures(response.responseText, options);
      }
    });
  }

  if (options.method == 'raw') {
    parseFeatures(options.raw, options);
  }

  if (options.method == 'views') {
    parseFeatures(options.features, options);
  }

  function parseFeatures(vector, options) {
    switch(options.format) {
      case 'GPX':
        var format = new OpenLayers.Format.GPX(options.formatOptions);
        var features = format.read(vector);
        break;
      case 'KML':
        var format = new OpenLayers.Format.KML(options.formatOptions);
        var features = format.read(vector);
        break;
      case 'features':
        // Create a method who extracts features properly
        Drupal.openlayers.addFeatures(map, layer, options.features);
        break;
    }

    // Add features, if needed
    if (features) {
      layer.addFeatures(features);
    }

    layer.events.triggerEvent('loadend');

  }

  return layer;
};
