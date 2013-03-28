/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

Drupal.openlayers.addBehavior('openlayers_behavior_refresh', function (context, options) {
  var map = context.openlayers;
  var layers = [];

  for (var i in options.layers) {
    layeroptions = options.layers[i];
    if (layeroptions.layer == 1) {
      var selectedLayer = map.getLayersBy('drupalID', i);
      if (typeof selectedLayer[0] != 'undefined') {
        layers.push(selectedLayer[0]);
      }
    }
  }

  // If no layer is selected, just return.
  if (layers.length < 1) {
    return;
  }

  jQuery(layers).each(function(index, layer){

    var layeroptions = options.layers[layer.drupalID];
    var interval = parseInt(layeroptions.interval, 10) * 1000;

    var refresh = new OpenLayers.Strategy.Refresh({
      force: true,
      interval: interval
    });

    refresh.setLayer(layer);
    refresh.activate();
  });
});
