<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml" xmlns:og="http://ogp.me/ns#">
<head>
<title>[$opts -> find('blogname')][If( $page_title != '')] - [$page_title][/If]</title>
<link rel="stylesheet" href="[$opts->find('siteurl')][$myStylesheet]" type="text/css" />
<style type="text/css">
<!--
/* 
Styles affecting the header DIV (the colored area at top of page)
Modify these to your liking
*/
    /* jussi hirvi specials */

    body {background-color: #FFF; }
    /* overrides the style sheet */

    div#jussiHeader { height:42px; padding:0;
	background-color: #7D9DC0;
	background-image: url(/jussihirvi/images/graniittikukka42px.jpg);
	border-bottom: 1px solid black;

    }

    div#jussiHeader h1 {padding: 10px 10px 10px 15px; color: black; 
    text-align:left; font-family: Verdana, sans-serif; font-size:2.2em;}

    div#menu {margin:14px auto 10px auto; text-align:center; }
    #menu p { font-size:0.8em;}

    #header {
        background-color: #fc9;
    }

#headerimg {
        margin: 0;
        padding: 3px 0 6px 0;
        width: 100%;
        }

    h1 {    
	text-align:center;
        font-size: 2.0em;
        padding-top: 4px;
        padding-left:15px;
        margin: 0;
        }

h1, h1 a, h1 a:hover, h1 a:visited, .description {
        text-decoration: none;
        color: black;
        }

    .description {
	text-align:center;
	font-size: 0.9em;
	font-weight:normal;
    }


-->
</style>
<!-- for Facebook integration -->
<meta property="og:title" content="[$opts->find('blogname')]" />
<meta property="og:url" content="[$opts->find('siteurl')]" />
<meta property="og:image" content="images/LassoBloggerLogo30x30.png" />
<meta property='og:type' content='article' /> 
<meta content='208685792608423' property='fb:app_id' />
<script src='http://connect.facebook.net/en_US/all.js#xfbml=1'></script> 
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.9.0/jquery.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){

    // comment form
    $("h3.respond a").click(function(event){
    event.preventDefault();
    var myid = $(this).attr('id');
    $("form#" + myid).toggle(200);
    if ($(this).hasClass('closed')) {
      $(this).removeClass("closed");
      $(this).addClass("open");
    }
    else if ($(this).hasClass('open')) {
      $(this).removeClass("open");
      $(this).addClass("closed");
    }
   });
    // comment list
    $("h3.comments a").click(function(event){
    event.preventDefault();
    var myid = $(this).attr('id');
    $("ol#" + myid).toggle(200);
    if ($(this).hasClass('closed')) {
      $(this).removeClass("closed");
      $(this).addClass("open");
    }
    else if ($(this).hasClass('open')) {
      $(this).removeClass("open");
      $(this).addClass("closed");
    }
   });

// for Facebook integration
    function commentToggle(selectTab) { 
      $(".comments-tab").addClass("inactive-select-tab"); 
      $(selectTab).removeClass("inactive-select-tab"); 
      $(".comments-page").hide(); 
      $(selectTab + "-page").show(); 
    } 
});
</script>
</head>
<body>
<?LassoScript
'<div id="page" class="round">\n';
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
</body>
</html>
