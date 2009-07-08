<?php
// $Id: rsvp.actions.inc,v 1.2.2.2 2009/02/07 01:10:30 ulf1 Exp $

/**
 * @module rsvp_action
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides rsvp actions that can be automatically called by other modules
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
* @ingroup rsvp_action
* 
* Implementation of hook_action_info()
*/
function rsvp_action_info() {
  
  return array(
      'rsvp_create_invitation_action' => array(
        'description' => t('RSVP: Create invitation'),
        'type' => 'node',
        'configurable' => FALSE,
        'hooks' => array(
          'nodeapi' => array('update', 'insert'),
        ),
      ),
      // the remove invitation action is not required for node removals because it is done automatically
      'rsvp_remove_invitation_action' => array(
        'description' => t('RSVP: Remove invitation'),
        'type' => 'node',
        'configurable' => true,
        'hooks' => array(
          'any' => true),
      ),
      'rsvp_add_guests_action' => array(
        'description' => t('RSVP: Add guests to invitation'),
        'type' => 'user',
        'configurable' => true,
        'hooks' => array(
          'user' => array('login', 'insert'),
        ),
      ),
      'rsvp_remove_guests_action' => array(
        'description' => t('RSVP: Remove guests from invitation'),
        'type' => 'user',
        'configurable' => true,
        'hooks' => array(
          'any' => true),
      ),
      'rsvp_send_invitations_action' => array(
        'description' => t('RSVP: Send invitation to guest'),
        'type' => 'user',
        'configurable' => true,
        'hooks' => array(
          'user' => array('login', 'insert'),
        ),
      ),
      'rsvp_send_message_action' => array(
        'description' => t('RSVP: Send message to guest'),
        'type' => 'node',
        'configurable' => true,
        'hooks' => array(
          'any' => true),
      ),

      
  );
}

 
  
/**
 * A nonconfigurable Drupal action. Creates invitation
 *   hook = nodeapi: Create RSVP for this node
 *
 *   Available contexts:
 *    $context['nid'] The node id you want to create an invitation for.
 *    $context['invite_text'] The invitation message you want to use.
 *    $context['startdate'] The startdate you want to write your invitation for (only required if you have events with multiple startdates).
 *
 */
function rsvp_create_invitation_action(&$node, $context = array()) {
  
	if (isset($node->nid)) {
      //pick always the first available startdate. What else do you want to do?
      $connector = new RsvpConnector();
	  
      if ($connector->hasMultipleDatesPerField()) {
        $field = rsvp_function_getFieldforContentType($node->type);
        $startdate = $connector->get_startdate($node, $field, 0);
      }
	  
	  
    rsvp_api_create_invitation($node, $node->body, $startdate);
	}
  elseif (isset($context['nid'])) {
    rsvp_api_create_invitation($context['nid'], $context['invite_text'], $context['startdate']);
  }

}


/**
 * A nonconfigurable Drupal action. Removes an invitation
 *   hook = any
 *
 *   Available contexts:
 *    $context['rid'] The invitation id you want to delete.
 *    or
 *    $context['nid'] The node id you want to delete the invitations for. (could potentially be multiple invitations)
 *
 */
function rsvp_remove_invitation_action(&$object, $context = array()) {
  
  if (($context['hook'] == 'nodeapi') || (isset($context['nid']))) {
    
  	if ($context['hook'] == 'nodeapi') {
  	  $node = $object;
      rsvp_api_remove_invitation(NULL, $node->nid);
  	}
  	else {
      rsvp_api_remove_invitation(NULL, $context['nid']);
  	}
  }
  elseif (isset($context['rid'])) {
    rsvp_api_remove_invitation($context['rid'], NULL);
	}
}

function rsvp_remove_invitation_action_form($context) {
  $form['rid'] = array(
    '#title' => t('Invitation ID'),
    '#type' => 'textfield',
    '#maxlength' => '10',
    '#description' => t('The ID of the invitation that you want to remove'),
    '#default_value' => $context['rid']
  );
  $form['nid'] = array(
    '#title' => t('Node ID'),
    '#type' => 'textfield',
    '#maxlength' => '10',
    '#description' => t('The ID of the node you want to remove all invitations for '),
    '#default_value' => $context['nid']
  );
  
  return $form;
}

