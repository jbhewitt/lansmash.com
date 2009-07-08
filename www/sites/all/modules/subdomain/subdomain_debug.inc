<?php

define('SUBDOMAIN_CHECK_DNS', 0);
define('SUBDOMAIN_CHECK_APACHE', 1);
define('SUBDOMAIN_CHECK_URL_INBOUND', 2);
define('SUBDOMAIN_CHECK_URL_OUTBOUND', 3);
define('SUBDOMAIN_CHECK_COOKIE_DOMAIN', 4);

function subdomain_check_result($error) {
  $file_suffix = $error ? 'error' : 'success';
  $suffix = $error ? '<b style="color:red">FAILED</b>' : '<b style="color:green">PASSED</b>';
  return theme('image', drupal_get_path('module','subdomain') ."/images/icon_$file_suffix.png",NULL,NULL,array('style'=>"float:left;margin-right:5px;")) ." $suffix";
}

function subdomain_check_server() {
  $output = "SUBDOMAIN TEST\n";
  $output.= $_SERVER['HTTP_HOST'] ."\n";
  $output.= ltrim(request_uri(),'/') ."\n"; // Use Drupal API's request URI, since REQUEST_URI Apache only
  $output.= ltrim($_SERVER['QUERY_STRING'], 'q=') ."\n";
  $output.= $_SERVER['SCRIPT_FILENAME'] ."\n";
  $output.= drupal_get_normal_path(ltrim(request_uri(),'/'));
  print $output;
  exit;
}

function subdomain_check_apache_details($url, $results) {
  $rows[] = array('HTTP_HOST:', $results[1]);
  $rows[] = array('QUERY_STRING:', $results[2]);
  $rows[] = array('REQUEST_URI:', $results[3]);
  $rows[] = array('SCRIPT_FILENAME:', $results[4]);
  $rows[] = array('drupal_get_normal_path:', $results[5]);
  $output = "<p>HTTP request to $url returned the following result:</p>";
  $output.= theme('table', array(), $rows);
  return $output;
}

function subdomain_check_watchdog_lastid() {
  return db_result(db_query("SELECT max(wid) FROM {watchdog}"));
}

function subdomain_check_server_results($results = NULL) {
  static $test_results;
  if (isset($results)) {
    $test_results = explode("\n", $results);
  }
  return $test_results;
}

function subdomain_check_apache() {
  $domain = subdomain_base_domain();
  $host = "subdomain-test.$domain";
  $url = "http://$host/subdomain/test";

  // Store watchdog count so we can check for errors after http attempt
  $old_watchdog_lastid = subdomain_check_watchdog_lastid();

  $page = file_get_contents($url,"r");
  $lines = subdomain_check_server_results($page);

  // Check for any errors (assumes 1 error for now)
  $new_watchdog_lastid = subdomain_check_watchdog_lastid();
  if ($new_watchdog_lastid > $old_watchdog_count) {
    $result = db_result(db_query("SELECT variables FROM {watchdog} WHERE wid = %d AND type = 'php'", $new_watchdog_lastid));
    $error = unserialize($result);
    $error404 = strpos($error['%message'], '404') !== FALSE ? TRUE : FALSE;
  }

  $result = subdomain_check_apache_details($url, $lines);
  if ($lines[0] == 'SUBDOMAIN TEST' && $lines[1] == $host && $lines[3] == 'subdomain/test') {
    return array('error' => FALSE, 'output' => $result);
  }
  else {
    if ($lines[0] != 'SUBDOMAIN TEST') {
      if ($error404) {
        $output = "<p>$url returned a '404 Page not found' error.</p>";
      }
      else {
        $output.= $result;
      }
      $output.= '<p>Your web server is not routing subdomains to your Drupal directory.</p>';
      $output.= "<strong>Possible Solutions:</strong>";
      $output.= "<ul><li>If you're running Apache, confirm you have the wildcard directive, <b>ServerAlias *.$domain</b>, in the VirtualHost declaration in your Apache configuration";
      $output.= "<li>Confirm there are no rogue URL rewrite rules in your Apache configuration or .htaccess file</ul>";
    }
    elseif ($lines[1] != $host) {
      $output = $result;
      $output.= "Your web server erased the subdomain and rewrote <b>$host</b> as <b>$domain</b>.";
      $output.= "<p>Possible solutions:</p>";
      $output.= "<ul><li>Check your web server configuration and/or .htaccess for renegade redirect or urlrewrite statements";
      $output.= "<li>The subdomain module's .htaccess patch conflicts with the globalredirect module. You may need to disable one or the other if you are using Apache and globalredirect.";
    }
    elseif ($lines[3] != 'subdomain/test') {
      $output = $result ."The REQUEST_URI should be 'subdomain/test'.";
      $output.= "<p>It's possible your web server is rewriting your URLs incorrectly.</p>";
      $output.= "<p>Possible solutions:</p>";
      $output.= "<ul><li>Confirm you don't have any renegade rewrite rules in your web server configuration.";
      $output.= "<li>Read through Readme.txt and make sure you've completed all the installation steps correctly</ul>";
    }
    return array('error' => TRUE, 'output' => $output);
  }
}

