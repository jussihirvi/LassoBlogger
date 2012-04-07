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
		font-size: 16px;
		font-weight: lighter;
	}
	
	h2 {
		font-size: 14px;
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
<p>This upgrader is for upgrading from 0.5.0 to 0.6.</p>
<p>For this to work, your LassoBlogger user must have privilege to use SQL (in LassoAdmin, that's defined in the Groups > Hosts page).</p>
<h2><A HREF="[$myfilename]?step=1">Do it!</A></h2>

<?LassoScript
Else: $step == '1'; // the rest of file

Variable:'temp_errorFlag' = '';

'<p>';
'Making a couple of new indices (they didn\t exist in v0.5)<br>';
'Marking newly activated options (status field value changed to "works")<br>';


Variable:'SQL_string' = '';

$SQL_string += '
ALTER TABLE posts ADD FULLTEXT pos_title (pos_title);
ALTER TABLE posts ADD FULLTEXT pos_excerpt (pos_excerpt);
ALTER TABLE posts ADD FULLTEXT pos_content (pos_content);

UPDATE options SET opt_status=\'works\' WHERE 
';

// first the v0.5 options (here just to remind, actually they're not updated)

$SQL_string += '
opt_name=\'siteurl\' OR
opt_name=\'blogname\' OR
opt_name=\'admin_email\' OR
opt_name=\'blogdescription\' OR 
';

// v0.6 new options

$SQL_string += '
opt_name=\'users_can_register\' OR
opt_name=\'date_format\' OR
opt_name=\'time_format\' OR
opt_name=\'default_post_edit_rows\' OR
opt_name=\'default_category\' OR
opt_name=\'new_users_can_blog\' OR
opt_name=\'posts_per_page\' OR
opt_name=\'posts_per_rss\' OR
opt_name=\'rss_use_excerpt\' OR
opt_name=\'default_comment_status\' OR
opt_name=\'comments_notify\'
;
';

'Now running the inline to execute these changes<br>';
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
			    /If;
		/If;
	    /Inline;

    If: $temp_errorFlag != 'err';
	'There was no error<br>';
	'See the upgrader code, if you want to see what has been done.<br>';
    /If;

/If; // end if step == 1
?>


