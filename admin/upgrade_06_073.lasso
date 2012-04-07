<?LassoScript
    Variable:'fromMetoRoot' = '../';
    Include: '../config.lasso';
    Include: '../inc/library.inc';
    Variable:'step' = (Action_Param:'step');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>LassoBlogger &rsaquo; Upgrader</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style type="text/css" media="screen">
	<!--
	html {
		background: #eee;
	}
	body {
		background: #fff;
		color: #000;
		font-family: Georgia, "Times New Roman", Times, serif;
		width:500px;
		margin-left: auto;
		margin-right: auto;
		padding: .2em 2em;
	}
	
	h1 {
		color: #006;
		font-size: 18px;
		font-weight: lighter;
	}
	
	h2 {
		font-size: 16px;
	}
	
	p, li, dt {
		line-height: 130%;
		padding-bottom: 2px;
	}

	ul, ol {
		padding: 5px 5px 5px 20px;
	}
	-->
	</style>
</head>
<body>
[If: $step == '']
<h1>Welcome to LassoBlogger upgrade script</h1>

<p>This upgrader is for upgrading your database <b>from 0.6 to 0.7.3</b>. After upgrading you should move this file to a safe place to hide it from malicious users.</p>

<p><strong>From 0.6 to 0.7</strong>: The option "permalink_structure" will be marked active, it's associated help text will be changed, and the field <code>pos_name</code> will be filled in all your existing posts (that field is used to create permalinks).</p> 

<p><strong>New in 0.7.2</strong>: The values of <code>pos_name</code> in posts will be rewritten due to a bug in the previous updater. A new field <code>cat_sortorder</code> will be created to the table <code>categories</code>. </p>

<p><strong>New in 0.7.3</strong>: About ten options will be edited and/or activated. They all connected to commenting control. <br>
<b>NOTE:</b> the option values for moderation_keys and blacklist_keys will be overwritten. This is normally no problem, as these options have been inactive until now.<br>
</p>


