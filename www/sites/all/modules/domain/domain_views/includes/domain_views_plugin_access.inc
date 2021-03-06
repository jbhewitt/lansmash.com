<?php

// $Id: domain_views_plugin_access.inc,v 1.1 2009/05/01 19:45:21 agentken Exp $

/**
 * @file
 *  Domain Views plugin that restricts View display based on the current domain.
 *  This plugin respects hook_domaingrants().

/**
 * Access plugin that provides permission-based access control.
 */
class domain_views_plugin_access extends views_plugin_access {
  function access($account) {
    $grants = domain_views_get_grants($account);
    foreach ($grants['domain_id'] as $grant) {
      if ($grant == 0) {
        $grant = -1;
      }
      if (in_array($grant, array_filter($this->options['domains']))) {
        return TRUE;
      }
    }
    return FALSE;
  }

  function get_access_callback() {
    return array('domain_views_access', array(array_filter($this->options['domains'])));
  }

  function summary_title() {
    return t('Domains');
  }

  function option_defaults(&$options) {
    $options['domains'] = array(-1);
  }

  function options_form(&$form, &$form_state) {
    $domains = domain_domains();
    $options = array();
    foreach ($domains as $domain) {
      // Checkboxes cannot handles zeros.
      if ($domain['domain_id'] == 0) {
        $domain['domain_id'] = -1;
      }
      $options[$domain['domain_id']] = check_plain($domain['sitename']);
    }
    $type = 'checkboxes';
    if (count($options) > 20) {
      $type = 'select';
    }
    $form['domains'] = array(
      '#type' => $type,
      '#multiple' => TRUE,
      '#options' => $options,
      '#title' => t('Domains'),
      '#default_value' => $this->options['domains'],
      '#description' => t('Only users with the selected permission flag will be able to access this display. Note that users with "access all views" can see any view, regardless of other permissions.'),
    );
  }
}
