<?php
// $Id: domain_views_handler_filter_domain_domain_id.inc,v 1.3 2009/05/01 19:51:16 agentken Exp $

/**
 * @file
 * Interface between domain_views.module and views.module.
 *
 * @ingroup domain_views
 */

/**
 * Filter for domain ids stored in the database, currently active domain and any domain
 */
class domain_views_handler_filter_domain_domain_id extends views_handler_filter_in_operator  {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Domain');
      $domain_options = array('***CURRENT_DOMAIN***' => t('Current Domain'));
      $domains = domain_domains();
      foreach ($domains as $domain) {
        $domain_options[$domain['domain_id']] = $domain['sitename'];
        }
      $this->value_options = $domain_options;
    }
  }
}
