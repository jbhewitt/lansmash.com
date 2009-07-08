<?php
// $Id: rsvp-invitation.tpl.php,v 1.2.2.1 2009/05/26 18:37:09 ulf1 Exp $

/**
 * @file rsvp-invitation.tpl.php
 * Displays the rsvp invitation.
 * All variables are sanitized to display directly
 *
 * Available variables:
 *
 * $totalsarray: Array with type as key and an array as value (number, gif, text)
 *
 * $max_guests
 * $is_openinvitation
 * $event_subject
 * $event_startdate
 * $event_link
 * $event_nid
 * $invitation_subject
 * $invitation_message
 * $guest_name
 * $organizer_name
 * $organizer_uid
 * $organizer_link
 * $invitation_rid
 * $show_openseats
 * $are_openseats_available
 * $openseats_count_maybe
 * $openseats_count_yes
 * 
 * $icon_path
 * $image
 * $backgroundimage
 * 
 * @see template_preprocess_rsvp_invitation()
 */
?>
  <br />
  <div class="rsvp_invitation">
    <div class="rsvp_invitation_salutation">
      <?php if ($guest_name): ?>
        <?php print t('Dear ') . $guest_name . ','?><br />
      <?php endif; ?>
    </div>
    <div class="rsvp_invitation_introduction">
      <?php print $organizer_link . t(' has invited you to \'!event\'', array('!event' => $event_subject)). '<br />' ?>
    </div>
    <div class="rsvp_invitation_header">
      <?php if ($is_openinvitation): ?>
        <?php print t('Open event:') ?> 
      <?php else : ?>
        <?php print t('Event:') ?> 
      <?php endif; ?>
      <?php print $event_link ?>

      <br /><?php print t('Event date:') ?><?php print $event_startdate ?>
      <br /><?php print t('Organizer: ') ?><?php print $organizer_link ?>

      <?php if ($show_openseats): ?>
        <?php if (are_openseats_available): ?>
          <br /><?php print t('Open seats left: '); print $openseats_count_yes . ' ' . $openseats_count_maybe; print t(' from '); print $max_guests ?>
        <?php else : ?>
          <br /><?php print t('No open seats available.') ?>
        <?php endif; ?>
      <?php endif; ?>
    </div>

    <br />
      <div class="rsvp_invitation_message">
        <?php if ($image): ?>
          <div class="rsvp_invitation_image">
            <?php print $image ?>
          </div>
        <?php endif; ?>

      <?php print $invitation_message ?>
    </div>
  </div>

    
