<?LassoScript
Var:'remove' = (Action_Param:'remove'); 
Var:'file'   = (Action_Param:'file'); 

    Var: 'uploadErr' = 0;


    // user privileges?
    If: $userlevel_ses >= (LB_GetOption:'fileupload_minlevel');
	$speakBubble += 'Your userlevel is too low to manipulate files. ' + 
		'See the admin page Options > Uploads<br>';
    /If;

    // has the user defined a file to remove?

    If: !($file);
	$speakBubble += 'A file has not been specified!<br>';
    /If;
    If: $speakBubble != '';
	Include: $fromMeToRoot + $myInc + 'speakBubble.inc';
	Abort;
    /If; 

// set username & password
    Inline: $dbConfig, -Nothing;

    // does the file exist?

    If: !(File_Exists: $file);
	$speakBubble += 'The specified file <br>"' + $file + 
	    '"<br> does not seem to exist!<br>';
	Include: $fromMeToRoot + $myInc + 'speakBubble.inc';
	Abort;
    /If;

    // check the path to trash folder

    Var:'trash_path' = (LB_getOption:'fileupload_realpath') ;
    If: !($trash_path -> (EndsWith:'/'));
	$trash_path += '/';
    /If;
    $trash_path += 'trash/';
    If: !(File_Exists: $trash_path) || !(File_IsDirectory:$trash_path);
	$speakBubble += 'The trash folder cannot be found in the ' + 
		'content directory specified in Options > Uploads!<br>';
	Include: $fromMeToRoot + $myInc + 'speakBubble.inc';
	Abort;
    /If;

    // extract the file name

    Var:'aTemp' = $file -> (Split:'/');
    Var:'temp' = $aTemp -> Last;

    // ok - move the file to trash

    File_Move: $file, ($trash_path + $temp), -FileOverwrite;

    If: (File_CurrentError) == 'No Error';
	If: $lang == 'fi';
	    $speakBubble = 'Tiedosto siirrettiin roskakoriin (' + 
		    $trash_path + ').';
	Else;
	    $speakBubble = 'The file was moved to the trash folder (' + 
		    $trash_path + ').';
	/If;
    Else;
	If: $lang == 'fi';
	    $speakBubble = 'Tiedoston siirto roskakoriin (' + 
	    	$trash_path + ') ei onnistunut.';
	    $speakBubble += '<br>Toiminto palautti virhekoodin "' 
				+ File_CurrentError + '"';
	Else;
	    $speakBubble = 'The file was NOT moved to the trash (' + 
	    	$trash_path + ').';
	    $speakBubble += '<br>The following error code was returned: "' 
				+ File_CurrentError + '"';
	/If;
    /If;

    If: $speakBubble != '';
	Include:$fromMetoRoot + 'inc/speakBubble.inc';
    /If;

/Inline;
?>
