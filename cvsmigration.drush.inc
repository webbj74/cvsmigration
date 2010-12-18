<?php
// $Id$

/**
 * @file
 * Implements drush command for exporting {cvs_migration} to flat file
 */

/**
 * Implements hook_drush_command().
 */
function cvsmigration_drush_command() {
  $items['cvsmigration-export'] = array(
    'description' => dt('Exports CVS Usernames and their corresponding email addresses.'),
    'arguments' => array(
      'path' => dt('Optional. A filename to output to. If omitted Drush will print to standard output.'),
    ),
  );
  return $items;
}

/**
 * A drush command callback.
 * Exports a CSV of CVS Usernames and their associated email addresses.
 */
function drush_cvsmigration_export() {

  $output = '';

  $result = drush_db_select('cvs_migration', '*', NULL, NULL, NULL, NULL, 'uid', 'ASC');
  while ($row = drush_db_fetch_object($result)) {
    $account = user_load(array('uid' => $row->uid));
    $fullnamefield = "full-name";
    $fullname = (!empty($account->$fullnamefield)) ? $account->$fullnamefield : $account->name;
    $invalid = '[^\x23-\x7E !]+';
    $valid_fullname = preg_replace("/$invalid/", '', $fullname);
    $output .= sprintf("\"%d\",\"%s\",\"%s\",\"%s\"" . PHP_EOL, $row->uid, addslashes($valid_fullname), addslashes($row->cvs_user), addslashes($row->repomail));
  }

  $args = func_get_args();
  if (isset($args[0])) {
    file_put_contents($args[0], $output);
  }
  else {
    print $output;
  }
}
