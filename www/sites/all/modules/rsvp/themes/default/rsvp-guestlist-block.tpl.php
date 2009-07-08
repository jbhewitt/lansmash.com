<?php
// $Id: rsvp-guestlist-block.tpl.php,v 1.2.2.2 2009/03/04 20:09:36 ulf1 Exp $

/**
 * @file rsvp-guestlist-block.tpl.php
 * Displays the rsvp guestlist as a block.
 *
 * Available variables:
 *
 * invitation_subject:
 * is_moderator:
 * 
 * $show_header: 
 * $show_export_link:
 * $show_whoiscoming:
 *   $totalsarray: Array with type as key and an array as value (number, gif, text)
 *   $show_responses:
 *   $text_whoiscoming
     $show_maybe
 *
 * $guestlistarray: Array with blocks of users that should be displayed
 *   $is_moderator:
 *   $is_anonymous:
 * 
 * $show_footer: 
 *   $show_viral: 
 *   $show_opensignup: 
 *   $sort_date_link: 
 *   $sort_alpha_link:
 *   $show_all_link: 
 *   $show_part_link: 
 *
 * $icon_path:
 * $image:
 * $backgroundimage:
 * 
 * $export_link:
 * $rid:
 * $now:  current date as string 
 *
 * @see template_preprocess_rsvp_guestlist_block()
 */
?>
  <div class="rsvp_guestlist_block">
    <?php if ($is_anonymous): ?>
      <div>
        <?php print t('Please sign-in to display eventually more details about other invitees.') ?>
      </div><br />
    <?php endif; ?>

    <?php if ($show_header): ?>
      <div class="rsvp_guestlist_block_header">
        <div class="rsvp_guestlist_block_header_left">
          For: <?php print $invitation_subject ?><br />
        </div>

      </div>
    <?php endif; ?>
    <div class="rsvp_clear">
      <?php if ($show_whoiscoming): ?>
        <div class="rsvp_guestlist_block_whoiscoming">
          <b><?php print $text_whoiscoming ?></b><br />
          <?php if ($show_responses) : ?>
            <b><?php print $totalsarray[RSVP_ATT_YES][0] ?></b>
            <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_YES][1], $totalsarray[RSVP_ATT_YES][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;' ?>
            <?php if ($show_maybe) : ?>
              <b><?php print $totalsarray[RSVP_ATT_MAYBE][0] ?></b>
              <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_MAYBE][1], $totalsarray[RSVP_ATT_MAYBE][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;' ?>
            <?php endif; ?>
            <b><?php print $totalsarray[RSVP_ATT_NO][0] ?></b>
            <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_NO][1], $totalsarray[RSVP_ATT_NO][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;' ?>
            <b><?php print $totalsarray[RSVP_ATT_NONE][0] ?></b>
            <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_NONE][1], $totalsarray[RSVP_ATT_NONE][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;' ?>
          <?php else : ?>
            <b><?php print $totalsarray[RSVP_ATT_ALL][0] ?></b>
            <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_ALL][1], $totalsarray[RSVP_ATT_ALL][2], '', array('width' => 17, 'height' => 17)) ?>
          <?php endif; ?>
        </div>
      <?php endif; ?>

      <?php foreach ($guestlistarray as $type => $typearray) { ?>
        <div class="rsvp_guestlist_block_answer_block_header <?php print "rsvp_guestlist_block_answer_block_header_$type" ?>">
          <div class="rsvp_guestlist_block_answer_block_header_flag">
            <?php print theme('image', $icon_path . $typearray['icon'], $typearray['answertext'], '', array('width' => 17, 'height' => 17)); ?>
          </div>
          <div class="rsvp_guestlist_block_answer_block_header_text">
            <b> <?php print $typearray['answertext'] . ' (' . $typearray['count'] . ')' ?></b>
          </div>
        </div>
  
        <div class="rsvp_guestlist_block_answer_block_content <?php print "rsvp_guestlist_block_answer_block_content_$type" ?>">
          <ul>
            <?php foreach ($typearray['guests'] as $guest) { ?>
              <li>
                <?php print $guest['username'] ?>
              </li>
            <?php } ?>
          </ul>
        </div>
      <?php } ?>
    </div>
  </div>
