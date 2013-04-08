/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Scale Line Behavior
 */
Drupal.openlayers.addBehavior('openlayers_behavior_compass', function (data, options) {
  var map = data.openlayers;

  var layer = new OpenLayers.Layer.Vector('Compass', {
    displayInLayerSwitcher:false,
    styleMap: new OpenLayers.StyleMap({
      "default": options.style
    })
  });

  layer.events.register('moveend', layer, function(event) {
    layer.removeAllFeatures();
    // TODO: Find a way to calculate the pixel automatically.
    var pixel = new OpenLayers.Pixel(map.size.w-90, 90);
    var coords = map.getLonLatFromPixel(pixel);
    // TODO: Is this really necessary ?
    var azimuth = 0;
    if (map.zoom > 0) {
      var npole = new OpenLayers.LonLat(0, 90).transform(new OpenLayers.Projection('EPSG:4326'), map.getProjectionObject());
      var center = map.getCenter();
      var deltay = npole.lat - center.lat;
      deltay = deltay != 0 ? deltay : 0.0000001;
      var deltax = npole.lon - center.lon;
      azimuth = Math.atan(deltax / deltay) * (180.0/Math.PI);
    }
    var feature = new OpenLayers.Feature.Vector(
      new OpenLayers.Geometry.Point(coords.lon, coords.lat), {angle: azimuth}
    );
    layer.addFeatures(feature);
  });

  map.addLayer(layer);
});
