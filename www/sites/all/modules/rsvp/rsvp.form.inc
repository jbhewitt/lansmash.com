<?php

// $Id: rsvp.form.inc,v 1.3.2.5 2009/03/11 18:29:23 ulf1 Exp $

/**
 * @module rsvp_form
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides form display helper functions.
 * @author crunchywelch (welch@advomatic.com)
 * @author Omar Abdel-Wahab (owahab@gmail.com)
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////

define('RSVP_STYLESHEET_WRAPPER', 'edit-stylesheet-wrapper');

/**
* Displays the 'Invitation message - view permissions' form element to set $rsvp->rsvp_view_roles
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_rsvp_view_roles($rsvp, $field_attr = array()) {

  $role_query_result = db_query("SELECT rid,name FROM {role} order by name");
  $role_options = array();
  while ($role = db_fetch_object($role_query_result)) {
    $role_options[$role->rid] = $role->name;
  }
	
	return array_merge($field_attr, array (
    '#type' => 'checkboxes',
    '#title' => t('Invitation message - view permissions'),
    '#options' => $role_options,
    '#description' => t('Select which roles have permissions to view the invitation besides the guests.'),
    '#default_value' => explode(';', $rsvp->rsvp_view_roles),
  ));
  
}

/**
* Displays the 'Guest list - view permissions' form element to set $rsvp->response_view_roles
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_response_view_roles($rsvp, $field_attr = array()) {

  $role_query_result = db_query("SELECT rid,name FROM {role} order by name");
  $role_options = array();
  while ($role = db_fetch_object($role_query_result)) {
    $role_options[$role->rid] = $role->name;
  }

  return array_merge($field_attr, array (
    '#type' => 'checkboxes',
    '#title' => t('Guest list - view permissions'),
    '#options' => $role_options,
    '#description' => t('Select which roles have permissions to see the guest list besides the guests.'),
    '#default_value' => explode(';', $rsvp->response_view_roles),
  ));
}


/**
* Displays the 'Display guest list' form element to set $rsvp->response_blind
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_response_blind($rsvp, $field_attr = array()) {

  $response_blind = array(RSVP_RESPONSE_SHOW => t('Show guest list'), 
                          RSVP_RESPONSE_HIDE => t('Hide guest list'), 
                          RSVP_RESPONSE_SHOWWHENREPLIED => t('Hide guest list until replied'));  

  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Display guest list'),
    '#options' => array_map('check_plain', $response_blind),
    '#default_value' => $rsvp->response_blind,
    '#description' => t('Select when guests are allowed to see the guest list.'),
  ));
}

/**
* Displays the 'Detail level' form element to set $rsvp->attendees_visible
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_attendees_visible($rsvp, $field_attr = array()) {

  $attendees_visible = array( RSVP_INVITEE_SHOW => t('Show invitees and responses'), 
                              RSVP_INVITEE_SHOWRESPONSEWHENREPLIED => t('Show invitees but hide their responses until replied'), 
                              RSVP_INVITEE_HIDE => t('Hide invitees'));  

  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Detail level'),
    '#options' => array_map('check_plain', $attendees_visible),
    '#default_value' => $rsvp->attendees_visible,
    '#description' => t('Select the detail level guests are allowed to see of the guest list. Field "Display guest list" has precedence.'),
  ));
}

/**
* Displays the 'Display guest list inside event/node' form element to set $rsvp->response_blind_node
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_response_blind_node($rsvp, $field_attr = array()) {

  $response_blind_node = array( RSVP_RESPONSE_SHOW => t('Show'), 
                                RSVP_RESPONSE_HIDE => t('Hide'));
                                  
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Display guest list inside event/node'),
    '#options' => array_map('check_plain', $response_blind_node),
    '#default_value' => $rsvp->response_blind_node,
    '#description' => t('Select when guests are allowed to see the guest list below each event(node). Fields "Display guest list" and "Detail level" have precedence.'),
  ));

}

/**
* Displays the 'Theme' form element to set $rsvp->theme
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_theme($rsvp, $field_attr = array()) {

	$theme_keys = array_keys(rsvp_function_getThemepathes());
	
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#required' => TRUE,
    '#options' => array_map('check_plain', array_comb($theme_keys, $theme_keys)),
    '#default_value' => check_plain($rsvp->theme),
    '#description' => t('Select the theme that you want to use for your invitation.'),
    '#ahah' => array(
      'path' => 'rsvp/js/stylesheet',
      'wrapper' => RSVP_STYLESHEET_WRAPPER,
      'effect' => 'fade',
      'method' => 'replace',
      )
  
  ));
}

/**
* Displays the 'Stylesheet' form element to set $rsvp->stylesheet
*
* @ingroup rsvp_form
* @param $theme The theme name instance to display.
* @param $stylesheet The stylesheet name instance to display.
* *  
* @return $form array.
*
*/
function rsvp_form_element_stylesheet($theme, $stylesheet, $field_attr = array()) {

  $css_keys = array_keys(rsvp_function_getStylesheets($theme));
	
  $arr = array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#required' => TRUE,
    '#options' => array_map('check_plain', array_comb($css_keys, $css_keys)),
    '#default_value' => check_plain($stylesheet),
    '#description' => t('Select the style that you want to use with your invitation.'),
  
  ));
  
  $arr['#prefix'] .= '<div id="' . RSVP_STYLESHEET_WRAPPER . '">';
  $arr['#suffix'] = '</div>' . $arr['#suffix'];   
  
  return $arr;
}

