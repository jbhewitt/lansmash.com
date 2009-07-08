<?php
// $Id: gravatar.admin.inc,v 1.1.2.21 2008/12/16 01:51:12 davereid Exp $

/**
 * @file
 * Admin page callbacks for the gravatar module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function gravatar_admin_settings() {
  $form = array();

  // Display settings.
  $form['display'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Display'),
    '#description' => t('Control how Gravatars are displayed'),
    '#collapsed'   => FALSE,
    '#collapsible' => TRUE,
    '#access'      => gravatar_var('toggle'),
  );
  $form['display']['gravatar_size'] = array(
    '#type'          => 'select',
    '#title'         => t('Image size'),
    '#description'   => t('Select your preferred gravatar image size (maximum 250 pixels). The user pictures size can be adjusted in the <a href="@user-picture-link">user pictures settings</a>.', array('@user-picture-link' => url('admin/user/settings', array('fragment' => 'edit-user-picture-default')))),
    '#default_value' => gravatar_var('size'),
    '#options'       => array(0 => t('User pictures size (@sizex@size)', array('@size' => min(explode('x', variable_get('user_picture_dimensions', '85x85')))))) + drupal_map_assoc(array(16, 22, 24, 32, 48, 64, 72, 80), '_gravatar_admin_settings_map_size'),
  );
  $form['display']['gravatar_rating'] = array(
    '#type'          => 'select',
    '#title'         => t('Maturity filter'),
    '#description'   => t('An optional maturity rating parameter with a value (MPAA nomenclatura) that determines the highest rating (inclusive) that will be returned.'),
    '#default_value' => gravatar_var('rating'),
    '#options'       => array(0 => 'None') + drupal_map_assoc(array('G', 'PG', 'R', 'X')),
  );
  $form['display']['gravatar_default'] = array(
    '#type'          => 'radios',
    '#title'         => t('Default avatar image'),
    '#description'   => t('Specifies an image that should be returned if either the requested email address has no associated Gravatar, or that Gravatar has a rating higher than is allowed by the "rating" parameter.'),
    '#options'       => array(
      GRAVATAR_DEFAULT_GLOBAL => t('Global default user image'),
      GRAVATAR_DEFAULT_MODULE => t('Module default image'),
      GRAVATAR_DEFAULT_IDENTICON => t('Gravatar.com identicon (generated)'),
      GRAVATAR_DEFAULT_WAVATAR => t('Gravatar.com wavatar (generated)'),
      GRAVATAR_DEFAULT_MONSTERID => t('Gravatar.com monsterid (generated)'),
      GRAVATAR_DEFAULT_LOGO => t('Gravatar.com logo'),
    ),
    '#prefix' => '<div class="picture js-show">'. theme('image', '', t('Default picture example'), t('Default picture example'), array('width' => _gravatar_get_size(), 'height' => _gravatar_get_size(), 'id' => 'gravatar-imagepreview'), FALSE) .'</div>',
  );

  // Add JavaScript and CSS to swap the defalut image previews.
  drupal_add_js(drupal_get_path('module', 'gravatar') .'/gravatar.js');
  drupal_add_css(drupal_get_path('module', 'gravatar') .'/gravatar.css');

  // Add default picture options manually so we can disable individual radios.
  foreach ($form['display']['gravatar_default']['#options'] as $option_index => $option_title) {
    $option_value = _gravatar_get_default_image($option_index);
    $option_element = &$form['display']['gravatar_default'][$option_index];
    $option_element = array(
      '#type' => 'radio',
      '#title' => $option_title,
      '#return_value' => $option_index,
      '#default_value' => gravatar_var('default'),
      '#parents' => array('gravatar_default'),
      '#disabled' => FALSE,
    );

    switch ($option_index) {
      case GRAVATAR_DEFAULT_GLOBAL:
        // If the global user picture is empty, disable the respective option.
        if (!variable_get('user_picture_default', '')) {
          $option_element['#disabled'] = TRUE;
          $option_element['#description'] = t('Looks like you have a blank default user picture! If you want to use this option, <a href="@user-picture-link">go add a default user picture</a>!', array('@user-picture-link' => url('admin/user/settings', array('fragment' => 'edit-user-picture-default'))));
        }
        else {
          $option_element['#description'] = t('This option can be adjusted in the <a href="@user-picture-link">user pictures settings</a>.', array('@user-picture-link' => url('admin/user/settings', array('fragment' => 'edit-user-picture-default'))));
        }
        break;
    }

    // Add an image to preview this default image option.
    $attributes = array(
      'width' => _gravatar_get_size(),
      'height' => _gravatar_get_size(),
      'id' => 'gravatar-imagepreview-'. $option_index,
      // Hide the image if the respective option is disabled.
      'class' => 'js-hide'. ($option_element['#disabled'] ? ' hide' : ''),
    );
    $option_element['#suffix'] = theme('image', _gravatar_get_gravatar(mt_rand(), $option_value, FALSE), $option_title, $option_title, $attributes, FALSE);
  }

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#description' => t("If you are not sure what you are doing, do not modify these settings!"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => gravatar_var('toggle'),
  );
  $form['advanced']['gravatar_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Gravatar URL'),
    '#default_value' => gravatar_var('url'),
  );

  return system_settings_form($form);
} // gravatar_admin_settings

/**
 * Tools page (and form builder)
 */
function gravatarcheckemail_form() {
  $form['email'] = array(
    '#title' => t('Email'),
    '#type'  => 'textfield',
  );
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Check !'),
  );
  return $form;
} // gravatarcheckemail_form

function gravatarcheckemail_form_validate($form, &$form_state) {
  if (empty($form['email']['#value']) || !valid_email_address($form['email']['#value'])) {
    form_error($form, t('Please enter a valid email address.'));
  }
} // gravatarcheckemail_form_validate

function gravatarcheckemail_form_submit($form, &$form_state) {
  drupal_goto('http://en.gravatar.com/site/check/'. $form_state['values']['email']);
} // gravatarcheckemail_form_submit

/**
 * Map pixel size to helper string
 *
 * Helper function
 *
 * @param $size
 *   Size in pixels.
 * @return
 *   A string in the format '{$size}x{$size}'.
 */
function _gravatar_admin_settings_map_size($size) {
  return str_replace('@num', $size, '@numx@num');
} // _gravatar_admin_settings_map_size
