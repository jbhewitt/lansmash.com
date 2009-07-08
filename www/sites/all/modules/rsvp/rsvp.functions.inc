<?php

// $Id: rsvp.functions.inc,v 1.3.2.12 2009/06/01 18:28:02 ulf1 Exp $

/**
 * @module rsvp_functions
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides rsvp internal functions that can be called by other rsvp functions
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * RSVP EMAIL ADDRESS USAGE
 */
define('RSVP_EMAIL_SITEADDRESS', 0);
define('RSVP_EMAIL_SETTINGS',    1);
define('RSVP_EMAIL_CREATOR',     2);

/**
 * RSVP ATTENDING OPTIONS (also stored in the database)
 */
define('RSVP_ATT_YES',    'yes');
define('RSVP_ATT_NO',     'no');
define('RSVP_ATT_MAYBE',  'maybe');
define('RSVP_ATT_NONE',   'none');
/**
 * ADDTL. RSVP ATTENDING OPTIONS (only used in memory)
 */
define('RSVP_ATT_ALL',    'all');
define('RSVP_ATT_NOTSEND','notsend');


define('RSVP_OPEN_INVITATION', 'open');


/**
 * RSVP RESPONSE OPTIONS
 */
define('RSVP_RESPONSE_HIDE',            0);
define('RSVP_RESPONSE_SHOW',            1);
define('RSVP_RESPONSE_SHOWWHENREPLIED', 2);

/**
 * RSVP REPLY OPTIONS
 */
define('RSVP_REPLY_HIDE', 0);
define('RSVP_REPLY_SHOW', 1);

/**
 * RSVP ATTENDEE_VISIBLE OPTIONS
 */
define('RSVP_INVITEE_HIDE', 0);
define('RSVP_INVITEE_SHOW', 1);
define('RSVP_INVITEE_SHOWRESPONSEWHENREPLIED', 2);

/**
 * RSVP SEND OPTIONS
 */
define('RSVP_SEND_NONE',        0);
define('RSVP_SEND_INVITEE',     1);
define('RSVP_SEND_RESPONDENT',  2);
define('RSVP_SEND_ATTENDEE',    3);

/**
 * RSVP INVITE OTHER OPTIONS
 */
define('RSVP_INVITE_NONE',        0);
define('RSVP_INVITE_INVITEE',     1);
define('RSVP_INVITE_RESPONDENT',  2);
define('RSVP_INVITE_ATTENDEE',    3);

/**
 * RSVP GENERAL OPTIONS
 */
define('RSVP_OPTION_NO',  0);
define('RSVP_OPTION_YES', 1);

/**
 * RSVP GUESTLIST SHOW/SORT OPTIONS
 */
define('RSVP_GUESTLIST_SORT_ALPHA', 'sa');
define('RSVP_GUESTLIST_SORT_DATE', 'sd');
define('RSVP_GUESTLIST_SHOW_ALL', 'all');
define('RSVP_GUESTLIST_SHOW_PART', 'part');

define('RSVP_GUESTLIST_SHOW_MAXITEMS', 10);

/**
 * PATH WHERE THE USER DEFINED THEMES ARE LOCATED
 */

define('RSVP_FILES_THEMES_PATH', 'rsvp/themes');
define('RSVP_FILES_ICONS_PATH', 'rsvp/icons');
define('RSVP_FILES_BACKGROUNDIMAGE_PATH', 'rsvp/backgrounds');
define('RSVP_FILES_IMAGE_PATH', 'rsvp/images');

define('RSVP_THEMES_PATH', 'themes');
define('RSVP_ICONS_PATH', 'icons');
define('RSVP_BACKGROUNDIMAGE_PATH', 'backgrounds');
define('RSVP_IMAGE_PATH', 'images');

define('RSVP_THEME_DEFAULT', 'default');
define('RSVP_STYLESHEET_DEFAULT', 'default');
define('RSVP_ICONSET_DEFAULT', 'flags');
define('RSVP_BACKGROUNDIMAGE_DEFAULT', 'none');
define('RSVP_IMAGE_DEFAULT', 'none');

/**
 * RSVP ACCESS OPTIONS
 */
define('RSVP_ACCESS_VIEW',      'v'); //maintainer, owner, co-moderator, guest, view by role permission
define('RSVP_ACCESS_REPLY',     'r'); //maintainer, owner, co-moderator, guest
define('RSVP_ACCESS_EDIT',      'e'); //maintainer, owner, co-moderator
define('RSVP_ACCESS_CREATE',    'c'); //maintainer, create permissions
define('RSVP_ACCESS_DELETE',    'd'); //maintainer, owner, co-moderator
define('RSVP_ACCESS_MAINTAIN',  'm'); //maintainer
define('RSVP_ACCESS_GUEST',     'g'); //maintainer, guest
define('RSVP_ACCESS_GUESTONLY', 'o'); //guest
define('RSVP_ACCESS_BYROLE',    'b'); //view by role permission


/**
 * RSVP PERMISSIONS
 */
define('RSVP_PERM_ADMIN',             'administer rsvp');
define('RSVP_PERM_MAINTAIN',          'maintain rsvp');
define('RSVP_PERM_ONEVENTS',          'rsvp on events');
define('RSVP_PERM_ONOWNEVENTS',       'rsvp on own events');
define('RSVP_PERM_INVITE_ROLE',       'rsvp invite by role');
define('RSVP_PERM_MULTI_INVITATIONS', 'rsvp multiple invitations per event');


/**
 * RSVP ERRORCODES
 */
define('RSVP_DELETED',                    true);
define('RSVP_ERROR_DELETE_REAL_NAME',     -1);
define('RSVP_ERROR_DELETE_USER_RECORDS',  -2);
define('RSVP_ERROR_DELETE_RSVP',          -3);


define('RSVP_VAR_CONTENT_TYPES', 'rsvp_content_types');
define('RSVP_VAR_CONNECTOR', 'rsvp_connector');


/**
 * RSVP SELECT Query statements that we use for all types of queries
 */
define('RSVP_QUERY_FROM_rsvp_invite', '{rsvp} r LEFT JOIN {rsvp_invite} u ON r.rid = u.rid LEFT JOIN {users} us ON u.uid = us.uid LEFT JOIN {rsvp_realname} n ON u.email = n.email '); 
define('RSVP_QUERY_FIELDS_rsvp_invite', 'r.nid,u.*,n.realname,us.name as username'); 

/**
* Creates a new invitation(rsvp).
* 
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $rsvp The rsvp object we want to create.
* @return int key id of the rsvp instance or false on failure.
*/

function rsvp_function_create_rsvp($rsvp) {
  global $user;

  $fields[] = 'timestamp';
  $vals[] = time();
  $markers[] = "%d";

  $fields[] = 'uid';
  $vals[] = $user->uid;
  $markers[] = "%d";
  
  foreach (array('nid', 'uid_moderator', 'reply_startdate', 'reply_enddate', 'max_guests', 'open_invitation', 'disable_maybe', 'send_privatemsg', 'send_conf_guest', 'send_conf_owner', 'invite_filterformat') as $key) {
    $fields[] = $key;
    $vals[] = $rsvp->$key;
    $markers[] = "%d";
  }
  
  foreach (array('name', 'startdate', 'theme', 'stylesheet', 'iconset', 'backgroundimage', 'image', 'rsvp_view_roles', 'response_view_roles', 'invite_text', 'response_blind', 'response_blind_node', 'reply_blind_node', 'attendees_visible', 'list_email', 'allow_invite', 'text_whoiscoming', 'text_yes', 'text_no', 'text_maybe') as $key) {
    $fields[] = $key;
    $vals[] = $rsvp->$key;
    $markers[] = "'%s'";
  }
  
  $sql = 'INSERT INTO {rsvp} ('. implode(", ", $fields) .') VALUES ('. implode(", ", $markers) .')';
  if (db_query($sql, $vals)) {
    $rsvp->rid = db_last_insert_id('rsvp', 'rid');
    
    return $rsvp->rid;
  }
  else {
    drupal_set_message(t('There was an error creating the invitation. Please try again'), 'error');
    return FALSE;
  }
}


/**
* Updates an invitation(rsvp)
* 
* Permission check needs to be done outside.
*
* @ingroup rsvp_functions
* @param $rsvp The rsvp object we want to update.
* @return boolean true on success.
*/

function rsvp_function_update_rsvp($rsvp) {
  
  $rid = $rsvp->rid;

  if (!$rid) {
    return false;
  }
  
  $fields[] = 'timestamp = %d';
  $vals[] = time();

  foreach (array('nid', 'uid', 'uid_moderator', 'reply_startdate', 'reply_enddate', 'max_guests', 'open_invitation', 'disable_maybe', 'send_privatemsg', 'send_conf_guest', 'send_conf_owner','invite_filterformat') as $key) {
    $fields[] = $key ." = %d";
    $vals[] = $rsvp->$key;
  }
  
  foreach (array('name', 'startdate', 'invite_text', 'theme', 'stylesheet', 'iconset', 'backgroundimage', 'image', 'rsvp_view_roles', 'response_view_roles', 'response_blind', 'response_blind_node', 'reply_blind_node', 'attendees_visible', 'list_email', 'allow_invite', 'text_whoiscoming', 'text_yes', 'text_no', 'text_maybe') as $key) {
    $fields[] = $key ." = '%s'";
    $vals[] = $rsvp->$key;
  }

  $vals[] = $rid;
  
  $sql = 'UPDATE {rsvp} SET '. implode(', ', $fields) .' WHERE rid = %d';

  if (db_query($sql, $vals)) {
    drupal_set_message(t('Invitation updated.'));
    return TRUE;
  }
  else {
    drupal_set_message(t('There was an error updating the invitation.'), 'error');
    return FALSE;
  }
}

/**
* Deletes an invitation(rsvp)
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $rid The rid of the rsvp to delete.
* @return boolean true on success.
*/
function rsvp_function_delete_rsvp($rid) {

  if (is_null($rid) || !is_numeric($rid)) { 
    drupal_set_message(t('Invitation %rid is not a valid type.', array('%rid' => $rid)), 'error');
    return RSVP_ERROR_DELETE_RSVP;
  }
    
  if (db_query('DELETE FROM {rsvp} WHERE rid = %d', $rid)) {
    if (db_query('DELETE FROM {rsvp_invite} WHERE rid = %d', $rid)) {
      return RSVP_DELETED;
    }
    else {
      return RSVP_ERROR_DELETE_USER_RECORDS;
    }
  }
  else {
    return RSVP_ERROR_DELETE_RSVP;
  }
}

