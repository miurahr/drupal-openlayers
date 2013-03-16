/**
 * @file
 * Layer handler for Vector layers
 */

/**
 * Openlayer layer handler for KML layer
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
    var uri = options.url;
    // Use an AJAX like call to get data from URL
    OpenLayers.Request.GET({
      url: uri,
      callback: function (response) {
        addFeatures(response.responseText, options);
      }
    });
  }

  if (options.method == 'raw') {
    addFeatures(options.raw, options);
  }

  function addFeatures(vector, options) {
    switch(options.format) {
      case 'GPX':
        var format = new OpenLayers.Format.GPX(options.formatOptions);
        break;
      case 'KML':
        var format = new OpenLayers.Format.KML(options.formatOptions);
        break;
    }
    var features = format.read(vector);
    // Add features, if needed
    if (features) {
      layer.addFeatures(features);
      layer.events.triggerEvent('loadend');
    }
  }

  return layer;
};
