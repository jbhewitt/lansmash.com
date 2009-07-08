<?php
// $Id: rsvp.mailer.inc,v 1.1.2.1 2009/02/27 21:03:54 ulf1 Exp $

/**
 * @module rsvp_mailer
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides functionality for mail templates.
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////


global $rsvp_mailer_ops;

$rsvp_mailer_ops = array('invitation' => 'Invitation', 'message_guest' => 'Guest messages', 'notification_guest' => 'Guest notification', 'notification_moderator' => 'Moderator notification', 'invitation_cancel' => 'Cancel Invitation');

/**
 * Replacements for mail messages
 * @param $rsvp The invitation object for which emails are being sent
 * @param $invite_target invitation object for which emails are being sent
 * @param $sender_name String the name of the sender
 * @param $message_subject String The subject line for the message
 * @param $message_body String The message that you want to send
 *  */
function rsvp_mailer_replacements($rsvp, $invite_target, $sender_name, $message_subject, $message_body) {

  $replacements = array(
    '@site'                     => variable_get('site_name', 'Drupal'),
    '@site_url'                 => $GLOBALS['base_url'],

    '@invitation_link'          => url("rsvp/email/{$invite_target->hash}/view", array('absolute'=>true)),
    '@event_link'               => url("node/{$rsvp->nid}", array('absolute'=>true)),

    '@sender_name'              => check_plain($sender_name),
    '@message_subject'          => check_plain($message_subject),
    '@message_body'             => drupal_html_to_text($message_body),
  );
  
  if (!is_null($invite_target)) {
    $replacements['@guest_name'] = rsvp_function_getGuestEmail($invite_target, true);
  }
  else {
    $replacements['@guest_name'] = '';
  }
  
  if (!is_null($rsvp)) {
  	$user = user_load($rsvp->uid);
    $replacements['@owner_name'] = check_plain($user->name);
    $replacements['@invitation_subject'] = check_plain($rsvp->name);
  }
  else {
    $replacements['@owner_name'] = '';
    $replacements['@invitation_subject'] = '';
  }
  
  
  return $replacements;
}





/**
 * Default invitation message
 */
function rsvp_mailer_invitation_default() {
  $subject = "[@site] Invitation - @invitation_subject";
  $message = <<<MESSAGE
Dear @guest_name,

You have been invited by @sender_name at @site.

You can view the full invitation, reply if you want to join or not, 
and add a comment by following this link:
@invitation_link
  
You can view the event by following this link:
@event_link

You also can visit the invitation by going to 'My account|Invitations'

Please do not reply to this email.

Regards,
@sender_name

***************************************************
@message_body
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}


/**
 * Default message_guest message
 */
function rsvp_mailer_message_guest_default() {
  $subject = "[@site] Invitation - @invitation_subject";
  $message = <<<MESSAGE
Dear @guest_name,

@sender_name at @site has sent you a message.

You can view the full invitation, reply if you want to join or not,
and add a comment by following this link:
@invitation_link
  
You can view the event by following this link:
@event_link

You also can visit the invitation by going to 'My account|Invitations'

Please do not reply to this email.

Regards,
@sender_name

***************************************************
@message_body
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}


/**
 * Default notification_guest message
 */
function rsvp_mailer_notification_guest_default() {
  $subject = "[@site] Invitation - @invitation_subject";
  $message = <<<MESSAGE
Dear @guest_name,

This email is a confirmation that you have changed your response for this invitation.

You can view the full invitation, reply if you want to join or not, 
and add a comment by following this link:
@invitation_link
  
You can view the event by following this link:
@event_link

You also can visit the invitation by going to 'My account|Invitations'

Please do not reply to this email.

Regards,
The @site team

***************************************************
@message_body
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}


/**
 * Default notification_moderator message
 */
function rsvp_mailer_notification_moderator_default() {
  $subject = "[@site] Invitation - @invitation_subject";
  $message = <<<MESSAGE
Dear Moderator,

This email is a confirmation that @guest_name at site @site has changed his response for this invitation.

You can view the full invitation, reply if you want to join or not, 
and add a comment by following this link:
@invitation_link
  
You can view the event by following this link:
@event_link

You also can visit the invitation by going to 'My account|Invitations'

Please do not reply to this email.

Regards,
The @site team

***************************************************
@message,_body
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}

/**
 * Default cancel request message
 */
function rsvp_mailer_invitation_cancel_default() {
  $subject = "[@site] Invitation - @invitation_subject";
	$message = <<<MESSAGE
Dear @guest_name,

@owner_name has canceled his invitation.

Regards,
@sender_name

MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}

