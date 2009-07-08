<?php
// $Id: picasa.parse.inc,v 1.1.2.4 2008/11/12 17:32:34 cyberswat Exp $

function _picasa_parse_link($obj) {
  $r = array();
  foreach ($obj as $link) {
    $l = array();
    $l['rel'] = (string)$link->attributes()->rel;
    $l['type'] = (string)$link->attributes()->type;
    $l['href'] = (string)$link->attributes()->href;
    $r[] = $l;
  }
  return $r;
}

function _picasa_parse_author($obj) {
  $r = array();
  foreach ($obj as $author) {
    $r['name'] = (string)$author->name;
    $r['uri'] = (string)$author->uri;
  }
  return $r;
}

function _picasa_parse_open_search($obj) {
  $r = array();
  foreach ($obj as $key =>$os) {
    $r[$key] = (string)$os;
  }
  return $r;
}

function _picasa_parse_gphoto($obj) {
  $r = array();
  foreach ($obj as $key =>$gp) {
    $k = strtolower((string)$key);
    switch($k) {
      case 'numphotos':
      case 'bytesused':
      case 'commentcount':
      case 'quotalimit':
      case 'quotacurrent':
      case 'maxphotosperalbum':
        $r[$k] = (int)$gp;
      break;
      default:
        $r[$k] = (string)$gp;
      break;
    }
  }
  return $r;
}

function _picasa_parse_georss($obj) {
  $r = array();
  foreach ($obj as $where) {
    $gml = $where->children('http://www.opengis.net/gml');
    foreach ($gml as $a => $b) {
      $a = (string)$a;
      if ($a == 'Point') {
        $r['point'] = (string)$b->pos;
      }
      if ($a == 'Envelope') {
        $r['envelope'] = array('lowercorner' => (string)$b->lowerCorner, 'uppercorner' => (string)$b->upperCorner);
      }
    }
  }
  return $r;
}

function _picasa_parse_exif($obj) {
  $r = array();
  foreach ((array)$obj->tags as $key =>$v) {
    $k = strtolower((string)$key);
    switch($k) {
      case 'fstop':
      case 'exposure':
      case 'focallength':
            $r[$k] = (float)$v;
      break;
      case 'time':
        $r[$k] = (int)$v;
      break;
      default:
        $r[$k] = (string)$v;
      break;
    }
  }
  return $r;
}

function _picasa_parse_media($obj) {
  $r = array();
  foreach($obj as $group) {
    $g = array();
    $g['title'] = (string)$group->title;
    $g['description'] = (string)$group->description;
    $g['keywords'] = (string)$group->keywords;
    $g['credit'] = (string)$group->credit;
    $g['content'] = _picasa_parse_attributes($group->content);
    $g['thumbnail'] = array();
    foreach($group->thumbnail as $thumbnail) {
      $g['thumbnail'][] = _picasa_parse_attributes($thumbnail);
    }
    $r[] = $g;
  }
  return $r;
}

function _picasa_parse_attributes($obj) {
  $r = array();
  foreach ($obj as $content) {
    foreach ($content->attributes() as $a => $b) {
      $r[(string)$a] = (string)$b;
    }
  }

  if (count($r) == 0) {
    foreach ($obj->attributes() as $a => $b) {
      $r[(string)$a] = (string)$b;
    }
  }
  return $r;
}