<p>For the updater to work, your LassoBlogger user must have privilege to use SQL (in LassoAdmin, that's defined in the Groups > Hosts page).</p>

<h2><A HREF="[$myfilename]?step=1">Do it!</A></h2>

<?LassoScript
Else: $step == '1'; // the rest of file

Variable:'temp_errorFlag' = '';

'<p>';

Variable:'SQL_string' = '';

$SQL_string +='
ALTER TABLE categories ADD cat_sortorder VARCHAR(255) NOT NULL DEFAULT \'descending\'; ';
 
// activated & changed options
// 0.7 permalink_structure: descr. changed, activated

$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_description=\'Use template tags to create a virtual site structure. <br>Currently permalinks are done with error.lasso method, and hence you should begin your entry with <code>.lasso</code> so that the file-not-found error is routed to Lasso. <br>Example (remember the dot): <code>.lasso/%year%/%monthnum%/%day%/%postname%</code> would result in <code>' + (LB_getOption:'siteurl') + '.lasso/2006/02/15/My_post</code>. Also available is the template tag <code>%postid%</code>\' WHERE opt_name=\'permalink_structure\'
;
';

// 0.7.3 comments_notify: new value 2, descr. 
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_value=\'2\', opt_description=\'Send email when a comment has been submitted and NOT held for moderation (0 = no email, 1 = to writer of the post, 2 = to administrator) \' WHERE opt_name=\'comments_notify\';
';
// 0.7.3 moderation_notify: activated, new value 2, descr. 
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_value=\'2\', opt_description=\'Send email when a comment is held for moderation (0 = no email, 1 = to writer of the post, 2 = to administrator)  \' WHERE opt_name=\'moderation_notify\';
';
// 0.7.3 comment_moderation: activated, descr. 
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_description=\'Comments always go to moderation queue (i.e. they must be approved by administrator) regardless of any matches below (0 or 1)\' WHERE opt_name=\'comment_moderation\';
';
// 0.7.3 comment_whitelist: activated, name, value  
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_value=\'0\', opt_name=\'commentor_whitelist\', opt_description=\'Comment goes to moderation queue unless (1) the author is registered, or (2) the author is registered AND has a previously approved comment, (0, 1 or 2)\' WHERE id=\'37\';
';
// 0.7.3 moderation_keys: activated, descr.,value
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_value=\';\', opt_description=\'Comment always goes to moderation queue, if it contains any of these strings in its name, email, website, title, or content. Separate multiple strings with a semicolon (;) and optional space. Example: <code>friggin; holy-liftin</code>  \' WHERE opt_name=\'moderation_keys\';
';
// 0.7.3 blacklist_keys: activated, descr.,value
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_value=\';\', opt_description=\'Comment is always discarded if it contains any of these strings in its name, email, website, title, or content. Separate multiple strings with a semicolon (;) and optional space. Example: <code>friggin; holy-liftin</code>. NOTE: Be careful: <code>ass</code> will trigger processing if the comment contains \"bass\" or \"assist\"\' WHERE opt_name=\'blacklist_keys\';
';
// 0.7.3 comment_max_links: name changed, activated, descr.
$SQL_string += '
UPDATE options SET opt_name=\'comment_link_limit\', opt_status=\'works\', opt_value=\'11\', opt_description=\'If the comment contains links, (0) this option is ignored, (10) comment goes to moderation queue, (11) comment goes to moderation queue, unless the author is registered (12) comment goes to moderation queue, unless the author is registered AND has a previously accepted comment (20) comment is discarded, (21) comment is discarded, unless the author is registered (22) comment is discarded, unless the author is registered AND has a previously accepted comment\' WHERE id=\'38\';
';
// 0.7.3 comment_registration: activ., descr.
$SQL_string += '
UPDATE options SET opt_status=\'works\', opt_description=\'Users must be registered and logged in to comment (0 or 1); see Discussion Options for softer ways to control commenting\' WHERE opt_name=\'comment_registration\';
';

// 0.7.3 require_name_email: activ. 
$SQL_string += '
UPDATE options SET opt_status=\'works\' WHERE opt_name=\'require_name_email\';
';


// // show: 
// $SQL_string; Abort;

'Now running the inline to execute the SQL<br>';
// do it
	    Inline:-Database=$myDb,
		-UserName=$dbUsername,
		-PassWord=$dbPassword,
		-SQL=$SQL_string;	
		    If: (Error_CurrentError) != (Error_NoError);
		    'Error in doing SQL: ';
		    (Error_CurrentError:-ErrorCode) + ', ' + 
			    (Error_CurrentError) + '<br><br>\r\r';
			    $temp_errorFlag = 'err';
			    If: (Error_CurrentError) >> 'duplicate key name';
				'It seems that you are trying to create an 
				index that has already been created. Maybe you 
				have already run this updater?<br><br>\r\r';
				'Or else you might have a version of <b>
				LassoBlogger</b> that doesn\'t need
				upgrading.<br><br>\r\r';
			    Else: (Error_CurrentError) >> 'duplicate column name \'cat_sortorder\'';
				'This script tries to create a field which already exists. <b>If you are upgrading from LB 0.7.2, you should comment out lines 76 and 77 from source (line 77 mentions cat_sortorder). Then please run this upgrader again!</b>';
			    /If;
		/If;
	    /Inline;

    If: $temp_errorFlag != 'err';
	'<b>No errors were detected in running SQL!</b><br>';
	'<b>The following SQL code was executed:</b><br>';
	Encode_html:$SQL_string;
    /If;

$temp_errorFlag = ''; // reset

'<br><br>Now updating your existing posts<br>';

     Inline:-findall,-Database=$myDb,-Table='posts',
        -UserName=$dbUsername,
        -PassWord=$dbPassword,
        -SortField='pos_date',
        -MaxRecords='all',
        -InlineName='postsearch';
    Records;
	Variable:'id' = (Field:'id');
	Variable:'title' = (Field:'pos_title');

    // process pos_title fto get pos_name
    // code must match that in admin/custom/validation/man-postedit.inc

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


     Inline:-update,-Database=$myDb,-Table='posts',
        -UserName=$dbUsername,
        -PassWord=$dbPassword,
	-KeyField='id',
	-KeyValue=$id,
	'pos_name' = $title;
		If: (Error_CurrentError) != (Error_NoError);
		    $temp_errorFlag = 'err';
		    'Error in updating posts: ';
		    (Error_CurrentError:-ErrorCode) + ', ' + 
			(Error_CurrentError) + '<br><br>\r\r';
			'Aborting.';
			Abort;
		/If;
	/Inline; // end update inline
    /Records;
/Inline;

    If: $temp_errorFlag == '';
	'<b>No errors were detected in updating posts!</b><br>';
    /If;
/If; // end if step == 1
?>


