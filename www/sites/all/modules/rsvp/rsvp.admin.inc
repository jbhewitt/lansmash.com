<?php

// $Id: rsvp.admin.inc,v 1.3.2.6 2009/05/28 18:23:42 ulf1 Exp $

/**
 * @module rsvp_admin
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides admin functionality
 * @author crunchywelch (welch@advomatic.com)
 * @author Omar Abdel-Wahab (owahab@gmail.com)
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pre Loading files that will be required in this module
foreach (array('mailer','form') as $file) {
  module_load_include("inc", "rsvp", "rsvp.{$file}");
}


/**
 * Menu callback: rsvp admin general form for rsvp.
 *
 * @see rsvp_admin_settings_general_submit()
 * @ingroup rsvp_admin
 */

function rsvp_admin_settings_general(&$form_state) {
  $form = array();
  
  $pick_address = RSVP_EMAIL_SITEADDRESS;
  $from_address = '';
  
  $emailfrom = variable_get('rsvp_from_address', NULL);
  if (isset($emailfrom)) {
    foreach ($emailfrom as $pick => $email) {
      $pick_address = $pick;
      $from_address = $email; 
    }
  }
  
  $form['rsvp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Installation information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['rsvp']['connector'] = array(
    '#type' => 'item',
    '#title' => t('Selected connector'),
    '#value' => t(variable_get(RSVP_VAR_CONNECTOR, t('NO RSVP CONNECTOR MODULE ENABLED!!'))),
  );

  $fieldarray = '';
  foreach (variable_get(RSVP_VAR_CONTENT_TYPES, array()) as $type => $field) {
    $fieldarray .= '[' . $type .']=>' . $field . ' <br>';    
  }
  
  $form['rsvp']['fields'] = array(
    '#type' => 'item',
    '#title' => t('Current associations'),
    '#value' => $fieldarray,
  );
  
  $form['rsvp_admin_settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sender information'),
    '#collapsible' => FALSE,
    '#description' => t('Default sender address that will be used for sending every email by rsvp.'),
  );

  $form['rsvp_admin_settings_general']['pick_from_address'] =  array(
    '#type' => 'select',
    '#title' => t('Pick which email address to use for emails'),
    '#default_value' => $pick_address,
    '#options' => array(RSVP_EMAIL_SITEADDRESS => t('The sites default address (ignore email address below)'), RSVP_EMAIL_SETTINGS => t('Use the email address below'), RSVP_EMAIL_CREATOR =>  t('Use the email address of user initiating the email (ignore email address below)')),
  );
  
  $form['rsvp_admin_settings_general']['rsvp_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email address'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => false,
    '#default_value' => $from_address,
  );

  $expired = variable_get('rsvp_for_expired_event', RSVP_OPTION_NO);
  
  $form['rsvp_for_expired_event'] =  array(
    '#type' => 'select',
    '#title' => t('Create invitations for expired events'),
    '#default_value' => $expired,
    '#options' => array(RSVP_OPTION_YES => t('Yes'), RSVP_OPTION_NO => t('No')),
    '#description' => t('Can users create invitations on already expired events (default is \'No\')'),
  );
  
  //print the email template functions
  $form['mail'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Email Options'),
  );

  global $rsvp_mailer_ops;
  //compose a list of possible replacement keys for email templates
  $replacement_keys = array_keys(rsvp_mailer_replacements(NULL, NULL, NULL, NULL, NULL));
  //these two are added per-receipient in user_relationship_mailer_send_email()
  $macro_replacements = array('%macros' => implode(', ', $replacement_keys));

  foreach ($rsvp_mailer_ops as $op => $op_name) {
    $defaults_function = "rsvp_mailer_{$op}_default";
    $defaults = $defaults_function();

    $form['mail'][$op] = array(
      '#type'         => 'fieldset',
      '#title'        => t($op_name),
      '#collapsible'  => TRUE,
      '#collapsed'    => TRUE
    );
    $form['mail'][$op]["rsvp_mailer_{$op}_subject"] = array(
      '#type'           => 'textfield',
      '#title'          => t('\'@Op_name\' email subject', array('@Op_name' => t($op_name))),
      '#default_value'  => variable_get("rsvp_default_mailer_{$op}_subject", $defaults['subject']),
    );
    $form['mail'][$op]["rsvp_mailer_{$op}_message"] = array(
      '#type'           => 'textarea',
      '#title'          => t('\'@Op_name\'  email message', array('@Op_name' => $op_name)),
      '#default_value'  => variable_get("rsvp_default_mailer_{$op}_message", $defaults['message']),
      '#description'    => t('Replacement strings are: %macros', $macro_replacements),
    );
  }
  
  $form['donate'] = array(
    '#type' => 'fieldset',
    '#title' => 'DONATE',
    '#collapsible' => FALSE
  );
  
  $donate_text .= '<div style="border-style:solid; padding:5px"><b>If you like to support my work on RSVP for Drupal 6, please consider donating. A paypal link can be found at the <a href="http://drupal.org/project/rsvp#drupal6" target="_blank">rsvp project</a> page at http://drupal.org/project/rsvp#drupal6</b></div>';   
  
  $form['donate']['info'] = array(
    '#type' => 'markup',
    '#value' => $donate_text,
    '#weight' => -1,
  );

  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 25,
    '#name' => 'op'
  );
  $form['reset_defaults'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 25,
    '#name' => 'op'
  );
  
  return $form;
}

