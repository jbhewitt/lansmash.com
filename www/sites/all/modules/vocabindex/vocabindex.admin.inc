<?php
// $Id: vocabindex.admin.inc,v 1.1.2.3.2.57.2.7 2009/02/16 00:28:00 xano Exp $

/**
 * @file
 * The Vocabulary Index administration section.
 */

/**
 * Menu callback: the VI administration page.
 */
function vocabindex_page_vi() {
  return theme('links', array(
    'add-page' => array(
      'title' => t('Add page'),
      'href' => 'admin/build/vocabindex/vi/add-page',
    ),
    'add-block' => array(
      'title' => t('Add block'),
      'href' => 'admin/build/vocabindex/vi/add-block',
    ),
  )) . vocabindex_vi_list();
}

/**
 * List all VIs.
 *
 * @return string
 *   A themed list of all VIs; a table by default.
 */
function vocabindex_vi_list() {
  
}

/**
 * Form: return the settings page.
 */
function vocabindex_form_settings() {
  $modules = module_implements('vocabindex', TRUE);
  $form = array();
  foreach($modules as $module) {
    $views = module_invoke($module, 'vocabindex');
    foreach ($views as $callback => $view) {
      $id = $module . '_' . $callback;
      $form[$id] = $view['settings'];
      $form[$id] += array(
        '#type' => 'fieldset',
        '#title' => t($view['title']),
        '#description' => t($view['description']),
      );
    }
  }

  return system_settings_form($form);
}

/**
 * Form validator: validate an integer form field.
 */
function vocabindex_validate_integer($element) {
  if (preg_match('/\D/', $element['#value']) == 1) {
    form_error($element, t('%element_title may only contain numbers.', array('%element_title' => $element['#title'])));
  }
}

/**
 * Form: the VI add form.
 *
 * @param $type string
 *   Either 'block' or 'page'.
 *
 * @return array
 *   A Drupal form.
 */
function vocabindex_form_vi($form_state, $vi, $type = NULL) {
  if (is_null($vi)) {
    $vi = vocabindex_vi($type);
  }

  $form['vi'] = array(
    '#type' => 'value',
    '#value' => $vi,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 255,
    '#size' => 30,
  );
  if ($vi->type == 'page') {
    $form['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#description' => !empty($vi->path) ? t('Currently located at !location.', array('!location' => l('/' . $vi->path, $vi->path))) : NULL,
      '#default_value' => check_plain($vi->path),
      '#required' => TRUE,
      '#maxlength' => '255',
      '#size' => 30,
      '#element_validate' => array('vocabindex_path_validate'),
    );
  }
  $form['node_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Node counts'),
    '#default_value' => $vi->node_count,
  );

  $vocs = taxonomy_get_vocabularies();
  $count = count($vocs);
  if ($count > 1) {
    $options = array();
    foreach ($vocs as $voc) {
      $options[$voc->vid] = $voc->name;
    }
    $form['vids'] = array(
      '#type' => 'select',
      '#title' => t('Vocabularies'),
      '#multiple' => TRUE,
      '#options' => $options,
      '#size' => $count > 5 ? 5 : $count,
      '#required' => TRUE,
    );
  }
  else {
    $voc = array_shift($vocs);
    $form['vids'] = array(
      '#type' => 'value',
      '#value' => $voc->vid,
    );
    $form['vids_display'] = array(
      '#type' => 'item',
      '#title' => t('Vocabulary'),
      '#value' => $voc->name,
    );
  }
  $form['view'] = array(
    '#type' => 'select',
    '#title' => t('View'),
    '#options' => vocabindex_views_get($vi->type),
    '#required' => TRUE,
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save index')
  );
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );

  return $form;
}

/**
 * Form: submission handler for vocabindex_form_vi().
 */
function vocabindex_form_vi_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  $view = explode('_', $values['view'], 2);
  $vi = $values['vi'];
  $vi->title = $values['title'];
  $vi->vids = $values['vids'];
  $vi->module = $view[0];
  $vi->view = $view[1];
  $vi->node_count = $values['node_count'];
  $vi->enabled = TRUE;
  if ($vi->type == 'page') {
    $vi->path = $values['path'];
  }

  vocabindex_vi_save($vi, FALSE);
}

/**
 * Return a blank VI.
 */
function vocabindex_vi($type) {
  return (object)array(
    'type' => $type,
    'vids' => array(),
    'module' => '',
    'view' => '',
    'node_count' => FALSE,
    'path' => '',
  );
}

/**
 * Return all available views for a given VI type.
 *
 * @param $type string
 *   Either 'block' or 'page';
 *
 * @return array
 *   Keys are the views' system names, values are their titles.
 */
function vocabindex_views_get($type) {
  $defaults = array(
    'page' => FALSE,
    'block' => FALSE,
  );

  $modules = module_implements('vocabindex', TRUE);
  $return_views = array();
  foreach($modules as $module) {
    $views = module_invoke($module, 'vocabindex');
    foreach ($views as $callback => $view) {
      $view += $defaults;
      if ($view[$type]) {
        $return_views[$module . '_' . $callback] = $view['title'];
      }
    }
  }
  asort($return_views);

  return $return_views;
}

/**
 * Validate a path.
 *
 * @param $element
 *   Type: array; The form element to validate.
 */
function vocabindex_path_validate($element) {
  $path = $element['#value'];
  $name = $element['#name'];

  // If the path is empty, return to prevent form errors from popping up.
  if (empty($path)) {
  return;
  }

  // Prepare the path for validation.
  $path = drupal_strtolower($path);
  $path = preg_replace('#^/|/$#', '', $path);

  // Check for illegal characters.
  if (strpos($path, '%') !== FALSE) {
    form_set_error($name, t('Paths may not contain a percentage sign.'));
  }

  // Check if this path isn't already used by an index to prevent people from
  // creating the same index twice.
  $vocabindex = db_result(db_query("SELECT COUNT(path) FROM {vocabindex} WHERE path = '%s'", $path));
  if ($vocabindex > 0) {
    form_set_error($name, t('Path is already used by another index.'));
  }
  else {
    // Check for existing menu paths.
    $count_menu_links = db_result(db_query("SELECT COUNT(link_path) FROM {menu_links} WHERE '%s' LIKE link_path", $path));
    if ($count_menu_links > 0) {
      form_set_error($name, t('Path is already used by a menu link.'));
    }
    // Check for existing aliases.
    elseif (drupal_lookup_path('source', $path) == TRUE) {
      form_set_error($name, t('Path is already used as an alias.'));
    }
  }
}

/**
 * Save a VI.
 *
 * @param $vi object
 *   The VI to save.
 * @param $update boolean
 *   Whether this is an update or not.
 */
function vocabindex_vi_save($vi, $update) {
  $update = $update ? 'viid' : array();
  drupal_write_record('vocabindex', $vi, $update);
}