/**
* Displays the 'Iconset' form element to set $rsvp->iconset
*
* @ingroup rsvp_form
* @param $iconsetname The iconset name to display.
* *  
* @return $form array.
*
*/
function rsvp_form_element_iconset($iconset, $field_attr = array()) {

  $iconset_keys = array_keys(rsvp_function_getIconsets());
  
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Icons'),
    '#required' => TRUE,
    '#options' => array_map('check_plain', array_comb($iconset_keys, $iconset_keys)),
    '#default_value' => check_plain($iconset),
    '#description' => t('Select the icons that you want to use with your invitation.'),
  ));
}

/**
* Displays the 'Backgroundimage' form element to set $rsvp->backgroundimage
*
* @ingroup rsvp_form
* @param $backgroundimagename The image name to display.
* *  
* @return $form array.
*
*/
function rsvp_form_element_backgroundimage($backgroundimagename, $field_attr = array()) {

  $images_keys = array_keys(rsvp_function_getBackgroundImages());
  $images_keys = array_merge(array('none' => '-none-'), $images_keys);
  
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Background image'),
    '#required' => TRUE,
    '#options' => array_map('check_plain', (!empty($images_keys)) ? array_comb($images_keys, $images_keys) : array()),
    '#default_value' => check_plain($backgroundimagename),
    '#description' => t('Select the background image that you want to use for your invitation.'),
  ));
}


/**
* Displays the 'Image' form element to set $rsvp->image
*
* @ingroup rsvp_form
* @param $imagename The image name to display.
* *  
* @return $form array.
*
*/
function rsvp_form_element_image($imagename, $field_attr = array()) {

  $images_keys = array_keys(rsvp_function_getImages());
  $images_keys = array_merge(array('none' => '-none-'), $images_keys);
  
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Image'),
    '#required' => TRUE,
    '#options' => array_map('check_plain', (!empty($images_keys)) ? array_comb($images_keys, $images_keys) : array()),
    '#default_value' => check_plain($imagename),
    '#description' => t('Select the image that you want to use for your invitation.'),
  ));
}


