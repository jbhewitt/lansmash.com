<?php
// $Id: rsvp_views_handler_field_rsvp_startdate.inc,v 1.1.2.4 2009/03/10 17:18:55 ulf1 Exp $


/**
 * Handler for RSVP startdate field.
 */

class rsvp_views_handler_field_rsvp_startdate extends views_handler_field_date {

  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
  }
  
  function render($values) {
    $connector = new RsvpConnector();

    $node = node_load($values->{$this->aliases['nid']});
    if ($node == false)
      return '';
           
    $fieldname = $this->field_alias;  
    $field = $connector->get_event_field($node->type);

    if (!isset($fieldname) ||  !isset($field))
      return '';
    
    $pos = $connector->get_posByHash($node, $field, $values->{$fieldname});
    if ($pos >= 0)
        $startdate = $connector->get_startdateAsString($node, $field, $pos);
      else
        $startdate = (t('UNKNOWN STARTDATE: %startdate', array('%startdate' => $values->{$fieldname})));
   
    return $startdate;
  }
  
}

