<?LassoScript //>
var('fromMetoRoot') = '';
Include('admin/inc/defaults.inc');
var('pageSecuritylevel') = 0;
// overrides
var('myStylesheet' = 'themes/default/style.css');
var('header'='inc/header.inc');
var('menu'='inc/menu.lasso');
var('footer'='inc/footer.inc');
var('size1default')='250px'; // default (text: size, textarea: cols)
var('size2default')='3';  // default (textarea: rows)
var('silentAdd') = 'yes';  // suppresses "a record was added" message

    If( integer($opts -> find('users_can_register')) != 1 );
        MO_speakBubble(str('Registration for this blog is not allowed.'));
abort;
    else;
        var('emptyform' = 'yes');
    /if;

var('myTable')='users';

var('rectype') = 'user';
var('rectype_pl') = 'users';
var('rectype_fi') = 'k�ytt�j�';
var('rectype_fi_part') = 'k�ytt�ji�';

var('contents')='admin/gen/monster.inc';

var('page_title')=str('Registration');

// for how to use $searchFields and $editFields, 
// see the beginning of adminpage_monster.inc

// patches
var('userInfo') = LB_Userinfo(
  -username=action_param('cusername'),
  -password=action_param('cpassword')
);
// vars related to authentication
var('showlogin') = action_param('showlogin'); // yes = user wants to see the login form
var('loginsubmitted') = Action_Param('loginsubmitted'); // "yes" means submitted
var('cusername') = action_param('cusername');
var('cpassword') = action_param('cpassword');

var('use_level') = $userInfo -> LB_getUse_level;


// search form
var('searchFields')=Array(
);

// edit form
var('editFields')=Array(
Array('id','','heading','','','','','',str('Fill in to register. ') + str('Fields marked with asterisk (*) are required')),
Array('use_login','Login Name','text','req,unique','','','Must be unique','','',''),
Array('use_pass','Password','password','req,password','','','','','','saveIfNotEmpty','md5'),
Array('use_level',str('User Level'),'replacecode','','','',$opts -> find('new_users_can_blog'),'','<input type="hidden" name="use_level" value="' + $opts -> find('new_users_can_blog') + '">'),
// posts?
Array('use_firstname','First Name','text','','','','','',''),
Array('use_lastname','Last Name','text','','','','','',''),
Array('use_nickname','Public Name','text','req,unique','','','Must be unique','',''),
Array('use_email','Email','text','req,email,unique','','','','',''),
Array('use_url','Website','text','','','','Without "http://"','',''),
Array('use_icq','ICQ','text','','','','Integer field','',''),
Array('use_aim','AIM','text','','','','','',''),
Array('use_msn','MSN','text','','','','','',''),
Array('use_yim','Yahoo','text','','','','','',''),
Array('use_description','Description','textarea','','','','','',''),
);

// add form

// var('addFields')=Array(
// Array('id','','nosave','','10','',str('Fill in to register. ') + str('Fields marked with asterisk (*) are required'),'',''),
// Array('use_login',str('Login Name'),'text','req;unique','10','','','','',''),
// Array('use_pass',str('Password'),'text','req;unique','10','','','','','','','md5'),
// Array('use_firstname',str('First Name'),'text','','','','','',''),
// Array('use_lastname',str('Last Name'),'text','','','','','',''),
// Array('use_nickname',str('Public Name'),'text','req;unique','','',str('May be your real name, or a nickname'),'',''),
// Array('use_email','Email','text','','','','','',''),
// Array('use_url',str('Website'),'text','','','',str('Without the "http://"'),'',''),
// Array('use_icq','ICQ','text','','','',str('Integer field'),'',''),
// Array('use_aim','AIM','text','','','','','',''),
// Array('use_msn','MSN','text','','','','','',''),
// Array('use_yim','Yahoo','text','','','','','',''),
// Array('use_description',str('Description'),'textarea','','40','3','','',''),
// );

// define sorting for the main search inline
var('sortstuff') = Array(
  -sortfield = 'use_nickname',
  -sortorder = 'descending'
);

var('viewtype') = 'formview'; // formview, listview

var( 'defaultSearch') = array;	
var('buttonlist') = map('add'='yes','update'='no','delete'='no');

// var('userlevel_ses') = integer; // cheat
// var('formTargetpage' = '');   // if target=self, comment this out

Var('customsearchValidations')='no';
var('customValidations')      = 'yes'; 
var('customPostprocess')      = 'no';		    
var('customPreform'           = 'yes'); 


Include('gen/frame_register.inc');

?>

