<?php
// $Id: picasa.photo.inc,v 1.1.2.2 2008/10/20 13:07:42 cyberswat Exp $

function picasa_photo_import($album, $token) {
		google_auth_required();
		$token = $_SESSION['google_auth_token'];
		$photos = picasa_photo_request($token, $album->paid);

		return $output;
}

function picasa_photo_request($token, $paid) {
		$feed = "http://picasaweb.google.com/data/feed/api/user/default/albumid/$paid";
		$headers['Authorization'] = 'AuthSub token="'.$token.'"';
		$request = drupal_http_request($feed, $headers);
		return picasa_photo_parse($request->data);
}

function picasa_photo_parse($data) {
		require_once('picasa.parse.inc');
		$photos = array();
		$xml = new SimpleXMLElement($data);
		$namespaces = $xml->getNamespaces(true);

		$photos['id'] = (string)$xml->id;
		$photos['updated'] = (string)$xml->updated;
		$photos['title'] = (string)$xml->title;
		$photos['subtitle'] = (string)$xml->subtitle;
		$photos['rights'] = (string)$xml->rights;
		$photos['icon'] = (string)$xml->icon;
		$photos['link'] = _picasa_parse_link($xml->link);
		$photos['author'] = _picasa_parse_author($xml->author);
		$photos['generator'] = (string)$xml->generator;
		$photos['open_search'] = _picasa_parse_open_search($xml->children($namespaces['openSearch']));
		$photos['author'] = _picasa_parse_author($xml->author);
		_picasa_parse_gphoto($xml->children($namespaces['gphoto']), $photos);
		$photos['georss'] = _picasa_parse_georss($xml->children('http://www.georss.org/georss'));

		$e = array();
		foreach ($xml->entry as $entry) {
					preg_match('/photoid\/(.*?)$/', $entry->id, $matches );
					$photid = $matches[1];
					$e[$photid]['id'] = (string)$entry->id;
					$e[$photid]['published'] = (string)$entry->published;
					$e[$photid]['updated'] = (string)$entry->updated;
					$e[$photid]['title'] = (string)$entry->title;
					$e[$photid]['summary'] = (string)$entry->summary;
					$e[$photid]['content'] = _picasa_parse_content($entry->content);
					$e[$photid]['link'] = _picasa_parse_link($entry->link);
					_picasa_parse_gphoto($entry->children($namespaces['gphoto']), $e[$photid]);
					$e[$photid]['exif'] = _picasa_parse_exif($entry->children($namespaces['exif']));
					$e[$photid]['media'] = _picasa_parse_media($entry->children($namespaces['media']));
		}
		$photos['entry'] = $e;
		return $photos;
}
