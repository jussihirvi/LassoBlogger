<?LassoScript

if: $action == 'add_action' || $action == 'update_action';

    // available are the vars corresponding to fieldnames

    // use_pass

	// field is required, if the action is add

	If: $action == 'add_action';
	    If: !(Var:'use_pass');
		If: $lang=='fi';
		$speakBubble +='Anna salasana!<br>';
		Else;
		$speakBubble +='Please give the password!<br>';
		/If;
	    /If;
	/If;

    // use_url


	// must not contain http

	If: (Var:'use_url') -> (BeginsWith: 'http');
	    If: $lang=='fi';
	    $speakBubble +='Anna Website ilman "http"-alkulitaniaa!<br>';
	    Else;
	    $speakBubble +='Please give Website without the "http://"!<br>';
	    /If;
	/If;
/If;
// speakBubble handler is in the monster file


?>
