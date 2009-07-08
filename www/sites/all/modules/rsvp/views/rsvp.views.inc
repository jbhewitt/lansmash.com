<?php
// $Id: rsvp.views.inc,v 1.1.2.7 2009/06/01 23:20:31 ulf1 Exp $

/**
 * @module rsvp
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides view support for rsvp.
 * @author crunchywelch (welch@advomatic.com)
 * @author Omar Abdel-Wahab (owahab@gmail.com)
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

/** 
* Declare rsvp tables to views.
* 
* @return array of tables.
 */
function rsvp_views_data() {

  //tables
  $data['rsvp']['table']['group'] = t('RSVP');
  
  // Advertise this table as a possible base table
  $data['rsvp']['table']['base'] = array(
    'field' => 'rid',
    'title' => t('Invitations'),
    'help' => t('Displaying invitations'),
  );
  
  $data['rsvp']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['rsvp_invite']['table']['group'] = t('RSVP');

  $data['rsvp_invite']['table']['join']['rsvp'] = array(
    'left_field' => 'rid',
    'field' => 'rid',
  );
  
  $data['rsvp_invite']['table']['join']['node'] = array(
    'left_table' => 'rsvp',
    'left_field' => 'rid',
    'field' => 'rid',
  );

  $data['users']['table']['join']['rsvp'] = array(
    'left_table' => 'rsvp_invite',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  

  $data['rsvp_realname']['table']['group'] = t('RSVP');

  $data['rsvp_realname']['table']['join']['rsvp'] = array(
    'left_table' => 'rsvp_invite',
    'left_field' => 'email',
    'field' => 'email',
  );
  
  
  //fields
  
  $data['rsvp']['rid'] = array(
    'title' => t('rid'),
    'help' => t('The id of the invitation.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_rid',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by invitation.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Invitation ids that guests belong to.'),
      'label' => t('Invitation Id'),
    ),
  );
  
  $data['rsvp']['uid'] = array(
    'title' => t('Organizer Id'),
    'help' => t('The user Id of the organizer who created the invitation.'),
    'field' => array(
      'handler' => 'views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('uid'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by user Id.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Ids of users that created invitations.'),
      'label' => t('Organizer Id'),
    ),
  );
  
  $data['rsvp']['nid'] = array(
    'title' => t('Node Id'),
    'help' => t('The node/event Id that the invitation is based on.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('nid'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by node/event Id.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Node/event ids that invitations have been created for.'),
      'label' => t('Node/Event Id'),
    ),
  );

  $data['rsvp']['name'] = array(
    'title' => t('Title'),
    'click sortable' => TRUE,
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_name',
      'help' => t('The subject(title) of the invitation'),
    ),
  );
  
  $data['rsvp']['timestamp'] = array(
    'title' => t('Last Update time'),
    'help' => t('Last time when invitation has been updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'help' => t('Date when invitation has been updated.'),
      'label' => t('Update time'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort by time of last update.'),
    ),
  );

  $data['rsvp']['startdate'] = array(
    'title' => t('Event date'),
    'help' => t('Date of invitation.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_startdate',
    ),
    'filter' => array(
      'handler' => 'rsvp_views_handler_filter_rsvp_startdate',
      'help' => t('Filter on startdate of invitation.'),
      'label' => t('Startdate'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort by time of last update.'),
    ),
  );
  
  $data['rsvp']['rsvp_user_is_organizer'] = array(
    'title' => t('Current user organizer'),
    'real field' => 'uid',
    'filter' => array(
      'handler' => 'rsvp_views_handler_filter_user_is_organizer',
      'help' => t('This allows you to filter by whether or not the logged in user is organizer of an invitation.'),
    ),
  );
  
/*  
  $data['rsvp']['total_rsvps'] = array(
    'title' => t(' Total Invitations'),
    'help' => t('Display total number of Invitations for the node.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_total_rsvps',
      'click sortable' => true,
    ),
//          'sort' => array(
//            'handler' => 'views_rsvp_sort_handler_total_rsvp',
//          'help' => t('Sort by the total RSVPs on a node'),
//        ),
        
  );  
*/  
  
  
  

  $data['rsvp_invite']['rid'] = array(
    'title' => t('Invitee rid'),
    'help' => t('The invitation (rsvp) this invitee belongs to.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_rid',
      'click sortable' => TRUE,
    ),
    
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'field' => 'rid', // the field to use.
      'name table' => 'rsvp', // the table to display in the summary.
      'name field' => 'name', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'rid',
    ),
/*
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Invitation ids (rsvp) this guest belongs to.'),
      'label' => t('Invitee rid'),
    ),
*/
  );
  
  $data['rsvp_invite']['uid'] = array(
    'title' => t('Invitee uid'),
    'field' => array(
      'handler' => 'views_handler_field_user_name',
      'help' => t('Uid of the user who received an invitation.'),
    ),
  );

  $data['rsvp_invite']['name'] = array(
    'title' => t('Invitee name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'help' => t('Name of the user who received an invitation.'),
    ),
  );

  $data['rsvp_invite']['comment'] = array(
    'title' => t('Invitee comment'),
    'field' => array(
      'handler' => 'views_handler_field',
      'help' => t('Comment of the user who received an invitation.'),
    ),
  );
  
  $data['rsvp_invite']['email'] = array(
    'title' => t('Invitee email address'),
    'click sortable' => TRUE,
    'field' => array(
      'handler' => 'views_handler_field_user_mail',
      'help' => t('Email address of the user who received an invitation.') .' <strong>'. t('WARNING: only expose this data in a view that is restricted to users whom you can trust with such sensitive information.') .'</strong>',
    ),
  );

  $data['rsvp_invite']['hash'] = array(
    'title' => t('Invitee hash'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_invite_hash_link',
      'help' => t('Hash of the user who received an invitation.'),
    ),
  );
  
  $data['rsvp_invite']['rsvp_user_invited'] = array(
    'title' => t('Current user invited'),
    'real field' => 'uid',
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'help' => t('This allows you to filter by whether or not the logged in user has been invited to an event.'),
    ),
  );

  $data['rsvp_invite']['rsvp_user_response'] = array(
    'title' => t('Invitee response'),
    'real field' => 'response',
    'field' => array(
      'handler' => 'views_handler_field',
      'help' => t('Response of the user who received an invitation.'),
    ),
    'filter' => array(
      'handler' => 'rsvp_views_handler_filter_user_response',
      'help' => t('This allows you to filter by the response the logged in user has replied to an event/node.'),
      'label' => t('Response:'),
    ),
  );

