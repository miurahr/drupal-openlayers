// $Id$

/**
 * @file
 * This file holds the main javascript API for OpenLayers. It is 
 * responsable for loading and displaying the map.
 *
 * @ingroup openlayers
 */

/**
 * Minimal OpenLayers map bootstrap.
 * All additional operations occur in additional Drupal behaviors.
 */
Drupal.behaviors.openlayers = function(context) {
  if (typeof(Drupal.settings.openlayers) == 'object' && Drupal.settings.openlayers.maps && !$(context).data('openlayers')) {
    $('.openlayers-map:not(.openlayers-processed)').each(function() {
      $(this).addClass('openlayers-processed');

      var map_id = $(this).attr('id');

      if (Drupal.settings.openlayers.maps[map_id]) {
        var map = Drupal.settings.openlayers.maps[map_id];
        
        $(this)
          // @TODO: move this into markup in theme function, doing this dynamically is a waste.
          .css('width', map.width)
          .css('height', map.height);

        // Process map option settings and prepare params for OpenLayers.
        if (map.options) {
          var options = map.options;
          options.projection = new OpenLayers.Projection('EPSG:' + map.projection);          
          options.displayProjection = new OpenLayers.Projection('EPSG:' + map.displayProjection);
          options.controls = [];
        }
        else {
          var options = {};
          options.projection = new OpenLayers.Projection('EPSG:' + map.projection);
          options.displayProjection = new OpenLayers.Projection('EPSG:' + map.displayProjection);
          options.maxExtent = new OpenLayers.Bounds(-20037508.34, -20037508.34, 20037508.34, 20037508.34);
          options.controls = [];
        }

        // Change image path if specified
        if (map.image_path) {
          if (map.image_path.substr(map.image_path.length - 1) != '/') {
            map.image_path = map.image_path + '/';
          }
          if (map.image_path.indexOf('://') >= 0) {
            OpenLayers.ImgPath = map.image_path;
          }
          else {
            OpenLayers.ImgPath = Drupal.settings.basePath + map.image_path;
          }
        }

        // Change css path if specified
        if (map.css_path) {
          if (map.css_path.indexOf('://') >= 0) {
            options.theme = map.css_path;
          }
          else {
            options.theme = Drupal.settings.basePath + map.css_path;
          }
        }

        // Initialize openlayers map
        var openlayers = new OpenLayers.Map(map.id, options);

        // Run the layer addition first
        Drupal.openlayers.addLayers(map, openlayers);

        // Attach data to map DOM object
        $(this).data('openlayers', {'map': map, 'openlayers': openlayers});

        // Finally, attach behaviors
        Drupal.attachBehaviors(this);
      }
    });
  }
};

/**
 * Collection of helper methods.
 */