/**
* Retrieves an invitation(rsvp) from the database.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $rid The rid of the rsvp instance.
* @return The $rsvp instance or false.
*/
function rsvp_function_load_rsvp($rid) {
	$rsvp = db_fetch_object(db_query('SELECT * FROM {rsvp} WHERE rid = %d', $rid));

  if ($rsvp == false) {
    drupal_set_message(t('The invitation you requested (rid=%rid) is invalid or has been deleted.', array('%rid' => $rid)));
  }  

  return $rsvp;
}

/**
* Retrieves a node from the database, based on a particular invitation.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $object The rsvp object that you want the node to load for or the $nid (numeric).
* @return The $node instance or false.
*/
function rsvp_function_load_node($object) {

	if (is_numeric($object)) {
    $node = node_load($object);
    if ($node == false) {
      drupal_set_message(t('The node you requested (nid=%nid) has been deleted.', array('%nid' => $object)));
    }
	}
  else {
  	//$object is a $rsvp instance
    $node = node_load($object->nid);
    if ($node == false) {
      drupal_set_message(t('The node you requested (nid=%nid) that belongs to invitation (rid=%rid) has been deleted.', array('%rid' => $object->rid, '%nid' => $object->nid)));
    }
  }
  
  return $node;
}

/**
* Returns a particular invitation that a user has received.
*
* @ingroup rsvp_functions
* @param $rid The rsvp id of the invitation.
* @param $uid The uid of the user. If NULL, use logged in users credentials
* @param $print Boolean print drupal message in case of error.
* * 
* @return invite object or false.
*/
function rsvp_function_load_invitation_rid($rid, $uid = NULL, $print = true) {

  $uid = _rsvp_function_check_uid($uid);	
	if (is_null($uid)) {
    return false;
	}
  //US orig $invite_target = db_fetch_object(db_query('SELECT * FROM {rsvp_invite} u LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.uid = %d AND u.rid = %d', $uid, $rid));
  $invite_target = db_fetch_object(db_query('SELECT ' . RSVP_QUERY_FIELDS_rsvp_invite . ' FROM  ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.uid = %d AND u.rid = %d', $uid, $rid));
  
  if (($invite_target == false) && ($print == true)) {
    drupal_set_message(t('The invitation (rid=%rid) that you (uid=%uid) requested is invalid or has been deleted.', array('%rid' => $rid, '%uid' => $uid)));
  }  

  return $invite_target;
}

/**
* Checks that the incoming $uid is not anonymous user.
*
* @ingroup rsvp_functions
* @param $uid The uid of the user. If NULL, use logged in users credentials. returns NULL if anonymous.
* 
* @return cleared $uid.
*/
function _rsvp_function_check_uid($uid) {
	if (is_null($uid) || ($uid == 0)) {
    if (user_is_anonymous())
      return NULL;
    
    global $user;
    $uid = $user->uid;
    if ($uid == 0)
      return NULL;
	}
	
	return $uid;
}

/**
* Returns an invite by hash value.
*
* @ingroup rsvp_functions
* @param $hash The hash value of the invitee.
* @return dbresultset of the requested invite or false.
*/
function rsvp_function_load_invitation_hash($hash) {

//US orig	$invite_target = db_fetch_object(db_query('SELECT u.*,n.realname,us.name as username FROM {rsvp} r LEFT JOIN {rsvp_invite} u ON r.rid = u.rid LEFT JOIN {users} us ON u.uid = us.uid LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.hash = \'%s\'', $hash)); 
  $invite_target = db_fetch_object(db_query('SELECT ' . RSVP_QUERY_FIELDS_rsvp_invite . ' FROM  ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.hash = \'%s\'', $hash)); 
	if ($invite_target == false) {
    drupal_set_message(t('The invitation (hash=%hash) that you requested is invalid or has been deleted.', array('%hash' => $hash)));
  }  

  return $invite_target;
	
}

/**
* Returns selective guests for an invitation based on the passed status
*
* @ingroup rsvp_functions
* @param $rid The rid of the rsvp instance.
* @param $status const of type (RSVP_ATT_ALL,...)
* @param $count Boolean. If true return an interger with the number of found elements. If false return elements.
* * 
* @return dbresultset of the requested attendees.
*/
function rsvp_function_load_guests($rid, $status = RSVP_ATT_ALL, $count = FALSE, $tablesort = ' ORDER BY u.email') {

	if ($status == RSVP_ATT_ALL || !isset($status))
    //US orig $result = db_query('SELECT '. ($count ? 'SUM(totalguests)' : 'u.*,n.realname,us.name as username') .' FROM {rsvp_invite} u LEFT JOIN {users} us ON u.uid = us.uid LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.rid = %d ' . $tablesort, $rid);
    $result = db_query('SELECT '. ($count ? 'SUM(totalguests)' : RSVP_QUERY_FIELDS_rsvp_invite) .' FROM ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.rid = %d ' . $tablesort, $rid);
  else
    //US orig $result = db_query('SELECT '. ($count ? 'SUM(totalguests)' : 'u.*,n.realname,us.name as username') .' FROM {rsvp_invite} u LEFT JOIN {users} us ON u.uid = us.uid LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.rid = %d AND response=\'%s\'' . $tablesort, $rid, $status);
    $result = db_query('SELECT '. ($count ? 'SUM(totalguests)' : RSVP_QUERY_FIELDS_rsvp_invite) .' FROM ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.rid = %d AND u.response=\'%s\'' . $tablesort, $rid, $status);
    
  if ($result == false) {
    drupal_set_message(t('The invitation you requested (rid=%rid) is invalid or has been deleted.', array('%rid' => $rid)));
  }  

  return $result;
}

/**
* Return totals for each type (RSVP_ATT_ALL,...) for an invitation
*
* @ingroup rsvp_functions
* @param $rid The rid of the rsvp instance.
* @return dbresultset of the requested attendees.
*/
function rsvp_function_load_totalguests($rid) {
	$skip = false;
	$responses = array(RSVP_ATT_NONE, RSVP_ATT_YES, RSVP_ATT_NO, RSVP_ATT_MAYBE);
  foreach ($responses as $response) {
    $total = db_result(db_query('SELECT SUM(totalguests) FROM {rsvp_invite} u WHERE u.rid = %d AND u.response = \'%s\'', $rid, $response));

     
    $totals[$response] = (int) $total;
    $totals[RSVP_ATT_ALL] += (int) $total;

  }
  return $totals;
}

/**
* Remove a guest from an invitation.
*
* @ingroup rsvp_functions
* @param $hash The hash value of the invitee.
* @return boolean true if successful.
*/
function rsvp_function_remove_guest($hash) {
	return db_query('DELETE FROM {rsvp_invite} WHERE hash = \'%s\'', $hash);
}

/**
* Checks if an invitation for a user (by email address) already exists and returns it.
*
* @ingroup rsvp_functions
* @param $rid The id of the rsvp instance.
* @param $email The email to check for.
* @return boolean false if the email don't exist for the rsvp, or the $invitation object.
*/
function rsvp_function_exists_guest_email($rid, $email) {
	//US orig return db_fetch_object(db_query('SELECT u.*,n.realname,us.name as username FROM {rsvp_invite} u LEFT JOIN {users} us ON u.uid = us.uid LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.rid = %d AND u.email = \'%s\'', $rid, $email));
  return db_fetch_object(db_query('SELECT ' . RSVP_QUERY_FIELDS_rsvp_invite . ' FROM  ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.rid = %d AND u.email = \'%s\'', $rid, $email));
}

/**
* Checks if an invitation for a user (by user id) already exists and returns it.
* *
* @ingroup rsvp_functions
* @param $rid The id of the rsvp instance.
* @param $uid The user uid to check for.
* @return boolean false if the email don't exist for the rsvp, or the $invitation object.
*/
function rsvp_function_exists_guest_uid($rid, $uid) {
	
  $uid = _rsvp_function_check_uid($uid);  
  if (is_null($uid)) {
    return false;
  }
  
	//US orig return db_fetch_object(db_query('SELECT u.*,n.realname FROM {rsvp_invite} u LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.rid = %d AND u.uid = %d', $rid, $uid));
  return db_fetch_object(db_query('SELECT ' . RSVP_QUERY_FIELDS_rsvp_invite . ' FROM  ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.rid = %d AND u.uid = %d', $rid, $uid));
}

/**
* Creates a new realname dataset(rsvp).
* 
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $invite_target The invite instance you want to create a realname entry for.
* @param $realname The real name you want to store.
* 
* @return true or false on failure.
*/

function rsvp_function_create_rsvp_realname($invite_target, $realname) {
  global $user;

  $fields[] = 'realname';
  $vals[] = $realname;
  $markers[] = "'%s'";

  $fields[] = 'email';
  $vals[] = $invite_target->email;
  $markers[] = "'%s'";
  

  $sql = 'INSERT INTO {rsvp_realname} ('. implode(", ", $fields) .') VALUES ('. implode(", ", $markers) .')';
  if (db_query($sql, $vals)) {
    return true;
  }
  else {
    drupal_set_message(t('There was an error creating the realname. Please try again'), 'error');
    return FALSE;
  }
}


/**
* Updates an rsvp_realname
* 
* Permission check needs to be done outside.
*
* @ingroup rsvp_functions
* @param $invite_target The invite instance you want to update the realname entry for.
* @param $realname The real name you want to store.
* @return boolean true on success.
*/

function rsvp_function_update_rsvp_realname($invite_target, $realname) {
  
  $fields[] = 'realname = \'%s\'';
  $vals[] = $realname;

  $vals[] = $invite_target->email;
  
  $sql = 'UPDATE {rsvp_realname} SET '. implode(', ', $fields) .' WHERE email = \'%s\'';
  
  if (db_query($sql, $vals)) {
    return TRUE;
  }
  else {
    drupal_set_message(t('There was an error updating the Real name.'), 'error');
    return FALSE;
  }
}

/**
* Deletes a rsvp_realname
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $invite_target The invite instance you want to update the realname entry for.
* @return boolean true on success.
*/
function rsvp_function_delete_rsvp_realname($invite_target) {
    
  if (db_query('DELETE FROM {rsvp_realname} WHERE email = \'%s\'', $invite_target->email)) {
    return RSVP_DELETED;
  }
  else {
    return RSVP_ERROR_DELETE_REAL_NAME;
  }
}

/**
* Checks if a rsvp_realname for an invite object (by email) already exists and returns it.
* *
* @ingroup rsvp_functions
* @param $invite_target The invite instance you want to update the realname entry for.
* @return boolean false if the realname don't exist for the invite object, or the realname object.
*/
function rsvp_function_exists_rsvp_realname($invite_target) {
  
  return db_fetch_object(db_query('SELECT * FROM {rsvp_realname} WHERE email = \'%s\'', $invite_target->email));
}




