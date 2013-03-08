<?php

/**
 * Check the plugin definition of a behavior.
 * Some field *MUST* be there to work correctly with OL.
 *
 * @ingroup openlayers_api
 * @param $definition
 * @return bool
 */

module_load_include('inc', 'openlayers','includes/openlayers_behavior.class.inc');

function _openlayers_behavior_definition_check($definition) {
  $mandatory_fields = array(
    array('title'),
    array('description'),
    array('name'),
    array('path'),
    array('type'),
    array('class')
  );

  foreach ($mandatory_fields as $field) {
    $missing = drupal_array_nested_key_exists($definition, $field);
    if (!$missing) {
      drupal_set_message(t("There's a problem in the definition of the behavior <em>!behavior</em>. The behavior will be disabled.", array('!behavior' => $definition['name'])), 'warning');
      watchdog('openlayers', 'Behavior !behavior is unavailable because its
                                plugin definition is incomplete.',
        array('!behavior' => $definition['name']),
        WATCHDOG_ERROR);
      return FALSE;
    }
  }

  return TRUE;
}



/**
 * Get all behaviors.
 *
 * @ingroup openlayers_api
 *
 * @param $reset
 *   Boolean whether to reset cache or not.
 * @return
 *   Array of behavior info.
 */
function openlayers_behaviors($reset = FALSE) {
  ctools_include('plugins');
  $behaviors = ctools_get_plugins('openlayers', 'behaviors');
  return array_filter($behaviors, '_openlayers_behavior_definition_check');

}