/*  
  $data['rsvp_invite']['total_invites'] = array(
    'title' => t('Total number of guests'),
    'help' => t('Display total number of guests for all invitations on the node.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_invite_total_invites',
      'click sortable' => true,
    ),
//          'sort' => array(
//            'help' => t('Sort by the total invites in all RSVPs on a node'),
//          ),

  );    
*/  

 
  return $data;
	
}

function rsvp_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'rsvp') .'/views',
    ),
    'handlers' => array(
      'rsvp_views_handler_filter_user_response' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'rsvp_views_handler_filter_rsvp_startdate' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'rsvp_views_handler_filter_user_is_organizer' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'rsvp_views_handler_field_rsvp_name' => array(
        'parent' => 'views_handler_field',
      ),
      'rsvp_views_handler_field_rsvp_startdate' => array(
        'parent' => 'views_handler_field_date',
      ),
      'rsvp_views_handler_field_rsvp_invite_hash_link' => array(
        'parent' => 'views_handler_field',
      ),
      'rsvp_views_handler_field_rsvp_rid' => array(
        'parent' => 'views_handler_field',
      ),
      
/*      'rsvp_views_handler_field_rsvp_total_rsvps' => array(
        'parent' => 'views_handler_field',
      ),
*/
            
/*      'rsvp_handler_field_rsvp_invite_total_invites' => array(
        'parent' => 'views_handler_field',
      ),
      'rsvp_handler_field_rsvp_invite_total_invites' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
*/    ),
  );
}

