<?LassoScript
// 'Test: logincount=' + $logincount_ses + ' valid=' + $valid_ses + '<br>';
// 'Test: valid=' + $valid_ses + ', user_ses=' + $user_ses + ', userlevel_ses=' + $userlevel_ses + '<br>';

var:'loginsubmitted'= (Action_Param:'loginsubmitted'); // "yes" means submitted
var:'cusername'	    = string;
var:'cpassword'	    = string;
var:'speakBubble'   = string; 
var:'hash' = (LB_createCookiehash);

var:'dbAction'=integer; 
	// serves same function as $dbChange in /gen/adminpage_monster.inc
	// here I use a differene name to avoid confusion with monster
    If: (Action_Param:'add_action')!='';
      $dbAction = 1;
    Else: (Action_Param:'update_action')!='';
      $dbAction = 1;
    Else: (Action_Param:'delete_action')!='';
      $dbAction = 1;
    Else: (Action_Param:'delet2_action')!='';
      $dbAction = 1;
    Else: (Action_Param:'upload')!='';
      $dbAction = 1;
    /If;

If: $valid_ses != 1;	// not logged in
    // first check for cookies
    If: $loginsubmitted != 'yes'; // login form has not been submitted
	If: (Cookie:('bloguser_' + $hash));
	    var:'cusername' = (Cookie:('bloguser_' + $hash));
	/If;
	If: (Cookie:('blogpass_' + $hash));
	    var:'cpassword' = (Cookie:('blogpass_' + $hash));
	/If;
    Else;		    // login form has been submitted 
	$cusername=(Action_Param:'cusername');
	$cpassword=(Encrypt_MD5:(Action_Param:'cpassword'));

	// be nasty to amnesiacs
	$logincount_ses += 1; 		// session var
	    var:'sleeptime'=(($logincount_ses - 1) * 500); 
	    Sleep: $sleeptime;	// childish, I know
    /If;
/If;

If: $cusername && $cpassword;	// check cusername & cpassword

    Inline: -search, 
	$dbConfig, 
	-table=$table_prefix+'users',
	-operator='eq',
	'use_login'=$cusername, 
	-operator='eq',
	'use_pass'=$cpassword;
    // use this error handler for testing
    // Include: $fromMetoRoot + $myInc + 'errorhandler_explicit.inc';

    If: (Error_CurrentError)==(Error_NoError)
		&& (Found_Count) == 1 
		&& $cusername == (Field:'use_login')
		&& $cpassword == (Field:'use_pass'); // un & pw were ok

	// write session vars
        var:'valid_ses'=1; 		// the user is valid
	var:'user_ses'=(Field:'ID');
	var:'userlevel_ses' = (Field:'use_level'); // admin default =10
						    // probably not used

        // write/refresh cookies
	var:'path' = (response_path);
	$path -> (RemoveTrailing:'admin/');
	    Cookie_Set: ('bloguser_' + $hash) 
	    = $cusername,-Expires=432000,-Domain=$myDomain,-Path=$path;
	    Cookie_Set: ('blogpass_' + $hash) 
	    = $cpassword,-Expires=432000,-Domain=$myDomain,-Path=$path;
	    // remember that cpassword is now md5-encrypted

    // if username & password were NOT ok

    Else: (Found_Count) >= 2 || 
	((Error_CurrentError:-ErrorCode) == '-1728') ||
	    (((Error_CurrentError) == (Error_NoError)) && 
	    (($cusername != (Field:'use_login')) ||
	    ($cpassword != (Field:'use_pass'))));
        var:'valid_ses' = -1;
	$speakBubble = (str:'Could not identify user. ');
	Include: $fromMetoRoot + $myInc + 'speakBubble.inc';

    Else: (Error_CurrentError)!=(Error_NoError);
        var:'valid_ses' = -2;
        $speakBubble= 'An error occurred!' + 
	    (Error_CurrentError:-ErrorCode)+', '+(Error_CurrentError);
	Include: $fromMetoRoot + $myInc + 'speakBubble.inc'; 
	$speakBubble = ''; // reset
    /if;


    // logincount - prevent flooding
    // temporarily disabled

?>

    [Output_None][NoProcess]

    If: $logincount_ses == 5;  /* flood attempt - send email */ 
	var:'emailbody' = 'A user has tried to login 5 times. ' + 
       'On the last trial, the username was \"' + $cusername + '\"';

	Email_Send: 				// fill in -Host, if needed
	-From = $basicOptions -> 'admin_email',
	-To   = $basicOptions -> 'admin_email',
	-Subject = ($basicOptions -> 'blogname') + ' - flood attempt detected',
	-Body = $emailbody;

	Else: $logincount_ses > 7;   // seven trials - say goodbye  
	    $speakBubble = (str:'Too many trials. Please try again later!');
	Include: $fromMetoRoot + $myInc + 'speakBubble.inc';
	Abort;
    /If; 			// end prevent flooding 

    [/NoProcess][/Output_None]

