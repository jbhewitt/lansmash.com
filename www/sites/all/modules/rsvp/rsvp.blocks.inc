<?php
// $Id: rsvp.blocks.inc,v 1.2.2.3 2009/03/07 22:00:26 ulf1 Exp $

/**
 * @module rsvp_blocks
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides block functionality
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

// Pre Loading files that will be required in this module
foreach (array('functions') as $file) {
  module_load_include("inc", "rsvp", "rsvp.{$file}");
}


/**
* Generate HTML for the rsvp blocks
* @param op the operation from the URL
* @param delta offset
* @returns block HTML
*/
function rsvp_block($op = 'list', $delta = 0, $edit = array()) {
 
// listing of blocks, such as on the admin/block page
  if ($op == "list") {
     $block[0]["info"] = t('RSVP Host toolbox');
     $block[0]['cache'] = BLOCK_NO_CACHE;
     $block[1]["info"] = t('RSVP Guest toolbox');
     $block[1]['cache'] = BLOCK_NO_CACHE;
     $block[2]["info"] = t('RSVP Guest list');
     $block[2]['cache'] = BLOCK_NO_CACHE;
     return $block;
  } else if ($op == 'view') {  
    switch ($delta) {
      case 0:
        return rsvp_block_toolbox_host();
      case 1:
        return rsvp_block_toolbox_guest();
      case 2:
        return rsvp_block_guestlist(variable_get('rsvp_block_d_'. $delta, 0));
    }
  } else if ($op == 'configure') {  
    if ($delta == 2) {
      $form['rsvp_block_d_'. $delta] = array(
        '#type' => 'radios',
        '#title' => t('Display'),
        '#options' => array('1' => t('Overview and Guest list'), '0' => t('Overview only')),
        '#description' => t("Do you want to display the short form or the lon form of teh guest list?"),
        '#default_value' => variable_get('rsvp_block_d_'. $delta, 0),
      );  
    	
    }
    return $form;
  } else if ($op == 'save') {  
    if ($delta == 2) {
      variable_set('rsvp_block_d_'. $delta, $edit['rsvp_block_d_'. $delta]);
    }
  }  
}

function rsvp_block_toolbox_host() {
  
	$block = array();
	$is_rsvp = false;
  $rid = NULL;
	
	if ( arg(0) =='rsvp') {
    if ( arg(1) =='email') {
      $invite_hash = arg(2);
    	$invite_target = rsvp_function_load_invitation_hash($invite_hash);
      if ($invite_target != false) {
      	$rid = $invite_target->rid;
      	$is_rsvp = true; 
      }
    }
    else{
      $rid = arg(1);
      $is_rsvp = is_numeric($rid);
    }
	}

  if ( $is_rsvp && $rid ) {
    $connector = new RsvpConnector();

    $rsvp=rsvp_function_load_rsvp($rid);
    if ($rsvp == false)
      return $block; 
    
    $perm_edit = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_EDIT, NULL);
    
    if ($perm_edit == true) {
      $links[] = l(t('Send a message to guests'), 'rsvp/'. $rsvp->rid .'/message', array('attributes' => array('title' => t('Send message to guests that you invited.'))));
	
      $links[] = l(t('Remove guests'), 'rsvp/'. $rsvp->rid .'/attendees', array('attributes' => array('title' => t('Remove guests from the invitation.'))));
      $links[] = l(t('Add guests'), 'rsvp/'. $rsvp->rid .'/attendees', array('attributes' => array('title' => t('Add more guests to the invitation.'))));
      if ($rsvp->send_conf_owner != RSVP_OPTION_YES)
        $links[] = l(t('Email me when guests reply'), 'rsvp/'. $rsvp->rid .'/msg_moderator', array('attributes' => array('title' => t('Enable notification messages for the event organizer(host).'))));

      $links[] = l(t('Export guest list'), 'rsvp/'. $rsvp->rid .'/attendees/csv', array('attributes' => array('title' => t('Export guest list into csv format.'))));

      $links[] = l(t('Edit guests replies'), 'rsvp/'. $rsvp->rid .'/attendees', array('attributes' => array('title' => t('Modify guest responses.'))));
      //$links[] = t('Cancel invitation');
      $links[] = l(t('Change organizer'), 'rsvp/'. $rsvp->rid .'/owner', array('attributes' => array('title' => t('Modify the event organizer(host).'))));
      $links[] = l(t('Change options'), 'rsvp/'. $rsvp->rid .'/edit', array('attributes' => array('title' => t('Modify the options of the current invitation.'))));
    }
    
    if (count($links) > 0) {
      $block['content'] = theme('item_list', $links);
      $block['subject'] = t('Host toolbox');
    }
  }
  
  return $block;
}

