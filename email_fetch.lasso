<?LassoScript //>
Variable:'fromMetoRoot' = '';
Include:'config.lasso';
Library:'inc/library.inc';

// init the basic options datatype
Variable:'basicOptions' = LB_basicOptions;

var:'testmode' = ''; 
// if set to any non-empty value, diagnostic code will be displayed on page
// otherwise the page will function normally

// get the relevant options

Inline:-search,$dbConfig,
    -Table=$table_prefix+'options',
    -logicalop='or',
    'opt_name'='mailserver_url',
    'opt_name'='mailserver_port', 
    'opt_name'='mailserver_login', 
    'opt_name'='mailserver_pass',
    'opt_name'='default_email_category';
	if: $testmode;
	    'test: found options = ' + found_count + '<br>';
	/if; 
	records;
	    var:(field:'opt_name') = (field:'opt_value');
	/records;
/inline;

var: 'myPOP' = (email_POP:
    -host = $mailserver_url,
    -port = $mailserver_port,
    -username = $mailserver_login,
    -password = $mailserver_pass,
    -get = 'UniqueID'
);

var:'myID' = integer;

// open pop connection
// note: 070827: the next only works if I first call $mypop -> size;
// weird!
    var:'testvar' = $myPOP -> size;

    iterate:$myPOP,(var:'myID');
	if: $testmode;
	    'test: myID = '; $myID; '<br>';
	/if;
	var:'myMSG' = $myPOP -> retrieve;
	$myPOP -> delete; // marks the message to be deleted

	var:'myParse' = (email_parse: $myMSG);
	var:'mysender' = $myParse -> (Header:'from',-extract);
	if: $testmode;
	    'test: mysender = '; $mysender; '<br>';
	/if;
	var:'myBody' = $myParse -> (body:'text/plain');
	var:'mySubject' = $myParse -> subject;

	// strip signature, if exists
	$myBody = $myBody -> (split:'\r\n--\r\n');
	$myBody = $myBody -> (Get:1);
	if: $testmode;
	    'test: myBody = '; $myBody; '<br>';
	/if;

	// check the user
	var:'userOK' = 'no';

	Inline:-search,$dbConfig,
	    -Table=$table_prefix+'users',
	    -op='eq',
	    'use_email' = $mySender;
		if: $testmode;
		    'test: users found = ' (found_count) + '<br>';
		/if;
		var:'nickname' = string;
		var:'userlevel' = 0;
		var:'userid' = 0;
		    records;
			if: (field:'use_level') > $userlevel;
			    $nickname = (field:'use_nickname');
			    $userid = (field:'id');
			    $userlevel = (field:'use_level');
			    if: $testmode;
				'test: nick = '; $nickname; '<br>';
				'test: level = '; $userlevel; '<br>';
			    /if;
			/if;
		    /records;
	    if: $userlevel > 1;
		$userOK = 'yes';
	    /if;
	/inline;
	
	var:'myAddError' = string;
	if: $testmode;
	    'test: userok = '; $userok; '<br>';
	/if;

	if: $userOK == 'yes';
	    // proceed to posting - first process some field values;
	    // the $title processing is copied from 
	    // admin/custom/validation/man-postedit.inc

		$myBody -> Trim;

	    // pos_name (temporary var: title)

		Variable:'title' = $mySubject;
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

	    Inline:-add,$dbConfig,
		-Table=$table_prefix+'posts',
		-KeyField='id',
		'pos_author'= $userid,
		'pos_date' = (Date_Format:(Date),-Format=$date_dbformat),
		'pos_title' = $mySubject,
		'pos_name' = $title,
		'pos_content' = $myBody,
		'pos_category'= $default_email_category,
		'pos_status' = 'publish',
		'pos_comment_status' = 'open'
		;
		    var:'myPostid' = (keyfield_value);
		    $myAddError = (Error_CurrentError);
	    /inline;

	/if; // end if userOK = yes

    // who to notify
    var:'notifyTo' = string;
    var:'notifySubject' = string;
    var:'notifyBody' = string;

    if: $userOK == 'yes' && $myAddError == 'No Error';
	    $notifyTo = $mySender;
	    $notifySubject = ($basicOptions -> 'blogname') 
		+ ': ' + (str:'Thank you for your blog posting');
	    $notifybody = (str:'The blog posting that you sent by email has just been successfully received. ');
	    $notifybody += (str:'You may browse your post here:');
	    $notifybody += '\n\n' + ($basicOptions -> 'siteurl') + '?p=' + $myPostID;
	    $notifybody += '\n\n...' + (str:'or edit the post here:');
	    $notifybody += '\n\n' + ($basicOptions -> 'siteurl') + 'admin/man-postedit.lasso?id=' + $myPostID;
    else;
	    $notifyTo = ($basicOptions -> 'admin_email');
	    $notifySubject = ($basicOptions -> 'blogname') 
		+ ': Error when someone tried to post by email';
	if: $userOK == 'yes';
	    // error in add operation
	    $notifybody = 'An add error ' + $myAddError + ' occurred when ';
	    $notifybody += $mySender + ' tried to submit a blog post by email.';
	    $notifybody += 'The email was deleted and not added to the blog.';
	    $notifybody += 'The body of the email was:\n\n' + $myBody;
	else;
	    // user was not OK
	    $notifyBody = 'An unknown or unprivileged user ' + $mysender;
	    $notifyBody += ' tried to post to your blog. ';
	    $notifybody += 'The email was deleted and not added to the blog.';
	    $notifybody += 'The body of the email was:\n\n' + $myBody;
	/if;
    /if;

    // send the notification 

    Email_Send: -From=($basicOptions -> 'admin_email'),
	    -To=$notifyTo,
	    -Subject=$notifySubject,
	    -Body=$notifyBody;


    // close the big iterate

    /iterate;


// close pop connection, and delete messages that have been read
    $myPOP -> close;

    
?>
