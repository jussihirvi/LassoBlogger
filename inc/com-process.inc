<?LassoScript
loop(1); // loop hack to enable mimicking abort

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

var('hash') = LB_createcookiehash;

// field-related settings

var('editfields' = array(
    array('com_post_id','message id','text','req'),
    array('com_author','author','text',''),
    array('com_author_email','email','text',''),
    array('com_author_url','url','text',''),
    array('com_title','title','text',''),
    array('com_content','comment text','text','req')
    ));

// action_params converted to var

    var('inlinestuff' = array, 'this' = array);

    iterate( $editfields, $this);
	var($this -> get(1)) = action_param($this -> get(1));
	var($this -> get(1)) -> trim;
	$inlinestuff -> insert($this -> get(1) = var($this -> get(1)));
    /iterate;

var('oldcase') = action_param('oldcase'); 
// either p (post) or page (= static page); will be used in emails

// $testNr usually refers to option ID

// set $notifyAdmin below to 'yes' for individual tests to get email about discarded comments

var(	'result'      = string, 
	'testNr'      = string, 
	'errText'     = string, 
	'adminErr'    = string,
	'notifyAdmin' = string);

// check the spamcontrol field

var('spamcontrol') = action_param('spamcontrol');
if( ! $spamcontrol );
	$testNr = '01';
	$result = 'discard';
	$errText = str('Illegal input for this page');
	$notifyAdmin = 'no';
/if;
if( ! $result );
    var('spamcontrol_fromform') = string(decrypt_blowfish(decode_base64($spamcontrol), -seed=$hash));
    if( $spamcontrol_fromform -> size != 18 );
	$testNr = '02';
	$result = 'discard';
	$errText = str('Illegal input for this page');
	$notifyAdmin = 'yes';
    /if;
/if;
if( ! $result );
    var('date_fromform' = date($spamcontrol_fromform -> substring(1,14)));
    var('randomnumber_fromform') = $spamcontrol_fromform -> substring(15,18);
    if( date($date_fromform) -> type != 'date' );
	$testNr = '03';
	$result = 'discard';
	$errText = str('Illegal input for this page');
	$notifyAdmin = 'yes';
    /if;
/if;
if( ! $result );
  // var('datediff') = date($date_fromform) -> difference(date);
  // if( $datediff > duration('0:10:00') );
  // comparing durations is broken in L9.1.4
      $date_fromform -> add(-minute=10);
      if($date_fromform < date);
      $testNr = '04';
      $result = 'discard';
      $errText = str('The comment is discarded - too much time has elapsed since the commenting form was loaded');
      $notifyAdmin = 'yes';
    /if;
/if;


// field validation(s) 

if( ! $result );
    if( $com_author_email );
	if( valid_email($com_author_email) );
	    // ok
	else;
	    $testNr = '1';
	    $result = 'discard';
	    $errText = str('Please check the email address');
	/if;
    /if;
/if;


//  == Unconditional  ==		32  default_comment_status

if( $opts -> find('default_comment_status') == '0' );
    $testNr = '32';
    $result = 'discard';
    $errText = str('Commenting is turned off for this blog');
else( $opts -> find('default_comment_status') != '1' );
    $adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id = 32');
/if;

//   == AuthorSpecific == 	 6  comment_registration (in General Options)

if( ! $result );
    $testNr = '6';
    if( $opts -> find('comment_registration') == '1' );
	if( !($userinfo -> LB_getUser_id) ); // registered?; 
	    $result = 'discard';
	    $errText = str('You have to be registered and logged in to comment');
	/if;
    /if;
/if;

//  == Keys ==			40  blacklist_keys

if( ! $result );
    var('keys') = $opts -> find('blacklist_keys');
    if( $keys && $keys != ';');
	 var('aKeys') = $keys -> split(';');
         var('i' = '', 'lc' = 0);
         iterate( $aKeys, $i);
	    $lc += 1;
	    $i -> trim;
	    if( $i && (
		    $com_author >> $i
		 || $com_author_email >> $i
		 || $com_author_url >> $i
		 || $com_title >> $i
		 || $com_content >> $i
		));
			$result = 'discard';
			$testNr = '40';
			$errText = str('Blacklisted strings were found');
	    /if;
         /iterate; 
    /if;
/if;