function rsvp_block_toolbox_guest() {

  $block = array();
  
  $invite_hash = arg(2);
  $is_invite_hash = (arg(0)=='rsvp') && (arg(1)=='email') && isset($invite_hash);

  if ( $is_invite_hash && $invite_hash ) {
    $connector = new RsvpConnector();

    $invite_target = rsvp_function_load_invitation_hash($invite_hash);
    if ($invite_target == false)
      return $block; 

    $rsvp=rsvp_function_load_rsvp($invite_target->rid);
    if ($rsvp == false)
      return $block; 
        
      
    list($perm_reply, $perm_guest) = rsvp_function_invitation_by_invite_accessarray($connector, $rsvp, $invite_target, NULL, array(RSVP_ACCESS_REPLY, RSVP_ACCESS_GUEST), NULL);

    // show only content for the toolbox only if we are the guest itself.
    if ($perm_guest == true) 
    {
      $perm_export = rsvp_csv_hash_form_access($connector, $rsvp, $invite_target);
      $perm_sendmessage = rsvp_hash_sendmessage_access($connector, $invite_target);
      $perm_addguests = rsvp_attendee_form_access($connector, $rsvp, $invite_target);

      if ($perm_sendmessage == true)
        $links[] = l(t('Send a message to guests'), 'rsvp/email/'. $invite_target->hash .'/message', array('attributes' => array('title' => t('Send message to guests that you invited.'))));
 
      if ($perm_addguests == true) { 
        $links[] = l(t('Add guests'), 'rsvp/email/'. $invite_target->hash .'/attendees', array('attributes' => array('title' => t('Add more guests to the invitation.'))));
      }
    
      if ($perm_export == true) 
        $links[] = l(t('Export guest list'), 'rsvp/email/'. $invite_target->hash .'/csv', array('attributes' => array('title' => t('Export guest list into csv format.'))));

      if ($perm_guest == true) 
        $links[] = l(t('Change your display name'), 'rsvp/email/'. $invite_target->hash .'/realname', array('attributes' => array('title' => t('Change your name that is being displayed in the guest list.'))));
      
      if (count($links) > 0) {
        $block['content'] = theme('item_list', $links);
        $block['subject'] = t('Your invitation');
      }
    }
    
  }
  return $block;
}


function rsvp_block_guestlist($extended_display = false) {
  $block = array();
  
  $is_invite_hash = false;
  $is_rsvp = false;
  $is_node = false;
  $uid = NULL;
  $rid = NULL;
  $nid = NULL;
  $content = '';
  
  
  if(arg(0)=='rsvp') {
  	
  	if (arg(1)=='email') {
      $invite_hash = arg(2);
  		$is_invite_hash = isset($invite_hash);
  	}
    else {
      $rid = arg(1);
      $is_rsvp = is_numeric($rid);
      
      if (!user_is_anonymous()) {  
        global $user;
        $uid = $user->uid;
      }
    }   
  }
  elseif(arg(0)=='node') {
    $nid = arg(1);
    $is_node = is_numeric($nid);
  }
  else {
  	return $block;
  }
  
  
  if ( $is_invite_hash ) {
    $invite_target = rsvp_function_load_invitation_hash($invite_hash);
    if ($invite_target == false)
      return $block; 

    $rsvp=rsvp_function_load_rsvp($invite_target->rid);
    if ($rsvp == false)
      return $block; 

    $perm_view = rsvp_function_invitation_by_invite_access($connector, $rsvp, $invite_target, $uid, RSVP_ACCESS_VIEW, NULL);
    if ($perm_view == true) {
      $content = rsvp_show_guestlist($rsvp, $invite_target, true, $extended_display);

      $block['content'] = $content;
      $block['subject'] = t('Guest list');
    }
  }
  elseif ( $is_rsvp ) {
  
    $rsvp=rsvp_function_load_rsvp($rid);
    if ($rsvp == false)
      return $block; 

    $perm_view = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, $uid, RSVP_ACCESS_VIEW, NULL);
    if ($perm_view == true) {
      $content = rsvp_show_guestlist($rsvp, NULL, true, $extended_display);

      $block['content'] = $content;
      $block['subject'] = t('Guest list');
    }
  }
  elseif ( $is_node ) {
  	  $display = false;
      //In a first  step collect all invitations for the node that a user has view permissions to.
      $rsvps = rsvp_function_load_invitations_owner($nid, NULL, NULL);
      while ($rsvp = db_fetch_object($rsvps)) {     
        $perm_view = rsvp_function_invitation_by_invite_access($connector, $rsvp, NULL, NULL, RSVP_ACCESS_VIEW, NULL);
    
        if ($perm_view == true) {  
          $content .= rsvp_show_guestlist($rsvp, NULL, true, $extended_display);
          $display = true;
        }
      }
          
      if ($display == true) {
        $block['content'] = $content;
        $block['subject'] = t('Guest list');
      }
      
  }
  
  
  
  return $block;
}


