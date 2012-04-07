<?LassoScript

if: $action == 'add_action' || $action == 'update_action';

    // some of these actions are duplicated in email_fetch.lasso

    // available are the vars corresponding to fieldnames

    // pos_name


	// pos_name value vill be generated

	    Variable:'title' = $pos_title;
	    $title -> Trim; 
	    $title = (String_ReplaceRegExp: $title, -Find='\\s+', -replace='_'); 
	    If: $title -> Size > 40;
		// shorten it 
		$title = $title -> (SubString:1,40);
		Variable:'aTitle' = $title -> (Split:'_');
		$aTitle -> (Remove:($aTitle -> (Size)));
		$title = $aTitle -> (Join:'_');
	    /If;

	    // illegal chars in URLs
	    $title -> (Replace:'<', '');
	    $title -> (Replace:'>', '');
	    $title -> (Replace:'#', '');
	    $title -> (Replace:'%', '');
	    $title -> (Replace:'{', '');
	    $title -> (Replace:'}','');
	    $title -> (Replace:'\'','');
	    $title -> (Replace:'"', '');
	    $title -> (Replace:'|', '');
	    $title -> (Replace:'\\', '');
	    $title -> (Replace:'^', '');
	    $title -> (Replace:'[', '');
	    $title -> (Replace:']', '');
	    $title -> (Replace:'©', '');
	    // reserved chars in URLs
	    $title -> (Replace:';', '');
	    $title -> (Replace:'/', '');
	    $title -> (Replace:'?', '');
	    $title -> (Replace:':', '');
	    $title -> (Replace:'@', '');
	    $title -> (Replace:'=', '');
	    $title -> (Replace:'&', '');
	    // non-ASCII chars
	    $title -> (Replace:'å', 'a');
	    $title -> (Replace:'Å', 'A');
	    $title -> (Replace:'ä', 'a');
	    $title -> (Replace:'Ä', 'A');
	    $title -> (Replace:'ö', 'o');
	    $title -> (Replace:'Ö', 'O');
	    $title -> (Replace:'ü', 'ue');
	    $title -> (Replace:'Ü', 'Ue');
	    $title -> (Replace:'¦', 'oe');
	    $title -> (Replace:'é', 'e');
	    $title -> (Replace:'É', 'E');
	    $title -> (Replace:'á', 'a');
	    $title -> (Replace:'Á', 'A');
    //	'Jussi is testing: '; $title; '<br>';
	    $inlinestuff -> (Insert:'pos_name' = $title);
/If; // end if action = add or update


// speakBubble handler is in the monster file

?>
