<?LassoScript

/*  OPTION GROUPING      OPTION ID  OPTION NAME        DEFAULT  NEGATIVE RESULT
    == Unconditional ==         32  default_comment_status  1    discard
    == AuthorSpecific ==         6  comment_registration    0    discard
    == Keys ==                  40  blacklist_keys          ;    discard
    == Links ==                 38  comment_link_limit      0    disc. or mod.
    == Unconditional  ==        35  comment_moderation      0    moderate
    == Keys ==                  39  moderation_keys         ;    moderate
    == AuthorSpecific ==        37  commentor_whitelist     0    moderate
    == Notifications ==         33  comments_notify         2     -
    == Notifications ==         34  moderation_notify       2     -
    == RequiredFields ==        36  require_name_email      1     -
*/

// basic vars
    Variable:'userInfo' = LB_Userinfo;
    // init the basic options datatype
    Variable:'basicOptions' = LB_basicOptions;

    If: (Action_Param:'action') != 'add';
	str:'Illegal input for this page';
	Abort;
    /If;

// field-related settings

Variable:'editFields' = (Array:
    (Array:'com_post_id','Message ID','text','req'),
    (Array:'com_author','Author','text',''),
    (Array:'com_author_email','Email','text',''),
    (Array:'com_author_url','URL','text',''),
    (Array:'com_title','Title','text',''),
    (Array:'com_content','Comment text','text','req')
    );

// action_params converted to variables

    Variable:'inlinestuff' = (Array), 'this' = (Array);

    Iterate: $editFields, $this;
	Variable:($this -> (Get:1)) = (Action_Param:($this -> (Get:1)));
	(Var:($this -> (Get:1))) -> trim;
	$inlineStuff -> (Insert:$this -> (Get:1) = (Var:($this -> (Get:1))));
    /Iterate;

Variable:'case'= (Action_Param:'case'); // either p or page (= static page)
				 	// value will be passed to front page


// $testNr usually refers to option ID

// set notifyAdmin to 'yes' for individual tests to get email about discarded comments

var:	'result'='', 
	'testNr'='', 
	'errText'='', 
	'adminErr'='',
	'notifyAdmin' = '';

// check the spamcontrol field

var:'spamcontrol' = (action_param:'spamcontrol');
if: !($spamcontrol);
	$testNr = '01';
	$result = 'discard';
	$errText = (str:'Illegal input for this page');
	$notifyAdmin = 'no';
/if;
If: !($result);
    var:'spamcontrol_fromform' = string(decrypt_blowfish2(decode_base64($spamcontrol), -seed=$hash));
    if:$spamcontrol_fromform -> size != 18;
	$testNr = '02';
	$result = 'discard';
	$errText = (str:'Illegal input for this page');
	$notifyAdmin = 'yes';
    /if;
/if;
If: !($result);
    var:'date_fromform' = $spamcontrol_fromform -> (substring:1,14);
    var:'randomnumber_fromform' = $spamcontrol_fromform -> (substring:15,18);
    if: date($date_fromform) -> type != 'date';
	$testNr = '03';
	$result = 'discard';
	$errText = (str:'Illegal input for this page');
	$notifyAdmin = 'yes';
    /if;
/if;
If: !($result);
    var:'datediff' = date($date_fromform) -> (difference(date));
    If: $datediff > (duration:'0:10:00');
	$testNr = '04';
	$result = 'discard';
	$errText = (str:'The comment is discarded - too much time has elapsed since the commenting form was loaded');
	$notifyAdmin = 'yes';
    /If;
/if;


// field validation(s) 

If: !($result);
    if: $com_author_email;
	If: (valid_email:$com_author_email);
	    // ok
	Else;
	    $testNr = '1';
	    $result = 'discard';
	    $errText = (str:'Please check the email address');
	/If;
    /If;
/If;


//  == Unconditional  ==		32  default_comment_status

If: (LB_getOption:'default_comment_status') == 0;
    $testNr = '32';
    $result = 'discard';
    $errText = (str:'Commenting is turned off for this blog');
Else: (LB_getOption:'default_comment_status') != 1;
    $adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 32';
/If;

//   == AuthorSpecific == 	 6  comment_registration (in General Options)

If: !($result);
    If: (LB_getOption:'comment_registration') == 1;
	If: !($userinfo -> LB_getUser_id); // registered?; 
	    $result = 'discard';
	    $testNr = '6';
	    $errText = (str:'You have to be registered and logged in to comment');
	/If;
    Else: (LB_getOption:'comment_registration') != 0;
	    $adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + $testNr;
    /If;
/If;

//  == Keys ==			40  blacklist_keys