/**
 * Returns a new $rsvp Object with default settings.
 *
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 */

function rsvp_function_initialize_default_rsvp() {
  
  //create a rsvp and use it to set the default values.
  $rsvp = new stdClass();

  $rsvp->text_yes = variable_get('rsvp_default_text_yes', t('Attend'));
  $rsvp->text_no = variable_get('rsvp_default_text_no', t('Attend not'));
  $rsvp->text_maybe = variable_get('rsvp_default_text_maybe', t('Undecided'));
  $rsvp->text_whoiscoming = variable_get('rsvp_default_text_whoiscoming', t('Responses'));
  $rsvp->response_blind = variable_get('rsvp_default_response_blind', RSVP_RESPONSE_SHOW);
  $rsvp->response_blind_node = variable_get('rsvp_default_response_blind_node', RSVP_RESPONSE_HIDE);
  $rsvp->attendees_visible = variable_get('rsvp_default_attendees_visible', RSVP_INVITEE_HIDE);
  $rsvp->response_view_roles = variable_get('rsvp_default_response_view_roles', '');
  $rsvp->rsvp_view_roles = variable_get('rsvp_default_rsvp_view_roles','');
  $rsvp->reply_blind_node = variable_get('rsvp_default_reply_blind_node', RSVP_REPLY_HIDE);
  $rsvp->list_email = variable_get('rsvp_default_list_email', RSVP_SEND_NONE);
  $rsvp->allow_invite = variable_get('rsvp_default_allow_invite', RSVP_INVITE_NONE);

  $rsvp->send_conf_owner = variable_get('rsvp_default_send_conf_owner', RSVP_OPTION_NO);
  $rsvp->send_conf_guest = variable_get('rsvp_default_send_conf_guest', RSVP_OPTION_NO);
  $rsvp->send_privatemsg = variable_get('rsvp_default_send_privatemsg', RSVP_OPTION_NO);
  $rsvp->disable_maybe = variable_get('rsvp_default_disable_maybe', RSVP_OPTION_NO);
  $rsvp->open_invitation = variable_get('rsvp_default_open_invitation', RSVP_OPTION_NO);
  $rsvp->max_guests = variable_get('rsvp_default_max_guests', 0);
  $rsvp->theme = variable_get('rsvp_default_theme', RSVP_THEME_DEFAULT);
  $rsvp->stylesheet = variable_get('rsvp_default_stylesheet', RSVP_STYLESHEET_DEFAULT);
  $rsvp->iconset = variable_get('rsvp_default_iconset', RSVP_ICONSET_DEFAULT);
  $rsvp->backgroundimage = variable_get('rsvp_default_backgroundimage', RSVP_BACKGROUNDIMAGE_DEFAULT);
  $rsvp->image = variable_get('rsvp_default_image', RSVP_IMAGE_DEFAULT);
  
  $rsvp->invite_filterformat = FILTER_FORMAT_DEFAULT;
  
  return $rsvp; 
}


/**
 * Internal function
 * 
 * Adds a number of guests to an invitation(rsvp)
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 *
 * @param $rsvp The rsvp instance.
 * @param $invite_caller The invitation object of the user who adds the guests, 
 *        or NULL if the rsvp owner, co_owner or moderator adds the guests,
 *        or const RSVP_OPEN_INVITATION for an open invitation
 * 
 * @param $guestsarray Stringarray Contains a mix of email addresses and drupal usernames.
 * 
 * @return array with failed emailaddresses or false
 */
function _rsvp_function_add_guests($rsvp, $invite_caller, $guestsarray) {

  $emails = array();

	// remove duplicates from guestlist array
  $guestsarray = array_unique($guestsarray);

  $rid = $rsvp->rid;

  if (is_null($invite_caller))
    $owner_hash = '';
  elseif ($invite_caller == RSVP_OPEN_INVITATION)
    $owner_hash = RSVP_OPEN_INVITATION;
  else
    $owner_hash = $invite_caller->hash;
  
  
  foreach ($guestsarray as $key => $value) {

    $list_key = NULL;
    $invite_hash = NULL;
    
    // strip whitespace
    $string = trim($value);
    
    if (strlen($string)) {
      // attempt to deal with the string as a drupal username
      // we do this before dealing it as an e-mail to handle users like 
      // foo@somesite.com
      $usr = user_load(array('name' => $string));
      if ($usr === FALSE) {
        // attempt to find a system user having this string as an e-mail address
        $usr = user_load(array('mail' => $string));
        if ($usr === FALSE) {
          // no user found: carry on dealing with the string as an e-mail address
          if (valid_email_address($string)) {
            $invite_target = rsvp_function_exists_guest_email($rid, $string);
            if (!$invite_target) {
              // if this e-mail is not in the attendee list,
              // add it
              $hash = md5($rid . $string . time());
              if (db_query('INSERT INTO {rsvp_invite} (rid, inviteehash, email, hash, timestamp, response, comment, totalguests) VALUES (%d, \'%s\', \'%s\', \'%s\', %d, \'%s\', \'%s\', %d)', $rid, $owner_hash, $string, $hash, time(), 'none', '', 1)) {
                $invite_hash = $hash;
                $list_key = 'success_add';
              }
            }
            else {
              // the e-mail is already in the attendee list
              $invite_hash = $invite_target->hash;
              $list_key = 'existing';
            }
          }
          else {
            // the e-mail is invalid
            $invite_hash = NULL;
            $list_key = 'invalid';
          }
        }
      }

      if ($usr != FALSE) {
        // a valid drupal user was found, make sure his/her e-mail isn't already
        // in the attendees list
        $invite_target = rsvp_function_exists_guest_uid($rid, $usr->uid);
        if (!$invite_target) {
          $hash = md5($rid . $string . time());
          if (db_query('INSERT INTO {rsvp_invite} (rid, uid, inviteehash, email, hash, timestamp, response, comment, totalguests) VALUES (%d, %d, \'%s\', \'%s\', \'%s\', %d, \'%s\', \'%s\', %d)', $rid, $usr->uid, $owner_hash, $usr->mail, $hash, time(), 'none', '', 1)) {
            $invite_hash = $hash;
            $list_key = 'success_add';
          }          
        }
        else {
          // the uid is already in the attendee list
          $invite_hash = $invite_target->hash;
          $list_key = 'existing';
        }
      }
      
      if ($invite_hash != NULL) {
        $invite_target = rsvp_function_load_invitation_hash($invite_hash);
        $emails[$list_key][] = rsvp_function_getGuestEmail($invite_target);
      }
      else {
        $emails[$list_key][] = $string;
      }
    }
  }

  return $emails;
}


/**
 * Adds a number of guests to an invitation(rsvp) and sends out emails
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 * 
 * @param $rsvp The rsvp instance.
 * @param $invite_caller The invitation object of the user who adds the guests, 
 *        or NULL if the rsvp owner, co-owner or maintainer adds the guests,
 *        or const RSVP_OPEN_INVITATION for an open invitation
 * @param $guestsarray Stringarray Contains a mix of email addresses and drupal usernames.
 * @param $send_rsvp Boolean Also send the invitations or just add the invitees to the invitation.

 * @return array with statusmessages or false
 * 
 */
function rsvp_function_add_guests($rsvp, $invite_caller, $guestsarray, $send_rsvp) {
	
  $content = '';
  
  $add_status = _rsvp_function_add_guests($rsvp, $invite_caller, $guestsarray);
  
  $send_status = array();
  
  if ($send_rsvp == true) {
    // send invitations
    if ($invite_caller == RSVP_OPEN_INVITATION)
      $invite_caller = NULL; //set $invite_caller back to NULL before sending the invitation.
      
    $send_status = rsvp_function_send_multiple_invitations($rsvp, $invite_caller);
  }

  return array_merge($add_status, $send_status);
}

/**
 * Remove a number of guests from an invitation(rsvp)
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 *
 * @param $rsvp The rsvp instance.
 * @param $guestsarray Stringarray Contains a mix of email addresses, drupal usernames and hashes.
 * 
 * @return array with failed emailaddresses or false
 */
function _rsvp_function_remove_guests($rsvp, $guestsarray) {

  $emails = array();

  // remove duplicates from guestlist array
  $guestsarray = array_unique($guestsarray);

  $rid = $rsvp->rid;
  
  foreach ($guestsarray as $key => $value) {

    $list_key = NULL;
    $invite_name = NULL;
    
    // strip whitespace
    $string = trim($value);
    
    if (strlen($string)) {
      // attempt to deal with the string as a drupal username
      // we do this before dealing it as an e-mail to handle users like 
      // foo@somesite.com
      $usr = user_load(array('name' => $string));
      if ($usr === FALSE) {
        // attempt to find a system user having this string as an e-mail address
        $usr = user_load(array('mail' => $string));
        if ($usr === FALSE) {
          // no user found: carry on dealing with the string as an e-mail address
          if (valid_email_address($string)) {
            $invite_target = rsvp_function_exists_guest_email($rid, $string);
            if ($invite_target) {
              $invite_name = rsvp_function_getGuestEmail($invite_target);
              $list_key = 'success_remove';
            	rsvp_function_remove_guest($invite_target->hash);
            }
            else {
              $invite_name = $string;
              $list_key = 'invalid';
            }
          }
          else {
            // the e-mail is invalid. Last possibility is that the string is a hash.
            $invite_target = rsvp_function_load_invitation_hash($string);
            if ($invite_target) {
              $invite_name = rsvp_function_getGuestEmail($invite_target);
              $list_key = 'success_remove';
              rsvp_function_remove_guest($invite_target->hash);
            }
            else {
              $invite_name = $string;
              $list_key = 'invalid';
            }
          }
        }
      }

      if ($usr != FALSE) {
        // a valid drupal user was found, make sure his/her e-mail is the guest list
        $invite_target = rsvp_function_exists_guest_uid($rid, $usr->uid);
        if ($invite_target) {
          $invite_name = rsvp_function_getGuestEmail($invite_target);
          $list_key = 'success_remove';
          rsvp_function_remove_guest($invite_target->hash);
        }
        else {
          $invite_name = $string;
          $list_key = 'invalid';
        }
        
      }
      
      $emails[$list_key][] = $invite_name;
      
    }
  }

  return $emails;
}

/**
 * Remove a number of guests from an invitation(rsvp)
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 *
 * @param $rsvp The rsvp instance.
 * @param $guestsarray Stringarray Contains a mix of email addresses, drupal usernames and hashes.
 * 
 * @return array with failed emailaddresses or false
 */