/**
* Displays the 'Responses' form element to set $rsvp->text_whoiscoming
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_text_whoiscoming($rsvp, $field_attr = array()) {

  return array_merge($field_attr, array(
    '#type' => 'textfield',
    '#title' => t('Responses'),
    '#default_value' => $rsvp->text_whoiscoming,
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 128,
    '#description' => t('The header text shown for the guest list.'),
  ));

}

/**
* Displays the 'Yes' form element to set $rsvp->text_yes
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_text_yes($rsvp, $field_attr = array()) {

  return array_merge($field_attr, array(
    '#type' => 'textfield',
    '#title' => t('Yes'),
    '#default_value' => $rsvp->text_yes,
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 128,
    '#description' => t('Text displayed inside the guest list for guests that will attend.'),
  ));
}

/**
* Displays the 'No' form element to set $rsvp->text_no
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_text_no($rsvp, $field_attr = array()) {

  return array_merge($field_attr, array(
    '#type' => 'textfield',
    '#title' => t('No'),
    '#default_value' => $rsvp->text_no,
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 128,
    '#description' => t('Text displyed inside the guest list for guests that will not attend.'),
  ));
}

/**
* Displays the 'Maybe' form element to set $rsvp->text_maybe
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_text_maybe($rsvp, $field_attr = array()) {
  
  return array_merge($field_attr, array(
    '#type' => 'textfield',
    '#title' => t('Maybe'),
    '#default_value' => $rsvp->text_maybe,
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 128,
    '#description' => t('Text displyed inside the guest list for guests that might attend.'),
  ));
}  

/**
* Displays the 'Send messages' form element to set $rsvp->list_email
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_list_email($rsvp, $field_attr = array()) {

  $response_send = array( RSVP_SEND_NONE => t('No'), 
                          RSVP_SEND_INVITEE => t('Invitees'), 
                          RSVP_SEND_RESPONDENT => t('Respondents'), 
                          RSVP_SEND_ATTENDEE => t('Attendees'));  
  
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Send messages'),
    '#options' => array_map('check_plain', $response_send),
    '#default_value' => $rsvp->list_email,
    '#description' => t('Select which guests are allowed to send messages to other guests of the event.')
  ));
}


/**
* Displays the 'Invite others' form element to set $rsvp->allow_invite
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_allow_invite($rsvp, $field_attr = array()) {

  $response_invite = array( RSVP_INVITE_NONE => t('No'), 
                            RSVP_INVITE_INVITEE => t('Invitees'), 
                            RSVP_INVITE_RESPONDENT => t('Respondents'), 
                            RSVP_INVITE_ATTENDEE => t('Attendees'));
                              
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Invite other guests'),
    '#options' => array_map('check_plain', $response_invite),
    '#default_value' => $rsvp->allow_invite,
    '#description' => t('Select which guests are allowed to invite more guests to the event.')
  ));
}


/**
* Displays the 'Display reply box inside event/node' form element to set $rsvp->reply_blind_node
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_reply_blind_node($rsvp, $field_attr = array()) {
  
  $reply_blind_node = array(  RSVP_REPLY_SHOW => t('Show'), 
                              RSVP_REPLY_HIDE => t('Hide'));
                                
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Display reply box inside event/node'),
    '#options' => array_map('check_plain', $reply_blind_node),
    '#default_value' => $rsvp->reply_blind_node,
    '#description' => t('Select if guests are allowed to see the reply box below each event/node.'),
  ));
}

/////////////////////////
/**
* Displays the 'Send confirmation message to host when gues replies' form element to set $rsvp->send_conf_owner
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_send_conf_owner($rsvp, $field_attr = array()) {
  
  $send_confimation = array(  RSVP_OPTION_YES => t('Yes'), 
                              RSVP_OPTION_NO => t('No'));
                                
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Send confirmation message to organizer when guest replies'),
    '#options' => array_map('check_plain', $send_confimation),
    '#default_value' => $rsvp->send_conf_owner,
    '#description' => t(''),
  ));
}

/**
* Displays the 'Send confirmation message to guest when he replies' form element to set $rsvp->send_conf_guest
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_send_conf_guest($rsvp, $field_attr = array()) {
  
  $send_confimation = array(  RSVP_OPTION_YES => t('Yes'), 
                              RSVP_OPTION_NO => t('No'));
                                
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Send confirmation message to guest when he replies'),
    '#options' => array_map('check_plain', $send_confimation),
    '#default_value' => $rsvp->send_conf_guest,
    '#description' => t(''),
  ));
}

/**
* Displays the 'Send messages to registered guests by privatemsg' form element to set $rsvp->send_privatemsg
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_send_privatemsg($rsvp, $field_attr = array()) {
  
  $send_privatemsg = array(  RSVP_OPTION_YES => t('Yes'), 
                              RSVP_OPTION_NO => t('No'));
                                
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Send messages to registered guests by privatemsg if possible'),
    '#options' => array_map('check_plain', $send_privatemsg),
    '#default_value' => $rsvp->send_privatemsg,
    '#description' => t(''),
  ));
}

/**
* Displays the 'Answer \'Maybe\' disabled' form element to set $rsvp->disable_maybe
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_disable_maybe($rsvp, $field_attr = array()) {
  
  $disable_maybe = array(  RSVP_OPTION_YES => t('Hide'), 
                           RSVP_OPTION_NO => t('Show'));
                                
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Display answer option \'Maybe\''),
    '#options' => array_map('check_plain', $disable_maybe),
    '#default_value' => $rsvp->disable_maybe,
    '#description' => t('When guests reply to invitations, they may choose \'Yes\', \'No\', or \'Maybe\' as their response. If you do not wish to offer the \'Maybe\' option, set this option to \'Hide\''),
  
  
  ));
}

/**
* Displays the 'Open invitation' form element to set $rsvp->open_invitation
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_open_invitation($rsvp, $field_attr = array()) {
  
  $open_invitation = array(  RSVP_OPTION_YES => t('Yes'), 
                             RSVP_OPTION_NO => t('No'));
                                
  return array_merge($field_attr, array(
    '#type' => 'select',
    '#title' => t('Open invitation'),
    '#options' => array_map('check_plain', $open_invitation),
    '#default_value' => $rsvp->open_invitation,
    '#description' => t('Select if invitation is open to uninvited guests. If you select \'yes\', you have to select roles under \'Invitation message - view permissions\' to make the invitation visible to those uninvited guests to begin with. Only authenticated persons can signup to open invitations. Anonymous users are not permitted.'),
  ));
}

/**
* Displays the 'Maximum guests allowed to the event' form element to set $rsvp->max_guests
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_max_guests($rsvp, $field_attr = array()) {
  
  return array_merge($field_attr, array(
    '#type' => 'textfield',
    '#title' => t('Maximum guests allowed to the event'),
    '#default_value' => $rsvp->max_guests,
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('Set upper limit for number of attending guests. Set to 0 for unlimited.'),
  ));
}  

/**
* Displays the 'Reply start date' form element to set $rsvp->reply_startdate
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_reply_startdate($rsvp, $field_attr = array()) {

	$date_local = $rsvp->reply_startdate;
	return array_merge($field_attr, array(
    '#type' => 'date',
    '#title' => t('Reply start date'),
    '#default_value' => array('year' => (int) date('Y',$date_local ), 'month' => (int) date('m',$date_local ), 'day' => (int) date('d',$date_local ) ),
    '#description' => t('Specify the start date of a time window guests can replying to the invitation.'),
/*    '#description' => ' <br /> sd1:' . $rsvp->reply_startdate 
                       . ' <br /> sd2:' . date('r', $rsvp->reply_startdate)
	                     . ' <br /> sd3:' . format_date($rsvp->reply_startdate, 'custom', 'U')
                       . ' <br /> sd4:' . format_date($rsvp->reply_startdate, 'custom')
                       . ' <br /> sd5:' . format_date($rsvp->reply_startdate, 'custom', 'r')
                       . ' <br /> sd6:' . format_date($rsvp->reply_startdate, 'small', 'U')
	                     . ' <br /> sd7:' . format_date($rsvp->reply_startdate, 'small'),
*/	
	
	
	  '#required' => FALSE
  ));  

}  

