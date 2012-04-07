<?LassoScript

if: $action == 'add_action' || $action == 'update_action';

    // available are the vars corresponding to fieldnames

    // use_pass
    
        If: $action == 'add_action' || 
		($action == 'update_action' && $use_pass != '');
        	// in updates, password may be left blank

	    If: $use_pass -> Size < 4 ||
	     $use_pass -> Size > 10;
		$speakBubble +=(str:'The password must be 4-10 chars long!<br>');
	    /If;

            If: !(String_IsAlphaNumeric: $use_pass);
                $speakBubble += (str:'Only alphanumeric characters (a-z, 
		   A-Z and 0-9) are allowed in the field') + ' Password<br>';
            /if;

    /If;

    // use_url


	// must not contain http

	If: $use_url -> (BeginsWith: 'http');
	    $speakBubble +=(str:'Please give')+' Website'+(str:' without the "http://"!') + '<br>';
	/If;
/If;

// speakBubble handler is in the monster file


?>