/*
 * @ingroup rsvp_admin
 */
function rsvp_admin_settings_general_validate($form, &$form_state) {

  if ($form_state['values']['pick_from_address'] == '1') {
    if (!valid_email_address($form_state['values']['rsvp_from_address'])) {
      form_set_error('rsvp_from_address', t("The sender's email address you supplied is not valid."));
    }
  }
}

/*
 * @ingroup rsvp_admin
 */
function rsvp_admin_settings_general_submit($form, &$form_state) {

  global $rsvp_mailer_ops;
	
  $op  = $form_state['values']['op'];
	
  if ($op == t('Save')) {
  
	  $pick = $form_state['values']['pick_from_address'];
	  $email = $form_state['values']['rsvp_from_address'];
	
	  //always store emailaddress, even if not used
	  variable_set('rsvp_from_address', array($pick => $email));
	
	  variable_set('rsvp_for_expired_event', $form_state['values']['rsvp_for_expired_event']);

    foreach ($rsvp_mailer_ops as $op => $op_name) {
    	variable_set("rsvp_default_mailer_{$op}_subject", $form_state['values']["rsvp_mailer_{$op}_subject"]);
      variable_set("rsvp_default_mailer_{$op}_message", $form_state['values']["rsvp_mailer_{$op}_message"]);
    }
	  
	  
	  drupal_set_message('Settings have been stored.');
  }
  elseif ($op == t('Reset to defaults')) {
    
    variable_del('rsvp_from_address');
    variable_del('rsvp_for_expired_event');
    
    foreach ($rsvp_mailer_ops as $op => $op_name) {
      variable_del("rsvp_default_mailer_{$op}_subject");
      variable_del("rsvp_default_mailer_{$op}_message");
    }
  	
  	drupal_set_message('General settings have been reset to their default values.');
  }
  
  
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Menu callback: rsvp admin set default values for invitations.
 *
 * @see rsvp_admin_settings_default_submit()
 * @ingroup rsvp_admin
 */

function rsvp_admin_settings_default(&$form_state) {
  
  //create a rsvp and use it to set the default values.
  $rsvp = rsvp_function_initialize_default_rsvp();
	
	$form = array();
  
  $form[] = array(
    '#value' => t('Please set default values for the following advanced options.<br />'),
  );

  
  $form['rsvp_field_6'] = array(
    '#type' => 'fieldset',
    '#title' => t('Invitation style'),
  );
  
  $form['rsvp_field_6']['theme'] = rsvp_form_element_theme($rsvp);
  $form['rsvp_field_6']['stylesheet'] = rsvp_form_element_stylesheet($rsvp->theme, $rsvp->stylesheet);
  $form['rsvp_field_6']['rsvp_addedit_form']['image'] = rsvp_form_element_image($rsvp->image);
  $form['rsvp_field_6']['rsvp_addedit_form']['backgroundimage'] = rsvp_form_element_backgroundimage($rsvp->backgroundimage);
  $form['rsvp_field_6']['iconset'] = rsvp_form_element_iconset($rsvp->iconset);
  
  $form['rsvp_view_roles'] = rsvp_form_element_rsvp_view_roles($rsvp);

  $form['open_invitation'] = rsvp_form_element_open_invitation($rsvp);
  $form['max_guests'] = rsvp_form_element_max_guests($rsvp);
        
  $form['rsvp_field_5'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification options'),
  );
    
  $form['rsvp_field_5']['send_conf_owner'] = rsvp_form_element_send_conf_owner($rsvp);
  $form['rsvp_field_5']['send_conf_guest'] = rsvp_form_element_send_conf_guest($rsvp);
  $form['rsvp_field_5']['send_privatemsg'] = rsvp_form_element_send_privatemsg($rsvp);
  
  $form['rsvp_field_2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Guest list options'),
  );

  $form['rsvp_field_2']['response_blind'] = rsvp_form_element_response_blind($rsvp);  
  $form['rsvp_field_2']['attendees_visible'] = rsvp_form_element_attendees_visible($rsvp);
  $form['rsvp_field_2']['response_blind_node'] = rsvp_form_element_response_blind_node($rsvp);
  $form['rsvp_field_2']['response_view_roles'] = rsvp_form_element_response_view_roles($rsvp);
  $form['rsvp_field_2']['text_whoiscoming'] = rsvp_form_element_text_whoiscoming($rsvp);
  $form['rsvp_field_2']['text_yes'] = rsvp_form_element_text_yes($rsvp);
  $form['rsvp_field_2']['text_no'] = rsvp_form_element_text_no($rsvp);
  $form['rsvp_field_2']['text_maybe'] = rsvp_form_element_text_maybe($rsvp);
  
  
  $form['rsvp_field_3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Guest options'),
  );
  
  $form['rsvp_field_3']['list_email'] = rsvp_form_element_list_email($rsvp);
  $form['rsvp_field_3']['allow_invite'] = rsvp_form_element_allow_invite($rsvp);

  
  $form['rsvp_field_4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reply options'),
  );
  
  $form['rsvp_field_4']['disable_maybe'] = rsvp_form_element_disable_maybe($rsvp);
  $form['rsvp_field_4']['reply_blind_node'] = rsvp_form_element_reply_blind_node($rsvp);
  
  
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 25,
  );
  
  return $form;
}