// check prev accepted comments for tests 38 and 37

    var('prevAcceptedComments') = string; // will be used in next check
    if( $userinfo -> LB_getUser_id ); //  if registered 
       inline(-search,$dbConfig,-Table=$table_prefix+'comments',
	    -KeyField='id',
	    -MaxRecords=1,
	    'com_user_id' = ($userinfo -> LB_getUser_id),
	    'com_approved' = 1);
	 if( found_count );
	    $prevacceptedcomments = 'yes'; // will be used in next check
	 /if;
      /inline;
    /if;

//  == Links ==			38  comment_link_limit 

// 	0 10 mod, 11 mod unless reg, 12 mod unless reg and prev com
// 	  20 dis, 21 dis unless reg, 22 dis unless reg and prev com

if( ! $result );
    var('optvalue') = $opts -> find('comment_link_limit');
    var('testvalue1') = 'http';
    var('testvalue2') = 'href';
    if( $optvalue );
	if(     $com_author       >> $testvalue1
	     || $com_author_email >> $testvalue1
	     || $com_title        >> $testvalue1
	     || $com_content      >> $testvalue1
	     || $com_author       >> $testvalue2
	     || $com_author_email >> $testvalue2
	     || $com_title        >> $testvalue2
	     || $com_content      >> $testvalue2);
	// yes, it contains links
	    if( $optvalue == '10' );
		$result = 'moderate';
		$testNr = '38';
		$errText = str('There are limitations to using links in comments');
	    else( $optvalue == '11' );
	        if( !($userinfo -> LB_getUser_id) ); // if not registered
		    $result = 'moderate';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/if;
	    else( $optvalue == '12' );
	        if( !($prevAcceptedComments == 'yes') ); // if reg & prev comm.
		    $result = 'moderate';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/if;
	    else( $optvalue == '20' );
		$result = 'discard';
		$testNr = '38';
		$errText = 'There are limitations to using links in comments';
	    else( $optvalue == '21' );
	        if( !($userinfo -> LB_getUser_id) ); // if not registered
		    $result = 'discard';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/if;
	    else( $optvalue == '22' );
	        if( !($prevAcceptedComments == 'yes') ); // if reg & prev comm.
		    $result = 'discard';
		    $testNr = '38';
		    $errText = 'There are limitations to using links in comments';
		/if;
	    else;
		$adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id = 38');
	    /if;
	/if;
    /if;
/if;

// 	'test:'; $result; ',test='; $testNr; 
// 	      ',optvalue='; $optvalue; 
// 	      ',user='; ($userinfo -> LB_getUser_id); '<br>';

//  == Unconditional  ==			35  comment_moderation

if( ! $result );
    if( $opts -> find('comment_moderation') == '1' );
	    $result = 'moderate';
	    $testnr = '35';
	    $errtext = '';
    else( $opts -> find('comment_moderation') != '0' );
	    $adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id = 35');
    /if;
/if;

//  == Keys ==			39  moderation_keys

if( ! $result );
    var('keys') = $opts -> find('moderation_keys');
    if( $keys != '' && $keys != ';');
	 var('akeys') = $keys -> split(';');
         var('i' = '', 'lc' = 0);
         iterate( $akeys, $i);
	    $lc += 1;
	    $i -> trim;
		If( $i && (
		    $com_author >> $i
		 || $com_author_email >> $i
		 || $com_author_url >> $i
		 || $com_title >> $i
		 || $com_content >> $i
		));
			$result = 'moderate';
			$testNr = '39';
			$errText = str('Blacklisted strings were found');
		/if;
         /iterate; 
    /if;
/if;

//  == AuthorSpecific ==	37  commentor_whitelist 

if( ! $result );
    var('optvalue') = $opts -> find('commentor_whitelist');
    if( $optvalue == '2' );
	if( $prevAcceptedComments != 'yes' ); // if not reg & prev comm.
	    $result = 'moderate';
	    $testNr = '37';
	    $errText = '';
	/if;
    else( $optvalue == '1' );
	if( !($userinfo -> lb_getuser_id) ); //  if not registered 
	    $result = 'moderate';
	    $testNr = '37';
	    $errText = '';
	/if;
    else( $optvalue );
	$adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id =  37 ('+'the value was' + ' "' + $optvalue + '")');
    /if;
/if;

// are comments allowed for this post? 

