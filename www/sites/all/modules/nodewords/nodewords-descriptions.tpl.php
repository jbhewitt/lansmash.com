<?php
// $Id: nodewords-descriptions.tpl.php,v 1.1.2.1 2008/02/15 14:08:32 robrechtj Exp $

/**
 * @file
 * Available variables:
 * - $descriptions: an array ($title => $description).
 */
?>
<dl class="nodewords-descriptions">
  <?php foreach ($descriptions as $title => $description) : ?>
  <dt><?php print $title; ?></dt>
  <dd><?php print $description; ?></dd>
  <?php endforeach; ?>
</dl>

