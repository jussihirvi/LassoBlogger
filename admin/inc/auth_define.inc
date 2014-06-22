<?LassoScript

    var('auth_source' = string); // cookie, form
    // cookies?
    If( $accept_cookies == 'yes'
        && $loginform_submitted != 'yes' 
        && (Action_Param('ignorecookie') != 'yes'));

	// ignorecookie is 'yes' in the link on the page admin/logout.lasso
	// to facilitate login with different user identities

 	var('hash' = LB_createCookiehash);
        $un_input = cookie('user_' + $hash);
        $pw_input = cookie('pass_' + $hash);
        $auth_source = 'cookie';
//        'cookies used. un is "'; $un_input; '", un type = ';$un_input->type;
//        '. pw is "'; $pw_input; '", un type = ';$pw_input->type;
//        '<br>';

    // form input?
    Else( $loginform_submitted == 'yes'); 
	$un_input = Action_Param('un_input');
 	$pw_input = Action_Param('pw_input');
        $auth_source = 'form';
	$un_input -> Trim;
	$pw_input -> Trim;
	If( !($un_input && $pw_input) );
          $speakBubble = str('Some of the required fields were empty.');
	/If;
	$logincount_ses += 1; 		// session var
        var('sleeptime'=(($logincount_ses - 1) * 500)); 
        sleep( $sleeptime);	// to annoy crackers
    /If;
?>
