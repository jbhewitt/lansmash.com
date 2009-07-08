<?php
// $Id: rsvp.api.inc,v 1.2.2.4 2009/03/01 22:26:13 ulf1 Exp $

/**
 * @module rsvp_api
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides rsvp api functionality that can be called by other modules
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pre Loading files that will be required in this module
foreach (array('functions') as $file) {
  module_load_include("inc", "rsvp", "rsvp.{$file}");
}

/**
 * Creates an invitation with the default settings.
 * The function requires the correct user permissions.
 * 
 * @ingroup rsvp_api
 * 
 * @param $nid The Id of the node you want to create an invitation for, or the $node instance itself if available.
 * @param $invite text String The invitation message you want to use.
 * @param $startdate unixdate The startdate you want to write your invitation for (only required if the event has multiple startdates)
 * 
 * @return rid (id of the rsvp) or false.
 */
function rsvp_api_create_invitation($nid, $invite_text, $startdate) {

  if (is_numeric($nid)) {
    $node = node_load($nid);
    if ($node == false)
      return false;
  }
  else {
    $node = $nid;
  }
  
  $connector = new RsvpConnector();
  
  
  if ($connector->is_event_enabled($node->type)) {
  	
    $perm_create = rsvp_function_create_invitation_access($connector, $node, RSVP_ACCESS_CREATE);
    if ($perm_create == true) {
      $do_create = true;    
      $rsvp = rsvp_function_initialize_default_rsvp();
      $rsvp->nid = $node->nid;
      $rsvp->name = $node->title;
      $rsvp->invite_text = $invite_text;

      //set $rsvp->startdate
      if ($connector->hasMultipleDatesPerField()) {
        $field = $connector->get_event_field($node->type);
        $count = $connector->get_datecount($node, $field);
        if ($count == 1) {
          //if count = 1 ignore parameter startdate completely because there is only one startdate
          $rsvp->startdate = $connector->get_startdate($node, $field, 0);
        }
        else {
          $do_create = false;
          //iterate through all specified startdates (in case of repeating dates)
          for ($i = 0; $i < $count; $i++) {
            $key = $connector->get_startdate($node, $field, $i);

            if ($key == $startdate) {
              $rsvp->startdate = $key;
              $do_create = true;
              break;
            }
          }
        }
      }

      if (do_create == true) {
        $rid = rsvp_function_create_rsvp($rsvp); 
        if ($rid != false) {
          drupal_set_message(t('New invitation %rsvp_name(%rid) has been created successfully.'), array('%rsvp_name' => $rsvp->name, '%rid' => $rid));
          watchdog('action', 'Successfully created invitation %rsvp_name(%rid) for node %nid.', array('%rsvp_name' => $rsvp->name, '%rid' => $rid, '%nid' => $node->nid)); 
          return $rid; 
        }
      }
    }
  }
  
  return false;
}


/**
 * Removes invitation(s).
 * The function requires the correct user permissions.
 * 
 * @ingroup rsvp_api
 * 
 * @param $rid The Id of the rsvp you want to remove or NULL.
 * @param $nid The Id of the node you want to remove all invitations for or NULL.
 * 
 * @return true or false.
 */
function rsvp_api_remove_invitation($rid, $nid) {

	if(!is_null($rid)) {
    $rsvp = rsvp_load($rid);
    if ($rsvp == false)
      return false;
		
    $perm_del = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_DELETE);
    if ($perm_del == true) {
      $success = rsvp_function_delete_rsvp($rid);
      if ($success == true) {
        drupal_set_message(t('Invitation %rsvp_name(%rid) has been removed successfully.'), array('%rsvp_name' => $rsvp->name, '%rid' => $rsvp->rid));
      	watchdog('action', 'Removed invitation %rsvp_name(%rid) successfully.', array('%rsvp_name' => $rsvp->name, '%rid' => $rsvp->rid)); 
        return true;
      }
    }
	}
	elseif(!is_null($nid)) {
    $node = node_load($nid);
    if ($node == false)
      return false;
		
    $connector = new RsvpConnector();
      
    if ($connector->is_event_enabled($node->type)) {
      
      //find the invitation for a specific node
      $list = rsvp_function_load_invitations($node->nid);

      while ($rsvp = db_fetch_object($list)) {
      	
        $perm_del = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_DELETE);
        if ($perm_del == true) {
          $success = rsvp_function_delete_rsvp($rsvp->rid);
          if ($success == true) {
            drupal_set_message(t('Invitation %rsvp_name(%rid) has been removed successfully.'), array('%rsvp_name' => $rsvp->name, '%rid' => $rsvp->rid));
            watchdog('action', 'Removed invitation %rsvp_name(%rid) successfully.', array('%rsvp_name' => $rsvp->name, '%rid' => $rsvp->rid)); 
          }
        }
      }
    }
    return true;
	}
}

