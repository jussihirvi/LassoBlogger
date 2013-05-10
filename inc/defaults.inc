<?lassoScript
var('fromMetoRoot' = '');
Include('config.lasso');
Library('inc/library.inc');

var('opts' = LB_siteOptions);
var('catz' = LB_cats);
// add missing options
$opts->insert('date_showformat' = $opts->find('date_format'));
$opts->insert('time_showformat' = $opts->find('time_format'));

// authentication - feed loginform output to LB_userinfo, if any
// otherwise, the type checks for cookies

var('userInfo') = LB_Userinfo(
  -username=action_param('cusername'),
  -password=action_param('cpassword')
);
var('use_level') = $userInfo -> LB_getUse_level;

//    /* DEBUG SPACE
//    'test - uselevel = ' + $use_level + '<br />';
//    'test - tst = ' + $userinfo -> 'tst' + '<br />';
//    'test - found = ' + $userinfo -> 'found' + '<br />';
//    'test - cusername = ' + $userinfo -> 'cusername' + '<br />';
//    'test - cpassword = ' + $userinfo -> 'cpassword' + '<br />';
//    */

var('speakBubble' = '');
var('br') = '<br />';
var('myInc' = 'inc/');
var('myImages' = 'images/');

var('error' = integer);
var('errorText' = string);

// vars related to authentication
var('showlogin') = action_param('showlogin'); // yes = user wants to see the login form
var('loginsubmitted') = Action_Param('loginsubmitted'); // "yes" means submitted
var('cusername') = action_param('cusername');
var('cpassword') = action_param('cpassword');

var('myStylesheet' = 'themes/' + $opts->find('theme') + '/style.css');
var('myThemeImages'= 'themes/' + $opts->find('theme') + '/images');
var('header'='inc/header.inc');
var('menu'='inc/menu.lasso');
var('footer'='inc/footer.inc');

// uploads - these values can be overridden in stub files (of course)
var('upload_path' = $opts -> find('fileupload_path'));
$upload_path -> removeTrailing('/');
$upload_path += '/';
var('upload_subpath') = 'uploads/'; 
var('upload_trashpath' = $upload_path + 'trash/'); 
?>
