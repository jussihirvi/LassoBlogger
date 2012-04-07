<?LassoScript
// ajax response file
// validate value
// action_params; abort;

// valid company ids
// 1363656-9
// FI13636569
// 0202403-0

var('frommetoroot') = '';
include($fromMetoRoot + 'admin/gen/session.inc');
include($fromMetoRoot + 'config.lasso');
library($fromMetoRoot + 'inc/library.inc');
library($fromMetoRoot + 'admin/inc/libtemp.inc'); // for testing
library($fromMetoRoot + 'admin/inc/libmonster.inc');
library($fromMetoRoot + 'admin/inc/liblocal.inc');
// START OPTIONS
  // LASSOBLOGGER - get opts from database
  var('opts')         = LB_siteOptions;
$opts->insert('webmaster_email' = $opts->find('admin_email'));
$opts->insert('sitename_long'   = $opts->find('blogname'));
$opts->insert('sitename'        = $opts->find('siteurl')->removeLeading('http://'));
$opts->insert('date_showformat' = $opts->find('date_format'));
$opts->insert('time_showformat' = $opts->find('time_format'));
$opts->insert('datetime_showformat' = '%-d.%-m.%Y %H.%M.%S');
$opts->insert('time_dbformat' = '%T');
$opts->insert('date_dbformat' = '%Q');
$opts->insert('datetime_dbformat' = '%Q %T');
$opts->insert('floatseparator_show' = ',');
$opts->insert('alertcolor' = '#066');// dark green; default: bright red, #c03
var('basicOptions') = $opts;
// END OPTIONS



var('valtypes' = action_param('valtypes') -> split(',')); 
var('fname' = action_param('fname'));
var('value' = action_param('value'));
var('label' = action_param('label'));
var('helptext' = action_param('helptext'));
var('myval' = string);    // val result

$value -> trim;
if($valtypes -> find('companyid')->size);
  $myval += MO_val_companyid(
      -fname=$fname,
      -label=$label,
      -value=$value,
      -short_output='yes');
/if;
if($valtypes -> find('email')->size);
  $myval += MO_val_email(
      -fname=$fname,
      -label=$label,
      -value=$value,
      -short_output='yes');
/if;
if($valtypes -> find('password')->size);
  $myval += MO_val_password(
      -fname=$fname,
      -label=$label,
      -value=$value,
      -short_output='yes');
/if;
if($valtypes -> find('unique')->size);
  $myval += MO_val_unique(
      -fname = $fname, 
      -label = $label, 
      -value = $value, 
      -mytable='users',
      -action = 'add',
      -short_output = 'yes');
/if;
if($valtypes -> find('date')->size);
  $myval += MO_val_date(
      -fname = $fname, 
      -label = $label, 
      -value = $value, 
      -mytable='users',
      -action = 'add',
      -short_output = 'yes');
/if;
if($valtypes -> find('datetime')->size);
  $myval += MO_val_datetime(
      -fname = $fname, 
      -label = $label, 
      -value = $value, 
      -mytable='users',
      -action = 'add',
      -short_output = 'yes');
/if;
if($valtypes -> find('time')->size);
  $myval += MO_val_time(
      -fname = $fname, 
      -label = $label, 
      -value = $value, 
      -mytable='users',
      -action = 'add',
      -short_output = 'yes');
/if;
if($valtypes -> find('int')->size);
  $myval += MO_val_int(
      -fname = $fname, 
      -label = $label, 
      -value = $value, 
      -short_output = 'yes');
/if;
if($value);
    if($myval);
    '<div class="fieldval_error">\n';
  // 'fname=';$fname;',value=';$value;',valtype=';$valtype;',myval=';$myval;
      $myval;
    '</div>\n';
    else;
    '<div class="fieldval_noerror">\n';
      'Ok';
    '</div>\n';
    /if;
  else;
  $helptext;
/if;
?>
