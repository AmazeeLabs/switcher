<?php

/**
 * @file
 *   Administration callbacks for the switcher module.
 */

/**
 * Menu callback page for the admin switcher page.
 */
function switcher_admin_switcher_page() {
  $header = array(t('ID'), t('Title'), t('Actions'));
  $rows = array();
  $switchers = switcher_load_all();
  $admin_path = switcher_get_admin_path();
  foreach ($switchers as $switcher) {
    $row = array();
    $row[] = $switcher->sid;
    $row[] = $switcher->title;
    $actions = array();
    $actions[] = l(t('edit'), $admin_path . '/edit/' . $switcher->sid);
    $actions[] = l(t('configure'), $admin_path . '/configure/' . $switcher->sid);
    $actions[] = l(t('delete'), $admin_path . '/delete/' . $switcher->sid);
    $row[] = implode(' | ', $actions);
    $rows[] = $row;
  }
  return array(
    'switcher_list' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    ),
    'add_form' => drupal_get_form('switcher_admin_add_edit_switcher_form'),
  );
}

/**
 * Form constructor for adding/editing a swticher.
 */
function switcher_admin_add_edit_switcher_form($form, &$form_state, $switcher = NULL) {
  $switcher_form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($switcher) ? $switcher->title : '',
    '#required' => TRUE,
  );
  if (!empty($switcher)) {
    $switcher_form['sid'] = array(
      '#type' => 'value',
      '#value' => $switcher->sid,
    );
  }
  $switcher_form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  if (!empty($switcher)) {
    $form = $switcher_form;
  }
  else {
    $form['switcher_form'] = $switcher_form;
    $form['switcher_form']['#type'] = 'fieldset';
    $form['switcher_form']['#title'] = t('Add a switcher');
    $form['switcher_form']['#collapsible'] = TRUE;
    $form['switcher_form']['#collapsed'] = TRUE;
  }
  return $form;
}

/**
 * Submit handler for the switcher add/edit form.
 */
function switcher_admin_add_edit_switcher_form_submit(&$form, &$form_state) {
  // Load the switcher if we have one, or create a new one otherwise.
  $message = t('The switcher has been saved.');
  if (!empty($form_state['values']['sid'])) {
    $switcher = switcher_load($form_state['values']['sid']);
    $message = t('The switcher has been updated.');
  }
  else {
    $switcher = new stdClass();
    $switcher->created = REQUEST_TIME;
  }

  // Add the new values into the object.
  $switcher->title = $form_state['values']['title'];
  switcher_save($switcher);
  drupal_set_message($message);
  $form_state['redirect'] = switcher_get_admin_path();
}

/**
 * Confirmation form for deleting a switcher.
 */
function switcher_admin_delete_confirm($form, &$form_state, $switcher) {
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $switcher->sid,
  );
  return confirm_form($form, t('Are you sure you want to delete the switcher?'), switcher_get_admin_path(), t('Are you sure you want to delete the switcher? This action cannot be undone.'));
}

/**
 * Submit handler for the delete switcher confirmation form.
 */
function switcher_admin_delete_confirm_submit(&$form, &$form_state) {
  switcher_delete($form_state['values']['sid']);
  drupal_set_message(t('The switcher has been deleted.'));
  $form_state['redirect'] = switcher_get_admin_path();
}

/**
 * Configuration page of a switcher.
 */
function switcher_admin_configure_switcher_page($switcher) {
  return array(
    'configuration_list' => drupal_get_form('switcher_admin_switcher_components_form', $switcher),
    'add_config' => drupal_get_form('switcher_admin_add_edit_config_form', $switcher),
  );
}

/**
 * Form constructor for the switcher components admin form.
 */
