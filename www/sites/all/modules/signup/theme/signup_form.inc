<?php
// $Id: signup_form.inc,v 1.3 2008/11/19 18:35:11 dww Exp $


/**
 * @file
 * Site-specific code related to the form when users signup for a node.
 */

/**
 * Return the site-specific custom fields for the signup user form.
 *
 * To customize this for your site, copy this entire function into
 * your theme's template.php file, rename the function to
 * phptemplate_signup_user_form(), and modify to taste.  Feel free to
 * alter any elements in this section, remove them, or add any others.
 *
 * WARNING: If your site allows anonymous signups and you alter the
 * 'Name' field in this function, you will probably have to implement a
 * version of theme_signup_email_token_anonymous_username() for your site.
 *
 * In order for the form to be rendered properly, the name of the form
 * element must be $form['signup_form_data']['NameOfDataField'], where
 * NameOfDataField is replaced with the actual name of the data field.
 * We suggest that the displayed name of the field (the '#title'
 * property) be the same as the name of the data field, but it's not
 * required.  See below for examples.
 *
 * Fieldsets are not currently supported in this form.  Any
 * '#default_value' will be filled in by default when the form is
 * presented to the user.  Any field marked '#required' must be filled
 * in before the user can sign up.
 *
 * If you do not want any additional fields, the function can simply
 * return an empty array: "return array();"
 *
 * @param $node
 *   The fully loaded node object where this signup form is appearing.
 *
 * @return
 *   Array defining the form to present to the user to signup for a node.
 *
 * @see theme_signup_email_token_anonymous_username()
 */
function theme_signup_user_form($node) {
  global $user;
  $form = array();
//	print_r($node);	


  if (!empty($node->signup_close_signup_limit)) {
    $slots = $node->signup_close_signup_limit - $node->signup_total;
    $form['signup_form_data']['limit'] = array(
      '#type' => 'markup',
      '#value' => t('Available event slots: @slots', array('@slots' => $slots)),
    );
  }

//show link for current signups
  $form['signup_form_data']['viewsignups'] = array(
    '#type' => 'markup',
    '#value' => t('<br/><a href="/node/'.$node->nid .'/signups">View current signups</a>' ),
  );


  // If this function is providing any extra fields at all, the following
  // line is required for form form to work -- DO NOT EDIT OR REMOVE.
  $form['signup_form_data']['#tree'] = TRUE;

  $form['signup_form_data']['Name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Name'),
    '#size' => 40, '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['signup_form_data']['Phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Phone number'),
    '#description' => t('Leave your mobile here for us to call you at the event'), 
    '#size' => 40, '#maxlength' => 64,
   
  );

// check to see if event is a LAN Party
	if (isset ( $node->taxonomy[5] ) ) 
	{ 
	  $form['signup_form_data']['PC'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Reserve a seat in the PC area'),
	    '#description' => t('Tick this box if you intend on bringing your own computer (laptop or desktop)'), 
	  );
	
	$form['signup_form_data']['Tournaments'] = array(
	 '#type' => 'fieldset',
	    '#title' => t('Tournament Sign Up\'s '),
	  );
	// check to see if event has "left4dead"
	if (isset ( $node->taxonomy[123] ) ) 
	{
	  $form['signup_form_data']['Tournaments']['LEFT4DEAD'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Left4Dead Tournament'),
	    '#description' => t('Tick this box if you intend on competing in the Left4Dead Tournament'), 
	  );
	}

	// check to see if event has "Halo 3"
		if (isset ( $node->taxonomy[144] ) ) 
		{ 
		  $form['signup_form_data']['Tournaments']['Halo3'] = array(
		    '#type' => 'checkbox',
		    '#title' => t('Halo3 Tournament'),
		    '#description' => t('Tick this box if you intend on bringing your own Xbox 360'), 
		  );
		}

	// check to see if event has "tf2"
	if (isset ($node->taxonomy[98]) OR isset ($node->taxonomy[183]) ) 
	{
	  $form['signup_form_data']['Tournaments']['TF2'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Team Fortress 2 Tournament'),
	    '#description' => t('Tick this box if you intend on competing in the Team Fortress 2 Tournament'), 
	  );
	}
	
	// check to see if event has "SF4"
	if (isset ($node->taxonomy[204]) OR isset ($node->taxonomy[203]) ) 
	{
	  $form['signup_form_data']['Tournaments']['SF4'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Street Fighter 4 Tournament'),
	    '#description' => t('Tick this box if you intend on competing in the Street Fighter 4 Tournament'), 
	  );
	}

	// check to see if event has "SF2"
	if (isset ($node->taxonomy[181]) OR isset ($node->taxonomy[146]) ) 
	{
	  $form['signup_form_data']['Tournaments']['SF2'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Street Fighter 2 Tournament'),
	    '#description' => t('Tick this box if you intend on competing in the Street Fighter 2 Tournament'), 
	  );
	}

	if (isset ($node->taxonomy[193]) OR isset ($node->taxonomy[95]) )  {
	  $form['signup_form_data']['Tournaments']['Smash Brothers'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Smash Brothers Tournament'),
	    '#description' => t('Tick this box if you intend on competing in the Smash Brothers tournament!'), 
	  );
	}
}


  $form['signup_form_data']['friends'] = array(
	'#type' => 'select',
	    '#title' => t('Reserve a ticket for your friend'),
	    '#default_value' => '0',
	    '#options' => array('0','1','2','3','4','5','6'),
	    '#description' => t('If you have a friend coming and wish to reserve a ticket for them please list how many here.  Please also leave a note in the comments section.'),
	  );
  $form['signup_form_data']['Other'] = array(
    '#type' => 'textarea',
    '#title' => t('Any Other Comments'),
    '#description' => t('Please leave a comment here if you have any other questions or requests'), 
  );




  // If the user is logged in, fill in their name by default.
  if ($user->uid) {
    $form['signup_form_data']['Name']['#default_value'] = $user->name;
  }

    return $form;
}

/**
 * Returns the value to use for the %user_name email token for anonymous users.
 *
 * WARNING: If you implemented your own version of theme_signup_form_data()
 * that changed or removed the custom 'Name' field and your site
 * allows anonymous signups, you will need to modify this, too.
 *
 * @param $form_data
 *   Array of custom signup form values for the current signup.
 * @param $email
 *   E-mail address of the anonymous user who signed up.
 * @return
 *   A string with the proper value for the %user_name email token.
 *
 * @see theme_signup_user_form()
 */
function theme_signup_email_token_anonymous_username($form_data, $email) {
  // In some cases, the best you can do is to use the anonymous user's
  // supplied email address, in which case, you should uncomment this:
  //return $email;

  // WARNING: This line is only valid if you left the 'Name' field in
  // your site's version of theme_signup_user_form().
  return $form_data['Name'];
}
