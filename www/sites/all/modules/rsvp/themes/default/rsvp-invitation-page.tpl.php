<?php
// $Id: rsvp-invitation-page.tpl.php,v 1.3.2.3 2009/05/26 18:37:09 ulf1 Exp $

/**
 * @file rsvp-invitation-page.tpl.php
 * Displays the rsvp invitation, replybox and guestlist.
 * All variables are sanitized to display directly
 *
 * Available variables:
 *
 * $invitation:
 * $replybox:
 * $addguests:
 * $guestlist:
 * $image:
 * $icon_path:
 * $backgroundimage:
 *
 * The idea of this theme is to have the following setup
 * 
 * ------------------------------
 * |       $invitation          | 
 * ------------------------------
 * | $guestlist | $addguests    | 
 * |            | $replybox     |
 * ------------------------------ 
 *
 * @see template_preprocess_rsvp-invitation-page()
 */
?>

  <div class="rsvp_invitation_view rsvp_color_inner">
    <?php if ($invitation): ?>
      <div class="rsvp_invitation_view_top">
        <?php print $invitation ?>
      </div>
    <?php endif; ?>
    <div class="rsvp_invitation_view_bottom">
      <?php if ($guestlist): ?>
        <div class="rsvp_invitation_view_bottom_left">
          <?php print $guestlist ?>
        </div>
      <?php endif; ?>

      <div class="rsvp_invitation_view_bottom_right">
        <?php if ($addguests) : ?>
          <?php print $addguests ?>
        <?php endif; ?>
        <?php if ($replybox) : ?>
          <?php print $replybox ?>
        <?php endif; ?>
      </div>
    </div>
    <div style="clear: both">
    </div>
    
  </div>
        
  