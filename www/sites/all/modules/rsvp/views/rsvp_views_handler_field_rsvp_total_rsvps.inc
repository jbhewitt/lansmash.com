<?php
// $Id: rsvp_views_handler_field_rsvp_total_rsvps.inc,v 1.1.2.1 2009/03/07 21:34:35 ulf1 Exp $




/**
 * Handler for total RSVPs field.
 */

class rsvp_views_handler_field_rsvp_total_rsvps extends views_handler_field {

  function query() {

  	$this->ensure_my_table();
    $formula = 'COUNT(DISTINCT ' . $this->table_alias . '.rid)';
    $this->query->add_field(NULL, $formula, $this->table_alias . '_total_rsvps');
    $this->query->add_groupby('nid');
  }
	
  function render($values) {

    $item = !empty($values->{$this->table_alias . '_total_rsvps'}) ? $values->{$this->table_alias . '_total_rsvps'} : 0;
    return $item;
  }
}