/*
 * @ingroup rsvp_admin
 */
function rsvp_admin_settings_default_submit($form, &$form_state) {

  //array_diff removes all '0' values from the returned selection 
  $vals_response_view_roles = implode(";", array_diff($form_state['values']['response_view_roles'], array('0')));

  //array_diff removes all '0' values from the returned selection 
  $vals_rsvp_view_roles = implode(";", array_diff($form_state['values']['rsvp_view_roles'], array('0')));
	
  
  foreach (array('max_guests', 'open_invitation', 'disable_maybe', 'send_privatemsg', 
                 'send_conf_guest', 'send_conf_owner',
                 'response_blind', 'response_blind_node', 
                 'reply_blind_node', 'attendees_visible', 'list_email', 'allow_invite', 
                 'text_whoiscoming', 'text_yes', 'text_no', 'text_maybe', 'theme', 'stylesheet', 'iconset', 'backgroundimage', 'image') as $key) {

    variable_set('rsvp_default_' . $key, $form_state['values'][$key]);
  }
  
  variable_set('rsvp_default_response_view_roles', $vals_response_view_roles);
  variable_set('rsvp_default_rsvp_view_roles', $vals_rsvp_view_roles);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Create the FAPI form element for the rsvp date field.
 *
 * @param $type
 *   The node type to generate the form element for.
 *
 * @return
 *   FAPI form array for the rsvp date field element.
 *
 * @see _rsvp_date_field_form_alter()
 * @see _rsvp_date_alter_node_type_form()
 */
function _rsvp_date_field_element($type, $field) {

  return array(
    '#type' => 'select',
    '#title' => t('Date field to use for Invitations'),
    '#options' => _rsvp_get_date_field_options($type, $field),
    '#default_value' => empty($field) ? 0 : $field['field_name'],
    '#description' => t('Select a date field of this content type to use for invitations. Select "%remove" to disable an existing association.', array('%remove' => t('<remove association>'))),
  );
}

/**
 * Helper function for the date field select to build its options.
 *
 * @param $type
 *   Content type whose date fields should be listed.
 *
 * @return
 *   Associative array with all date fields of the given content type plus
 *   'None' or 'Remove association'.
 */
function _rsvp_get_date_field_options($type, $field) {
  
  $options = array();
  
  // Add "None" if the user never selected a field.
  if (empty($field)) {
      $options = array(0 => t('<None>'));
  }
  else {
      $options = array(0 => t('<Remove association>'));
  }
  // Add any date fields from this node type.
  $options += _rsvp_field_names($type);

  return $options;
}

/**
 * Alters the form for administrator settings per node type.
 * (admin/content/types)
 */
function rsvp_form_node_type_form_alter(&$form, &$form_state) {

  $form['rsvp'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSVP settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['rsvp']['connector'] = array(
    '#type' => 'item',
    '#title' => t('Selected connector'),
    '#value' => t(variable_get(RSVP_VAR_CONNECTOR, NULL)),
  );
  
  // Determine if the logged-in user 
  //1) has admin permissions and 
  //2) the current connector supports content-type associations or not
  $connector = new RsvpConnector();
  
  if (!$connector->isTypesAreSelectable() || 
     (user_access(RSVP_PERM_ADMIN) ==  false))
    return;
  
  $type = $form['old_type']['#value'];
  
  $field = $connector->get_event_field($type);
  
  $form['rsvp']['rsvp_node_datefields'] = _rsvp_date_field_element($type, $field);
  $form['#submit'][] = '_rsvp_date_field_form_submit';
  
}

/**
 * Alter the form for configuring CCK date fields on node types.
 *
 * Hooks into CCK Date fields to provide an option to use the current
 * field as the RSVP date field.
 *
 */
function rsvp_form_content_field_edit_form_alter(&$form, &$form_state) {

  $form['rsvp'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSVP settings'),
    '#collapsible' => TRUE,
    '#weight' => 1,
  );

  $form['rsvp']['connector'] = array(
    '#type' => 'item',
    '#title' => t('Selected connector'),
    '#value' => t(variable_get(RSVP_VAR_CONNECTOR, NULL)),
  );
  
  // Determine if the logged-in user 
  //1) has admin permissions and 
  //2) the current connector supports content-type associations or not
  $connector = new RsvpConnector();
  
  if (!$connector->isTypesAreSelectable() || 
     (user_access(RSVP_PERM_ADMIN) ==  false))
    return;
  
  $type = $form['type_name']['#value'];

  if (in_array($form['#field']['type'], array('date', 'datestamp', 'datetime'))) {

    $field = $connector->get_event_field($type);
    
    $form['rsvp']['rsvp_node_datefields'] = _rsvp_date_field_element($type, $field);
    $form['#submit'][] = '_rsvp_date_field_form_submit';
    // Make sure the submit button comes after the rsvp settings fieldset.
    $form['submit']['#weight'] = 50;
  }
}

/**
 * Custom submit handler for the CCK date field editing form.
 *
 * @see _rsvp_date_field_form_alter()
 */
function _rsvp_date_field_form_submit($form, &$form_state) {
  $type = $form_state['values']['type'];
  $field = $form_state['values']['rsvp_node_datefields'];
  
  $connector = new RsvpConnector();
  
  if (empty($field)) {
    $connector->disable_event($type);
  }
  else {
    $connector->enable_event($type, $field);
  }
}


function _rsvp_field_names($content_type = NULL) {
  $fields = array();
  $content_type_info = _content_type_info();
  if ($content_type_info['content types'][$content_type]) {
    foreach ($content_type_info['content types'][$content_type]['fields'] as $field) {
      if (in_array($field['type'], array('date', 'datestamp', 'datetime'))) {
        $fields[$field['field_name']] = $field['widget']['label'];
      }
    }
  }
  return $fields;
}


