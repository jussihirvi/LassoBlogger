<?LassoScript
    var('fromMetoRoot') = '../';
    include( '../config.lasso');
    include( '../inc/library.inc');
    var('step') = action_param('step');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>LassoBlogger &rsaquo; Upgrader</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="inc/upgradescript_style.css" type="text/css">
</head>
<body>
[if( ! $step )]
<h1>Welcome to LassoBlogger upgrade script</h1>

<p>This upgrader is for upgrading your database <b>from 0.9.2 to 0.9.3</b>. After upgrading you should move this file to a safe place to hide it from malicious users.</p>

<p><strong>New in 0.9.3</strong>: </p>
<ul>
<li>new option for selection of theme (it's about time)
<li>new option for Facebook app ID; this is needed for integration of your blog with Facebok
</ul>

<p>For the updater to work, your LassoBlogger user must have privilege to use SQL (in Lasso 8 SiteAdmin, that's defined in the Security > Hosts page).</p>

<h2><A HREF="[$myfilename]?step=1">Do it!</A></h2>

<?LassoScript
else($step == '1'); // the rest of file

var('temp_errorFlag') = string;

'<p>';

var('SQL_string') = '';

// new db field: link address to make an uploaded image into a link

$SQL_string += "
INSERT INTO options SET opt_name='theme',opt_value='default',opt_description='Path for stylesheet and images',opt_group='gene',opt_status='works';
INSERT INTO options SET opt_name='facebook_app_id',opt_value='',opt_description='Facebook application ID, needed for Facebook integration',opt_group='gene',opt_status='works';
INSERT INTO options SET opt_name='facebook_app_secret',opt_value='',opt_description='Facebook application secret (you get this from Facebook)',opt_group='gene',opt_status='works';
DELETE FROM options WHERE opt_name='home';
DELETE FROM options WHERE opt_name='stylesheet';
DELETE FROM options WHERE opt_name='template';
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


