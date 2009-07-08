Last.fm Simple Stats
--------------------

This is a simple module which allows you to embed Last.fm statistics for a user
inside a node. The module is able to fetch multiple sorts of feeds from
Audioscrobbler's web services. Feeds are cached in the database and updated on
request or using Drupal cron on a customisable interval. Checking is done
intelligently using If-Modified-Since and Expires headers.

To use the module inside a node, call lastfmsimple_show_feed(). This takes three
arguments: a Last.fm user name, a feed name and a number of items to parse/show.

Examples:
  * lastfmsimple_show_feed('user', 'recenttracks', 10)
  * lastfmsimple_show_feed('user2', 'weeklyartistchart', 5)

The function returns an array of item objects. Object properties are named
equally to the feed item's elements.

A full example:

<ol>
<?php foreach ( lastfmsimple_show_feed('user', 'weeklyartistchart', 5) as $item ): ?>
    <li>
    	<a href="<?php echo $item->url; ?>"><?php echo $item->name; ?></a>
    	(<?php echo $item->playcount; ?> plays)
    </li>
<?php endforeach; ?>
</ol>

For more information on Audioscrobbler web services, see:
http://www.audioscrobbler.net/data/webservices/

Notes
-----

  * This module only parses user statistics.
  * For now, it has only been tested with recenttracks and weeklyartistchart
  feeds.

Author
------

Dietrich Moerman -- http://dmoerman.be