function rsvp_remove_invitation_action_submit($form, $form_state) {
  return array('nid' => $form_state['values']['nid'], 'rid' => $form_state['values']['rid']);
}
  
  
/**
 * A nonconfigurable Drupal action. Adds guests to an invitation (host permissions) and send the invitation message
 *   hook = any
 * 
 *   Required contexts:
 *    $context['rid'] The invitation id you want to add guests to.
 *   Optional contexts: 
 *    $context['guestarray'] The array with guests that you want to add.
 *    $context['send_rsvp'] Boolean also send the invitations or just add the guests to the invitation.
 *
 */

function rsvp_add_guests_action(&$object, $context = array()) {

  if (!isset($context['rid'])) {
    return false;
  }
  
  $send_rsvp = ($context['send_rsvp'] == true);
  
  $rid = $context['rid'];
  
  $guestarray = $context['guestarray'];
  if (!isset($guestarray)) {
    $guestarray = array();
  }
  
  if ($context['hook'] == 'user') {
    $user = $object;
    
    $guestarray = array_merge($guestarray, $user->email);
  }

  rsvp_api_add_guests($rid, NULL, $guestarray, $send_rsvp);
  
}	

function rsvp_add_guests_action_form($context) {
  $form['rid'] = array(
    '#title' => t('Invitation ID'),
    '#type' => 'textfield',
    '#maxlength' => '10',
    '#description' => t('The ID of the invitation that you want to add the guest to.'),
    '#default_value' => $context['rid']
  );
  
  //calling the field 'recipients' will disable fck editor for this field. I hope that works for other editors as well. 
  $form['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => isset($context['guestarray']) ? $context['guestarray'] : '',
    '#required' => TRUE,
    '#rows' => '8',
    '#description' => t('Enter email addresses or !site usernames that you want to add as guests each in a separate line.', array('!site'=>variable_get("site_name", "drupal"))),
  );
  
  $send = array(1 => t('Yes'), 0 => t('No'));
  $form['send_rsvp'] = array(
    '#type' => 'radios',
    '#title' => t('Send invitations immediatly'),
    '#default_value' => isset($context['send_rsvp']) ? $context['send_rsvp'] : '0',
    '#options' => $send,
    '#required' => TRUE,
  );
  
  return $form;
}

function rsvp_add_guests_action_submit($form, $form_state) {
    $attendee_list = $form_state['values']['recipients'];
    $attendee_list = trim($attendee_list);
    //convert to array
    $guestsarray = explode("\r\n", $attendee_list);
    
    return array('rid' => $form_state['values']['rid'], 'send_rsvp' => $form_state['values']['send_rsvp'], 'guestarray' => $guestsarray);
    
}



/**
 * A nonconfigurable Drupal action. Remove guests from an invitation (host permissions)
 *   hook = any
 *
 *   Required contexts:
 *    $context['rid'] The invitation id you want to remove guests from.
 *   Optional contexts:
 *    $context['guestarray'] Stringarray Contains a mix of email addresses, drupal usernames and hash.
 *
 */

function rsvp_remove_guests_action(&$object, $context = array()) {

  if (!isset($context['rid'])) {
    return false;
  }
  
  $rid = $context['rid'];
  
  $guestarray = $context['guestarray'];
  if (!isset($guestarray)) {
    $guestarray = array();
  }
  
  if ($context['hook'] == 'user') {
    $user = $object;
    
    $guestarray = array_merge($guestarray, $user->email);
  }
  

  rsvp_api_remove_guests($rid, NULL, $guestarray);
} 


function rsvp_remove_guests_action_form($context) {
  $form['rid'] = array(
    '#title' => t('Invitation ID'),
    '#type' => 'textfield',
    '#maxlength' => '10',
    '#description' => t('The ID of the invitation that you want to remove the guests from.'),
    '#default_value' => $context['rid']
  );
  
  //calling the field 'recipients' will disable fck editor for this field. I hope that works for other editors as well. 
  $form['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => isset($context['guestarray']) ? $context['guestarray'] : '',
    '#required' => TRUE,
    '#rows' => '8',
    '#description' => t('Enter email addresses or !site usernames that you want to remove each in a separate line.', array('!site'=>variable_get("site_name", "drupal"))),
  );
  
  return $form;
}

