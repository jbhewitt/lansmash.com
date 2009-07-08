Drupal.subdomain_style_toggle = function() {
  if ( $("select[@name='subdomain_style']").val() == 2) {
    $("#subdomain_custom").show();
  }
  else {
    $("#subdomain_custom").hide();
  }
}

Drupal.subdomain_type_toggle = function() {
  var type = '';
  switch ($("select[@name='subdomain_type']").val()) {
    case '1':
      type = 'Organic Group';
      $('#edit-subdomain-allow-onlyalpha-wrapper').show().children('label').html($('#edit-subdomain-allow-onlyalpha-wrapper label').html().replace('user','group'));
      $('#edit-subdomain-allowchange-wrapper').show().children('label').html($('#edit-subdomain-allowchange-wrapper label').html().replace('user','group'));
      $('#vocab').hide();
      break;

    case '2':
      type = 'Node Author';
      $('#edit-subdomain-allow-onlyalpha-wrapper').show().children('label').html($('#edit-subdomain-allow-onlyalpha-wrapper label').html().replace('group','user'));
      $('#edit-subdomain-allowchange-wrapper').show().children('label').html($('#edit-subdomain-allowchange-wrapper label').html().replace('group','user'));
      $('#vocab').hide();
      break;

    case '3':
      type = 'Taxonomy Term';
      $('#edit-subdomain-allow-onlyalpha-wrapper').hide();
      $('#edit-subdomain-allowchange-wrapper').hide();
      $('#vocab').show();
      break;

    case '4':
      type = 'Content Type';
      $('#edit-subdomain-allow-onlyalpha').parent().parent().hide();
      $('#edit-subdomain-allowchange').parent().hide();
      $('#vocab').hide();
      break;
  }
  $("select[@name='subdomain_style'] option[@value=1]").html('the name of the '+type);

  $('select[@Name="subdomain_style"]').click();
}

if (Drupal.jsEnabled) {
  $(document).ready( function() {
    $('select[@Name="subdomain_type"]').click(function() { Drupal.subdomain_type_toggle(); }).click();
    $('select[@Name="subdomain_style"]').click(function() { Drupal.subdomain_style_toggle(); }).click();
    }
  );
}