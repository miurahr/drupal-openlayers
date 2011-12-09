/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Navigation Behavior
 */
Drupal.openlayers.behaviors.openlayers_behavior_navigation = {
  attach: function (context, map, behavior, openlayers) {
    behavior.documentDrag = !!behavior.documentDrag;
    Drupal.openlayers.addControl(openlayers, 'Navigation', behavior);
  }
};