if( ! $result );
    inline( -search, $dbconfig, -table=$table_prefix+'posts',
          -KeyField='id',
          -eq,
          'id'=$com_post_id);
	var('pos_comment_status') = field('pos_comment_status');
    /inline;

    if( $pos_comment_status == 'open' );
        // ok
    else;
	$testnr = '999';
	$result = 'discard';
	$errText = str('Sorry, commenting is not allowed for this item');
        $errText += ', ' $pos_comment_status;
        $errText += ', status=' $pos_comment_status;
    /if;
/if;

// Required fields
// soft error messages, speakBubble used instead of errText
    iterate( $editfields, $this);
        if( ($this -> get(4)) == 'req' );
            if( ! var($this -> get(1)) );
                $speakBubble += (str('Please give') + ' ' + 
			str($this->get(2)) + '!<br>');
                $testNr = '1001';
                $result = 'discard';
            /if;
        /if;
    /iterate;
    //  			36 require_name_email 
    if( $opts -> find('require_name_email') == '1' );
	if( $com_author_email == '' );
          $speakBubble += str('Please give') + ' ' + str('email address') + '!<br>';
          $testNr = '1001';
          $result = 'discard';
	/if;
	if( ! $com_author );
          $speakBubble += str('Please give') + ' ' + str('your name') + '!<br>';
          $testNr = '1001';
          $result = 'discard';
	/if;
    Else( $opts -> find('require_name_email') );
	$adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id =  36');
    /If;
        if($speakbubble);
          MO_speakBubble($speakbubble,-style='color:#c03;');
          loop_continue; // mimics abort
        /if; 

// END CHECKING

// add more data to $inlinestuff (to be written to db)

if( $userinfo -> lb_getuser_id );
    $inlinestuff -> insert('com_user_id' = $userinfo -> LB_getUser_id);
/if;
if( $result == 'moderate' );
    $inlinestuff -> insert('com_approved' = '0');
    // 1 is the db default
/if;
$inlinestuff -> insert( 'com_date' = $nowMySQL);
$inlinestuff -> insert( 'com_author_ip' = string(client_IP));



// handle notifications
//  == Notifications ==		33  comments_notify
//  == Notifications == 	34  moderation_notify

var('email_recipient') = string;

if( $result != 'discard' );
    if( $opts -> find('comments_notify') == '1' );
        $errText += '<br>' + str('The writer of the post will be notified of your comment.');
	$email_recipient = 'writer';
    else( $opts -> find('comments_notify') == '2' );
        $errText += '<br>' + str('The administrator will be notified of your comment.');
	$email_recipient = 'admin';
    else( $opts -> find('comments_notify') != '0' );
	$adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id = 33');
    else( $result == 'moderate' );
      if( $opts -> find('moderation_notify') == '1' );
        $errText += '<br>' + str('The writer of the post will be notified of your comment.');
	$email_recipient = 'writer';
      else( $opts -> find('moderation_notify') == '2' );
        $errText += '<br>' + str('The administrator will be notified of your comment.');
	$email_recipient = 'admin';
      else( $opts -> find('moderation_notify') );
	$adminErr += ('Problem in reading option value. The value might be empty or illegal. Option id = 34');
      /if;
   /if;
/if;


// display verdict 

if( $result == 'discard' );
    // assemble the text
    $speakBubble = str('Sorry, your comment was <b>NOT</b> saved.'); 
    $speakbubble +='<br>'+str('It didn\'t pass test number')+' '+$testNr+'.';
    $speakbubble +='<br>'+str('The tests are used to control commenting and to prevent spam comments.');
    if( $errtext != '' );
       $speakbubble += '<br>' + str('More info') + ': <b>' + $errText + '</b>.';
    /if;
    $speakbubble += '<br>'+str('You can try again or contact the administrator at'); 
    $speakbubble += (' ' + $opts -> find('admin_email')); 

    // display
    MO_speakBubble($speakbubble,-style='color:#c03;');

    // notify admin, if needed
    if( $notifyAdmin == 'yes' );
	var('temp') = $text;
	$temp -> replace('<br>','\n');
	var('email_body') = 'This is an automatic message from your blog. Your comment processing system has been set to notify the blog administrator of a comment evaluation. Below is the error message presented to the commentor.\n\n' + $temp;
	$email_body += '\n\nTo turn off these notification emails, open cont/com-posted.inc and set the var notifyAdmin to \'no\' for individual tests.';
	email_send: -from=($opts -> find('admin_email')),
		    -to=($opts -> find('admin_email')),
		    -subject=($opts -> find('blogname')) + '( ' 
			+ 'a comment has been discarded',
		    -body=$email_body);
    /if;
    loop_continue; // mimics abort

