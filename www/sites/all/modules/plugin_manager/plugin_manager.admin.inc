<?php
// $Id: plugin_manager.admin.inc,v 1.52.2.60 2008/11/26 22:45:39 jabapyth Exp $

/**
 * @file
 * The Forms used by the plugin manager.
 */

/**
 * FAPI for overview/search page.
 */
function plugin_manager_find($form_state) {
  if (!plugin_manager_runnable()) {
    return array();
  }

  // If the repository is over 24 hours old then update it.
  if (variable_get("plugin_manager_last_reload", 0) + 604800 < date('U')) {
    drupal_set_message(l(t("The repository might be out of date.  Update it by clicking here."), "admin/plugin_manager/reload"));
  }
  
  $form['as'] = array(
    '#type' => 'item',
    '#title' => '',
    '#value' => l(t('Try the AJAX Search'),'admin/plugin_manager/search'),
  );
  $form['search_box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['search_box']['search'] = array(
    '#title' => t('Search by name'),
    '#type' => 'textfield',
  );
  $form['search_box']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'select',
    '#options' => array('None' => 'No Filter Selected', 'Modules' => 'Modules', 'Themes' => 'Themes'),
  );

  // Get a list of all of the tags.
  $form['browse_box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browse'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['browse_box']['category'] = array(
    '#title' => t('Browse by category'),
    '#type' => 'select',
    '#options' => array('None' => 'Categories:'),
  );
  $result = db_query(
    "SELECT tag
     FROM {plugin_manager_taxonomy}
     WHERE tag NOT IN('Modules', 'Translations', 'Themes')
     GROUP BY tag
     ORDER BY tag"
  );
  while ($row = db_fetch_array($result)) {
    $form['browse_box']['category']['#options'][$row['tag']] = $row['tag'];
  }
  $form[] = array(
    '#type' => 'submit',
    '#value' => t('View Matches'),
  );

  // Get a list of queued changes.
  $queue = variable_get("plugin_manager_queue", array());

  // Make a list that shows the queue.
  if (!empty($queue)) {
    $form['queue_box'] = array(
      '#type' => 'fieldset',
      '#title' => t('Install Queue'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    foreach ($queue AS $item) {
      $result = db_fetch_array(db_query(
        "SELECT title
         FROM {plugin_manager_repository}
         WHERE short_name = '%s'", $item));
      $form['queue_box']['plugin_'. $item] = array(
        '#type' => 'checkbox',
        '#name' => 'plugin_'. $item,
        '#title' => htmlentities($result['title']),
        '#default_value' => TRUE,
      );
    }
  }

  // If we are searching for something
  if (isset($form_state['storage']['search'])) {
    $form['browse_box']['#collapsed'] = TRUE;
    $form['search_box']['#collapsed'] = TRUE;

    // Get a list of all of the tags.
    $form['result_box'] = array(
      '#type' => 'fieldset',
      '#title' => t('Results'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    // Build the where clause.
    $where = array('1');
    $terms = array();

    // If the search string is non-empty, then add it to our where clause.
    if (!empty($form_state['storage']['search'])) {
      $where[] = "title LIKE '%%%s%%'";
      $terms[] = $form_state['storage']['search'];
    }

    // See if a filter has been selected.
    if ($form_state['storage']['filter'] != 'None') {
      $where[] = "tag = '%s'";
      $terms[] = $form_state['storage']['filter'];
    }
    elseif ($form_state['storage']['category'] != 'None') {
      $where[] = "tag = '%s'";
      $terms[] = $form_state['storage']['category'];
    }

    $where = implode(' AND ', $where);

    // Get a list of the non-installed plugins from the category.
    $result = db_query(
       "SELECT title, pmr.short_name AS short_name
        FROM {plugin_manager_repository} pmr
        LEFT JOIN {plugin_manager_taxonomy} pmt
        ON(pmt.short_name = pmr.short_name)
        WHERE $where
        ORDER BY title", $terms[0], $terms[1]
    );
  
    $files = module_rebuild_cache();
  
    while ($row = db_fetch_array($result)) {
      if (isset($files[$row['short_name']]))continue;
      $form['result_box']['plugin_'. $row['short_name']] = array(
        '#type' => 'checkbox',
        '#name' => 'plugin_'. $row['short_name'],
        '#title' => htmlentities($row['title']),
        '#default_value' => isset($queue[$row['short_name']]),
      );
    }
  }

  // If the queue box or result box shows, then show the submit button.
  if (isset($form_state['storage']['search']) OR !empty($queue)) {
    $form['queue'] = array(
      '#type' => 'submit',
      '#value' => t('Queue for Installation'),
    );
  }
  return $form;
}

function plugin_manager_find_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'View Matches') {
    $form_state['storage']['search'] = $form_state['values']['search'];
    $form_state['storage']['filter'] = $form_state['values']['filter'];
    $form_state['storage']['category'] = $form_state['values']['category'];
    return;
  }

  // See what is already queued.
  $queue = variable_get("plugin_manager_queue", array());

  // Go through each value in the list.
  foreach ($form_state['values'] AS $theme => $value) {
    if (substr($theme, 0, strlen('plugin_')) != 'plugin_') {
      continue;
    }
    $theme = substr($theme, strlen('plugin_'));
    if ($value) {
      // Add it to the queue if needed.
      $queue[$theme] = $theme;
    }
    else {
      // Remove it from the queue otherwise.
      unset($queue[$theme]);
    }
  }

  // Set the new queue equal to our changed queue.
  variable_set("plugin_manager_queue", $queue);
  drupal_set_message(t("The queue has been updated. ") . l(t('Click Here to install your queued plugins.'), 'admin/plugin_manager/install'));
}

function plugin_manager_modules_redirect() {
    drupal_set_message("Search for the modules you want to install, add them to your queue, and then install them.");
    drupal_goto('admin/plugin_manager/search');
}

function plugin_manager_themes_redirect() {
    drupal_set_message("Search for the themes you want to install, add them to your queue, and then install them.");
    drupal_goto('admin/plugin_manager/search');
}

/**
 * FAPI callback for the manual plugin manager installer.
 */
function plugin_manager_manual($form_state) {
  drupal_set_message(t('WARNING: This manual install page is to be used with great caution. *Always* inspect an archive before you upload it to your site if you are unsure of the source. It is suggested that you not use this page during normal operations, but use instead the ') . l('regular install page', 'admin/plugin_manager/install'), 'error');
  drupal_set_title('Manual Install');
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['tarball'] = array(
    '#type' => 'file',
    '#title' => t('Plugin Tarball'),
  );
  $form['backend'] = array(
    '#title' => t('Installation method'),
    '#type' => 'select',
    '#options' => plugin_manager_backends(),
  );
  $form['host'] = array(
    '#title' => t('Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => '',
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => 'modules',
    '#options' => array('themes' => 'Theme', 'modules' => 'Module'),
    '#description' => t('This lets the system know where to install the plugin.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Install'),
  );

  return $form;
}

/**
 * Submit callback for plugin_manager_manual().
 */
function plugin_manager_manual_submit($form, &$form_state) {
  $backend_list = plugin_manager_backends();
  $backend = $backend_list[$form_state['values']['backend']];
  $file = file_save_upload('tarball');
  $host = $form_state['values']['host'];
  $user = $form_state['values']['username'];
  $pass = $form_state['values']['password'];
  $type = $form_state['values']['type'];

  // Get the file
  if (!$file) {
    drupal_set_message(t('File could not be uploaded.'), 'error');
    return;
  }

  // Then finally install it.
  $files = plugin_manager_untar($file->filepath);
  //plugin_manager_ftp_copy($extracted, $type, $host, $user, $pass);
  
  // Copy the extracted files
  $copy = call_user_func_array($backend .'_plugin_manager_copy',
    array($files, $type, $host,
    $user, $pass));
    
  // If it failed, stop now.
  if (!$copy) {
    drupal_set_message(t("Unable to install ") . $name, 'error');
    return;
    }
    
  foreach (array_reverse($files) AS $file) {
    // Mark files as installed
    if ($copy) {
      db_query("INSERT INTO {plugin_manager_files} VALUES('%s', '%s')",
        $name, $type .'/'. $file);
    }

    // Remove the extracted files.
    $file = file_directory_path() .'/plugin_manager_extraction/'. $file;
    if (is_dir($file)) {
      rmdir($file);
    }
    else {
      unlink($file);
    }
  }
    
  drupal_set_message(t("Successfully installed plugin.") . l(t('Go Here'), 'admin/build/'. strtolower($type)) . t(' to enable it.'));
}


/**
 * FAPI for the first page of the installer
 *
 * @ingroup forms
 * @see plugin_manager_install_1()
 */
function plugin_manager_page_1() {
  $queue_info = variable_get("plugin_manager_queue_info", array());

  // Put a version drop down for each of the files
  foreach ($queue_info AS $project => $info) {
    $form[$project .'_version'] = array(
      '#title' => t('%title version', array('%title' => $info['title'])),
      '#type' => 'select',
      '#options' => $info['version'],
    );
  }

  // Put the continue button up.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue to Step 2'),
  );
  return $form;
}

/**
 * FAPI for the second page of the installer
 *
 * @ingroup forms
 * @see plugin_manager_install_2()
 */
function plugin_manager_page_2($form_state) {
  // On to page two.
  $queue_info = variable_get("plugin_manager_queue_info", array());

  $link_attr['attributes']['target'] = 'blank';

  // Make each one of the md5sum boxes.
  foreach ($queue_info AS $name => $plugin) {
    $version = $form_state['storage'][1][$name .'_version'];
    $link = $plugin['release'][$version]['release_link'];
    $form[$name .'_md5sum'] = array(
      '#title' => t('%plugin_title md5_file hash (copy/paste from above or <a href="!uri">available here</a>)', array('%plugin_title' => $plugin['title'], '!uri' => url($link, $link_attr))),
      '#type' => 'textfield',
      '#size' => 32,
      '#prefix' => "<iframe src='$link' style='width:100%;height:260px;'></iframe>",
      '#required' => TRUE,
    );
  }

  // Put the continue button up.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue to Step 3'),
  );
  return $form;
}

/**
 * FAPI for the third page of the installer
 *
 * @ingroup forms
 * @see plugin_manager_install_3()
 */
function plugin_manager_page_3($form_state) {
  $form['backend'] = array(
    '#title' => t('Installation method'),
    '#type' => 'select',
    '#options' => plugin_manager_backends(),
  );
  $form['host'] = array(
    '#title' => t('Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['save_username'] = array(
    '#title' => t('Save Username'),
    '#type' => 'checkbox',
  );
  $form['password'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Install'),
  );
  return $form;
}

function plugin_manager_page_4($form_state) {
  update_get_available(TRUE); // refresh the update database
  $form[] = array(
    '#value' => t("Now you're done.  Unless it is stated otherwise above, your modules and/or themes should be properly installed now.  So what should you do now? ") .'<p>'. l(t("If you installed new modules, you can enable them here."), 'admin/build/modules') .'</p><p>'. l(t('If you installed new themes, you can select them here.'), 'admin/build/themes') .'</p>',
  );
  return $form;
}

/**
 * A page to list all of the queued changes and begin installation of them.
 */
function plugin_manager_install_form($form_state) {
  // Make sure that the plugin manager can be run.
  if (!plugin_manager_runnable()) {
    return array();
  }

  // Show the congrats page.
  if (isset($form_state['storage']['page']) AND $form_state['storage']['page'] == 4) {
    return plugin_manager_page_4($form_state);
  }

  // If there aren't any, then just tell the just that.  And quit.
  $queue = variable_get("plugin_manager_queue", array());
  if (empty($queue)) {
    drupal_set_message("Nothing has been selected to install. ".l("Search for modules to install","admin/plugin_manager/search"));
    return array();
  }

  // @todo Examine this code.
  // Get the total list and grab the release_history for each and set the download links.
  $queue_info = plugin_manager_get_release_history($queue);
  variable_set("plugin_manager_queue_info", $queue_info);

  // If we don't need to select a version, then skip that page.
  $latest = variable_get('plugin_manager_latest_version', 1);
  if ($latest AND !isset($form_state['storage']['page'])) {
    $files = array();
    foreach ($queue_info AS $name => $plugin) {
      $link = $plugin['release'][0]['download_link'];
      $files[$name] = plugin_manager_get($link);
      // Check for an error.
      if ($files[$name] == FALSE) {
        drupal_set_message(t('Could not download @name', array('@name' => $name)), 'error');
        return;
      }
      else {
        drupal_set_message(t('Downloaded @name', array('@name' => $name))); 
      }
    }
    variable_set("plugin_manager_downloaded_files", $files);
    $form_state['storage']['page'] = 2;
    foreach ($queue AS $name) {
      $form_state['storage'][1][$name .'_version'] = 0;
    }
  }

  // Allow the choice of versions on the first page.
  if (!isset($form_state['storage']['page'])) {
    return plugin_manager_page_1();
  }
  elseif ($form_state['storage']['page'] == 2) {
    return plugin_manager_page_2($form_state);
  }
  else {
    return plugin_manager_page_3($form_state);
  }
}

/**
 * Process the first page of the installer, by downloading the needed files.
 */
function plugin_manager_install_1($form, &$form_state) {
  // Download the files.
  $files = array();
  $queue_info = variable_get("plugin_manager_queue_info", array());
  foreach ($queue_info AS $name => $plugin) {
    $version = $form_state['storage'][1][$name .'_version'];
    $link = $plugin['release'][$version]['download_link'];
    $files[$name] = plugin_manager_get($link);

    // Check for an error.
    if ($files[$name] == FALSE) {
      drupal_set_message(t('Could not download @name', array('@name' => $name)), 'error');
      return;
    }
  }
  variable_set("plugin_manager_downloaded_files", $files);

  // Proceed to the next page.
  $form_state['storage']['page'] = 2;
  return;
}

/**
 * Process the second page of the installer by comparing calculated md5sums
 * to the md5sums supplied by the user.
 */
function plugin_manager_install_2($form, &$form_state) {
  $form_state['storage'][2] = $form_state['values'];
  // Compare the calculated and supplied md5sums.
  $files = variable_get("plugin_manager_downloaded_files", array());

  foreach ($files AS $name => $file) {
    $md5 = $form_state['storage'][2][$name .'_md5sum'];
    if (md5_file($file) != $md5) {
      drupal_set_message(t('The md5sum is incorrect for ') . $plugin['title'], 'error');
      $form_state['storage']['page'] = 2;
      return;
    }
  }

  $form_state['storage']['page'] = 3;
  return;
}

/**
 * Process the third page of the installer.  Extract, install, log and
 * clean up the files.
 */
function plugin_manager_install_3($form, &$form_state) {
  // Load the appropriate backend.
  $backend_list = plugin_manager_backends();
  $backend = $backend_list[$form_state['values']['backend']];
  $form_state['storage']['page'] = 3;

  // Save the ftp_host variable
  variable_set("plugin_manager_ftp_hostname", $form_state['values']['host']);
  if ($form_state['values']['save_username'] == '1') {
    variable_set("plugin_manager_username", $form_state['values']['username']);
  }

  // Go through each on of the files.
  $plugins = variable_get("plugin_manager_downloaded_files", array());
  foreach ($plugins AS $name => $plugin) {
    // Extract the files
    $files = plugin_manager_untar($plugin);

    // Find out whether the thing is a module or theme.
    $row = db_fetch_array(
      db_query(
        "SELECT lower(tag) AS tag
         FROM {plugin_manager_taxonomy}
         WHERE short_name = '%s'
         AND tag IN('Modules', 'Themes')", $name
      )
    );

    // Copy the extracted files
    $copy = call_user_func_array($backend .'_plugin_manager_copy',
      array($files, $row['tag'], $form_state['values']['host'],
        $form_state['values']['username'], $form_state['values']['password']));

    foreach (array_reverse($files) AS $file) {
      // Mark files as installed
      if ($copy) {
        db_query("INSERT INTO {plugin_manager_files} VALUES('%s', '%s')",
          $name, $row['tag'] .'/'. $file);
      }

      // Remove the extracted files.
      $file = file_directory_path() .'/plugin_manager_extraction/'. $file;
      if (is_dir($file)) {
        rmdir($file);
      }
      else {
        unlink($file);
      }
    }

    // If it failed, stop now.
    if (!$copy) {
      drupal_set_message(t("Unable to install ") . $name, 'error');
      return;
    }

    drupal_set_message(t("Successfully installed @name.", array('@name' => $name)));
    // rebuild caches
    drupal_rebuild_theme_registry();
    node_types_rebuild();
    menu_rebuild();
    cache_clear_all('schema', 'cache');
    module_rebuild_cache();
    unlink($plugin);
  }

  variable_set("plugin_manager_queue_info", array());
  variable_set("plugin_manager_queue", array());
  $form_state['storage']['page'] = 4;
}

/**
 * Process the data presented in the plugin_manager_install form.
 */
function plugin_manager_install_form_submit($form, &$form_state) {
  // If page one was submitted, download the requested files.
  if (!isset($form_state['storage']['page']) OR ($form_state['storage']['page'] == 1)) {
    $form_state['storage'][1] = $form_state['values'];
    return plugin_manager_install_1($form, $form_state);
  }
  elseif ($form_state['storage']['page'] == 2) {
    return plugin_manager_install_2($form, $form_state);
  }
  return plugin_manager_install_3($form, $form_state);
}

/**
 * FAPI for the uninstall page.
 *
 * Shows a list of packages to the user, allowing them to mark packages for removal.
 * @ingroup forms
 * @see plugin_manager_uninstall_submit()
 */
function plugin_manager_uninstall_form($form_state) {

  // Get a list of the non-installed plugins.
  $result = db_query(
    "SELECT filename,name,type
     FROM {system}
     WHERE status = 0
     ORDER BY type"
  );
  while ($row = db_fetch_array($result)) {
    if (strpos($row['filename'],'sites/all/'.$row['type'].'s/'.$row['name'])!==0 || !file_exists($row['filename']))continue;
    $info_file = join('.',array_slice(explode('.',$row['filename']),0,-1)).'.info';
    $info = drupal_parse_info_file($info_file);
    if (!$info['name'])$info['name'] = $row['name'];
    if (!$form[$row['type']]){
        $form[$row['type']] = array(
        '#type' => 'fieldset',
        '#title' => htmlentities('Disabled '.ucwords($row['type']).'s'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        );
    }
    $form[$row['type']][$row['name']] = array(
      '#type' => 'checkbox',
      '#name' => $row['name'],
      '#title' => htmlentities($info['name']),
    );
  }
  $form['backend'] = array(
    '#title' => t('Removal method'),
    '#type' => 'select',
    '#options' => plugin_manager_backends(),
  );
  $form['host'] = array(
    '#title' => t('Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => '',
  );
  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Uninstall'),
  );
  return $form;
}

/**
 * Remove package files from the server.
 */
function plugin_manager_uninstall_form_submit($form, &$form_state) {
  $backend_list = plugin_manager_backends();
  $backend = $backend_list[$form_state['values']['backend']];

  foreach ($form_state['values'] AS $plugin => $value) {

    // If it isn't supposed to be removed, then skip it.
    if ($value != 1) {
      continue;
    }

    // Get the files to remove.
    /*$result = db_query(
      "SELECT file_path
       FROM {plugin_manager_files}
       WHERE short_name = '%s'
       ORDER BY file_path DESC", $plugin
    );*/
    $row = db_fetch_array(db_query(
        "SELECT filename, name, type
         FROM {system}
         WHERE name='%s'",array($plugin)));
    if (strpos($row['filename'],'sites/all/'.$row['type'].'s/'.$row['name'].'/')!==0 || !file_exists($row['filename'])){
        drupal_set_message("Illegal Uninstall. Please report this bug.");
        return;
    }
    // Remove the files / folders requested.
    $remove = call_user_func_array($backend .'_plugin_manager_remove',
      array('sites/all/'.$row['type'].'s/'.$row['name'], $form_state['values']['host'],
        $form_state['values']['username'], $form_state['values']['password']));
    if ($remove){
        drupal_set_message("Successfully uninstalled $plugin.");
    }else
        drupal_set_message("Unable to uninstall $plugin.",'error');
  }
}

/** Package Update Section **/

/**
 * @callback for admin/plugin_manager/update
 * List packages w/ available updates and give the option to install.
 * Makes use of the core "update" module.
 */
function plugin_manager_update_form() {
  $available = update_get_available(TRUE);
  $projects = update_calculate_project_data($available);

  $some = FALSE;
  foreach ($projects as $name => $project) {
    if ($project['status'] == UPDATE_NOT_CURRENT) {
      $some=TRUE;
      $form[$name] = array(
        '#type' => 'checkbox',
        '#title' => $project['title'],
        '#default_value' => 'true'
      );
    }
  }//}
  if (!$some)
      drupal_set_message("There are no available updates for your drupal installation.");
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Install Updates'
  );
  return $form;
}

function plugin_manager_update_form_submit($form, &$form_state) {
  $queue = array();
  foreach ($form_state['values'] AS $name => $value) {
    if ($value == '1') {
      $queue[$name] = $name;
    }
  }
  if ($queue) {
    drupal_set_message("Queue: ". join(", ", $queue));
    variable_set("plugin_manager_queue", $queue);
    drupal_goto("admin/plugin_manager/install");
  }
}

/**
 * Alert the user to outdated plugins, and give them an opporunity to
 * automatically update them.
 * @alters the system_modules and system_themes pages (admin/modules | themes)
 */
function plugin_manager_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'system_modules' || $form_id == 'system_themes' ) {
    $reqs = update_requirements('runtime');
    if (isset($reqs['update_contrib']['reason'])) {
      drupal_set_message('It looks like you have some outdated modules. '. l('Go Here', 'admin/plugin_manager/update') .' to update your site automatically.');
    }
  }
}

/** Plugin Manager Search Section **/

/**
 * This is the main function for the Ajax Search page
 * of Plugin Manager.
 */
function plugin_manager_search() {
  drupal_add_js(drupal_get_path("module", "plugin_manager") ."/plugin_manager_search.js");
  drupal_add_css(drupal_get_path("module", "plugin_manager") ."/plugin_manager_search.css");

  // If the repository is over 24 hours old then update it.
  if (variable_get("plugin_manager_last_reload", 0) + 604800 < date('U')) {
    drupal_set_message(l(t("The repository might be out of date.  Update it by clicking here."), "admin/plugin_manager/reload"));
  }
  $headers = array('Queue', 'Name', 'Tags', 'Project Page');
  $letters = array('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z');

  $html = t("This is a prototype of an interactive search for the Plugin Manager Module.  Try typing in 'module' or 'image' or 'cck'")."<br/>";
  $html .= "<fieldset><legend>".t("Search")."</legend><input id='sinput'/> <button id='search'>".t("Search")."</button>";
  $html .= t("Filter").": <a id='showall' class='selected' href='javascript:void(0)'>".t('All')."</a> <a id='showmodules' href='javascript:void(0)'>Modules</a> <a id='showthemes' href='javascript:void(0)'>Themes</a>";
  $html .= "</fieldset>";
  $html .= "<fieldset><legend>".t('Browse')."</legend>".t('By Category').": <select onchange='pmsearch.showtag(this.value)'><option> -- Categories -- </option>";

  $result = db_query(
    "SELECT tag
     FROM {plugin_manager_taxonomy}
     WHERE tag NOT IN('Modules')
     GROUP BY tag
     ORDER BY tag"
  );

  while ($row = db_fetch_array($result)) {
    $html .= '<option value="'. $row['tag'] .'">'. $row['tag'] .'</option>';
  }
  $html .= "</select><br/>";
  $html .= "Alphabetical: ";
  for ($i=0; $i<26; $i++) {
    $html .= '<a class="showletter" href="javascript:pmsearch.showletter(\''. $letters[$i] .'\')">'. $letters[$i] .'</a> ';
  }
  $html .= "</fieldset>";

  $html .= "<fieldset><legend><a id='queuelink' href='javascript:void(0)'>Loading...</a></legend>";
  $html .= theme('table', $headers, array(), array('id' => 'queue'));
  $html .= "</fieldset>";

  $html .= drupal_get_form('plugin_manager_search_form');
  $html .= theme('table', $headers, array(), array('class' => 'pmsearch'));

  return $html;
}

function plugin_manager_search_form() {
  $queue = variable_get("plugin_manager_queue",array());
  foreach($queue as $name=>$value){
      $form[$name] = array('#type'=>'hidden','#value'=>'true');
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Install Queued Plugins'));
  return $form;
}

function plugin_manager_search_form_submit($form, &$form_state) {
  $queue = array();
  foreach ($form['#post'] AS $name => $value) {
    if ($value=='true') {
      $queue[$name] = $name;
    }
  }
  if ($queue) {
    drupal_set_message("Queue: ". join(", ", $queue));
    variable_set("plugin_manager_queue", $queue);
    drupal_goto("admin/plugin_manager/install");
  }
}

function plugin_manager_search_ajax_callback(){
    
  $result = db_query(
    "SELECT title, short_name
     FROM {plugin_manager_repository}
     ORDER BY title"
  );
  
  $files = module_rebuild_cache();
  
  while ($row = db_fetch_array($result)) {
    if (isset($files[$row['short_name']]))continue;
    $tags_res = db_query(
      "SELECT pmt.tag AS tag
       FROM {plugin_manager_taxonomy} pmt
       WHERE pmt.short_name='%s'
       ORDER BY tag", array($row['short_name'])
    );

    $tags = array();
    while ($tag = db_fetch_array($tags_res)) {
      $tags[] = $tag['tag'];
    }
    echo $row['short_name'].":".$row['title'] .":". join(' | ', $tags)."\n";
  }
  exit(0);
}

function plugin_manager_settings_form() {

  // If there are no backends, then stop.
  $backends = plugin_manager_backends();
  if (empty($backends)) {
    drupal_set_message(t("There are no available backends for use."), "error");
    return array();
  }

  $form = array();


  $form['plugin_manager_username'] = array(
    '#title' => t('Stored Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['plugin_manager_hostname'] = array(
    '#title' => t('Stored Hostname'),
    '#type' => 'textfield',
    '#description' => t('(Usually localhost) For non-standard port, put host:port (eg. localhost:27).'),
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['plugin_manager_latest_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Choose Latest Version'),
    '#description' => t('If this is selected, the plugin manager will skip version selection and automatically select the most recent version.'),
    '#default_value' => variable_get('plugin_manager_latest_version', 1),
  );

  // Get settings from the individual backends.
  foreach ($backends AS $backend) {
    $form[$backend] = array(
      '#type' => 'fieldset',
      '#title' => t('@backend Settings', array('@backend' => strtoupper($backend))),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    if (function_exists($backend .'_plugin_manager_settings')) {
      $form[$backend][] = call_user_func($backend .'_plugin_manager_settings');
    }
    else {
      // Show that there are no configuration options.
      $form[$backend][] = array(
        '#value' => t('This backend provides no configuration options.'),
      );
    }
  }

  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  return $form;
}

function plugin_manager_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values'] AS $name => $value) {
    if (!strcmp(drupal_substr($name, 0, drupal_strlen('plugin_manager_')), 'plugin_manager_')) {
      variable_set($name, $value);
    }
  }
  drupal_set_message(t("The settings have been saved."));
}