If: !($result);
    Variable:'keys' = (LB_getOption:'blacklist_keys');
    If: $keys != '' && $keys != ';';
	 Variable:'aKeys' = $keys -> (Split:';');
         Variable:'i' = '', 'lc' = 0;
         Iterate: $aKeys, $i;
	    $lc += 1;
	    $i -> Trim;
	    If: $i && (
		    $com_author >> $i
		 || $com_author_email >> $i
		 || $com_author_url >> $i
		 || $com_title >> $i
		 || $com_content >> $i
		);
			$result = 'discard';
			$testNr = '40';
			$errText = (str:'Blacklisted strings were found');
	    /If;
         /Iterate; 
    /If;
/If;

// check prev accepted comments for tests 38 and 37

    Variable:'prevAcceptedComments' = ''; // will be used in next check
    If:($userinfo -> LB_getUser_id); //  if registered 
       Inline:-search,$dbConfig,-Table=$table_prefix+'comments',
	    -KeyField='id',
	    -MaxRecords=1,
	    'com_user_id' = ($userinfo -> LB_getUser_id),
	    'com_approved' = 1;
	 If: (Found_Count);
	    $prevAcceptedComments = 'yes'; // will be used in next check
	 /If;
      /Inline;
    /If;

//  == Links ==			38  comment_link_limit 

// 	0 10 mod, 11 mod unless reg, 12 mod unless reg and prev com
// 	  20 dis, 21 dis unless reg, 22 dis unless reg and prev com

If: !($result);
    Var:'optvalue' = (LB_getOption:'comment_link_limit');
    Var:'testvalue1' = 'http';
    Var:'testvalue2' = 'href';
    If: $optvalue != '0';
	If:     $com_author       >> $testvalue1
	     || $com_author_email >> $testvalue1
	     || $com_title        >> $testvalue1
	     || $com_content      >> $testvalue1
	     || $com_author       >> $testvalue2
	     || $com_author_email >> $testvalue2
	     || $com_title        >> $testvalue2
	     || $com_content      >> $testvalue2;
	// yes, it contains links
	    If:   $optvalue == '10';
		$result = 'moderate';
		$testNr = '38';
		$errText = (str:'There are limitations to using links in comments');
	    Else: $optvalue == '11';
	        If: !($userinfo -> LB_getUser_id); // if not registered
		    $result = 'moderate';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/If;
	    Else: $optvalue == '12';
	        If: !($prevAcceptedComments == 'yes'); // if reg & prev comm.
		    $result = 'moderate';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/If;
	    Else: $optvalue == '20';
		$result = 'discard';
		$testNr = '38';
		$errText = 'There are limitations to using links in comments';
	    Else: $optvalue == '21';
	        If: !($userinfo -> LB_getUser_id); // if not registered
		    $result = 'discard';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/If;
	    Else: $optvalue == '22';
	        If: !($prevAcceptedComments == 'yes'); // if reg & prev comm.
		    $result = 'discard';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/If;
	    Else;
		$adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 38';
	    /If;
	/If;
    /If;
/If;

// 	'test:'; $result; ',test='; $testNr; 
// 	      ',optvalue='; $optvalue; 
// 	      ',user='; ($userinfo -> LB_getUser_id); '<br>';

//  == Unconditional  ==			35  comment_moderation

If: !($result);
    If: (LB_getOption:'comment_moderation') == 1;
	    $result = 'moderate';
	    $testNr = '35';
	    $errText = '';
    Else: (LB_getOption:'comment_moderation') != 0;
	    $adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 35';
    /If;
/If;

//  == Keys ==			39  moderation_keys

If: !($result);
    Variable:'keys' = (LB_getOption:'moderation_keys');
    If: $keys != '' && $keys != ';';
	 Variable:'aKeys' = $keys -> (Split:';');
         Variable:'i' = '', 'lc' = 0;
         Iterate: $aKeys, $i;
	    $lc += 1;
	    $i -> Trim;
		If: $i && (
		    $com_author >> $i
		 || $com_author_email >> $i
		 || $com_author_url >> $i
		 || $com_title >> $i
		 || $com_content >> $i
		);
			$result = 'moderate';
			$testNr = '39';
			$errText = (str:'Blacklisted strings were found');
		/If;
         /Iterate; 
    /If;
/If;

//  == AuthorSpecific ==	37  commentor_whitelist 

