<?php
/* $Id: og_rsvp.views.inc,v 1.1 2008/10/16 02:20:52 mradcliffe Exp $ */

 function og_rsvp_views_data()
 {
	$data['og_rsvp']['table']['group'] = t('Organic groups');

	$data['og_rsvp']['table']['join'] = array(
		'node' => array(
			'left_field' => 'nid',
			'field' => 'nid',
			'extra' => " {node}.type = 'event' ",
		),
	);

	$data['og_rsvp']['status'] = array(
		'title' => t('OG RSVP: Status'),
		'help' => t('The integer value for the event attendee status.'),
		'field' => array(
			'handler' => 'views_handler_field_numeric',
			'click sortable' => TRUE,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_numeric',
		),
	);


	$data['og_rsvp']['uid'] = array(
		'title' => t('OG RSVP: User Id'),
		'help' => t('The user id associated with the event attendee status and node'),
		'field' => array(
			'handler' => 'views_handler_field',
			'click sortable' => TRUE,
		),
		'relationship' => array(
			'handler' => 'views_handler_relationship',
			'base' => 'users',
			'relationship field' => 'uid',
			'label' => t('OG RSVP: Users'),
		),
	);
	return $data;

 } // function og_rsvp_views_data