/**
 * Adds a number of guests to an invitation(rsvp) and sends out the invitations
 * 
 * The function requires the correct user permissions.
 * 
 * @ingroup rsvp_api
 * 
 * @param $rid The Id of the rsvp instance.
 * @param $hash The hash of the invitation object of the user who adds the invitees, 
 *        or NULL if the rsvp owner adds the invitees.
 * @param $guestsarray Stringarray Contains a mix of email addresses and drupal usernames.
 * @param $send_rsvp Boolean Also send the invitations or just add the guests to the invitation.
 */
function rsvp_api_add_guests($rid, $hash, $guestsarray, $send_rsvp) {
	
	$invite = NULL;
	
  if(!is_null($rid)) {
    $rsvp = rsvp_function_load_rsvp($rid);
    if ($rsvp == false)
      return false;
  }
  
  if(!is_null($hash)) {
    $invite = rsvp_function_load_invitation_hash($hash);
    if ($invite == false)
      return false;
  }
  
  $connector = new RsvpConnector();
  $perm_add = rsvp_add_guests_access($connector, $rsvp, $invite);
  if ($perm_add == true) {
    rsvp_function_add_invitees($rsvp, $invite, $guestsarray, $send_rsvp, $is_moderator);
    watchdog('action', 'Added invitees to invitation %rsvp_name(%rid) successfully.', array('%rsvp_name' => $rsvp->name, '%rid' => $rsvp->rid)); 
  }
}


/**
 * Remove a number of guests from an invitation(rsvp).
 * 
 * The function requires the correct user permissions.
 * 
 * @ingroup rsvp_api
 * 
 * @param $rid The Id of the rsvp instance.
 * @param $hash_caller The hash of the invitation object of the user who adds the invitees, 
 *        or NULL if the moderator adds the invitees.
 * @param $guestsarray Stringarray Contains a mix of email addresses, drupal usernames and hashes.
 */
function rsvp_api_remove_guests($rid, $hash_caller, $guestsarray) {

  if(!is_null($rid)) {
    $rsvp = rsvp_function_load_rsvp($rid);
    if ($rsvp == false)
      return false;
  }
  
  $connector = new RsvpConnector();
  $perm_remove = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_DELETE);
  if ($perm_remove == true) {
    rsvp_function_remove_guests($rsvp, $guestsarray);
    watchdog('action', 'Removed invitees from invitation %rsvp_name(%rid) successfully.', array('%rsvp_name' => $rsvp->name, '%rid' => $rsvp->rid)); 
  }
}


/**
 * Sends the invitation to all invitees of an rsvp instance.
 * 
 * The function requires the correct user permissions.
 *
 * @ingroup rsvp_api
 *
 * @param $rid The Id of the rsvp instance.
 * @param $hash_caller The hash of the invitation object of the user sending the invitation or NULL (if NULL, moderator sends invitation). 
 * @param $resend If true, sends to all guests even when received flag is already set. default: false.
 *
 */
function rsvp_api_send_invitations($rid, $hash_caller = NULL, $resend = FALSE) {

  $invite_caller = NULL;
	
  if(!is_null($rid)) {
    $rsvp = rsvp_function_load_rsvp($rid);
    if ($rsvp == false)
      return false;
  }
  
  if(!is_null($hash_caller)) {
    $invite_caller = rsvp_function_load_invitation_hash($hash_caller);
    if ($invite_caller == false)
      return false;
  }
  
  $connector = new RsvpConnector();
  
  $perm_edit = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_EDIT);
  if ($perm_edit == false)
    return false;
  
  rsvp_function_send_multiple_invitations($rsvp, $invite_caller, $resend);
}

/**
 * Send a message to selective guests of an invitation.
 *
 * The function requires the correct user permissions.
 * 
 * @ingroup rsvp_functions
 * 
 * @param $rid The Id of the rsvp instance.
 * @param $audience Const The audience for the message (RSVP_ATT_ALL,...).
 * @param $rsvp_mailer_op String One of the mailer operations in rsvp.mailer.inc.
 * @param $subject String The subject you want to send.
 * @param $body String The body you want to send.
 * @param $hash_caller The invitee hashcode of the person triggering the message or NULL if moderator. 
 *
 * @return string of formatted recipients, or empty string depending on $confirm.
 */
function rsvp_api_send_message($rid, $audience, $rsvp_mailer_op, $subject, $body, $hash_caller = NULL) {

  $invite_caller = NULL;
  
  if(!is_null($rid)) {
    $rsvp = rsvp_function_load_rsvp($rid);
    if ($rsvp == false)
      return false;
  }

  if(!is_null($hash_caller)) {
    $invite_caller = rsvp_function_load_invitation_hash($hash_caller);
    if ($invite_caller == false)
      return false;
  }

  $connector = new RsvpConnector();
  $perm_edit = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_EDIT);
  if ($perm_edit == false)
    return false;
  
  rsvp_function_send_message_guests($rsvp, $audience, $rsvp_mailer_op, $subject, $body, $invite_caller);
  
}