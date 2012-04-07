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

<p>This upgrader is for upgrading your database <b>from 0.7.3 to 0.8</b>. After upgrading you should move this file to a safe place to hide it from malicious users.</p>

<p><strong>New in 0.8</strong>: </p>
<ul>
<li>
Two new db tables are created to facilitate file uploads: uploadcats, uploads (they will be used to keep a catalog of uploaded files)
</li> 
<li>
In the options table, two new fields: <code>opt_group</code> and <code>opt_sortcode</code>, to facilitate intuitive grouping of options on admin pages  
</li> 
<li>
Options related to file uploads (ID's 44...49) are activated
</li> 
<li>
New option "Multiuser Blog", which triggers author name display (in displaying posts) and author list display (in the sidebar)
</li> 
<li>
Options (as they are shown on admin pages) are regrouped
</li> 
</ul>


<p>For the updater to work, your LassoBlogger user must have privilege to use SQL (in LassoAdmin, that's defined in the Groups > Hosts page).</p>

<h2><A HREF="[$myfilename]?step=1">Do it!</A></h2>

<?LassoScript
Else: $step == '1'; // the rest of file

Variable:'temp_errorFlag' = '';

'<p>';

Variable:'SQL_string' = '';

// two new tables - for facilitating file uploads

$SQL_string +="
CREATE TABLE IF NOT EXISTS " + $myDb + ".uploadcats (
  ID bigint(20) NOT NULL auto_increment,
  upc_name varchar(55) NOT NULL default '',
  upc_nicename varchar(200) NOT NULL default '',
  upc_description longtext NOT NULL,
  upc_parent int(5) NOT NULL default '0',
  upc_sortorder VARCHAR(255) NOT NULL default 'descending', 
  PRIMARY KEY  (ID),
  INDEX upc_nicename (upc_nicename)
);
CREATE TABLE IF NOT EXISTS " + $myDb + ".uploads (
  ID bigint(20) unsigned NOT NULL auto_increment,
  upl_catid int(4) NOT NULL default '-1',
  upl_postid bigint(20) NOT NULL default '0',
  upl_name varchar(60) NOT NULL default '',
  upl_path varchar(255) NOT NULL default '',
  upl_description varchar(255) NOT NULL default '',
  upl_created datetime NOT NULL default '0000-00-00 00:00:00',
  upl_modified datetime NOT NULL default '0000-00-00 00:00:00',
  upl_userid int(4) NOT NULL,
  upl_imagedisplay enum('right','left','block','link') NOT NULL,
  PRIMARY KEY  (ID),
  INDEX upl_postid (upl_postid)
  );
";

// new upload category

$SQL_string +="
INSERT INTO uploadcats SET upc_name='File Vault General',upc_nicename='General',upc_description='My first category';
";

// new fields in table options: opt_group & opt_sortcode
// new option: multiuser_blog

$SQL_string +="

ALTER TABLE options ADD opt_group ENUM('gene','writ','read','disc','perm','file','misc') NOT NULL; 
ALTER TABLE options ADD opt_sortcode VARCHAR(255) NOT NULL; 

INSERT INTO options SET opt_name='multiuser_blog',opt_value='0',opt_description='If set to 1, author names are displayed for each post, and author list is displayed in sidebar (0 or 1)',opt_group='gene',opt_status='';

";
 
// activated & changed options
// options related to file upload

$SQL_string += "
UPDATE options SET opt_status='works' WHERE
opt_name='use_fileupload' OR
opt_name='fileupload_realpath' OR
opt_name='fileupload_url' OR
opt_name='fileupload_maxk' OR
opt_name='fileupload_allowedtypes' OR
opt_name='fileupload_minlevel';
";

$SQL_string += "
UPDATE options SET opt_description='File upload main switch (0,1)' WHERE opt_name='use_fileupload';
UPDATE options SET opt_description='Destination directory. May very well be outside the blog directory' WHERE opt_name='fileupload_realpath';
UPDATE options SET opt_description='Allowed file extensions. Remember to allow Lasso to process them (in Lasso Admin)' WHERE opt_name='fileupload_allowedtypes';
UPDATE options SET opt_description='Minimum user level for file uploads (0...10)' WHERE opt_name='fileupload_minlevel';
";

// set opt_group for each option (sigh)

$SQL_string += "
UPDATE options SET opt_group='gene' WHERE
opt_name='siteurl' OR
opt_name='blogname' OR
opt_name='admin_email' OR
opt_name='blogdescription' OR
opt_name='users_can_register' OR
opt_name='gmt_offset' OR
opt_name='date_format' OR
opt_name='time_format' OR
opt_name='start_of_week';
UPDATE options SET opt_group='writ' WHERE
opt_name='advanced_edit' OR
opt_name='default_post_edit_rows' OR
opt_name='use_smilies' OR
opt_name='use_balanceTags' OR
opt_name='default_category' OR
opt_name='new_users_can_blog' OR
opt_name='mailserver_url' OR
opt_name='mailserver_port' OR
opt_name='mailserver_login' OR
opt_name='mailserver_pass' OR
opt_name='default_email_category' OR
opt_name='ping_sites' OR
opt_name='blog_charset';
UPDATE options SET opt_group='read' WHERE
opt_name='posts_per_page' OR
opt_name='what_to_show' OR
opt_name='posts_per_rss' OR
opt_name='rss_use_excerpt' OR
opt_name='gzipcompression';
UPDATE options SET opt_group='disc' WHERE
opt_name='comment_registration' OR
opt_name='default_comment_status' OR
opt_name='comments_notify' OR
opt_name='moderation_notify' OR
opt_name='comment_moderation' OR
opt_name='require_name_email' OR
opt_name='commentor_whitelist' OR
opt_name='comment_link_limit' OR
opt_name='moderation_keys' OR
opt_name='blacklist_keys' OR
opt_name='open_proxy_check';
UPDATE options SET opt_group='perm' WHERE
opt_name='permalink_structure' OR
opt_name='category_base';
UPDATE options SET opt_group='file' WHERE
opt_name='use_fileupload' OR
opt_name='fileupload_realpath' OR
opt_name='fileupload_url' OR
opt_name='fileupload_maxk' OR
opt_name='fileupload_allowedtypes' OR
opt_name='fileupload_minlevel';
UPDATE options SET opt_group='misc' WHERE
opt_name='home' OR
opt_name='default_pingback_flag' OR
opt_name='default_ping_status' OR
opt_name='use_linksupdate' OR
opt_name='hack_file' OR
opt_name='rss_excerpt_length' OR
opt_name='links_updated_date_format' OR
opt_name='links_recently_updated_prepend' OR
opt_name='links_recently_updated_append' OR
opt_name='links_recently_updated_time' OR
opt_name='active_plugins' OR
opt_name='recently_edited' OR
opt_name='template' OR
opt_name='stylesheet' OR
opt_name='page_uris' OR
opt_name='rss_language' OR
opt_name='html_type' OR
opt_name='use_trackback';
";

// * * * show: (comment out when the file is ready) * * *  
// $SQL_string; Abort;

'Now running the inline to execute the SQL<br>';
// do it
	    Inline:$dbConfig,
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
			    Else: (Error_CurrentError) >> 'duplicate column name';
				'This script tried to create a field (= column) that already exists. You have probably already run this script. If you want to run it again, please comment out the appropriate lines in the code of this script (search for with the column name to find the exact spot). Remember to comment out the WHOLE variable definition surrounding the offending SQL statement(s).';
			    /If;
		/If;
	    /Inline;

    If: $temp_errorFlag != 'err';
	'<p>';
	'<b>No errors were detected in running SQL!</b>';
	'</p>';
	'<p>';
	'<b>Remember to give your blog user all permissions for the new tables (uploads and uploadcats) in Lasso Admin!</b>';
	'</p>';
	
	'<b>The following SQL code was executed:</b><br>';
	$SQL_string = (Encode_html:$SQL_string);
	$SQL_string -> (Replace:'\n','<br>');
	$SQL_string -> (Replace:'\r','<br>');
	$SQL_string;
	'</p>';
    /If;

$temp_errorFlag = ''; // reset

/If; // end if step == 1
?>


