<?php
// $Id: emfield.admin.inc,v 1.1.2.9 2009/05/29 00:16:36 aaron Exp $

/**
 * @file
 * The administrative settings page.
 */

/**
 * Callback for admin/content/emfield.
 */
function emfield_settings() {
  if (!module_exists('emvideo') && !module_exists('emimage') && !module_exists('emaudio')) {
    drupal_set_message(t('The Embedded Media Field module does nothing on its own. You should also install the Embedded Video Field, Embedded Image Field, and/or Embedded Audio Field modules from the <a href="@modules">modules administration page</a>. (If you do not see them listed there, under the CCK section, you may need to <a href="@download">download the module</a> from its project page. They are all in the same package.)', array('@download' => 'http://drupal.org/project/emfield', '@modules' => url('admin/build/modules'))), 'error');
  }
  $form = array();
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#description' => t('These features will be generally available for use by related modules as needed.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (module_exists('swfobject_api')) {
    $swfobject_desc = t('As you have the <a href="@swfobject_api" target="_blank">SWFObject API</a> module installed, Embedded Media Field will use those settings, assuming you have configured them properly. Visit its <a href="@settings">settings page</a> for more information.', array('@swfobject_api' => 'http://drupal.org/project/swfobject_api', '@settings' => url('admin/settings/swfobject_api')));
  }
  else {
    $swfobject_desc = t('If you have the swfobject.js file installed on your system, you can make it available to Embedded Media Field and its related modules by entering the information here. You can download and find out more about <a href="@here" target="_blank">SWFObject here</a>. You may also choose to install the <a href="@swfobject_api" target="_blank">SWFObject API</a> module, which will integrate automatically with this module..', array('@here' => 'http://code.google.com/p/swfobject/', '@swfobject_api' => 'http://drupal.org/project/swfobject_api'));
  }
  $form['general']['swfobject'] = array(
    '#type' => 'fieldset',
    '#title' => t('SWF Object'),
    '#description' => $swfobject_desc,
    '#collapsible' => TRUE,
  );
  $form['general']['swfobject']['emfield_swfobject'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use SWFObject'),
    '#default_value' => variable_get('emfield_swfobject', FALSE),
    '#description' => t('When checked, then Embedded Media Field will use the SWFObject javascript library when it is able.'),
  );

  if (!module_exists('swfobject_api')) {
    $form['general']['swfobject']['emfield_swfobject_location'] = array(
      '#type' => 'textfield',
      '#title' => t('SWFObject location'),
      '#default_value' => variable_get('emfield_swfobject_location', ''),
      '#description' => t('Enter the path to the swfobject.js file, relative to the web root, without the preceding slash (/).'),
    );
  }
  $header = array(t('Feature'), t('Supported'), t('Notes'));

  foreach (module_implements('emfield_info', TRUE) as $module) {
    $emfield_info = module_invoke($module, 'emfield_info');
    $providers = emfield_system_list($module);
    $form[$module] = array(
      '#type' => 'fieldset',
      '#title' => t('@neighborhood', array('@neighborhood' => $emfield_info['#name'])),
      '#description' => $emfield_info['#settings_description'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$module]['providers'] = array(
      '#type' => 'fieldset',
      '#title' => t('Providers'),
      '#description' => t('The following settings determine what providers are allowed, and what provider-specific options, if any, are set.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    foreach ($providers as $provider) {
      $info = emfield_include_invoke($module, $provider->name, 'info');
      $form[$module]['providers'][$provider->name] = array(
        '#type' => 'fieldset',
        '#title' => t('@provider configuration', array('@provider' => $info['name'])),
        '#description' => $info['settings_description'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      if (is_array($info['supported_features']) && !empty($info['supported_features'])) {
        $form[$module]['providers'][$provider->name]['supported_features'] = array(
          '#type' => 'fieldset',
          '#title' => t('Supported features'),
          '#description' => t('This is a list of the current state of support for the following features by %provider:', array('%provider' => $info['name'])),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#weight' => 7,
        );
        $form[$module]['providers'][$provider->name]['supported_features']['features'] = array(
          '#type' => 'markup',
          '#value' => theme('table', $header, $info['supported_features']),
        );
      }
      $form[$module]['providers'][$provider->name]['emfield_'. $module .'_allow_'. $provider->name] = array( '#type' => 'checkbox',
        '#title' => t('Allow content from %provider', array('%provider' => $info['name'])),
        '#description' => t('If checked, then content types may be created that allow content to be provided by %provider.', array('%provider' => $info['name'])),
        '#weight' => -10,
        '#default_value' => variable_get('emfield_'. $module .'_allow_'. $provider->name, TRUE),
      );
      $form[$module]['providers'][$provider->name][] = emfield_include_invoke($module, $provider->name, 'settings');
    }
    $form[$module] = array_merge($form[$module], module_invoke($module, 'emfield_settings'));
  }

  $form = system_settings_form($form);

  // Custom valiation callback so we can validate the SWFObject path.
  $form['#validate'][] = 'emfield_settings_validate';

  return $form;
}

/**
 *  Validation for emfield_settings form, callback for /admin/content/emfield.
 *  Ensure we have a valid SWFObject path.
 */
function emfield_settings_validate($form, $form_state) {
  if ($form_state['values']['emfield_swfobject'] && $form_state['values']['emfield_swfobject_location']) {
    if (!file_exists($form_state['values']['emfield_swfobject_location'])) {
      form_set_error('emfield_swfobject_location', t('The SWFObject %file file was not found at that location. Please specify a valid location.', array('%file' => $form_state['values']['emfield_swfobject_location'])));
    }
  }
}

/**
 * Menu callback settings form.
 * If job_queue module is installed, admins can reload embedded media data in bulk
 */
function emfield_settings_jobqueue() {
  $form = array();
  $form['job_queue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reload Embedded Media Data'),
    '#description' => t('You can reload the embedded media data on all nodes on your site if necessary.  This action will be queued to run on cron via the job queue module.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $types = content_types();
  $options = array();
  foreach ($types as $type) {
    foreach ($type['fields'] as $field) {
      if (module_hook($field['type'], 'emfield_info')) {
        $options[$type['type']] = $type['name'];
      }
    }
  }
  if (count($options)) {
    $form['job_queue']['types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Types to reload'),
      '#options' => $options,
      '#description' => t('Choose the content types to reload'),
    );

    $form['job_queue']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }

  return $form;
}

/**
 * For each selected content type, add all of its nodes to the job queue for reloading.
 */
function emfield_settings_jobqueue_submit($form, &$form_state) {
  foreach($form_state['values']['types'] as $type => $value) {
    if ($value) {
      $result = db_query("SELECT nid FROM {node} WHERE type = '%s'", $type);
      while ($nid = db_result($result)) {
        job_queue_add('emfield_reload', 'refresh emfield info', array($nid), drupal_get_path('module', 'emfield') .'/emfield.admin.inc', TRUE);
      }
      drupal_set_message(t('The %type nodes have been queued to reload emfield data upon cron.', array('%type' => $type)));
    }
  }
}

/**
 *  This reloads and saves the data for a single node.
 */
function emfield_reload($nid) {
  if ($node = node_load($nid)) {
    $type = content_types($node->type);
    foreach ($type['fields'] as $field) {
      if (module_hook($field['type'], 'emfield_info')) {
        $items = $node->{$field['field_name']};
        emfield_emfield_field('submit', $node, $field, $items, FALSE, FALSE, $field['type']);
        $node->{$field['field_name']} = $items;
        node_save($node);
      }
    }
  }
}