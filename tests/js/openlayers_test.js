// $Id$

/**
 * @file
 * JS file for testing JS stuff in OpenLayers
 */

/**
 * Global object for OL Tests
 */
var OLTest = OLTest || {};
OLTest.Testing = OLTest.Testing || {};

/**
 * Test Callback for Style Context
 */
OLTest.Testing.StyleContextCallback = function(mapid, layerid, render_intent) {
  return {
    'getColor': function(feature) {
      var colors = ["red", "green", "blue"];
      return colors[Math.floor(Math.random() * 4)];
    },
    'getRadius': function(feature) {
      return Math.floor(Math.random() * 8) + 4;
    }
  };
}