function rsvp_remove_guests_action_submit($form, $form_state) {
    $attendee_list = $form_state['values']['recipients'];
    $attendee_list = trim($attendee_list);
    //convert to array
    $guestsarray = explode("\r\n", $attendee_list);
    
    return array('rid' => $form_state['values']['rid'], 'guestarray' => $guestsarray);
    
}





/**
 * A nonconfigurable Drupal action. Sends the invitation to all invitees of an rsvp instance (host permissions).
 *   hook = any
 *
 *   Required contexts:
 *    $context['rid'] The invitation id you want to send invitations to.
 *   Optional contexts:
 *    $context['resend'] Boolean If true, sends to all guests even when received flag is already set. default: false.
 *
 */

function rsvp_send_invitations_action(&$object, $context = array()) {

  if (!isset($context['rid'])) {
    return false;
  }
  
  $resend = ($context['resend'] == true);
  $rid = $context['rid'];
  
  rsvp_api_send_invitations($rid, NULL, $resend);
} 

function rsvp_send_invitations_action_form($context) {
  $form['rid'] = array(
    '#title' => t('Invitation ID'),
    '#type' => 'textfield',
    '#maxlength' => '10',
    '#description' => t('The ID of the invitation that you want to send invitations to.'),
    '#default_value' => $context['rid']
  );
  
  $send = array(1 => t('Yes'), 0 => t('No'));
  $form['resend'] = array(
    '#type' => 'radios',
    '#title' => t('Resend invitations to all, even if guests already received an invitation.'),
    '#default_value' => isset($context['resend']) ? $context['resend'] : '0',
    '#options' => $send,
    '#required' => TRUE,
  );
  
  return $form;
}

function rsvp_send_invitations_action_submit($form, $form_state) {
    
    return array('rid' => $form_state['values']['rid'], 'resend' => $form_state['values']['resend']);
    
}




/**
 * A nonconfigurable Drupal action. Send a message to selective guests of an invitation (host permissions).
 *   hook = any
 *
 *   Required contexts:
 *    $context['rid'] The invitation id you want to send a message to.
 *    $context['audience'] Const The audience for the message (RSVP_ATT_ALL,...)
 *    $context['subject'] String The subject you want to send.
 *    $context['body'] String The body you want to send.
 * 
 */

function rsvp_send_message_action(&$object, $context = array()) {

  
  if ((!isset($context['rid'])) || (!isset($context['audience'])) || (!isset($context['subject'])) || (!isset($context['body']))  ) {
    return false;
  }
	
  rsvp_api_send_message($context['rid'], $context['audience'], $context['subject'], $context['body'], NULL);
} 

function rsvp_send_message_action_form($context) {
  $form['rid'] = array(
    '#title' => t('Invitation ID'),
    '#type' => 'textfield',
    '#maxlength' => '10',
    '#description' => t('The ID of the invitation that you want to send messages to.'),
    '#default_value' => $context['rid']
  );
  $form['audience'] =  array(
    '#type' => 'select',
    '#title' => t('Message audience'),
    '#default_value' => RSVP_ATT_ALL,
    '#options' => array(RSVP_ATT_ALL => t('Entire guest list'), RSVP_ATT_YES => t('Guests who attend'), RSVP_ATT_NO =>  t('Guests who not attend'), RSVP_ATT_MAYBE =>  t('Guests who might attend'), RSVP_ATT_NONE =>  t('Guests who have not replied yet.')),
    '#description' => t('Select to which guests you like to send a message.'),
  );
  
  
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject'),
    '#default_value' => '',
    '#size' => 40,
    '#maxlength' => 40,
    '#description' => t('Enter the subject for your email message'),
  );

  //calling the field 'recipients' will disable fck editor for this field. I hope that works for other editors as well.   
  $form['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Message body'),
    '#default_value' => check_markup(''),
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('Enter the email message you like to send to the selected audience.'),
  );
    
  return $form;
}

function rsvp_send_message_action_submit($form, $form_state) {
    
    return array('rid' => $form_state['values']['rid'], 
                 'audience' => $form_state['values']['audience'], 
                 'subject' => $form_state['values']['subject'], 
                 'body' => $form_state['values']['body']); 
    
}


