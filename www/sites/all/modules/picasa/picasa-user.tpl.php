<?php
// $Id: picasa-user.tpl.php,v 1.1.2.2 2008/10/17 16:46:24 cyberswat Exp $
/**
* @file comment-folded.tpl.php
* Default theme implementation for folded comments.
*
* Available variables:
* - $title: Linked title to full comment.
* - $new: New comment marker.
* - $author: Comment author. Can be link or plain text.
* - $date: Date and time of posting.
* - $comment: Full comment object.
*
* @see template_preprocess_comment_folded()
* @see theme_comment_folded()
*/
?>

<?php if ($id): ?>
		<h1><?php print $id; ?></h1>
<?php endif;?>

<?php if ($icon): ?>
		<img src="<?php print $icon; ?>" />
<?php endif;?>

<?php if ($updated): ?>
		<h3><?php print $updated; ?></h3>
<?php endif;?>

<?php if ($title): ?>
		<h3><?php print $title; ?></h3>
<?php endif;?>

<?php if ($subtitle): ?>
		<h3><?php print $subtitle; ?></h3>
<?php endif;?>

<?php if ($author_name): ?>
		<h3><?php print $author_name; ?></h3>
<?php endif;?>

<?php if ($author_uri): ?>
		<h3><?php print $author_uri; ?></h3>
<?php endif;?>

<?php if ($generator): ?>
		<h3><?php print $generator; ?></h3>
<?php endif;?>

<?php if ($author_name): ?>
		<h3><?php print $author_name; ?></h3>
<?php endif;?>

<?php if ($opensearch_totalresults): ?>
		<h3><?php print $opensearch_totalresults; ?></h3>
<?php endif;?>

<?php if ($opensearch_startindex): ?>
		<h3><?php print $opensearch_startindex; ?></h3>
<?php endif;?>

<?php if ($opensearch_itemsperpage): ?>
		<h3><?php print $opensearch_itemsperpage; ?></h3>
<?php endif;?>

<?php if ($gphoto_user): ?>
		<h3><?php print $gphoto_user; ?></h3>
<?php endif;?>

<?php if ($gphoto_nickname): ?>
		<h3><?php print $gphoto_nickname; ?></h3>
<?php endif;?>

<?php if ($gphoto_thumbnail): ?>
		<h3><?php print $gphoto_thumbnail; ?></h3>
<?php endif;?>

<?php if ($gphoto_quotalimit): ?>
		<h3><?php print $gphoto_quotalimit; ?></h3>
<?php endif;?>

<?php if ($gphoto_quotacurrent): ?>
		<h3><?php print $gphoto_quotacurrent; ?></h3>
<?php endif;?>

<?php if ($gphoto_maxphotosperalbum): ?>
		<h3><?php print $gphoto_maxphotosperalbum; ?></h3>
<?php endif;?>

<?php if ($link_slideshow): ?>
		<h3><?php print $link_slideshow; ?></h3>
<?php endif;?>

<?php if ($link_feed): ?>
		<h3><?php print $link_feed; ?></h3>
<?php endif;?>

<?php if ($link_post): ?>
		<h3><?php print $link_post; ?></h3>
<?php endif;?>

<?php if ($link_alternate): ?>
		<h3><?php print $link_alternate; ?></h3>
<?php endif;?>

<?php if ($link_self): ?>
		<h3><?php print $link_self; ?></h3>
<?php endif;?>