function subdomain_check_url_rewrite_inbound() {

  $server_results = subdomain_check_server_results();
  $internal_path = $server_results[5];
  $url = "http://subdomain-test.". subdomain_base_domain() ."/subdomain/test";

  if ($internal_path == '~subdomain-test/subdomain/test') {
    $output = "HTTP request to $url resulted in an internal Drupal path of $internal_path";
    return array('error' => FALSE, 'output' => $output);
  }
  else {
    $output = "HTTP request to $url resulted in an internal Drupal path of $internal_path. It should have been ~subdomain-test/subdomain/test<p />";
    if (!function_exists('custom_url_rewrite_inbound')) {
      return array('error' => TRUE, 'output' => $output .'The custom_url_rewrite_inbound() function is missing from settings.php. It appears you haven\'t yet patched settings.php. See readme.txt for instructions.');
    }
    else {
      return array('error' => TRUE, 'output' => $output .'The custom_url_rewrite_inbound() function in settings.php is rewriting the internal path incorrectly. Did you patch settings.php? See readme.txt for instructions.');
    }
  }

}


function subdomain_check_url_rewrite_outbound() {

  $correct_url = "http://subdomain-test.". subdomain_base_domain() ."/subdomain/test";
  $drupal_url = url('~subdomain-test/subdomain/test');
  
  if ($drupal_url == $correct_url) {
    $output = "Drupal API: url('~subdomain-test/subdomain/test') returned '$drupal_url'";
    return array('error' => FALSE, 'output' => $output);
  }
  else {
    $output = "Drupal API: url('~subdomain-test/subdomain/test') returned '$drupal_url' but should have returned '$correct_url'<p />";
    if (!function_exists('custom_url_rewrite_outbound')) {
      return array('error' => TRUE, 'output' => $output .'The custom_url_rewrite_outbound() function is missing from settings.php. It appears you haven\'t yet patched settings.php. See readme.txt for instructions.');
    }
    else {
      return array('error' => TRUE, 'output' => $output .'The custom_url_rewrite_outbound() function in settings.php is rewriting subdomains incorrectly. Did you patch settings.php? See readme.txt for instructions.');
    }
  }
}

function subdomain_check_cookie_domain() {
  $settings = file_get_contents(conf_path() . '/settings.php');
  $settings = str_replace(chr(13).chr(10), "\n", $settings);
  $settings = explode("\n",$settings);

  foreach ($settings as $setting) {
    if (strpos($setting, '$cookie_domain') !== FALSE) {
      eval($setting);
      break;
    }
  }

  if (isset($cookie_domain)) {
    return array('error' => FALSE, 'output' => 'The $cookie_domain variable in settings.php is set to \''. $cookie_domain .'\'');
  }
  else {
    return array('error' => TRUE, 'output' => 'The $cookie_domain variable in settings.php is not set. Uncomment and set the $cookie_domain variable to your base domain (e.g. "example.com")');
  }
}

function subdomain_toggle_link($title, $id) {
  return l($title,'',array('attributes'=>array('onclick'=>'$("#'. $id .'").toggle("slow");return false;')));
}

function subdomain_check_show_details($show) {
  if (!$show) {  
    return 'style="display:none"';
  }
}

