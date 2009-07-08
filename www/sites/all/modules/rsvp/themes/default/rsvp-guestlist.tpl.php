<?php
// $Id: rsvp-guestlist.tpl.php,v 1.2.2.4 2009/03/16 18:17:32 ulf1 Exp $

/**
 * @file rsvp-guestlist.tpl.php
 * Displays the rsvp guestlist.
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
 *   $show_maybe
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
 * $export_link:
 * $rid:
 * $now:  current date as string 
 *
 * @see template_preprocess_rsvp_guestlist()
 */
?>
  <div class="rsvp_form rsvp_guestlist_form rsvp_color_border rsvp_color_inner">

    <?php if ($show_header): ?>
      <div class="rsvp_form_header rsvp_guestlist_header rsvp_color_outer">
        <div class="rsvp_form_header_left rsvp_guestlist_header_left rsvp_color_outer">
          <?php print t('GUEST LIST') ?>
        </div>
        <?php if ($show_export_link) { ?>
          <div class="rsvp_form_header_right rsvp_guestlist_header_print rsvp_color_outer">
            <?php print l('Export guest list', $export_link, array('attributes' => array('title' => t('Export guest list into csv format.')))); ?>
          </div>
        <?php } ?>
      </div>
    <?php endif; ?>
    <div class="rsvp_clear">

        <?php if ($show_whoiscoming): ?>
          <div class="rsvp_form_content rsvp_guestlist_whoiscoming rsvp_color_inner">
            <?php if ($is_anonymous): ?>
              <div>
                <?php print t('Please sign-in to display eventually more details about other invitees.') ?>
              </div><br />
            <?php endif; ?>
            <b><?php print $text_whoiscoming ?></b><br />
            <?php if ($show_responses) : ?>
              <b><?php print $totalsarray[RSVP_ATT_YES][0] ?></b>
              <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_YES][1], $totalsarray[RSVP_ATT_YES][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' ?>
              <?php if ($show_maybe) : ?>
                <b><?php print $totalsarray[RSVP_ATT_MAYBE][0] ?></b>
                <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_MAYBE][1], $totalsarray[RSVP_ATT_MAYBE][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' ?>
              <?php endif; ?>
              <b><?php print $totalsarray[RSVP_ATT_NO][0] ?></b>
              <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_NO][1], $totalsarray[RSVP_ATT_NO][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' ?>
              <b><?php print $totalsarray[RSVP_ATT_NONE][0] ?></b>
              <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_NONE][1], $totalsarray[RSVP_ATT_NONE][2], '', array('width' => 17, 'height' => 17)). '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' ?>
            <?php else : ?>
              <b><?php print $totalsarray[RSVP_ATT_ALL][0] ?></b>
              <?php print theme('image', $icon_path . $totalsarray[RSVP_ATT_ALL][1], $totalsarray[RSVP_ATT_ALL][2], '', array('width' => 17, 'height' => 17)) ?>
            <?php endif; ?>
            <br /><?php print t('As of ') . $now ?><br />
          </div>
        <?php endif; ?>

        <?php foreach ($guestlistarray as $type => $typearray) { ?>
          <div class="rsvp_form_header rsvp_guestlist_answer_block_header rsvp_color_border rsvp_color_inner <?php print "rsvp_guestlist_answer_block_header_$type" ?>">
            <div class="rsvp_guestlist_answer_block_header_flag">
              <?php print theme('image', $icon_path . $typearray['icon'], $typearray['answertext'], '', array('width' => 17, 'height' => 17)); ?>
            </div>
            <div class="rsvp_guestlist_answer_block_header_text">
              <b> <?php print $typearray['answertext'] . ' (' . $typearray['count'] . ')' ?></b>
            </div>
          </div>

          <div class="rsvp_form_content rsvp_guestlist_answer_block_content rsvp_color_inner <?php print "rsvp_guestlist_answer_block_content_$type" ?>">
            <ul>
              <?php foreach ($typearray['guests'] as $guest) { ?>
                <li>
                  <?php if ($guest['is_currentuser']) {
                          $active = theme('image', $icon_path . 'current.gif', t('Active user'), t('Active user'), array('width' => 17, 'height' => 17));
                          if ($guest['is_guest']) {
                            $active = l($active, 'rsvp/email/'. $guest['userhash'] .'/realname', array('html' => true, 'attributes' => array('title' => t('Change your name that is being displayed in the guest list.'))));
                          }
                          print $active;  
                        } 
                  ?>
                  <?php if ($guest['user_link']) {
                          print $guest['user_link'];
                        }
                        else {
                          print $guest['username'];
                        } 
                  ?>
                  <?php if ($guest['is_host']) : ?>
                          <b><?php print t('(The organizer)  ') ?></b>
                  <?php endif; ?>

                  <?php if ($guest['is_viral']) : ?>
                    <?php print theme('image', $icon_path . 'viral.gif', t('Invited by a guest'), t('Invited by a guest'), array('width' => 17, 'height' => 17)) ?>
                  <?php endif; ?>

                  <?php if ($guest['is_opensignup']) : ?>
                    <?php print theme('image', $icon_path . 'openinvitation.gif', t('Self registration'), t('Open invitation'), array('width' => 17, 'height' => 17)) ?>
                  <?php endif; ?>

                  <?php if ($is_moderator) : ?>
                    <?php print l(theme('image', $icon_path . 'view.gif', 'view', '', array('width' => 17, 'height' => 17)), 'rsvp/email/'. $guest['userhash'] .'/view', array('html' => true, 'attributes' => array('title' => t('View invitation like the guest would see it.')))) ?>
                    <?php print l(theme('image', $icon_path . 'profile.gif', 'status', '', array('width' => 17, 'height' => 17)), 'rsvp/' . $rid . '/attendees/status/'. $guest['userhash'], array('html' => true, 'attributes' => array('title' => t('View status of invitation')))) ?>
                    <?php print l(theme('image', $icon_path . 'delete.gif', 'remove', '', array('width' => 17, 'height' => 17)), 'rsvp/' . $rid . '/attendees/remove/'. $guest['userhash'], array('html' => true, 'attributes' => array('title' => t('Remove attendee from invitation list')))) ?>
                    <?php print l(theme('image', $icon_path . 'message.gif', 'send message', '', array('width' => 17, 'height' => 17)), 'rsvp/' . $rid . '/message/'. $guest['userhash'], array('html' => true, 'attributes' => array('title' => t('Send message to guest')))) ?>
                    <?php print l(theme('image', $icon_path . 'invitation.gif', 'send invitation', '', array('width' => 17, 'height' => 17)), 'rsvp/' . $rid . '/attendees/send/'. $guest['userhash'], array('html' => true, 'attributes' => array('title' => t('Send invitation message to guest')))) ?>
                    <?php print $guest['lastaccess'] ?><br />
              
                  <?php endif; ?>
                          
                  <?php if ($typearray['show_comment']) : ?>
                    <?php print $guest['usercomment'] ?>
                  <?php endif; ?>

                </li>
              <?php } ?>
              <?php if ($typearray['cut_off']) : ?>
                <li>
                . . . .
                </li>
              <?php endif; ?>
            </ul>
          </div>
        <?php } ?>
      </div>

        <?php if ($show_footer): ?>
          <div class="rsvp_form_footer rsvp_guestlist_footer rsvp_color_outer">
            <div class="rsvp_guestlist_footer_sort">
              <?php if ($sort_alpha_link): ?>
                <?php print t('View: Date | ') . $sort_alpha_link ?> 
              <?php endif; ?>
              <?php if ($sort_date_link): ?>
                <?php print t('View: ') . $sort_date_link . t(' | Alphabetically') ?>   
              <?php endif; ?>
            </div>
              
            <div class="rsvp_guestlist_footer_show">
              <?php if ($show_part_link): ?>
                <?php print $show_part_link ?>   
              <?php endif; ?>
              <?php if ($show_all_link): ?>
                <?php print $show_all_link ?>   
              <?php endif; ?>
            </div>
          </div>

          <?php if ($show_viral): ?>
            <div class="rsvp_guestlist_footer_viral rsvp_color_outer">
              <?php 
                print theme('image', $icon_path .'viral.gif', 'Invited by another guest', 'Invited by a guest.', array('width' => 17, 'height' => 17));
                print t('=Invited by a guest.');
              ?>
            </div>
          <?php endif; ?>
          <?php if ($show_opensignup): ?>
            <div class="rsvp_guestlist_footer_opensignup rsvp_color_outer">
              <?php 
                print theme('image', $icon_path . 'openinvitation.gif', 'Self registration', 'Open invitation', array('width' => 17, 'height' => 17));
                print t('=Signed up due to open registration.');
              ?>
            </div>
          <?php endif; ?>
        <?php endif; ?>
  </div>