<?LassoScript

    /Inline;

/If;          /* end check username & password */

    /*   // debug space

    // 'Test: logincount=' + $logincount_ses + 
	' valid=' + $valid_ses + '<br>';
    'valid=' + $valid_ses + '<br>'; 'userlevel=' + $userlevel_ses + '<br>';

    // end debug space

    */

If: ((Variable_Defined:'valid_ses') == false) || (($valid_ses != 1));

// if user is not valid, show login form
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<HTML><HEAD>

<META NAME="ROBOTS" CONTENT="NOINDEX">
<META NAME="ROBOTS" CONTENT="NOFOLLOW">
<META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">

<TITLE>[str:'Login']</TITLE>

<link rel="stylesheet" href="[$myStylesheet]" type="text/css">
<style type="text/css">
<!--
-->
</style>

</HEAD>
<BODY OnLoad="javascript:document.MyForm.cusername.focus();">

<div id="login">

<?LassoScript

If: !($dbAction);
    
    /***  if dbAction *is* 1, a speakBubble is presented. It's
    meant to calm down the user, whose session has been
    broken, just when (s)he tried to edit the db  ***/
?>
    <p STYLE="text-align:center;">
    [str:'Please login']
    </p>
[/If]

<form action="[$myfilename]" method="POST" id="loginform" name="MyForm">

<input type="hidden" name="loginsubmitted" value="yes">
<?LassoScript

