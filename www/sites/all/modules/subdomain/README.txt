===========================================================
OVERVIEW
===========================================================
The subdomain module joins forces with pathauto to automatically 
place Drupal site content onto subdomains.

Currently, it supports 4 modes:

 Node Author:
  EXAMPLE: A user named "Sayuko" and her content would be located at:
  http://sayuko.example.com

 Node Content Type:
  EXAMPLE: Nodes with a content type of "News" would have URLs like
  http://news.example.com/drupal-is-the-greatest-web-tool-ever

 Organic Groups:
  EXAMPLE: A group named "Pizza lovers" & content would be located at:
  http://pizza-lowers.example.com

 Taxonomy:
  EXAMPLE: A term named "Seattle" & associated content would be located at:
  http://seattle.example.com

===========================================================
UPGRADE
===========================================================
Subdomain no longer needs the .htaccess patch, so if you
are upgrading from a previous version, either:

1) restore your old .htaccess file

  or
  
2) locate and delete the following from your .htaccess file:

  # REQUIRED BY SUBDOMAIN.MODULE
  # Moves subdomain to URI path
  # e.g: mysubdomain.example.com 
  # becomes example.com/index.php?_mysubdomain/
  # NOTE: does not rewrite subdomain if it is "www". 
  # If you want it to rewrite www, disable the 2nd line
  RewriteCond %{REQUEST_FILENAME} !-f
  RewriteCond %{HTTP_HOST} !^www\.([^.]+)\.([^.]+)$
  RewriteCond %{HTTP_HOST} ^([^.]+)\.([^.]+)\.([^.]+)$
  RewriteRule ^(.*)$ index.php?q=~%1/$1 [L,QSA]


===========================================================
INSTALL
===========================================================
STEP 1: Copy and paste the following to the bottom of your settings.php file

  // ------------- BEGIN COPYING BELOW THIS LINE -------------
  function custom_url_rewrite_outbound(&$path, &$options, $original_path) {

    // Used by the Subdomain module to generate URLs with subdomains
    if (module_exists('subdomain')) {
      subdomain_url_rewrite_outbound($path, $options); 
    }
  }

  function custom_url_rewrite_inbound(&$result, $path, $path_language) {

    // Used by the Subdomain module to correctly interpret URLs with subdomains
    if (module_exists('subdomain')) {
      subdomain_url_rewrite_inbound($result, $path, $path_language); 
    }
  }  
  // ----------------- COPY UNTIL THIS LINE ------------------


STEP 2: Edit settings.php & set $cookie_domain to your site domain. e.g.:
 $cookie_domain = "example.com";

STEP 3: Enable wildcard DNS on your DNS hosting provider (e.g. *.example.com)

STEP 4: Configure wildcard virtual hosts. For apache or lighttpd,
 see below. For other web servers, consult their documentation.

STEP 5: Enable Subdomain settings (URL aliases -> Subdomain settings
 select mode (og, node author, taxonomy vocabulary)
 and additional settings as desired

STEP 6: Configure Pathauto (URL aliases -> Automated alias settings):
 1) Go to "Punctuation Settings" and set "Tilde ~:" to "No action"
 2) Place [subdomain] at the *start* of all paths whose content you
    want placed on a subdomain


===========================================================
CONFIGURING APACHE
===========================================================
STEP 1: Edit httpd.conf, enable mod_rewrite and configure
wildcard virtual hosts. An example configuration that you 
can append to your httpd.conf is below. Replace the domain 
name and directory as appropriate, save, and restart apache.

NameVirtualHost *:80
<VirtualHost *:80>
    ServerAdmin webmaster@example.com
    DocumentRoot /var/www/html
    ServerName example.com
    ServerAlias *.example.com

    <Directory "/var/www/html">
      AllowOverride All
    </Directory>
</VirtualHost>


===========================================================
CONFIGURING LIGHTTPD
===========================================================
STEP 1: Enable wildcard DNS on your DNS hosting provider
 (e.g. *.example.com)

STEP 2: Edit your lighttpd.conf file, enable the evhost module, 
and append the following, modifying directories as appropriate:

$HTTP["host"] =~ "([^.]+)\.([^.]+)$" {
        evhost.path-pattern = "/var/www/%2.%1/html/"
}

# Apply the following drupal rewrite rules to subdomain URLs
# moving subdomain to the 1st argument in the path
# e.g. bob.example.com news.mysite.net 
$HTTP["host"] =~ "^([^.]+)\.([^.]+)\.([^.]+)$" {
url.rewrite-final = (
  # More than one argument
  "^/([^.?]*)\?(.*)$" => "/index.php?q=_%1/$1&$2",
  # No arguments
  "^/([^.?]*)$" => "/index.php?q=_%1/$1",
 )
}


===========================================================
CREDITS
===========================================================
Authored and maintained by Stein Setvik <setvik AT gmail DOT com>