/**
* Displays the 'Reply end date' form element to set $rsvp->reply_enddate
*
* @ingroup rsvp_form
* @param $rsvp The rsvp instance to display.
*  
* @return $form array.
*
*/
function rsvp_form_element_reply_enddate($rsvp, $field_attr = array()) {

  $date_local = $rsvp->reply_enddate;
	
  return array_merge($field_attr, array(
    '#type' => 'date',
    '#title' => t('Reply end date'),
    '#default_value' => array('year' => (int) date('Y', $date_local ), 'month' => (int) date('m', $date_local ), 'day' => (int) date('d', $date_local )),
    '#description' => t('Specify the end date of a time window guests can replying to the invitation.'),
    '#required' => FALSE
    ));  
    
}  


/**
 * AJAX callback to replace the stylesheet select options.
 *
 * This function is called when the theme has changed.  It updates the
 * cached form addedit and returns rendered output to be used to replace the select containing the stylesheets.
 *
 * @param $build_id
 *   The form's build_id.
 * @param $theme
 *   The selected theme name.
 * @return
 *   Prints the replacement HTML in JSON format.
 */
function rsvp_stylesheet_options_update() {
  $theme = $_POST['theme'];
  $stylesheet = $_POST['stylesheet'];
  if ($form = form_get_cache($_POST['form_build_id'], $form_state)) {

    // Validate the theme.
    if (isset($form['rsvp_addedit_form']['rsvp_field_6']['theme']['#options'][$theme])) {
    	
      // Get the new options and update the cache.
      $form['rsvp_addedit_form']['rsvp_field_6']['stylesheet'] = rsvp_form_element_stylesheet($theme, $stylesheet);
      
      form_set_cache($_POST['form_build_id'], $form, $form_state);

      // Build and render the new select element, then return it in JSON format.
      $form_state = array();
      $form['#post'] = array();
      $form = form_builder($form['form_id']['#value'] , $form, $form_state);
      $output = drupal_render($form['rsvp_addedit_form']['rsvp_field_6']['stylesheet']);
      drupal_json(array('status' => TRUE, 'data' => $output));
    }
    else {
      drupal_json(array('status' => FALSE, 'data' => ''));
    }
  }
  else {
    drupal_json(array('status' => FALSE, 'data' => ''));
  }
  exit();
}


/* array_combine is not available in php4. Therefore use our own function*/
function array_comb($arr1,$arr2) {
  $out = array();
  foreach($arr1 as $key1 => $value1)    {
    $out[$value1] = $arr2[$key1];
  }
  return $out;
} 