else( $result == 'moderate' ); // see down
else;                          // see down
/if;


// from now on, result is 'moderate' or success

// #1 add new record

inline( $dbconfig, -table=$table_prefix+'comments',
  $inlinestuff,
  -KeyField='id',
  -add);

var('commentid') = string;
    if( error_currenterror != error_noerror );
	$speakBubble += str('The add operation FAILED. More info:') + ' ';
        $speakbubble += error_code + ', ' + error_currenterror;
        MO_speakBubble($speakBubble,-style='color:#c03;');
        MO_errorhandler(-level='debug');
    else; // case noerror
        $speakBubble = str('Thanks for your comment!');
        $commentID = keyfield_value;
    /if;
/inline;

// moderation text to user
if( $result == 'moderate' );
    $speakBubble += ' <br>' + str('It has to be moderated by administrator before it can be shown.'); 
/if;

MO_speakBubble($speakBubble);

// #2 email

var('pos_author' = string, 'emailto' = string);

if( $email_recipient == 'writer' ); // find out email address
    inline(-search,$dbconfig,-table=$table_prefix+'posts',
        -keyfield='id',
	'id' = $com_post_id);
	if( found_count == 1 );
	    $pos_author = Field('pos_author');
	/if;
    /inline;
    if( $pos_author );
    inline(-search,$dbconfig,-table=$table_prefix+'users',
        -keyfield='id',
	'id' = $pos_author);
	if( (found_count) == 1 );
	    if( field('use_email') );
		$emailto = field('use_email');
	    /if;
	/if;
    /inline;
    /if;
/if;
if( ! $emailto );
    $emailTo = $opts -> find('admin_email');
/if;

// compose and send it

if(    ($opts -> find('comments_notify')   >= '1' && $result == '')  
    || ($opts -> find('moderation_notify') >= '1' && $result == 'moderate'));
var('emailsubject' = $opts -> find('blogname') + ': '
    + str('Comment added'));
    if( $result == 'moderate' ); 
	$emailsubject += ', ' + 'waiting for moderation'; 
    /if;
var('emailbody') = str('a comment has been added to your blog at');
$emailbody += '\n\n' + ($opts -> find('siteurl')) + 
		'?' + $oldcase + '=' + $com_post_id;
$emailbody += '\n\n' + str('You may edit or moderate the comment here:');
$emailbody += '\n\n' + ($opts -> find('siteurl')) + 'admin/man-commedit.lasso';
$emailbody += '?id=' + $commentID;
email_send( -from=($opts -> find('admin_email')),
	-to=$emailto,
	-subject=$emailsubject,
	-body=$emailbody);

// 'test: to='; $emailTo; ', subject='; $emailsubject; ', body='; $emailbody; ', result='; $result; 


/if;

// notify administrator about admin errors

if( $adminerr );
var('emailsubject' = ($opts -> find('blogname')) + ': '
    + 'Admin error detected');
var('emailbody') = 'This is an automatic email by your blog solution.';
$emailbody += ('\n\nThere seems to be an error in your blog settings. The page /cont/com-posted.inc (comment response page) reported: ');
$emailbody += '\n\n' + $adminErr;
email_send( -from=($opts -> find('admin_email')),
	-to=($opts -> find('admin_email')),
	-subject=$emailsubject,
	-body=$emailbody);
/if;

// #3 check & write cookies

if( ! cookie('comment_author_' + $hash) );
    cookie_set( ('comment_author_' + $hash) = $com_author,-expires=432000,-domain=$mydomain);
/if;
if( ! cookie('comment_author_email' + $hash) );
    cookie_set( ('comment_author_email_' + $hash) = $com_author_email,-expires=432000,-domain=$mydomain);
/if;
if( ! cookie('comment_author_url' + $hash) );
    cookie_set( ('comment_author_url_' + $hash) = $com_author_url,-expires=432000,-domain=$mydomain);
/if;

/loop; // see first line
?>