function switcher_admin_switcher_components_form($form, &$form_state, $switcher) {
  $configuration_components = switcher_configuration_load_all($switcher->sid);
  if (!empty($configuration_components)) {
    $header = array('title' => t('Title'), 'configuration' => t('Configuration'), 'actions' => t('Actions'), 'weight' => t('Weight'));

    $rows = array();
    $weight_delta = round(count($configuration_components) / 2) + 10;
    foreach ($configuration_components as $configuration) {
      $row = array();
      $row[] = $configuration->title;
      $row[] = switcher_admin_configuration_summary($configuration);
      $actions = array();
      $actions[] = l(t('Edit'), switcher_get_admin_path() . '/configure/' . $switcher->sid . '/edit/' . $configuration->scid);
      $actions[] = l(t('Delete'), switcher_get_admin_path() . '/configure/' . $switcher->sid . '/delete/' . $configuration->scid);
      $row[] = implode(' | ', $actions);

      $rows[$configuration->scid] = array('data' => $row, 'class' => array('draggable'));
      $form['weight_' . $configuration->scid] = array(
        '#type' => 'weight',
        '#default_value' => $configuration->weight,
        '#delta' => $weight_delta,
        '#attributes' => array('class' => array('configuration-weight')),
        '#title' => t('Weight'),
        '#title_display' => 'invisible',
      );
      $scids[] = $configuration->scid;
    }
    $form['scids'] = array(
      '#type' => 'value',
      '#value' => $scids,
    );
    $form['#header'] = $header;
    $form['#rows'] = $rows;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  else {
    $form['no_items']['#markup'] = t('There are no configurations defined at the moment.');
  }
  return $form;
}

/**
 * Theme function for the switcher configuration table form.
 */
function theme_switcher_admin_switcher_components_form($variables) {
  $form = $variables['form'];
  $output = '';
  if (!empty($form['#header'])) {
    $table_id = 'switcher-configuration-table';
    foreach ($form['#rows'] as $scid => &$value) {
      $form['#rows'][$scid]['data']['weight'] = drupal_render($form['weight_' . $scid]);
    }
    $output = theme('table', array('header' => $form['#header'], 'rows' => $form['#rows'], 'attributes' => array('id' => $table_id)));
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'configuration-weight');
  }
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Submit handler fro the switcher configuration table form.
 *
 * This basically just updates the weight of the configuration items.
 */
function switcher_admin_switcher_components_form_submit(&$form, &$form_state) {
  foreach ($form_state['values']['scids'] as $scid) {
    $configuration = switcher_configuration_load($scid);
    $configuration->weight = $form_state['values']['weight_' . $scid];
    switcher_configuration_save($configuration);
  }
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Form constructor for adding/editing a switcher configuration.
 */
function switcher_admin_add_edit_config_form($form, &$form_state, $switcher, $configuration = NULL) {
  $conf_form['sid'] = array(
    '#type' => 'value',
    '#value' => $switcher->sid,
  );
  if (!empty($configuration)) {
    $conf_form['scid'] = array(
      '#type' => 'value',
      '#value' => $configuration->scid,
    );
    $conf_form['sid'] = array(
      '#type' => 'value',
      '#value' => $configuration->sid,
    );
  }
  $switcher_components = switcher_get_all_components();
  $conf_form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($configuration) ? $configuration->title : '',
    '#description' => t('Used just for administration purposes.'),
  );
  foreach ($switcher_components as $key => $component) {
    $conf_form[$key] = array(
      '#title' => $component->getTitle(),
      '#type' => 'select',
      '#options' => $component->getAvailableOptions(),
      '#default_value' => !empty($configuration) ? $configuration->configuration['components'][$key] : NULL,
    );
  }
  $conf_form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  if (!empty($configuration)) {
    $form = $conf_form;
  }
  else {
    $form['conf_form'] = $conf_form;
    $form['conf_form']['#type'] = 'fieldset';
    $form['conf_form']['#title'] = t('Add a configuration');
    $form['conf_form']['#collapsible'] = TRUE;
    $form['conf_form']['#collapsed'] = TRUE;
    $form['conf_form']['#description'] = t('By adding a configuration, the switcher will contain all the possible combinations of the options selected in the components.');
  }
  return $form;
}

/**
 * Submit handler for adding/editing a switcher confguration.
 */
function switcher_admin_add_edit_config_form_submit(&$form, &$form_state) {
  // Load the switcher configuration if we have one, or create a new one
  // otherwise.
  $message = t('The configuration has been saved.');
  if (!empty($form_state['values']['scid'])) {
    $switcher_configuration = switcher_configuration_load($form_state['values']['scid']);
    $message = t('The configuration has been updated.');
  }
  else {
    $switcher_configuration = new stdClass();
    $switcher_configuration->created = REQUEST_TIME;
    $switcher_configuration->weight = 0;
    $switcher_configuration->sid = $form_state['values']['sid'];
  }

  // Add the new values into the object.
  $switcher_configuration->title = $form_state['values']['title'];
  $switcher_components = switcher_get_all_components();
  foreach ($switcher_components as $key => $component) {
    $switcher_configuration->configuration['components'][$key] = $form_state['values'][$key];
  }
  switcher_configuration_save($switcher_configuration);
  drupal_set_message($message);
  $form_state['redirect'] = switcher_get_admin_path() . '/configure/' . $form_state['values']['sid'];
}

/**
 * Confirmation form for deleting a configuration item.
 */
function switcher_admin_delete_config_confirm($form, &$form_state, $configuration) {
  $form['scid'] = array(
    '#type' => 'value',
    '#value' => $configuration->scid,
  );
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $configuration->sid,
  );
  return confirm_form($form, t('Are you sure you want to delete the configuration?'), switcher_get_admin_path() . '/configure/' . $configuration->sid, t('Are you sure you want to delete the configuration? This action cannot be undone.'));
}

/**
 * Submit handler for the delete configuration item form.
 */
function switcher_admin_delete_config_confirm_submit(&$form, &$form_state) {
  switcher_configuration_delete($form_state['values']['scid']);
  drupal_set_message(t('The configuration item has been deleted.'));
  $form_state['redirect'] = switcher_get_admin_path() . '/configure/' . $form_state['values']['sid'];
}

/**
 * Helper function to show a summary of a configuration item.
 */
function switcher_admin_configuration_summary($configuration) {
  $components = switcher_get_all_components();
  $summary = array();
  foreach ($components as $component) {
    $summary[] = $component->summary($configuration->configuration['components']);
  }
  return implode('</br>', $summary);
}
