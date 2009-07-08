<?php
// $Id: rsvp_views_handler_field_rsvp_rid.inc,v 1.1.2.1 2009/06/01 23:20:32 ulf1 Exp $

class rsvp_views_handler_field_rsvp_rid extends views_handler_field {

  function init(&$view, &$data) {
    parent::init($view, $data);
    if (!empty($this->options['link_to_invitation'])) {
      $this->additional_fields['rid'] = 'rid';
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_invitation'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_invitation'] = array(
      '#title' => t('Link this field to its invitation'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['link_to_invitation'],
    );
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_invitation']) /*&& user_access('access user profiles')*/ && $values->{$this->aliases['rid']} && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "rsvp/" . $values->{$this->aliases['rid']};
    }
    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}
