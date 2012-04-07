<?LassoScript //>
include($fromMetoRoot + 'admin/gen/session.inc');
include($fromMetoRoot + 'config.lasso');
library($fromMetoRoot + 'inc/library.inc');
library($fromMetoRoot + 'admin/inc/libtemp.inc'); // for testing
library($fromMetoRoot + 'admin/inc/libmonster.inc');
library($fromMetoRoot + 'admin/inc/liblocal.inc');

// START OPTIONS
if( variable_defined('basicOptions') ); // usually siteconfig
  var('opts') = $basicOptions;
else;
  // LASSOBLOGGER - get opts from database
  var('opts')         = LB_siteOptions;
/if;
if( ! variable_defined('basicOptions') ); // add missing opts
$opts->insert('webmaster_email' = $opts->find('admin_email'));
$opts->insert('sitename_long'   = $opts->find('blogname'));
$opts->insert('sitename'        = string($opts->find('siteurl')));
// string is there to break reference to siteurl in L9
$opts->find('sitename') -> removeLeading('http://');
$opts->insert('date_showformat' = $opts->find('date_format'));
$opts->insert('time_showformat' = $opts->find('time_format'));
$opts->insert('datetime_showformat' = '%-d.%-m.%Y %H.%M.%S');
$opts->insert('time_dbformat' = '%T');
$opts->insert('date_dbformat' = '%Q');
$opts->insert('datetime_dbformat' = '%Q %T');
$opts->insert('floatseparator_show' = ',');
$opts->insert('alertcolor' = '#066');// dark green; default: bright red, #c03
/if;
// myserver, secureserver; for LassoBlogger, replace with literal definition
if( ! variable_defined('myserver'));
    var('myserver' = 'http://' + server_name); 
/if;
if( ! variable_defined('secureserver'));
    var('secureserver' = 'https://' + server_name); 
/if;
$myserver     = $opts -> find('siteurl');
$secureserver = $opts -> find('siteurl');

var('basicOptions') = $opts;
// END OPTIONS

// AUTH CONFIG
  var('debug'           = ''); // any value will enable auth debug
  var('accept_cookies'  = 'yes'); // yes,no
  var('userlist'        = 'database'); // database, adminusername
    // params for database check
    var('db_authtable'    = 'users'); // no table_prefix here
    var('db_unfield'      = 'use_login');
    var('db_pwfield'      = 'use_pass');
    var('db_pw_encrypt'   = 'md5');
// END CONFIG

// caution for undefined vars
if( ! variable_defined('myadminpages') ); // usually in siteconfig
  var('myadminpages') = 'admin';
/if;
var('myp') = action_param('myp');
// LASSOBLOGGER START
var('filetagsUsername') = $dbUsername;
var('filetagsPassword') = $dbPassword;
// LASSOBLOGGER END

// uploads - these values can be overridden in stub files (of course)
var('upload_path' = $opts -> find('fileupload_path'));
$upload_path -> removeTrailing('/');
$upload_path += '/';
  var('upload_subpath') = 'uploads/'; 
  if( $myfilename == 'man-post-form.lasso' );
      $upload_subpath += 'post-related/';
  else;
      $upload_subpath += 'filevault/';
  /if;
var('upload_trashpath' = $upload_path + 'trash/'); 
var('allowed_file_suffixes' = $opts -> find('fileupload_allowedtypes'));
var('upload_maxk' = $opts -> find('fileupload_maxk'));
// image sizes for server-processed images
//  slot 1) subdir name (check manually that they exist)
//  slot 2) size in pixels  
//  slot 3) checkbox legend for upload form
var('upload_imagesizes') = array(
  array('halfcolumn','240','half column wide'),
  array('fullcolumn','540','full column wide'),
  array('fullscreen','1200','full screen wide')
); 


// initialize the navigation data object
var('navig'=MO_adminnavig(
    -page=$myfilename,
    -includefile=$fromMetoRoot + 'admin/inc/navig.inc')); 
var('thiscatid') = $navig -> get_catid;

var('contents')	    = $fromMetoRoot + 'admin/gen/monster.inc';
var('header')       = $fromMetoRoot + 'admin/inc/header.inc';
var('footer')       = $fromMetoRoot + 'admin/inc/footer.inc';
var('myStylesheet') = $fromMetoRoot + 'admin/inc/style.css';
var('myimages')     = $fromMetoRoot + 'admin/images';

if(var('ckeditor') != 'enable'); // var probably not defined
  var('ckeditor' = 'disable');
/if;
var('pageSecuritylevel') = 0; 

var('maxrecords_formview') = 1;   
var('maxrecords_listview') = 25;   
var('dbtype') = 'mysql';   // fmp, mysql (vaikuttaa pvm-käsittelyyn

var('size1default')='450px'; // default (text: size, textarea: cols)
var('size2default')='4';  // default (textarea: rows)
var('defaultClass')='pieni'; // normaalisti "pieni"; deprecated?? 
var('inputtextClass')='normaalikoko'; 
    //  "pieni,puolipieni,normaalikoko,isohko,iso"
var('textClass')='pieni';   
    //  "pieni,puolipieni,normaalikoko,isohko,iso"

if($myfilebody >> '-form');
  var('viewtype' = 'formview' );
  var('updatebehavior' = 3 );
else($myfilebody >> '-list');
  var('viewtype' = 'listview' );
  var('updatebehavior' = 1 );
else; // guess
  var('viewtype' = 'listview' );
  var('updatebehavior' = 1 );
/if;
// updatebehavior #1: search kept, updated rec shown only if search allows this
//              (keyvalue nulled after update)
//              (good for listview)
// updatebehavior #2: search bypassed, only updated rec shown
//              (keyvalue kept, hakulomake emptied)
//              (good for formview)
// updatebehavior #3: search kept, updated record added
//              (keyvalue kept, searchform kept)
//              (good for formview)
var('searchBehavior') = 1;   // 1 or 2 (see below)
// searchBehavior #1: if searchform is empty, then find all
// searchBehavior #2: if searchform is empty, then find nothing
var('silentAdd') = 'no';  // suppresses "a record was added" message
var('must_authenticate') = 'yes'; 
var('confirm_add') = 'no'; // confirmation text in custom/addconfirmation 
var('confirm_update') = 'no'; // confirmation text in custom/updateconfirmation 

var('publicPage')=''; // don't comment out

// * * * ID field insert begin * * * 

var('id_insert' = string);
// to be inserted in editform
// example here
// Array('ID','ID','replacecode','','','','','',$id_insert,''),

if($myfilebody >> '-form');
  $id_insert = '<font size="-2"><b>[$fieldvalue]</b> (luotu <b>[Field(\'creationdate\')]</b>, muokattu <b>[Field(\'moddate\')]</b>)</font>';
else($myfilebody >> '-list');
  $myfilebody -> replace('-list','-form');
  $id_insert = '<a href="' + $myfilebody + '.html?id=[$fieldvalue]" TITLE="Edit this item">[$fieldvalue]</a>';
  // change it back! Needed in Lasso 9
  $myfilebody -> replace('-form','-list');
else; // guess
/if;
// * * * ID field insert end * * * 
var('br') = '<br>';
var('speakBubble')  = string;

?>