function rsvp_function_remove_guests($rsvp, $guestsarray) {
  
  $content = '';
  
  $remove_status = _rsvp_function_remove_guests($rsvp, $guestsarray);
  
  $content .= theme('rsvp_status', $remove_status);
  drupal_set_message($content);
}

/**
 * Sends the invitation to all invitees of an rsvp instance.
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 *
 * @param $rsvp The rsvp object.
 * @param $invite_caller The invite object of the user sending the invitation or NULL (if NULL, moderator sends invitation). 
 * @param $resend True: sends to all guests even when received flag is already set. default: false.
 *
 * @return array of status values.
 */
function rsvp_function_send_multiple_invitations($rsvp, $invite_caller = NULL, $resend = FALSE) {

	$status['success_send_invitation'] = array();
  $status['failed_send_invitation'] = array();
  
  $invitees = rsvp_function_load_guests($rsvp->rid);
  while ($one_invite = db_fetch_object($invitees)) {
    if (!($resend && $one_invite->received) && !($one_invite->invited)) {
      if (_rsvp_function_send_one_invitation($rsvp, $one_invite, $invite_caller)) { 
        $status['success_send_invitation'][] = rsvp_function_getGuestEmail($one_invite);
      }
      else {
        $status['failed_send_invitation'][] = rsvp_function_getGuestEmail($one_invite);
      }
    }
  }
  return $status;
}

/**
 * Sends the invitation to one guest.
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 *
 * @param $rsvp The rsvp object.
 * @param $invite_target The invite object to send the invitation to.
 * @param $invite_caller The invite object of the user sending the invitation or NULL (if NULL, moderator sends invitation).
 *
 * @return array of status values.
 */
function rsvp_function_send_one_invitation($rsvp, $invite_target, $invite_caller = NULL) {

  $status['success_send_invitation'] = array();
  $status['failed_send_invitation'] = array();

  if (_rsvp_function_send_one_invitation($rsvp, $invite_target, $invite_caller)) { 
    $status['success_send_invitation'][] = rsvp_function_getGuestEmail($invite_target);
  }
  else {
    $status['failed_send_invitation'][] = rsvp_function_getGuestEmail($invite_target);
  }
  
  return $status;
}

/**
 * Internal function
 * 
 * Sends an invitation to one guest.
 * 
 * Permission check needs to be done outside.
 *
 * @ingroup rsvp_functions
 * @param $rsvp The rsvp object.
 * @param $invite_target The invite object to send the invitation to.
 * @param $invite_caller The invite object of the user sending the invitation or NULL (if NULL, moderator sends invitation).
 * @return boolean. True if message has been successfully send.
 */
function _rsvp_function_send_one_invitation($rsvp, $invite_target, $invite_caller = NULL) {
	global $language;
  
  if (!$invite_caller || is_null($invite_caller)) {
    global $user;
    $sender_name = $user->name;
    $from = rsvp_function_email_getFromField($user->name, $user->mail);
  }
  else {
    $sender_name = $invite_caller->email;
    $from = rsvp_function_email_getFromField($sender_name, $invite_caller->email);
  }
  
  $u = user_load($rsvp->uid);
  $preferred_language = user_preferred_language($u);

  $to = $invite_target->email;

  $replacements = rsvp_mailer_replacements($rsvp, $invite_target, $sender_name, '', $rsvp->invite_text);

  $ret = drupal_mail('rsvp', 'invitation', $to, $preferred_language, $replacements, $from);

  if ($ret == true) {
    $fields[] = 'invited = %d';
    $fields[] = 'timestamp = %d';
    
    $vals[] = 1;
    $vals[] = time();
    $vals[] = $invite_target->hash;
    
    $sql = 'UPDATE {rsvp_invite} SET '. implode(', ', $fields) .' WHERE hash = \'%s\'';
  
    db_query($sql, $vals);
  }
  
  return $ret;      
}

/**
* Send a message to selective guests of an invitation.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* 
* @param $rsvp The rsvp object.
* @param $audience Const The audience for the message (RSVP_ATT_ALL,...).
* @param $rsvp_mailer_op String One of the mailer operations in rsvp.mailer.inc.
* @param $subject String The body you want to send.
* @param $body String The subject you want to send.
* @param $invite_caller The invitee instance of the person triggering the message or NULL if moderator. 
*
* @return string of formatted recipients, or empty string depending on $confirm.
*/
function rsvp_function_send_message_guests($rsvp, $audience, $rsvp_mailer_op, $subject, $body, $invite_caller = NULL) {
	
  // check whether the message is sent from a user who's invited
  // yet allowed to message other attendees
  if (!$invite_caller || is_null($invite_caller)) {
    global $user;
    $sender_name = $user->name;
    $from = rsvp_function_email_getFromField($user->name, $user->mail);
  }
  else {
    $sender_name = $invite_caller->email;
    $from = rsvp_function_email_getFromField($sender_name, $invite_caller->email);
  }

  $status['success_send_message'] = array();
  $status['failed_send_message'] = array();

  $attendees = rsvp_function_load_guests($rsvp->rid);
  while ($attendee = db_fetch_object($attendees)) {
    $invite = rsvp_function_load_invitation_hash($attendee->hash);
    
    if ($audience == RSVP_ATT_ALL || $invite->response == $audience) {
      $inviteuser = user_load($invite->uid);
      $to = $attendee->email;

      $preferred_language = user_preferred_language($inviteuser);

      $replacements = rsvp_mailer_replacements($rsvp, $attendee, $sender_name, $subject, $body);

      if (drupal_mail('rsvp', $rsvp_mailer_op, $to, $preferred_language, $replacements, $from)) {
        $status['success_send_message'][$invite->email] = rsvp_function_getGuestEmail($invite);
      }
      else {
        $status['failed_send_message'][$invite->email] = rsvp_function_getGuestEmail($invite);
      }
    }
  }

  return $status;
}

/**
* Send a message to a particular guest of an invitation.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* 
* @param $rsvp The rsvp object.
* @param $hash The hashcode of the person we want to send the message to.
* @param $rsvp_mailer_op String One of the mailer operations in rsvp.mailer.inc.
* @param $body String The body you want to send.
* @param $subject String The subject you want to send.
* @param $invite_caller The invitee instance of the person triggering the message or NULL if moderator. 
*
* @return string of formatted recipients, or empty string depending on $confirm.
*/
function rsvp_function_send_message_guest($rsvp, $hash, $rsvp_mailer_op, $subject, $body, $invite_caller = NULL) {

//get rid of subject

  // check whether the message is sent from a user who's invited
  // yet allowed to message other attendees
  if (!$invite_caller || is_null($invite_caller)) {
    global $user;
    $sender_name = $user->name;
    $from = rsvp_function_email_getFromField($sender_name, $user->mail);
  }
  else {
    $sender_name = $invite_caller->email;
    $from = rsvp_function_email_getFromField($sender_name, $invite_caller->email);
  }

  $status['success_send_message'] = array();
  $status['failed_send_message'] = array();

  $invite = rsvp_function_load_invitation_hash($hash);
    
  $inviteuser = user_load($invite->uid);
  $to = $invite->email;

  $preferred_language = user_preferred_language($inviteuser);

  $replacements = rsvp_mailer_replacements($rsvp, $invite, $sender_name, $subject, $body);

  if (drupal_mail('rsvp', $rsvp_mailer_op, $to, $preferred_language, $replacements, $from)) {
    $status['success_send_message'][$invite->email] = rsvp_function_getGuestEmail($invite);
  }
  else {
    $status['failed_send_message'][$invite->email] = rsvp_function_getGuestEmail($invite);
  }

  return $status;
}

/**
* Send a message to the owner/co-owner of an invitation.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* 
* @param $rsvp The rsvp object.
* @param $rsvp_mailer_op String One of the mailer operations in rsvp.mailer.inc.
* @param $subject String The subject you want to send.
* @param $body String The body you want to send.
* @param $invite_target The invitee instance of the person triggering the message or NULL if no trigger. 
* *
* @return string of formatted recipients, or empty string depending on $confirm.
*/
function rsvp_function_send_message_moderator($rsvp, $rsvp_mailer_op, $subject, $body, $invite_target = NULL) {
	
  $owner_user = user_load($rsvp->uid);
  if ($rsvp->uid_moderator > 0)
    $moderator_user = user_load($rsvp->uid_moderator);
  else
    $moderator_user = false;
    
  global $user;
  $sender_name = $user->name;
  $from = rsvp_function_email_getFromField($sender_name, $user->mail);

  $status['success_send_message'] = array();
  $status['failed_send_message'] = array();

  
  //send to owner
  $to = $owner_user->mail;
  $preferred_language = user_preferred_language($owner_user);

  $replacements = rsvp_mailer_replacements($rsvp, $invite_target, $sender_name, $subject, $body);

  if (drupal_mail('rsvp', $rsvp_mailer_op, $to, $preferred_language, $replacements, $from)) {
    $status['success_send_message'][$to] = rsvp_function_getGuestEmail($invite_target);
  }
  else {
    $status['failed_send_message'][$to] = rsvp_function_getGuestEmail($invite_target);
  }
  
  //send to co-moderator
  if ($moderator_user == true) {
    $to = $moderator_user->mail;
    $preferred_language = user_preferred_language($moderator_user);

    if (drupal_mail('rsvp', $rsvp_mailer_op, $to, $preferred_language, $replacements, $from)) {
      $status['success_send_message'][$to] = rsvp_function_getGuestEmail($invite_target);
    }
    else {
      $status['failed_send_message'][$to] = rsvp_function_getGuestEmail($invite_target);
    }
  }

  return $status;
}

/**
* Returns the invitations that a user is the owner or co_maintainer of.
*
* Permission check needs to be done outside.
*
* @ingroup rsvp_functions
* @param $nid The nid of the node.Can be one, array of nodes or NULL which means all nodes of the user.
* @param $uid The uid of the user.Can be a user, array of users or NULL which means rsvp for all users.
* @return dbresultset of invitations.
*/
function rsvp_function_load_invitations_owner($nid = NULL, $uid = NULL, $tablesort = ' ORDER BY nid') {
	
  $query = 'SELECT r.*, n.title FROM {rsvp} r INNER JOIN {node} n ON r.nid = n.nid '; //r.uid = %d';
  
  if (!is_null($uid) || !is_null($nid)) {
    $query .= 'WHERE '; 
  }
  
  //$query = 'SELECT r.*, n.title FROM {rsvp} r INNER JOIN {node} n ON r.nid = n.nid WHERE r.uid = %d';
  if (!is_null($nid)) {
    if (is_array($nid)) {
      $query .= 'r.nid IN (%s) ';
      $nid = implode(', ', $nid);
    }
    elseif (is_numeric($nid)) {
      $query .= 'r.nid = %d ';
    }
  }
  
  if (!is_null($uid)) {
    if (!is_null($nid)) {
      $query .= 'AND ';
    }

    //load if uid is owner or co_moderator.
    if (is_array($uid)) {
      $query .= '( r.uid IN (%s) ';
      $query .= ' OR r.uid_moderator IN (%s) )';
      $uid = implode(', ', $uid);
    }
    elseif (is_numeric($uid)) {
      $query .= '( r.uid = %d ';
      $query .= ' OR r.uid_moderator = %d )';
    }
  }
  
  $query .= $tablesort;
  
  if (is_null($nid)) {
    return db_query($query, $uid, $uid);  
  }
  else {
    return db_query($query, $nid, $uid, $uid);
  }
  
}

