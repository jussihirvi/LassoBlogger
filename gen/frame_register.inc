<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML><HEAD>
<META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<TITLE>[$opts -> find('blogname')][If( $page_title != '')] - [$page_title][/If]</TITLE>
<link rel="stylesheet" href="[$myStylesheet]" type="text/css">
<style type="text/css">
<!--

/* 
Styles affecting the header DIV (the colored area at top of page)
Modify these to your liking
*/

#header {
	padding: 0;
	margin: 0 auto;
	height: 70px;
	width: 100%;
	background-color: #73a0c5;
	}

#headerimg {
	margin: 0;
	height: 70px;
	width: 100%;
/* uncomment & adjust to include your custom header image */
/*	background-repeat: no-repeat;			  */
/*	background-image: url(themes/default/header.jpg); */
	}


h1, h1 a, h1 a:hover, h1 a:visited {
	padding-top: 4px;
	margin: 0;
	color:white;
	font-family: 'Trebuchet MS', 'Lucida Grande', Verdana, Sans-Serif;
	font-weight: bold;
	font-size: 1.8em;
	text-align: center;
	text-decoration: none;
	}

.description {
	text-align: center;
	color:white;
	font-size: 1.2em;
	text-align: center;
	}

-->
</style>

<script type="text/javascript" src="admin/inc/jquery-1.6.3.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){
  <?lassoscript
    if(variable_defined('editfields'));
        JH_ajaxvalidatejs($editfields);
    /if;
  ?>
});
</script>
</HEAD>
<BODY>
<div id="page">
<?LassoScript
if( $header );
    include( $header);
/if;
if( $use_level < 1 && ($showlogin == 'yes' || $loginsubmitted == 'yes') );
  include('inc/loginform.inc');
else;
  include( Var('contents'));
/if;

If( $footer );
    include( $footer);
/If;
?>

</div> <!-- end pagearea -->
</BODY>
</HTML>
