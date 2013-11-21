<?LassoScript
include('inc/defaults.inc');
    Session_End(-Name=$sesname); // defined in session.inc
          var('hash' = (LB_createCookiehash));
          Cookie_Set( ('user_' + $hash) 
              = $un_input,-Expires=(-1),-Domain=$myDomain,-Path='/');
          Cookie_Set( ('pass_' + $hash) 
              = $pw_input,-Expires=(-1),-Domain=$myDomain,-Path='/');
?>

<div id="logout" style="padding:0px 0 30px 0;font-size:1.4em;font-weight:bold;">

    <p style="text-align:center;">
	[If( $lang == 'fi')]Olet kirjautunut ulos sivustolta.
	[Else]You have logged out of the site.
	[/If]
    </p>
    <p style="text-align:center;">
    <A HREF="index.lasso?ignorecookie=yes">[If( $lang == 'fi')]Kirjaudu uudelleen sis‰‰n[Else]Login again[/If]</A></p>
</div>

