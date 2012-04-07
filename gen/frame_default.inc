<?LassoScript

// login procedure here is similar to that in gen/frame_admin.inc
// except that here no session is initiated; user data
// (username & encrypted password) is stored only in cookies

var:'showlogin' = (action_param:'showlogin'); // yes = user wants to see the login form
var:'loginsubmitted' = (Action_Param:'loginsubmitted'); // "yes" means submitted
var:'valid' = (integer);
var:'cusername' = (string);
var:'cpassword' = (string);
var:'speakBubble' = (string); 
var:'hash' = (LB_createCookiehash);

// 'test: cookie = ' + (Cookie:('bloguser_' + $hash));

// look for cusername & cpassword

If: $use_level <= 0;	     // not logged in; for var def, see stub file
    // first check for cookies
    if: $loginsubmitted != 'yes'; // login form has not been submitted
	If: (Cookie:('bloguser_' + $hash));
	    $cusername = (Cookie:('bloguser_' + $hash));
	/If;
	If: (Cookie:('blogpass_' + $hash));
	    $cpassword = (Cookie:('blogpass_' + $hash));
	/If;
    else;		// login form has been submitted
	$cusername=(Action_Param:'cusername');
	$cpassword=(Encrypt_MD5:(Action_Param:'cpassword'));
    /If;
/If;

If: $cusername  && $cpassword;	    // check input values
    Inline: -search, 
	$dbConfig, 
	-table=$table_prefix+'users',
	-operator='eq',
	'use_login'=$cusername, 
	-operator='eq',
	'use_pass'=$cpassword;

    // use this error handler for testing
    // Include: $fromMetoRoot + $myInc + 'errorhandler_explicit.inc';

    // if username & password were ok

    If: (Error_CurrentError)==(Error_NoError)
		&& (Found_Count) == 1 
		&& $cusername == (Field:'use_login')
		&& $cpassword == (Field:'use_pass'); // un & pw were ok

	// write vars
	$valid = 1;
	$use_level = (field:'use_level');

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
        $valid = -1;
	$speakBubble = (str:'Could not identify user. ');
	Include: $fromMetoRoot + $myInc + 'speakBubble.inc';

    Else: (Error_CurrentError)!=(Error_NoError);
        var:'valid' = -2;
        $speakBubble= 'An error occurred!' + 
	    (Error_CurrentError:-ErrorCode)+', '+(Error_CurrentError);
	Include: $fromMetoRoot + $myInc + 'speakBubble.inc'; 
    /if;

    /Inline;

/If;          /* end check username & password */

    // debug space

    //   'Test: valid = ' + $valid + '<br>'; 
    //   'use_level=' + $use_level + '<br>';

    // end debug space

If: $valid != 1 && ($showlogin == 'yes' || $loginsubmitted == 'yes');

// show login form
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<HTML><HEAD>

<META NAME="ROBOTS" CONTENT="NOINDEX">
<META NAME="ROBOTS" CONTENT="NOFOLLOW">
<META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">

<TITLE>[str:'Login']</TITLE>

<link rel="stylesheet" href="admin/admin.css" type="text/css">
<style type="text/css">
<!--
-->
</style>

</HEAD>
<BODY OnLoad="javascript:document.MyForm.cusername.focus();">
<div id="page">

<div id="login">
    <p STYLE="text-align:center;">
    [str:'Please login']
    </p>

<form action="[$myfilename]" method="POST" id="loginform" name="MyForm">
<input type="hidden" name="loginsubmitted" value="yes">

<?LassoScript
    iterate: (client_getparams), local:'i';
	if: !((#i -> first) -> (beginswith: '__')) &&
	  !((#i -> first) -> (beginswith: '-')) &&
	  !((#i -> first) == 'loginsubmitted') &&
	  !((#i -> first) == 'showlogin') &&
	  !((#i -> first) == 'cusername') &&
	  !((#i -> first) == 'cpassword');
	  '<input type=\"hidden\" name=\"' + (#i ->first) + '\" value=\"' + 
		    (#i -> second) + '\">\r';
    //      var:(#i ->first) = (#i -> second);
	/if;
    /iterate;
?>

<table id="logintable" cellpadding="0" cellspacing="5" border="0" STYLE="margin-left:auto; margin-right:auto;">
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
<INPUT TYPE="Hidden" NAME="-nothing" VALUE="Kirjaudu">
</p>
</FORM>

<ul>
	<li><a href="[$basicOptions -> 'siteurl']" title="are you lost?">&laquo; [str:'Back to blog']</a></li>
<!--	<li><a href="[$myFilename]?action=lostpassword" title="password lost and found">lost your password? (not built yet)</a></li> -->
</ul>
</div>
</div>
</BODY>
</HTML>

<?LassoScript
/If;  // end show login form 

// If: $valid >= 0 && $showlogin != 'yes'; // old code

if: $valid == 1 || ($showlogin != 'yes' && $loginsubmitted != 'yes');
// true always when login form is *not* shown

// now we get the real content
?>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<HTML><HEAD>

<META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">

<TITLE>[$basicOptions -> 'blogname'][If: $page_title != ''] - [$page_title][/If]</TITLE>


<link rel="stylesheet" href="[$myStylesheet]" type="text/css">
<style type="text/css">
<!--

/* 
Styles affecting the header DIV (the colored area at top of page)
Modify these to your liking
*/

#header {
	padding: 0;
	margin: 0 auto;
	height: 70px;
	width: 100%;
	background-color: #73a0c5;
	}

#headerimg {
	margin: 0;
	height: 70px;
	width: 100%;
/* uncomment & adjust to include your custom header image */
/*	background-repeat: no-repeat;			  */
/*	background-image: url(themes/default/header.jpg); */
	}


h1, h1 a, h1 a:hover, h1 a:visited {
	padding-top: 4px;
	margin: 0;
	color:white;
	font-family: 'Trebuchet MS', 'Lucida Grande', Verdana, Sans-Serif;
	font-weight: bold;
	font-size: 1.8em;
	text-align: center;
	text-decoration: none;
	}

.description {
	text-align: center;
	color:white;
	font-size: 1.2em;
	text-align: center;
	}

-->
</style>

</HEAD>
<BODY>
<div id="page">

<?LassoScript
If: $header != '';
    Include: $header;
/If;
Include: (Var:'contents');
If: $footer != '';
    Include: $footer;
/If;
?>

</div> <!-- end pagearea -->
</BODY>
</HTML>
[/if]