function subdomain_check_requirements() {
  // theme_status_report relies on constants in install.inc
  include_once './includes/install.inc';

  $data = subdomain_check_wildcard_dns();

  $requirements[SUBDOMAIN_CHECK_DNS] = array(
    'title' => 'DNS server - '. subdomain_toggle_link('show/hide details', 'check-dns'),
    'error' => $data['error'],
    'value' => $data['error'] ? 'Not working yet' : 'Working',
    'severity' => $data['error'] ? 2 : 0,
    'description' => '<div id="check-dns" '. subdomain_check_show_details($data['error']) . '>'. $data['output'] .'</div>',
  );

  if ($requirements[SUBDOMAIN_CHECK_DNS]['error']) {
    $requirements[SUBDOMAIN_CHECK_APACHE] = array(
      'title' => 'Web server',
      'value' => 'unable to test',
      'severity' => 1,
      'description' => "Unable to test Apache settings until DNS wildcard subdomains are working",
    );
  }
  else {
    $data = subdomain_check_apache();
    $requirements[SUBDOMAIN_CHECK_APACHE] = array(
      'title' => 'Web server - '. subdomain_toggle_link('show/hide details', 'check-apache'),
      'value' => $data['error'] ? 'Not working yet' : 'Working',
      'severity' => $data['error'] ? 2 : 0,
      'description' => '<div id="check-apache" '. subdomain_check_show_details($data['error']) . '>'. $data['output'] .'</div>',
    );
  }
  $data = subdomain_check_url_rewrite_inbound();
  $requirements[SUBDOMAIN_CHECK_URL_INBOUND] = array(
    'title' => "Drupal's inbound URL rewriting - ". subdomain_toggle_link('show/hide details', 'check-url-rewrite-inbound'),
    'value' => $data['error'] ? 'Not working yet' : 'Working',
    'severity' => $data['error'] ? 2 : 0,
    'description' => '<div id="check-url-rewrite-inbound" '. subdomain_check_show_details($data['error']) . '>'. $data['output'] .'</div>',
  );
  $data = subdomain_check_url_rewrite_outbound();
  $requirements[SUBDOMAIN_CHECK_URL_OUTBOUND] = array(
    'title' => "Drupal's outbound URL rewriting - ". subdomain_toggle_link('show/hide details', 'check-url-rewrite-outbound'),
    'value' => $data['error'] ? 'Not working yet' : 'Working',
    'severity' => $data['error'] ? 2 : 0,
    'description' => '<div id="check-url-rewrite-outbound" '. subdomain_check_show_details($data['error']) . '>'. $data['output'] .'</div>',
  );
  $data = subdomain_check_cookie_domain();
  $requirements[SUBDOMAIN_CHECK_COOKIE_DOMAIN] = array(
    'title' => '$cookie_domain in settings.php - '. subdomain_toggle_link('show/hide details', 'check-cookie-domain'),
    'value' => $data['error'] ? 'Not yet set' : 'Set',
    'severity' => $data['error'] ? 2 : 0,
    'description' => '<div id="check-cookie-domain" '. subdomain_check_show_details($data['error']) . '>'. $data['output'] .'</div>',
  );

  return theme('status_report', $requirements);
}

function subdomain_check_wildcard_dns() {
  $error = FALSE;

  $base_domain = subdomain_base_domain();
  $domains_to_test = array("test-1.$base_domain","test-2.$base_domain", "test-3.$base_domain");

  foreach($domains_to_test as $subdomain) {
    $result = gethostbyname($subdomain);
    if ($result === $subdomain) {
      $output.= "<div>$subdomain <span style='color:red'>failed to resolve</span></div>";
      $error = TRUE;
    }
    else {
      $output.= "<div>$subdomain <span style='color:green'> successfully resolved to $result</span></div>";
    }
  }
  if ($error) {
    $output.= "<p>It appears your DNS server is not setup to handle wildcard subdomains (*.$base_domain) for $base_domain. Edit the DNS zone file or settings for your server to fix.</p>";
    $output.= "<p>PHP caches DNS lookups, so you'll want to restart your web server after making changes to your DNS server to see the results of your changes on this page.</p>";
  }
  return array('error' => $error, 'output' => $output);
}