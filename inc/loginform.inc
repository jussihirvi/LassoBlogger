<div id="login">
    <h1>
    [str('Please login')]
    </h1>

<form action="[$myfilename]" method="POST" id="loginform" name="MyForm">
<input type="hidden" name="loginsubmitted" value="yes">

<?LassoScript
    iterate( client_getparams, local('i') );
	if( !((#i -> first) -> (beginswith: '__')) &&
	  !((#i -> first) -> (beginswith: '-')) &&
	  !((#i -> first) == 'loginform_submitted') &&
	  !((#i -> first) == 'showlogin') &&
	  !((#i -> first) == 'cusername') &&
	  !((#i -> first) == 'cpassword'));
	  '<input type=\"hidden\" name=\"' + (#i ->first) + '\" value=\"' + 
		    (#i -> second) + '\">\r';
    //      var((#i ->first) = (#i -> second));
	/if;
    /iterate;
?>

<div class="label">[str('Username')]</div>
<input id="username" type="text" name="cusername" value="[$cusername]">
<div style="clear:both;"></div>

<div class="label">[str('Password')]</div>
<input id="password" type="password" name="cpassword" value="">
<div style="clear:both;"></div>
<div class="label">&nbsp;</div>
<INPUT TYPE="Submit" NAME="-nothing" id="submit" VALUE="[str('Login')] &raquo;">
<INPUT TYPE="Hidden" NAME="-nothing" VALUE="Kirjaudu">
<div style="clear:both;"></div>
</FORM>

<ul>
	<li><a href="[$opts -> find('siteurl')]" title="are you lost?">&laquo; [str('Back to the site')]</a></li>
	<li><a href="/lostpassword.html" title="[str('Forgot your password?')]">[str('Forgot your password?')]</a></li> 
</ul>
</div>