If: $dbAction; 
    $speakBubble = 'The operation will be continued after reauthentication.';
    Include: $fromMetoRoot + $myInc + 'speakBubble.inc';

    iterate: (Action_Params), local:'i';
	if: !((#i -> first) -> (beginswith: '__')) &&
	  !((#i -> first) -> (beginswith: '-')) &&
	  !((#i -> first) == 'loginsubmitted') &&
	  !((#i -> first) == 'cusername') &&
	  !((#i -> first) == 'cpassword');
	    Output: '<input type=\"hidden\" name=\"' + (#i ->first) + '\" value=\"' + 
		    (#i -> second) + '\">\r',-encodeNone;
    //      var:(#i ->first) = (#i -> second);
	/if;
  /iterate;

/If;
?>
<table cellpadding="0" cellspacing="5" border="0" STYLE="margin-left:auto; margin-right:auto;">
<tr>
<td align="right">
[str:'Username']
</td>
<td>
<input type="text" name="cusername" size="20" value="">
</td>
<tr>
<td align="right">
[str:'Password']
</td>
<td>
<input type="password" name="cpassword" size="20" value="">
</td>
</tr>
</tr>
<tr><td colspan="2" align="center">
</td></tr>
</table>

<p class="submit">
<INPUT TYPE="Submit" NAME="-nothing" id="submit" VALUE="[str:'Login'] &raquo;">
<INPUT TYPE="Hidden" NAME="-nothing" VALUE="Login">
</p>
</FORM>
<ul>
	<li><a href="[$basicOptions -> 'siteurl']" title="are you lost?">&laquo; [str:'Back to blog']</a></li>
<!--	<li><a href="[$myFilename]?action=lostpassword" title="password lost and found">lost your password? (not built yet)</a></li> -->
</ul>
</div>
</BODY>
</HTML>

<?LassoScript
/If;  //end if valid not 1 

If: $valid_ses == 1;

// now we get the real content
?>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<HTML><HEAD>

<META NAME="ROBOTS" CONTENT="NOINDEX">
<META NAME="ROBOTS" CONTENT="NOFOLLOW">
<META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">

<TITLE>[$basicOptions -> 'blogname'] Admin - [$page_title]</TITLE>

<link rel="stylesheet" href="[$myStylesheet]" type="text/css">
<style type="text/css">
    [HTML_comment]
    [/HTML_comment]
</style>

</HEAD>

<?LassoScript

// one more check... 

If: $userlevel_ses < (Integer:$pageSecuritylevel); 
/* if user level is too low for access */
	str:'You do not have enough privileges to access this page.';
    Include: $fromMetoRoot + $myInc + 'speakBubble.inc';
Include: $footer;
Else; /* if user level is enough, show the page */



'<BODY>';

// Admin menu 

// first parameter: link address (minus .lasso), 
// second parameter: visible link text

var:'aAdminMenu' = (Array:
'index.lasso' 			= 'Dashboard',
'man-postedit.lasso?emptyForm=yes' = 'Write',
'man-postlist.lasso' 		= 'Manage',
'lin-list.lasso' 		= 'Links',
'upl-filelist.lasso' 		= 'Uploads',
// 'themes.lasso'		= 'Presentation*',
// 'plugins.lasso' 		= 'Plugins*',
'use-list.lasso' 		= 'Users',
'opt-general.lasso' 		= 'Options'
);

// draw the main menu

    '<ul id="adminmenu">';

    Iterate: $aAdminMenu, (Var:'i');
	    '<li><a href="';
	    $i -> (Get:1) + '"';
		// detect current
		If: ($i == 'index.lasso' && $myfilebody == 'index')
		  || ($i == 'man-postlist.lasso' && $myfilebody -> (BeginsWith:'man-'))
		  || ($i == 'lin-list.lasso' && $myfilebody -> (BeginsWith:'lin-'))
		  || ($i == 'upl-filelist.lasso' && $myfilebody -> (BeginsWith:'upl-'))
		  || ($i == 'use-list.lasso' && $myfilebody -> (BeginsWith:'use-'))
		  || ($i == 'opt-general.lasso' && $myfilebody -> (BeginsWith:'opt-'));
		' class="current"';
		/If;
	    '>' + $i -> (Get:2);
	    '</a></li>\r';
    /Iterate;
    '</ul>\r';

var:'aManageSubmenu' = (Array:
'man-postlist'	= 'List Posts',
'man-postedit'	= 'Write & Edit Posts',
'man-pagelist'	= 'List Pages',
'man-pageedit'	= 'Write & Edit Pages',
'man-cats'	= 'Categories',
'man-commlist'	= 'List Comments',
'man-commedit'	= 'Edit Comments',
'man-moderation'= 'Awaiting Moderation*'
);

var:'aLinksSubmenu' = (Array:
'lin-list'	= 'Links',
'lin-cats'	= 'Link Categories',
'lin-import'	= 'Import Links*'
);

var:'aUploadsSubmenu' = (Array:
'upl-cats'	= 'File Categories',
'upl-filelist'	= 'File List',
'upl-fileedit'	= 'Manage Files',
);
    If: $userlevel_ses < 8;
	var:'myText' = 'Edit your user info';
	Else;
	var:'myText' = 'Edit users';
    /If;
var:'aUsersSubmenu' = (Array:
'use-list'	= 'List users',
'use-edit'	= $myText
);

var:'aOptionsSubmenu' = (Array:
'opt-general'	= 'General',
'opt-writing'	= 'Writing',
'opt-reading'	= 'Reading',
'opt-discussion'= 'Discussion',
'opt-permalink'	= 'Permalinks',
'opt-uploads'	= 'Uploads',
'opt-misc'	= 'Miscellaneous'
);

// submenus - custom tag

Define_Tag: 'LB_submenu',-Required='aSubmenu';
	Local:'output' = '<ul id="adminmenu2">\r';
	Iterate: #aSubmenu, (Local:'i');
	    #output += '<li><a href=\'';
	    #output += #i -> (Get:1) + '.lasso\'';
		If: $myfilebody == #i -> (Get:1);
		#output += ' class="current"';
		/If;
	    #output += '>' + #i -> (Get:2);
	    #output += '</a></li>\r';
	/Iterate;
	#output += '</ul>\r';
	Return:#output;
/Define_Tag;

// draw submenus
    // Manage
	If: $myfilename -> (BeginsWith:'man-');
	    LB_submenu:-aSubmenu = $aManageSubmenu,-encodeNone; 
	/If;
    // Links
	If: $myfilename -> (BeginsWith:'lin-');
	    LB_submenu:-aSubmenu = $aLinksSubmenu,-encodeNone; 
	/If;
    // Uploads
	If: $myfilename -> (BeginsWith:'upl-');
	    LB_submenu:-aSubmenu = $aUploadsSubmenu,-encodeNone; 
	/If;
    // Users
	If: $myfilename -> (BeginsWith:'use-');
	    LB_submenu:-aSubmenu = $aUsersSubmenu,-encodeNone; 
	/If;
    // Options
	If: $myfilename -> (BeginsWith:'opt-');
	    LB_submenu:-aSubmenu = $aOptionsSubmenu,-encodeNone; 
	/If;
    ?>


[Output_None]

*** inactive submenus *** 

PRESENTATION submenu

<ul id="adminmenu2">
	<li><a href='themes.lasso' class="current">Themes</a></li>
	<li><a href='theme-editor.lasso'>Theme Editor</a></li>
</ul>
[/Output_None]

[Output_None]
PLUGINS submenu
<ul id="adminmenu2">
	<li><a href='plugins.lasso' class="current">Plugins</a></li>
	<li><a href='plugin-editor.lasso'>Plugin Editor</a></li>
</ul>
[/Output_None]

<div class="wrap">

[Include: $contents]
[Include: $footer]

</div> <!-- end wrap -->
</BODY>
</HTML>

[/If] <!-- if privileges -->
[/If] <!-- if username & password -->
