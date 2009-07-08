<?php
// $Id: rsvp_views_handler_field_rsvp_name.inc,v 1.1.2.2 2009/03/01 22:26:13 ulf1 Exp $


/**
 * Handler for RSVP name (=title) field.
 */

class rsvp_views_handler_field_rsvp_name extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['rid'] = array('table' => 'rsvp', 'field' => 'rid');
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['alter']['contains']['make_link'] = array('default' => TRUE);
    return $options;
  }
  
  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (($this->options['alter']['make_link'] == TRUE) && $data !== NULL && $data !== '') {
      $this->options['alter']['path'] = "rsvp/" . $values->{$this->aliases['rid']};
    }
    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
  
  
  
	
/*  function render($values) {
    $rid = $values->{$this->aliases['rid']};
    $name = $values->{$this->aliases['rname']};
    
    return l($name, 'rsvp/'. $rid . '/view', array('attributes' => array('title' => 'View Invitation')));
  }
*/
}