/**
* Returns the invitations that a user has received.
* If nid is NULL, return all invitation for the user. Otherwise only invitation of that particluar node 
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $uid The uid of the user.
* @param $nid The nodeid we want to query for. If Null, select for nodes
* @param $count returns only the total number of invitations for a particular uid/nid.
* @return array of events or integer if count=true.
*/
function rsvp_function_load_invitations_user($uid = NULL, $nid = NULL, $count = false) {
  
	$uid = _rsvp_function_check_uid($uid);  
  if (is_null($uid)) {
  	if ($count)
  	  return 0;
  	else
      return array();
  }
  
  //US orig $query = 'SELECT '. ($count ? 'COUNT(*)' : 'u.*,r.nid,n.realname') .' FROM {rsvp} r LEFT JOIN {rsvp_invite} u ON r.rid = u.rid LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE u.uid = %d';
  $query = 'SELECT '. ($count ? 'COUNT(*)' : RSVP_QUERY_FIELDS_rsvp_invite) .' FROM ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE u.uid = %d';
  
  if (!is_null($nid)) {
    $query .= ' AND r.nid = %d';
  }

  return db_query($query, $uid, $nid);
}

/**
* Returns the invitations that a registered user has received for a specific event node.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $nid The nid of the node.
* @param $uid The uid of the user. If null, return all from all users. 
*             uid can not be 0, because it would return all invitations from unregistered attendees.
* @return sql result or false.
*/
function rsvp_function_load_invitations_node2($nid, $uid = NULL, $count = false) {
	
  
  //US orig $query = 'SELECT '. ($count ? 'COUNT(*)' : 'u.*,n.realname') .' FROM {rsvp} r LEFT JOIN {rsvp_invite} u ON r.rid = u.rid LEFT JOIN {rsvp_realname} n ON u.email = n.email WHERE r.nid = %d';
  $query = 'SELECT '. ($count ? 'COUNT(*)' : RSVP_QUERY_FIELDS_rsvp_invite) .' FROM ' . RSVP_QUERY_FROM_rsvp_invite . ' WHERE r.nid = %d';
  
  if (!is_null($uid)) {
    if ($uid != 0)
      $query .= ' AND u.uid = %d';
    else
      return false;
  }
  
  return db_query($query, $nid, $uid);
}

/**
* Returns the invitations for a specific event node.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $nid The nid of the node.
* @return sql result or false.
*/
function rsvp_function_load_invitations($nid, $count = false) {
  
  
  $query = 'SELECT '. ($count ? 'COUNT(*)' : '*') .' FROM {rsvp} r WHERE r.nid = %d';
  
  return db_query($query, $nid);
}


