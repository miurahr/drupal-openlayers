// $Id$

/**
 * @file
 * JS file for testing JS stuff in OpenLayers
 */

/**
 * Global object for OL Tests
 */
var OL = OL || {};
OL.Testing = OL.Testing || {};

/**
 * Test Callback for Style Context
 */
OL.Testing.StyleContextCallback = function(mapid, layerid, render_intent) {
  alert(mapid);
  return {};
}