If: !($result);
    Variable:'optvalue' =  (LB_getOption:'commentor_whitelist');
    If: $optvalue == '2';
	If: !($prevAcceptedComments == 'yes'); // if not reg & prev comm.
	    $result = 'moderate';
	    $testNr = '37';
	    $errText = '';
	/If;
    Else: $optvalue == '1';
	If:!($userinfo -> LB_getUser_id); //  if not registered 
	    $result = 'moderate';
	    $testNr = '37';
	    $errText = '';
	/If;
    Else: $optvalue != '0';
	$adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 37 (' 
		+ 'the value was' + ' "' + $optvalue + '")';
    /If;
/If;

// are comments allowed for this post? 

If: !($result);
    Inline: $dbConfig, -table=$table_prefix+'posts',
	-KeyField='id',
	-Op='eq',
	'id'=$com_post_id,
	-search;

	Variable:'pos_comment_status' = (Field:'pos_comment_status');
    /Inline;

    If: $pos_comment_status == 'open';
        // ok
    Else;
	$testNr = '999';
	$result = 'discard';
	$errText = (str:'Sorry, commenting is not allowed for this item');
    /If;
/If;

// END CHECKING

//  == Notifications ==		33  comments_notify
//  == Notifications == 	34  moderation_notify

Variable:'email_recipient' = '';

If: $result != 'discard';
    If: (LB_getOption:'comments_notify') == 1;
        $errText += '<br>' + (str:'The writer of the post will be notified of your comment.');
	$email_recipient = 'writer';
    Else: (LB_getOption:'comments_notify') == 2;
        $errText += '<br>' + (str:'The administrator will be notified of your comment.');
	$email_recipient = 'admin';
    Else: (LB_getOption:'comments_notify') != 0;
	$adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 33';
    Else: $result == 'moderate';
      If: (LB_getOption:'moderation_notify') == 1;
        $errText += '<br>' + (str:'The writer of the post will be notified of your comment.');
	$email_recipient = 'writer';
      Else: (LB_getOption:'moderation_notify') == 2;
        $errText += '<br>' + (str:'The administrator will be notified of your comment.');
	$email_recipient = 'admin';
      Else: (LB_getOption:'moderation_notify') != 0;
	$adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 34';
      /If;
   /If;
/If;


// display verdict 

If: $result == 'discard';
    // assemble the text
    var:'text' = (str:'Sorry, your comment was <b>NOT</b> saved.',-encodenone); 
    $text +='<br>'+(str:'It didn\'t pass test number')+' '+$testNr+'.';
    $text +='<br>'+(str:'The tests are used to control commenting and to prevent spam comments.');
    If: $errText != '';
       $text += '<br>' + (str:'More info') + ': <b>' + $errText + '</b>.';
    /If;
    $text += '<br>'+(str:'You can try again or contact the administrator at'); 
    $text += ' ' + ($basicOptions -> 'admin_email'); 
    // display
	'<table><tr><td id="widecolumn">';
	    '<p>';
	    $text;
	    '</p>\r'; 
	'</td></tr></table>\r'; 
    // notify admin, if needed
    if: $notifyAdmin == 'yes';
	var:'temp' = $text;
	$temp -> (replace:'<br>','\n');
	var:'email_body' = 'This is an automatic message from your blog. Your comment processing system has been set to notify the blog administrator of a comment evaluation. Below is the error message presented to the commentor.\n\n' + $temp;
	$email_body += '\n\nTo turn off these notification emails, open cont/com-posted.inc and set the variable notifyAdmin to \'no\' for individual tests.';
	Email_Send: -From=($basicOptions -> 'admin_email'),
		    -To=($basicOptions -> 'admin_email'),
		    -Subject=($basicOptions -> 'blogname') + ': ' 
			+ 'a comment has been processed',
		    -Body=$email_body;
    /if;
    Abort;

Else: $result == 'moderate'; // cannot be shown on this page
// will be forwarded, see bottom of page
Else; // same goes for the regular thank-you text...
/If;


// add more data to $inlinestuff (to be written to db)

If: ($userinfo -> LB_getUser_id) != '';
    $inlinestuff -> (insert:'com_user_id' = ($userinfo -> LB_getUser_id));
/If;
If: $result == 'moderate';
    $inlinestuff -> (insert:'com_approved' = '0');
    // 1 is the db default
/If;

$inlinestuff -> (Insert: 'com_date' = $nowMySQL);
$inlinestuff -> (Insert: 'com_author_ip' = (Client_IP));

// #1. check required fields

    Iterate: $editFields, $this;
        If: ($this -> (Get:4)) == 'req';
            If: (Var:($this -> (Get:1))) == '';
                $speakBubble += (str:'Please give') + ' ' + 
			(str:($this->(Get:2))) + '!<br>';
            /If;
        /If;
    /Iterate;

    //  			36 require_name_email 

    If: (LB_getOption:'require_name_email') == '1';
	If: $com_author_email == '';
	$speakBubble += (str:'Please give') + ' ' + (str:'email address') + '!<br>';
	/If;
	If: $com_author == '';
	$speakBubble += (str:'Please give') + ' ' + (str:'your name') + '!<br>';
	/If;
    Else: (LB_getOption:'require_name_email') != '0';
	$adminErr += 'Problem in reading option value. The value might be empty or illegal. Option id = ' + ' 36';
    /If;

        If: $speakBubble != '';
        Include: $myInc + 'speakBubble.inc';
        Abort;
        /If; 


// #2 add new record

Inline: $dbConfig, -table=$table_prefix+'comments',
$inlinestuff,
-KeyField='id',
-add;

Variable:'commentID' = '';
    If: (Error_CurrentError)!=(Error_NoError);
	$speakBubble += (str:'The add operation FAILED. More info:') + ' ';
    $speakBubble += (Error_CurrentError:-ErrorCode) + ', ' + (error_currenterror);
    Include: $myInc + 'errorhandler_debug.inc';

    Else; // case NoError
	$speakBubble += (str:'Thanks for your comment!');
    $commentID = (KeyField_Value);
    /If;

/Inline;

// moderation text to user
If: $result == 'moderate';
    $speakBubble += ' <br>' + (str:'It has to be moderated by administrator before it can be shown.'); 
    HTML_Comment;
	'\rTest number'; $testNr; 
	If: $errText != '';
	   '. More info: '; $errText;
	/If;
    /HTML_Comment;
/If;

// #3 email

Variable:'pos_author' = '', 'emailTo' = '';

If: $email_recipient == 'writer'; // find out email address
    Inline:-search,$dbConfig,-Table=$table_prefix+'posts',
        -KeyField='id',
	'id' = $com_post_id;
	If: (Found_Count) == 1;
	    $pos_author = (Field:'pos_author');
	/If;
    /Inline;
    If: $pos_author != '';
    Inline:-search,$dbConfig,-Table=$table_prefix+'users',
        -KeyField='id',
	'id' = $pos_author;
	If: (Found_Count) == 1;
	    If: (Field:'use_email') != '';
		$emailTo = (Field:'use_email');
	    /If;
	/If;
    /Inline;
    /If;
/If;
If: $emailTo == '';
    $emailTo = ($basicOptions -> 'admin_email');
/If;

// compose and send it

If:    ((LB_getOption:'comments_notify')   >= '1' && $result == '')  
    || ((LB_getOption:'moderation_notify') >= '1' && $result == 'moderate');
Variable:'emailsubject' = ($basicOptions -> 'blogname') + ': '
    + (str:'Comment added');
    If: $result == 'moderate'; 
	$emailsubject += ', ' + 'waiting for moderation'; 
    /If;
Variable:'emailbody' = (str:'A comment has been added to your blog at');
$emailbody += '\n\n' + ($basicOptions -> 'siteurl') + 
		'?' + $case + '=' + $com_post_id;
$emailbody += '\n\n' + (str:'You may edit or moderate the comment here:');
$emailbody += '\n\n' + ($basicOptions -> 'siteurl') + 'admin/man-commedit.lasso';
$emailbody += '?id=' + $commentID;
Email_Send: -From=($basicOptions -> 'admin_email'),
	-To=$emailTo,
	-Subject=$emailsubject,
	-Body=$emailbody;

// 'test: to='; $emailTo; ', subject='; $emailsubject; ', body='; $emailbody; ', result='; $result; 


/If;

// notify administrator about admin errors

If: $adminErr != '';
Variable:'emailsubject' = ($basicOptions -> 'blogname') + ': '
    + 'Admin error detected';
Variable:'emailbody' = 'This is an automatic email by your blog solution.';
Variable:'emailbody' += '\n\nThere seems to be an error in your blog settings. The page /cont/com-posted.inc (comment response page) reported: ';
$emailbody += '\n\n' + $adminErr;
Email_Send: -From=($basicOptions -> 'admin_email'),
	-To=($basicOptions -> 'admin_email'),
	-Subject=$emailsubject,
	-Body=$emailbody;
/If;

// #4 check & write cookies

Variable:'hash' = (LB_createCookiehash);
If: !(Cookie:('comment_author_' + $hash));
    (Cookie_Set: ('comment_author_' + $hash) = $com_author,-Expires=432000,-Domain=$myDomain);
/If;
If: !(Cookie:('comment_author_email' + $hash));
    (Cookie_Set: ('comment_author_email_' + $hash) = $com_author_email,-Expires=432000,-Domain=$myDomain);
/If;
If: !(Cookie:('comment_author_url' + $hash));
    (Cookie_Set: ('comment_author_url_' + $hash) = $com_author_url,-Expires=432000,-Domain=$myDomain);
/If;

// redirect, and pass on the speakBubble value
// case is either p or page

Redirect_URL:$pathtome + '?' + $case + '=' + $com_post_id + '&speakBubble=' + (Var:'speakBubble',-encodeURL);

?>

