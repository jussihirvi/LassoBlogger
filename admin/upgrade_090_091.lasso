<?LassoScript
    var('fromMetoRoot') = '../';
    Include( '../config.lasso');
    Include( '../inc/library.inc');
    var('step') = Action_Param('step');
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
[If( $step == '')]
<h1>Welcome to LassoBlogger upgrade script</h1>

<p>This upgrader is for upgrading your database <b>from 0.9.0 to 0.9.1</b>. After upgrading you should move this file to a safe place to hide it from malicious users.</p>

<p><strong>New in 0.9.1</strong>: </p>
<ul>
<li>
Possibility to make a post-related image into a link that points to another file in the blog directory or anywhere on the net (for example the link could point to a high-resolution version of the same image)
</li> 
</ul>


<p>For the updater to work, your LassoBlogger user must have privilege to use SQL (in LassoAdmin, that's defined in the Groups > Hosts page).</p>

<h2><A HREF="[$myfilename]?step=1">Do it!</A></h2>

<?LassoScript
Else: $step == '1'; // the rest of file

var('temp_errorFlag') = '';

'<p>';

var('SQL_string') = '';

// activated options
// options related to "blog by email" feature
// and the smilies option

// $SQL_string += "
// UPDATE options SET opt_status='works' WHERE
// opt_name='mailserver_url' OR
// opt_name='mailserver_port' OR
// opt_name='mailserver_login' OR
// opt_name='mailserver_pass' OR
// opt_name='default_email_category' OR
// opt_name='use_smilies';
// ";


// new db field: link address to make an uploaded image into a link

$SQL_string += "
ALTER TABLE " + $table_prefix + "uploads
    ADD upl_linkurl VARCHAR(255) NOT NULL;
";
// new option for image display
$SQL_string += "
ALTER TABLE " + $table_prefix + "uploads
	CHANGE upl_imagedisplay upl_imagedisplay ENUM('right','left','block','link','none') NOT NULL;
";

// * * * show the SQL: (diagnostic code for developer) * * *  

// $SQL_string; Abort;

'Now running the inline to execute the SQL<br>';

// do it
	    Inline:$dbConfig,
		-SQL=$SQL_string;	
		    If( (Error_CurrentError) != (Error_NoError));
		    'Error in doing SQL: ';
		    Error_CurrentError(-ErrorCode) + ', ' + 
			    (Error_CurrentError) + '<br><br>\r\r';
			    $temp_errorFlag = 'err';
			    If( (Error_CurrentError) >> 'duplicate key name');
				'It seems that you are trying to create an 
				index that has already been created. Maybe you 
				have already run this updater?<br><br>\r\r';
				'Or else you might have a version of <b>
				LassoBlogger</b> that doesn\'t need
				upgrading.<br><br>\r\r';
			    Else( (Error_CurrentError) >> 'duplicate column name');
				'This script tried to create a field (= column) that already exists. You have probably already run this script. If you want to run it again, please comment out the appropriate lines in the code of this script (search for with the column name to find the exact spot). Remember to comment out the WHOLE variable definition surrounding the offending SQL statement(s).';
			    /If;
		/If;
	    /Inline;

    If( $temp_errorFlag != 'err');
	'<p>';
	'<b>No errors were detected in running SQL!</b>';
	'</p>';
	
	'<b>The following SQL code was executed:</b><br>';
	$SQL_string = Encode_html($SQL_string);
	$SQL_string -> Replace('\n','<br>');
	$SQL_string -> Replace('\r','<br>');
	$SQL_string;
	'</p>';
    /If;

$temp_errorFlag = ''; // reset

/If; // end if step == 1
?>