Drupal.openlayers = {
  'addLayers': function(map, openlayers) {
    for (var key in map['layers']) {
      var layer;      
      var options = map['layers'][key];
      switch (options['layer_handler']) {
        case 'Vector':
          var styleMap = Drupal.openlayers.getStyleMap(map, options.name);
          var layer = new OpenLayers.Layer.Vector(key, {'styleMap': styleMap});
          if (options.features) {
            Drupal.openlayers.addFeatures(map, layer, options.features);
          }
          layer.title = options.title;
          break;
        case 'WMS':
          if (typeof(options.params.format) == "undefined"){
            options.params.format = "image/png";
          }
          var layer = new OpenLayers.Layer.WMS(key, options.url, options.params, options.options);
          break;
        case 'TMS':
          var styleMap = Drupal.openlayers.getStyleMap(map, options.name);
          if (typeof(options.options.maxExtent) !== 'undefined') {
            options.options.maxExtent = new OpenLayers.Bounds.fromArray(options.options.maxExtent);
          }
          if (typeof(options.options.type) == "undefined"){
            options.options.type = "png";
          }
          var layer = new OpenLayers.Layer.TMS(key, options.url, options.options);
          layer.styleMap = styleMap;
          break;
        case 'OSM':
          if (typeof(options.options.maxExtent) !== 'undefined') {
            options.options.maxExtent = new OpenLayers.Bounds.fromArray(options.options.maxExtent);
          }
          if (typeof(options.options.type) == "undefined"){
            options.options.type = "png";
          }
          var layer = new OpenLayers.Layer.OSM(key, options.url, options.options);  
          layer.attribution = "(c) OpenStreetMap (and) contributors, CC-BY-SA";
          break;
      }
      if (layer) {
        if (!map['layer_activated'] || map['layer_activated'][key]) {
          layer.visibility = true;
        }
        else {
          layer.visibility = false;
        }
        if (map.center.wrapdateline == '1') {
          layer.wrapDateLine = true;
        }
        openlayers.addLayer(layer);
      }
    }
    
    // Set our default base layer
    for (var layer in openlayers.layers) {      
      if (openlayers.layers[layer].name == map.layers[map.default_layer].name) {
        openlayers.setBaseLayer(openlayers.layers[layer]); 
      }
    }
    
    // Zoom & center
    if (map.center.initial) {
      var center = new OpenLayers.LonLat.fromString(map.center.initial.centerpoint);
      var zoom = parseInt(map.center.initial.zoom, 10);
      openlayers.setCenter(center, zoom, false, false);
    }

    // Set the restricted extent if wanted.
    // Prevents the map from being panned outside of a specfic bounding box.
    // TODO: needs to be aware of projection: currently the restrictedExtent string is always latlon
    if (typeof map.center.restrict != 'undefined') {
      openlayers.restrictedExtent = new OpenLayers.Bounds.fromString(map.center.restrict.restrictedExtent);
    }
  },

  'addFeatures': function(map, layer, features) {
    var wktFormat = new OpenLayers.Format.WKT();
    var newFeatures = [];

    // Go through features
    for (var key in features) {
      var feature = features[key];

      // Extract geometry either from wkt property or lon/lat properties
      if (feature.wkt) {
        // Check to see if it is a string of wkt, or an array for a multipart feature.
        if (typeof(feature.wkt) == "string") {
          var wkt = feature.wkt;
        }
        else if (typeof(feature.wkt) == "object" && feature.wkt !== null && feature.wkt.length !== 0) {
          var wkt = "GEOMETRYCOLLECTION(" + feature.wkt.join(',') + ")";
        }
        var newFeatureObject = wktFormat.read(wkt);
      }
      else if (feature.lon) {
        var newFeatureObject = wktFormat.read("POINT(" + feature.lon + " " + feature.lat + ")");
      }

      // If we have successfully extracted geometry add additional
      // properties and queue it for addition to the layer
      if (newFeatureObject) {
        var newFeatureSet = [];

        // Check to see if it is a new feature, or an array of new features.
        if (typeof(newFeatureObject[0]) == 'undefined'){
          newFeatureSet[0] = newFeatureObject;
        }
        else{
          newFeatureSet = newFeatureObject;
        }

        // Go through new features
        for (var i in newFeatureSet) {
          var newFeature = newFeatureSet[i];

          // Transform the geometry if the 'projection' property is different from the map projection
          if (feature.projection) {
            if (feature.projection != map.projection){
              var featureProjection = new OpenLayers.Projection("EPSG:" + feature.projection);
              var mapProjection = new OpenLayers.Projection("EPSG:" + map.projection);
              newFeature.geometry.transform(featureProjection, mapProjection);
            }
          }

          // Add attribute data
          if (feature.attributes){
            newFeature.attributes = feature.attributes;
            newFeature.data = feature.attributes;
          }

          // Add style information
          if (feature.style) {
            newFeature.style = jQuery.extend({}, OpenLayers.Feature.Vector.style['default'], feature.style);
          }

          // Push new features
          newFeatures.push(newFeature);
        }
      }
    }

    // Add new features if there are any
    if (newFeatures.length !== 0){
      layer.addFeatures(newFeatures);
    }
  },

  'getStyleMap': function(map, layername) {
    if (map.styles) {
      var stylesAdded = {};
      // Grab and map base styles.
      for (var style in map.styles) {
        stylesAdded[style] = new OpenLayers.Style(map.styles[style]);
      }
      // Implement layer-specific styles.
      if (map.layer_styles[layername]) {
        var style = map.layer_styles[layername];
        stylesAdded['default'] = new OpenLayers.Style(map.styles[style]);
      }
      return new OpenLayers.StyleMap(stylesAdded);
    }
    // Default styles
    return new OpenLayers.StyleMap({
      'default': new OpenLayers.Style({
        pointRadius: 5,
        fillColor: "#ffcc66",
        strokeColor: "#ff9933",
        strokeWidth: 4,
        fillOpacity:0.5
      }),
      'select': new OpenLayers.Style({
        fillColor: "#66ccff",
        strokeColor: "#3399ff"
      })
    });
  }
};
