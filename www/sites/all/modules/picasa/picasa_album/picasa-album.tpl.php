<?php if ($teaser): ?>
<table>
  <tr>
    <td><?php print $thumbnail; ?></td>
    <td width="100%" valign="top">
      <table>
        <tr class="odd">
          <th colspan="4" style="text-align: center;"><?php print $title; ?></th>
        </tr>
        <tr class="even">
          <td style="font-weight: bold; text-align: right;">author:</td>
          <td><?php print $author_name; ?></td>
          <td style="font-weight: bold; text-align: right;">rights:</td>
          <td><?php print $gphoto['access']; ?></td>
        </tr>
        <tr class="odd">
          <td style="font-weight: bold; text-align: right;">published:</td>
          <td><?php print $gphoto['timestamp']; ?></td>
          <td style="font-weight: bold; text-align: right;">updated:</td>
          <td><?php print $updated; ?></td>
        </tr>
        <tr class="even">
          <td style="font-weight: bold; text-align: right;">location:</td>
          <td><?php print $gphoto['location']; ?></td>
          <td style="font-weight: bold; text-align: right;">photos:</td>
          <td><?php print $gphoto['numphotos']; ?></td>
        </tr>
        <tr class="odd">
          <td colspan="4"><?php print $summary; ?></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<?php endif;?>

<?php if ($page): ?>
    <h1>page</h1>
<?php endif;?>
