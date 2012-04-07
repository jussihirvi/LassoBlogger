<?LassoScript
    If: $action == '' || ($action == 'lisaa' && $dbchange_error);
	var:'emptyform' = 'yes';
	If: $lang == 'fi';
	    $speakBubble = 'Ole hyvä ja täytä lomake!';
	Else;
	    $speakBubble = 'Please fill in and submit!';
	/If;
	Include:$fromMetoRoot + $myInc + 'speakBubble.inc';
    /if;
?>
