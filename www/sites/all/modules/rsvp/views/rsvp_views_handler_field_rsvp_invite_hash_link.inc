<?php
// $Id: rsvp_views_handler_field_rsvp_invite_hash_link.inc,v 1.1.2.1 2009/06/01 23:20:31 ulf1 Exp $

class rsvp_views_handler_field_rsvp_invite_hash_link extends views_handler_field {
  
  function construct() {
    parent::construct();
      $this->additional_fields['hash'] = 'hash';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
      return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
      $form['text'] = array(
        '#type' => 'textfield',
        '#title' => t('Text to display'),
        '#default_value' => $this->options['text'],
      );
  }

   // An example of field level access control.
  //function access() {
    
    //return user_access('access user profiles');
  //}
  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $hash = $values->{$this->aliases['hash']};
    return l($text, "rsvp/email/$hash/view");
  }
}