/**
* Checks to see if a logged in user has been invited to an event.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $nid The node id of the event.
* @param $uid The uid of the user. If null, it uses global $user->uid.
* @return boolean. True if user has been invited to the event.
*/
function rsvp_function_is_invited($nid, $uid = NULL) {
	
  $uid = _rsvp_function_check_uid($uid);  
  if (is_null($uid)) {
  	return false;
  }

  if (db_result(db_query('SELECT COUNT(*) FROM {rsvp} r LEFT JOIN {rsvp_invite} u ON r.rid = u.rid WHERE r.nid = %d AND u.uid = %d', $nid, $uid)) > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
* Checks to see if responses are visible to other attendees.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $rid The rid of the rsvp.
* @return Int. 0:Show responses, 1:Hide responses, 2:Hide responses until replied 
* 
*/

function rsvp_function_is_response_blind($rid) {
	$status = db_fetch_object(db_query('SELECT response_blind FROM {rsvp} WHERE rid = %d', $rid));

  return $status->response_blind;
}

/**
* Checks to see if a user is the owner of an rsvp.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $rid The id of the rsvp.
* @param $uid The uid of the user. If null, it uses global $user->uid.
* @return boolean. True if user is the owner of an rsvp to the event.
*/
function rsvp_function_is_owner($rid, $uid = NULL) {

  $uid = _rsvp_function_check_uid($uid);  
  if (is_null($uid)) {
  	return false;
  }

  if (db_result(db_query('SELECT COUNT(*) FROM {rsvp} WHERE rid = %d AND uid = %d', $rid, $uid)) > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
* Checks to see if a user has any invitations.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $uid The uid of the user. If null, it uses global $user->uid.
* @return boolean. True if user is the owner of an rsvp to the event.
*/
function rsvp_function_exists_invitation_uid($uid = NULL) {
	
  $uid = _rsvp_function_check_uid($uid);  
  if (is_null($uid)) {
  	return false;
  }

  if (db_result(db_query('SELECT COUNT(*) FROM {rsvp} WHERE uid = %d', $uid)) > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
* Checks to see if a user is the owner of an rsvp for an event node.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions
* @param $nid The id of the event node.
* @param $uid The uid of the user. If null, it uses global $user->uid.
* @return boolean. True if user is the owner of an rsvp to the event.
*/
function rsvp_function_exists_invitation_nid($nid, $uid = NULL) {
  
	$uid = _rsvp_function_check_uid($uid);  
  if (is_null($uid)) {
  	return false;
  }

  if (db_result(db_query('SELECT COUNT(*) FROM {rsvp} WHERE nid = %d AND uid = %d', $nid, $uid)) > 0) {
    return TRUE;
  }
  return FALSE;
}


/**
* Checks to see if the current user (represented by invite) can invite other users.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions.
* @param $rsvp The rsvp object the current user wants to add guests to.
* @param $invite_caller The invite object of the user we want to check for. If null, use the current user.
* @return boolean. True if user is allowed inviting other persons.
*/
function rsvp_function_user_viral_access($rsvp, $invite) {
	
  switch($rsvp->allow_invite) {
    case RSVP_INVITE_NONE:
      return false;
    case RSVP_INVITE_INVITEE:
      return true;
    case RSVP_INVITE_RESPONDENT:
      return  ($invite->response != RSVP_ATT_NONE);
    case RSVP_INVITE_ATTENDEE:
      return  ($invite->response == RSVP_ATT_YES);
    default:
      return false;
  }
}

/**
* Checks to see if an invitation has been created through viral process.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions.
* @param $invite_target The invite object the user we want to check for.
* @return boolean. True if invitation has been created due to viral process.
*/
function rsvp_function_is_invitation_viral($invite_target) {

	if (isset($invite_target->inviteehash) && 
	     (strlen($invite_target->inviteehash) > 0) && 
	     ($invite_target->inviteehash != RSVP_OPEN_INVITATION))
	  return true;
	else
	  return false;
}

/**
* Checks to see if an invitation has been created through open signup.
*
* Permission check needs to be done outside.
* 
* @ingroup rsvp_functions.
* @param $invite_target The invite object the user we want to check for.
* @return boolean. True if invitation has been created through open signup.
*/
function rsvp_function_is_invitation_opensignup($invite_target) {

  if (isset($invite_target->inviteehash) && 
       (strlen($invite_target->inviteehash) > 0) && 
       ($invite_target->inviteehash == RSVP_OPEN_INVITATION))
    return true;
  else
    return false;
}


/*
 * Plausabiliy check if an invitation is a valid object
 * 
 * Permission check needs to be done outside.
 * 
 * @ingroup rsvp_functions
 *  
 * @param $invite The invitation object 
 * @param $die Boolean, if true send us back with a proper message. If false, return result of check.
 * 
 * @returns The invitation object or false. The drupal_goto should send the request directly back to the user.
 */
function rsvp_function_check_invitation($invite, $die = true) {

  if (is_null($invite) || $invite == false) {
    if ($die == true) {
      drupal_set_message(t('The invitation you requested is invalid.'));
      drupal_goto();
    }
    return false;
  }  
  return $invite;
}

/*
 * Plausabiliy check if an rsvp is a valid object
 * 
 * Permission check needs to be done outside.
 * 
 * @ingroup rsvp_functions
 *  
 * @param $rsvp The rsvp object 
 * @param $die Boolean, if true send us back with a proper message. If false, return result of check.
 *  * 
 * @returns The rsvp object or false. The drupal_goto should send the request directly back to the user.
 */
function rsvp_function_check_rsvp($rsvp, $die = true) {

  if (is_null($rsvp) || $rsvp == false) {
    if ($die == true) {
      drupal_set_message(t('The RSVP you requested is invalid.'));
      drupal_goto();
    }
    return false;
  }  
  return $rsvp;
}


/**
 * Certain users are not allowed to see a guests email address. In that case return the username instead.
 * also allows to hide e-mails for system users in general
 *
 * @ingroup rsvp_functions
 * 
 * Permission check needs to be done outside.
 * 
 * @param $invite_target The invitation object.
 * @param $stripEmailAddress Boolean show onlypart of the emailaddress.
 * @param $honorRealname Boolean show realname if available.
 * @return email 
 */
function rsvp_function_getGuestEmail($invite_target, $stripEmailAddress = true, $honorRealname = true) {
	
	if (($honorRealname == true) && isset($invite_target->realname) && strlen($invite_target->realname) > 0)
	 return $invite_target->realname;
	
  if ($invite_target->uid) {
    if ($invite_target->username)
      return $invite_target->username;
    else
      die("NO USERNAME");
    //US orig $u = db_fetch_object(db_query("SELECT uid, name FROM {users} WHERE uid = %d", $invite_target->uid));
    //US orig return $u->name;
  }
  else {
    return (!$stripEmailAddress ? $invite_target->email : substr($invite_target->email, 0, strpos($invite_target->email, '@')));
  }
}

/**
 * Depending on the global rsvp setting, the From address is being returned.
 *   
 * @ingroup rsvp_functions
 * 
 * Permission check needs to be done outside.
 *
 * @param $username The user name that initiates the email.
 * @param $useremail The user email that initiates the email.
 * @return emailaddress as String. 
 */
function rsvp_function_email_getFromField($username, $useremail) {
	
  $pick_address = RSVP_EMAIL_CREATOR;
  //$from_target = $from_creator = $username .' <'. $useremail .'>';
  $from_target = $from_creator = $useremail;
  
  $emailfrom = variable_get('rsvp_from_address', NULL);
  if (isset($emailfrom)) {
    foreach ($emailfrom as $pick => $email) {
      $pick_address = $pick;
      $from_address = $email; 
    }
  }

  switch($pick_address) {
    case RSVP_EMAIL_SITEADDRESS: //The sites default address
      $from_target =  variable_get('site_mail', ini_get('sendmail_from'));
      break;
      case RSVP_EMAIL_SETTINGS: //Use the email adress selected in the options menue
      $from_target = from_address;
      break;
    default:
    case RSVP_EMAIL_CREATOR: //Use the invitations creator email
      $from_target =  $from_creator;
      break;
      
  }
  return $from_target;  
}


/**
 * Verifies if a node or rsvp for a particular node is expired or not.
 *
 * @ingroup rsvp_functions
 *  
 * Permission check needs to be done outside.
 * 
 * @param $node The node object.
 * @param $rsvp The rsvp object or NULL.
 * @return true or false. 
 *
 * If $rsvp is null we check all available enddates (reocurring dates) for at least one not expired date.
 * If $rsvp is not null we check only the enddate for the rsvp
 * 
 */
function rsvp_function_is_expired($node, $rsvp = NULL) {
	
  $connector = new RsvpConnector();

  if (is_null($node))
    return true;

  $now = time();
    
  $field = $connector->get_event_field($node->type);
      
  if (is_null($rsvp)) {
    $allExpired = true;
    //debug $end_dates = array();
    $count = $connector->get_datecount($node, $field);
    //iterate through all specified enddates (in case of repeating dates)
    //debug $end_dates[] = array('current time stamp' => $time, 'current time readable' => date('r', $time) );
    for ($i = 0; $i < $count; $i++) {
      $t2 = $connector->get_enddate($node, $field, $i);
      //debug $end_dates[] = array('field' => $field, 'stamp' => $t2, 'readable' => date('r', $t2) );
          
      if ($t2 > $now) {
        $allExpired = false;
        break;
      }
    }
    //debug var_dump($allExpired, $end_dates);
    
    return $allExpired;
    
  }
  else {
    $pos = $connector->get_posByHash($node, $field, $rsvp->startdate);
  
    return ($now > $connector->get_enddate($node, $field, $pos));
    
  }
  
}

/**
 * Verifies if a user can reply to a rsvp at this time or not.
 *
 * @ingroup rsvp_functions
 *  
 * Permission check needs to be done outside.
 * 
 * @param $rsvp The rsvp object or NULL.
 * @return true or false. 
 * 
 */
function rsvp_function_is_replypermitted($rsvp) {
  
  $connector = new RsvpConnector();

  $now = time();

  if (($now >= $rsvp->reply_startdate) && 
      ($now <= $rsvp->reply_enddate))
    return true;
    
  return false;
}


/**
 * Verifies if a user (by role) is entitled to view an invitation or not.
 *
 * @ingroup rsvp_functions
 * 
 * Permission check needs to be done outside.
 *
 * @param $rsvp The rsvp object.
 * @param $usr The user object you want to check for. If NULL check for the current user.
 * @return true or false. 
 * 
 */
function rsvp_function_view_invitation_access($rsvp, $usr) {
  return _rsvp_function_view_roles_access($usr, $rsvp->rsvp_view_roles);
}

/**
 * Verifies if a user (by role) is entitled to view a guestlist(responses) or not.
 *
 * @ingroup rsvp_functions
 * 
 * Permission check needs to be done outside.
 * 
 * @param $rsvp The rsvp object.
 * @param $usr The user object you want to check for. If NULL check for the current user.
 * @return true or false. 
 * 
 */
function rsvp_function_view_guestlist_access($rsvp, $usr) {
	return _rsvp_function_view_roles_access($usr, $rsvp->response_view_roles);
}

/**
 * Verifies if a user is entitled to view an object or not.
 *
 * @ingroup rsvp_functions
 * 
 * Permission check needs to be done outside.
 * 
 * @param $usr The user object you want to check for. If NULL check for the current user.
 * @param $roles String a string with roles(separated by semicolon) 
 * @return true or false. 
 * 
 */
function _rsvp_function_view_roles_access($usr, $roles) {

  $role_view = false;
  
  $_roles = explode(';', $roles);
  
  
  if (is_null($usr)) {
    if (user_is_anonymous()) {
      $usr_roles = array(1); //=anonymous
    }
    else {
      global $user;
      $usr_roles = array_keys($user->roles);
    }
  }
  else {
    //get roles from $usr allowed to access the rsvp.
    $usr_roles = array_keys($usr->roles);
  }

  if (count(array_intersect($_roles, $usr_roles)) > 0) {
    $role_view = true;
  }
  
  return $role_view;
}


/**
* Helperfunction to store/load the caller in a session object.
*
* @ingroup rsvp_functions
*
* Permission check needs to be done outside.
* 
* @param $parent The link of the page that has to be stored as parent or NULL.
* @param $remove Boolean remove the link from the top of the stack if $parent is NULL.
* @return a link which represents the parent frame, or NULL 
*
* This function works as a setter and getter.
* - if $parent = NULL, then it returns the last parent link on the stack.
* - if $parent != NULL, then it stores parameter parent on the stack.
*/
function rsvp_function_session_parent($parent = NULL, $remove = false) {
	
  if (!isset($_SESSION['rsvp_par']))
    $_SESSION['rsvp_par'] = array();

  if (is_null($parent)) {
    if ($remove == true)
      return array_shift($_SESSION['rsvp_par']);
    else
        return $_SESSION['rsvp_par'][0];
  }
  else {
    // check the stack for existence. If available remove everything in front of it. Atterwise put on stack
    $exist = in_array ( $parent  , $_SESSION['rsvp_par'] );
    if ($exist == true) {
      while($_SESSION['rsvp_par'][0]) {
        if ($_SESSION['rsvp_par'][0] != $parent)  
          array_shift($_SESSION['rsvp_par']);
        else
          break;
      }
    }
    else {
      array_unshift($_SESSION['rsvp_par'], $parent);
    }
  }
}

/**
* Helperfunction to store/load if a user selected date or alphabetical sortorder
*
* @ingroup rsvp_functions
*
* Permission check needs to be done outside.
* 
* @param $rsvp The rsvp instance.
* @param $status const (RSVP_GUESTLIST_SORT_ALPHA | RSVP_GUESTLIST_SORT_DATE) or NULL.
* @return The status, or NULL 
*
* This function works as a setter and getter.
* - if $status = NULL, then it returns the current status.
* - if $status != NULL, then it stores the new status.
*/
function rsvp_function_guestlist_sortorder($rsvp, $status = NULL) {

	/**
 * RSVP GUESTLIST SHOW/SORT OPTIONS
 */
  $handle = 'rsvp_guestlist_sort_' . $rsvp->rid; 	

  if (is_null($status)) {
    return (isset($_SESSION[$handle]) ? $_SESSION[$handle] : RSVP_GUESTLIST_SORT_ALPHA);  
  }
  else {
  	$_SESSION[$handle] = $status;
  }
  	
}

/**
* Helperfunction to store/load if a user want to see the whole guestlist or only part of it
*
* @ingroup rsvp_functions
*
* Permission check needs to be done outside.
* 
* @param $rsvp The rsvp instance.
* @param $status const (RSVP_GUESTLIST_SHOW_ALL | RSVP_GUESTLIST_SHOW_PART) or NULL.
* @return The status or NULL 
*
* This function works as a setter and getter.
* - if $status = NULL, then it returns the current status.
* - if $status != NULL, then it stores the new status.
*/
function rsvp_function_guestlist_showall($rsvp, $status = NULL) {

  $handle = 'rsvp_guestlist_showall_' . $rsvp->rid;  

  if (is_null($status)) {
    return (isset($_SESSION[$handle]) ? $_SESSION[$handle] : RSVP_GUESTLIST_SHOW_PART);  
  }
  else {
    $_SESSION[$handle] = $status;
  }
      
}


/**
 * Displays html formatted guest reply status of $hash_target
 *
 * @ingroup rsvp_view
 * @param $rsvp The rsvp object to send the message for.
 * @param $invite_target The invite object the current user wants to see the status of.
 *
 * @return html formatted view of the requested invite status.
 */
function rsvp_function_guest_status($rsvp, $invite_target) {
  
  if ($invite_target == false || $rsvp == false)
    return '';
  
  $list[] = ($invite_target->invited ? t('Guest has received an invitation.') : t('Guest has not received an invitation.'));
  $list[] = ($invite_target->received ? t('Guest has viewed the invitation.') : t('Guest has not viewed the invitation.'));
  if ($invite_target->response != RSVP_ATT_NONE) {
    $list[] = t('Guest response: %response', array('%response' => $invite_target->response));
  }
  if ($invite_target->response == RSVP_ATT_YES) { 
    $list[] = t('Guest will bring a total of %guests guests.', array('%guests' => $invite_target->totalguests));
  }
  
  
  if ($invite_target->comment) {
    $list[] = t('Guest comment: %comment', array('%comment' => $invite_target->comment));
  }
  if (rsvp_function_is_invitation_viral($invite_target) == true) {
    $invite_creator = rsvp_function_load_invitation_hash($invite_target->inviteehash);
    $list[] = t('Guest has been invited by %user.', array('%user' => rsvp_function_getGuestEmail($invite_creator)));
  }

  if (rsvp_function_is_invitation_opensignup($invite_target) == true) {
    $list[] = t('Guest has signed up through open invitation.');
  }
  
  $output = theme('item_list', $list, 'Guest \'' . rsvp_function_getGuestEmail($invite_target) . '\'');

  return $output;
}

/*
 * Internal access function to determine if current user is allowed to access a particular invitation
 * 
 * @param $rsvp The rsvp object you want to check. If null, load it from $invite_target 
 * @param $invite_target The invite object that the current user wants to check permissions for. 
 *    If NULL check for usr_target (which might be different from the caller => moderator accesses function in name of user).
 * @param $uid_target Hint about the id of the user that the current user wants to check permissions for or NULL.
 * @param $access The access type we want to check for the user on the rsvp object.
 * (RSVP_ACCESS_VIEW, RSVP_ACCESS_VIEW, RSVP_ACCESS_CREATE, RSVP_ACCESS_DELETE)  
 * @param $addtlparam String, check for an additional permission.
 *
 * @return true or false
 *  
 * returns true if current user is allowed to access the invitation. 
 *     could be anonymous, user, invitee, owner, admin, moderator or co-moderator
 *
 */
function rsvp_function_invitation_by_invite_access($connector, $rsvp, $invite_target, $uid_target, $access, $addtlparam = NULL) {
	
  $accessarray = array($access);
  
  $resultarray = rsvp_function_invitation_by_invite_accessarray($connector, $rsvp, $invite_target, $uid_target, $accessarray, $addtlparam);

  return $resultarray[0];
}

/*
 * Internal access function to determine if current user is allowed to access a particular invitation
 * 
 * @param $rsvp The rsvp object you want to check. If null, load it from $invite_target 
 * @param $invite_target The invite object that the current user wants to check permissions for. 
 *    If NULL check for uid_target (which might be different from the caller => moderator accesses function in name of user).
 * @param $uid_target Hint about the id of the user that the current user wants to check permissions for or NULL.
 * @param $access An array of accesstypes we want to check for the user on the rsvp object.
 * (RSVP_ACCESS_VIEW, RSVP_ACCESS_VIEW, RSVP_ACCESS_CREATE, RSVP_ACCESS_DELETE)  
 * @param $addtlparam String, check for an additional permission.
 *
 * @return array with permissions
 *  
 * returns true if current user is allowed to access the invitation. 
 *     could be anonymous, user, invitee, owner, admin, moderator or co-moderator
 *
 */
function rsvp_function_invitation_by_invite_accessarray($connector, $rsvp, $invite_target, $uid_target, $accessarray, $addtlparam = NULL) {
	
  $usr_target = NULL;
  $usr_caller = NULL;
  $is_guest = false;
  $is_owner = false;
  $is_maintainer = false;
  $is_co_owner = false;
  $special_access_view = false;
  $addtlperm = true;

  if (!user_is_anonymous()) {
    global $user;
    $usr_caller = $user;

    //check for valid object other then anonymous (uid > 0)
    if ($usr_caller->uid == 0)
      return false;
  }
  
  // The following block is here to find out if the current user is a guest of an invitation
  // or not.
  if (is_null($invite_target)) {
    if (!is_null($rsvp)) {
  	
  	  //happens when user accesses pages like rsvp/%rsvp/...
      if (!is_null($usr_caller)) {
        // load invitation for current user (if he is not anonymous) 
        $invite_target = rsvp_function_load_invitation_rid($rsvp->rid, $usr_caller->uid, false);
        if ($invite_target == false) {
          $invite_target = NULL;  
        }
        else {
          if ((is_null($uid_target)) || ($uid_target == $invite_target->uid))
            $is_guest = true; 
        }
      }
      else {
        //we assume that an anonymous user has no access to an invitation page through rsvp/%rsvp/....
      }
    }
    else {
      //happens if moderator enters the function through admin/content/rsvp
    }
  }
  else {
    //happens when user accesses pages like rsvp/mail/%rsvp_hash/...
    //Can be called potentially be an anonymous user or a logged in user.
    
    //$rsvp might be null if $invite_target is set.
    if (is_null($rsvp)) {
      $rsvp = rsvp_function_load_rsvp($invite_target->rid);
      $rsvp = rsvp_function_check_rsvp($rsvp);
      if ($rsvp == false)
        return false;
    }

    //Tricky:
    if (!is_null($usr_caller)) {
      //authenticated user tries to access via rsvp/mail/%rsvp_hash/...
      if ($invite_target->uid == $usr_caller->uid) {
        //if it is his invitation (=his hash) 
        if ((is_null($uid_target)) || ($uid_target == $invite_target->uid))
        $is_guest = true;    
      }
    }
    else {
      //anonymous user tries to access via rsvp/mail/%rsvp_hash/...
      if ((is_null($uid_target)) || ($uid_target == $invite_target->uid))
       $is_guest = true;    
    }
  }
  
  //another plausability check.
  if (!is_null($invite_target) && ($invite_target->rid != $rsvp->rid))
    return false;

  if (!is_null($addtlparam)) {  
    $addtlperm = user_access($addtlparam);
  }
  
  //check for role settings only if we have not been identified as the guest yet and we check for ourselve if we have a hint.
  if (($is_guest == false) && (!is_null($rsvp))) { 
    if ((is_null($uid_target)) || ($uid_target == $usr_caller->uid)) {
      $special_access_view = (rsvp_function_view_invitation_access($rsvp, $usr_caller) || 
                  rsvp_function_view_guestlist_access($rsvp, $usr_caller));
    }
  }
  
  // only identified users have moderator rights (have to be logged in ).
  if (!is_null($usr_caller)) {
    if (!is_null($rsvp)) {
      $is_owner = ($rsvp->uid == $usr_caller->uid);
      $is_co_owner = ($rsvp->uid_moderator != 0) ? ($rsvp->uid_moderator == $usr_caller->uid) : false;
    }
    $is_maintainer = user_access(RSVP_PERM_MAINTAIN);    
  }
  
  //after collecting the variables, now process the access array.
  $resultarray = array();
  foreach($accessarray as $access) {
    switch($access) {
      case RSVP_ACCESS_BYROLE:
        $resultarray[] = ( (  $special_access_view ) 
               && $addtlperm && user_access('access content'));
        break;
      case RSVP_ACCESS_GUEST:
        $resultarray[] = ( (  $is_maintainer || $is_guest ) 
               && $addtlperm && user_access('access content'));
        break;
      case RSVP_ACCESS_GUESTONLY:
        $resultarray[] = ( (  $is_guest ) 
               && $addtlperm && user_access('access content'));
        break;
      case RSVP_ACCESS_MAINTAIN:
        $resultarray[] = ( ( $is_maintainer ) 
               && $addtlperm && user_access('access content'));
        break;
      case RSVP_ACCESS_CREATE:
        $resultarray[] = false;
        break;
      case RSVP_ACCESS_DELETE:
      case RSVP_ACCESS_EDIT:
        $resultarray[] = ( (  $is_maintainer || $is_owner || $is_co_owner ) 
               && $addtlperm && user_access('access content'));
               break;
      case RSVP_ACCESS_REPLY:
        $resultarray[] = ( (  $is_maintainer || $is_owner || $is_co_owner || $is_guest ) 
               && $addtlperm && user_access('access content'));
               break;
      case RSVP_ACCESS_VIEW:
        $resultarray[] = ( (  $is_maintainer || $is_owner || $is_co_owner || $is_guest || $special_access_view ) 
               && $addtlperm && user_access('access content'));
              break;
      default:
        $resultarray[] = false;
    }
  }
  
  return $resultarray;
}

/* Interal access function to determine if the current user is allowed to create a rsvp for a node
 * 
 * @param $node The passed node object you want to check for.
 * @param $access The access type we want to check for the user on the node object. 
 *            (RSVP_ACCESS_VIEW,RSVP_ACCESS_EDIT,RSVP_ACCESS_CREATE,RSVP_ACCESS_DELETE)  
 * @param $addtlparam String, check for an additional permission.
 * @return true or false.
 *  
 * returns true if the current user is allowed to create an invitation based on the $node in terms of
 * rsvp privilegues. (Could be user, admin, moderator or anonymous)
 *
 * With this method you can find out if the current user is
 *   - allowed to create a rsvp on the node
 */

function rsvp_function_create_invitation_access($connector, $node, $access, $addtlparam = NULL) {

  $usr = NULL;
  
  if (!user_is_anonymous()) {
    global $user;
    $usr = $user;
  }

  $is_owner = false;
  $is_moderator = false;
  $addtlperm = true;
  $allExpired = true;
  $too_many_invitations = true;
  
  if (!$connector->is_event_enabled($node->type))
    return false;

  $allExpired = rsvp_function_is_expired($node);
  if (variable_get('rsvp_for_expired_event', RSVP_OPTION_NO) == RSVP_OPTION_YES) {
    $allExpired = false;
  }
    
  if (!is_null($addtlparam)) {  
    $addtlperm = user_access($addtlparam);
  }

  $too_many_invitations = ((db_result(rsvp_function_load_invitations($node->nid, true)) > 0) &&
                            !user_access(RSVP_PERM_MULTI_INVITATIONS));
  
  $is_moderator = user_access(RSVP_PERM_MAINTAIN);    
  $is_node_owner = (!is_null($usr)) && ($usr->uid > 0) && ($node->uid == $usr->uid);
  
  switch($access) {
    case RSVP_ACCESS_CREATE:
      return  (!$too_many_invitations && !$allExpired && $addtlperm &&
                ($is_moderator || 
                 user_access(RSVP_PERM_ONEVENTS) || 
                 (user_access(RSVP_PERM_ONOWNEVENTS) && $is_node_owner)
                ) 
              );
    case RSVP_ACCESS_VIEW:
    case RSVP_ACCESS_REPLY:
    case RSVP_ACCESS_DELETE:
    case RSVP_ACCESS_EDIT:
    default:
      return false;
            
  }
}


/*
 * collect all rsvp themes that are available.
 *
 * @return array with pairs of themename => themepath (the part that is userdefined)
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/themes  (themes that come with the module).
 *      - sites/default/files/rsvp/themes (themes that the user provieds).
 *
 */

function rsvp_function_getThemepathes() {

	static $available_themespathes;


  if (count($available_themespathes) ==  0) {
    $available_themespathes = array();
    
    $dir_regex = '^.*';
    $ignore = array('.', '..', 'CVS');
    
  	//1) collect all folders under sites/all/modules/rsvp/themes/THEME
  	$themesdirectory = drupal_get_path('module', 'rsvp') .'/'. RSVP_THEMES_PATH;
    $dirs = file_scan_directory($themesdirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($dirs as $filename => $theme) {
      $available_themespathes[$theme->basename] = RSVP_THEMES_PATH . '/' . $theme->basename;
    }
  	
    //2) collect all folders under .../files/rsvp/themes/THEME
    $themesdirectory = file_directory_path() . '/' . RSVP_FILES_THEMES_PATH;
    $dirs = file_scan_directory($themesdirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($dirs as $filename => $theme) {
      $available_themespathes[$theme->basename] = RSVP_FILES_THEMES_PATH . '/' . $theme->basename;
    }
  }
  
  return $available_themespathes;
}

/*
 * returns the path to the rsvp tpl.php template file based on the globals rsvp setting
 *
 * @param $themename The theme name you want to use
 * @param $templatename The name of the template file without extension tpl.php.
 * @return String path to the tpl.php file (without the file extension tpl.php).
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/themes  (themes that come with the module).
 *      - sites/default/files/rsvp/themes (themes that the user provieds).
 * 
 * If the file can not be found, fall back to the default tpl.php file.
 *
 */
function rsvp_function_getTemplatePath($themename, $templatename) {

  $available_themepathes = rsvp_function_getThemepathes();

  $theme_path = $available_themepathes[$themename];
  
  $template = $theme_path . '/' . $templatename.'.tpl.php';

  //1) check if template exists under sites/all/modules/rsvp/themes/THEME
  $themesdirectory = drupal_get_path('module', 'rsvp');
  $exists = file_exists($themesdirectory . '/' . $template);
  if ($exists == false) {
    
    //2) check if template exists under .../files/rsvp/themes/THEME
    $themesdirectory = file_directory_path();
    $exists = file_exists($themesdirectory . '/' . $template);
    if ($exists == false) {
  
      //not found ...
      //avoid recursion in case theme default is not available
      if ($themename == RSVP_THEME_DEFAULT) 
        return false;
  	  	
      //fall back to default if possible
      return rsvp_function_getTemplatePath(RSVP_THEME_DEFAULT, $templatename);
    }
    
  }    
  
  return $theme_path . '/' . $templatename; 
}


/*
 * returns the stylesheet file (css file) inclusive full path
 *
 * @param $themename The name of the theme
 * @param $cssname The name of the css file in the form 'theme/stylesheet' without extension css
 * @return String path to the css file from root.
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/themes/THEME  (themes that come with the module).
 *      - sites/default/files/rsvp/themes/THEME (themes that the user provieds).
 * 
 * If the file can not be found, fall back to the default css file.
 *
 */
function rsvp_function_getStylesheet($themename, $cssname) {

  $available_stylesheets = rsvp_function_getStylesheets($themename);
	
  //1) find the right stylesheet
  foreach($available_stylesheets as $available_stylesheet => $dir) {
    if ($available_stylesheet == $cssname) {
    	return $dir . '.css';
    }
  }
  //avoid recursion in case theme default is not available
  if ($themename == RSVP_THEME_DEFAULT) 
    return false;
    
  //2) If the stylesheet does not match, fallback to the default
  return rsvp_function_getStylesheet(RSVP_THEME_DEFAULT, RSVP_STYLESHEET_DEFAULT);
  
  return false; 
}


/*
 * returns all css stylesheets available for a specific theme
 *
 * @param $theme The theme name
 * @return array with pairs of theme/stylesheet(no extension) => dir/theme/stylesheet(no extension)
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/themes/THEME  (themes that come with the module).
 *      - sites/default/files/rsvp/themes/THEME (themes that the user provieds).
 * 
 */
function rsvp_function_getStylesheets($theme) {

  static $available_stylesheets;


  if (count($available_stylesheets) ==  0) {
    $available_stylesheets = array();
    
    $css_regex = '\.[cC][sS][sS]$';
    $ignore = array('.', '..', 'CVS');
	
	
    $available_themepathes = rsvp_function_getThemepathes();
  
    //1) find the right template
    foreach($available_themepathes as $available_theme => $dir) {
      if ($available_theme == $theme) {
    	 //2) collect all css under sites/all/modules/rsvp/themes/$template
        $themesdirectory = drupal_get_path('module', 'rsvp') .'/'. RSVP_THEMES_PATH .'/'. $available_theme;
        $files = file_scan_directory($themesdirectory, $css_regex, $ignore, 0, FALSE);      
        foreach ($files as $filename => $stylesheet) {
          $available_stylesheets[$stylesheet->name] = $themesdirectory . '/' . $stylesheet->name;
        }

        //3) collect all css under .../files/rsvp/themes/$template
        $themesdirectory = file_directory_path() . '/' . RSVP_FILES_THEMES_PATH  .'/'. $available_theme;
        $files = file_scan_directory($themesdirectory, $css_regex, $ignore, 0, FALSE);      
        foreach ($files as $filename => $stylesheet) {
          $available_stylesheets[$stylesheet->name] = $themesdirectory . '/' . $stylesheet->name;
        }
        break;
      }
    }
    
  }
  
  return $available_stylesheets; 
}


/*
 * returns the path to the selected iconset
 *
 * @param $iconsetname The name of the iconset
 * @return String path to the iconset from root.
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/icons/ICONSET  (iconsets that come with the module).
 *      - sites/default/files/rsvp/icons/ICONSET (iconsets that the user provides).
 *
 * If the file can not be found, fall back to the default iconset.
 * 
 */
function rsvp_function_getIconset($iconsetname) {

  $available_iconsets = rsvp_function_getIconsets();
  
  //1) find the right iconset
  foreach($available_iconsets as $available_iconset => $dir) {
    if ($available_iconset == $iconsetname) {
      return $dir;
    }
  }
  
  //avoid recursion in case iconset default is not available
  if ($iconsetname == RSVP_ICONSET_DEFAULT) 
    return false;
    
  //2) If the iconsetname does not match, fallback to the default
  return rsvp_function_getIconset(RSVP_ICONSET_DEFAULT);
  
  return false; 
}

/*
 * collect all rsvp iconsets that are available.
 *
 * @return array with pairs of iconsetname => iconsetpath (the part that is userdefined)
 *  
 * iconsets can be stored in two locations
 *      - modules/rsvp/icons/ICONSET  (iconsets that come with the module).
 *      - sites/default/files/rsvp/icons/ICONSET (iconsets that the user provieds).
 *
 */
function rsvp_function_getIconsets() {

	static $available_iconsets;


  if (count($available_iconsets) ==  0) {
    $dir_regex = '^.*';
    $ignore = array('.', '..', 'CVS');
    
  	//1) collect all icons under sites/all/modules/rsvp/icons/ICONSET
  	$iconsdirectory = drupal_get_path('module', 'rsvp') .'/'. RSVP_ICONS_PATH;
    $dirs = file_scan_directory($iconsdirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($dirs as $filename => $icon) {
      $available_iconsets[$icon->basename] = $iconsdirectory . '/' . $icon->basename;
    }
  	
    //2) collect all icons under .../files/rsvp/icons/ICONSET
    $iconsdirectory = file_directory_path() . '/' . RSVP_FILES_ICONS_PATH;
    $dirs = file_scan_directory($iconsdirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($dirs as $filename => $icon) {
      $available_iconsets[$icon->basename] = $iconsdirectory . '/' . $icon->basename;
    }
  }
  
  return $available_iconsets;
}

/*
 * returns the path to the selected backgroundimage
 *
 * @param $backgroundimagename The name of the backgroundimage
 * @return String path to the backgroundimage from root or false.
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/backgrounds/  (backgroundimage that come with the module).
 *      - sites/default/files/rsvp/backgrounds/ (backgroundimage that the user provides).
 *
 * If the file can not be found, fall back to the default backgroundimage.
 * 
 */
function rsvp_function_getBackgroundImage($backgroundimagename) {

  $available_images = rsvp_function_getBackgroundImages();
  
  //1) find the right backgroundimage
  foreach($available_images as $image => $dir) {
    if ($image == $backgroundimagename) {
      return $dir;
    }
  }
  
  //avoid recursion in case theme default is not available
  if ($backgroundimagename == RSVP_BACKGROUNDIMAGE_DEFAULT) 
    return false;
    
  //2) If the name does not match, fallback to the default
  return rsvp_function_getBackgroundImage(RSVP_BACKGROUNDIMAGE_DEFAULT);
  
  return false; 
}


/*
 * collect all backgroundimages that are available.
 *
 * @return array with pairs of backgroundimagename => backgroundimagepath (the part that is userdefined)
 *  
 * backgroundimages can be stored in two locations
 *      - modules/rsvp/backgrounds/  (background images that come with the module).
 *      - sites/default/files/rsvp/backgrounds/ (background images that the user provieds).
 *
 */
function rsvp_function_getBackgroundImages() {

  static $available_backgroundimages = NULL;

  if (is_null($available_backgroundimages)) {
    $bp = base_path();
    
    $available_backgroundimages = array();
    $dir_regex = '^.*';
    $ignore = array('.', '..', 'CVS');
    
    //1) collect all images under sites/all/modules/rsvp/backgrounds/
    $imagedirectory = drupal_get_path('module', 'rsvp') .'/'. RSVP_BACKGROUNDIMAGE_PATH;
    $files = file_scan_directory($imagedirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($files as $filename => $image) {
      $available_backgroundimages[$image->name] = $bp . $image->filename;
    }
    
    //2) collect all images under .../files/rsvp/backgrounds/
    $imagedirectory = file_directory_path() . '/' . RSVP_FILES_BACKGROUNDIMAGE_PATH;
    $files = file_scan_directory($imagedirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($files as $filename => $image) {
      $available_backgroundimages[$image->name] = $bp . $image->filename;
    }
  }
  
  return $available_backgroundimages;
}


/*
 * returns the path to the selected image
 *
 * @param $imagename The name of the image
 * @return String path to the image from root.
 *  
 * Themes can be stored in two locations
 *      - modules/rsvp/images/  (image that come with the module).
 *      - sites/default/files/rsvp/images/ (image that the user provides).
 *
 * If the file can not be found, fall back to the default image.
 * 
 */
function rsvp_function_getImage($imagename) {

  $available_images = rsvp_function_getImages();
  
  //1) find the right image
  foreach($available_images as $image => $dir) {
    if ($image == $imagename) {
      return $dir;
    }
  }
  
  //avoid recursion in case theme default is not available
  if ($imagename == RSVP_IMAGE_DEFAULT) 
    return false;
    
  //2) If the name does not match, fallback to the default
  return rsvp_function_getImage(RSVP_IMAGE_DEFAULT);
  
  return false; 
}




/*
 * collect all images that are available.
 *
 * @return array with pairs of imagename => imagepath (the part that is userdefined)
 *  
 * images can be stored in two locations
 *      - modules/rsvp/images/  (images that come with the module).
 *      - sites/default/files/rsvp/images/ (images that the user provieds).
 *
 */
function rsvp_function_getImages() {

  static $available_images = NULL;


  if (is_null($available_images)) {
    $available_images = array();
    $dir_regex = '^.*';
    $ignore = array('.', '..', 'CVS');
    
    //1) collect all images under sites/all/modules/rsvp/images/
    $imagedirectory = drupal_get_path('module', 'rsvp') .'/'. RSVP_IMAGE_PATH;
    $files = file_scan_directory($imagedirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($files as $filename => $image) {
      $available_images[$image->name] = $image->filename;
    }
    
    //2) collect all images under .../files/rsvp/images/
    $imagedirectory = file_directory_path() . '/' . RSVP_FILES_IMAGE_PATH;
    $files = file_scan_directory($imagedirectory, $dir_regex, $ignore, 0, FALSE);      
    foreach ($files as $filename => $image) {
      $available_images[$image->name] = $image->filename;
    }
  }
  
  return $available_images;
}

