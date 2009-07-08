<?php
// $Id: rsvp_views_handler_field_rsvp_invite_total_invites.inc,v 1.1.2.1 2009/03/07 21:34:35 ulf1 Exp $

/**
 * Handler for total invites field.
 */

class rsvp_views_handler_field_rsvp_invite_total_invites extends views_handler_field {

  function query() {

    $this->ensure_my_table();
    $formula = 'COUNT(' . $this->table_alias . '.hash' . ')';
    $this->query->add_field(NULL, $formula, $this->table_alias . '_total_invites');
    $this->query->add_groupby('rsvp.nid');
  }
    
  function render($values) {

    $item = !empty($values->{$this->table_alias . '_total_invites'}) ? $values->{$this->table_alias . '_total_invites'} : 0;
    return $item;
  }
}
