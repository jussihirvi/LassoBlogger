<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<HTML><HEAD>
<META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<TITLE>[str($opts -> find('sitename_long'))] &ndash; [$page_title]</TITLE>
<link rel="stylesheet" href="[$myStylesheet]" type="text/css">
  <style type="text/css">
    [HTML_comment]
        /* relative url does not work from stylesheet */
        ul.menulist ul li {
          list-style-image:url(images/triangle_empty.png);
        }
        div#mainmenu ul.menulist ul li.selected {
          list-style-image:url(images/triangle.png);
        }
    [/HTML_comment]
  </style>
[if($ckeditor == 'enable')]
<script type="text/javascript" src="/ckeditor/ckeditor.js"></script>
[/if]

<script type="text/javascript" src="inc/jquery-1.6.3.min.js"></script>

[if($myfilename=='upload.html')]
        <link rel="stylesheet" href="/uploadify_lasso/uploadify-custom.css" type="text/css" media="screen">
	<script type="text/javascript" src="/uploadify_lasso/js/swfobject.js"></script>
	<script type="text/javascript" src="/uploadify_lasso/js/jquery.uploadify.js"></script>
[/if]


<script type="text/javascript">
// login form
$(document).ready(function(){

   // admin menu - show and hide submenus
   // code copied from enqa.eu/gadocs/includes/folding.js
    $(function() {
        // Find list items representing folders and turn them
        // into links that can expand/collapse the tree leaf.
        $('li.menucat').each(function(i) {
            // Temporarily decouple the child list, wrap the
            // remaining text in an anchor, then reattach it.
            var sub_ul = $(this).children().remove();
            $(this).wrapInner('<a/>').find('a').mouseover(function() {
                // Make the anchor toggle the leaf display.
                sub_ul.toggle();
            });
            $(this).append(sub_ul);
        });

        // Hide all lists except the outermost.
        $('ul ul.hidechildren').hide();
    });



   // mini loginform  can be toggled on by clicking on link
   $("a#showloginform").click(function(event){
   event.preventDefault();
   $("form#LoginFormMinimal").removeClass("display_none");
   $("form#LoginFormMinimal").addClass("display_block");
   $("div#langselect").addClass("display_none");
   });
    // next line, to make select onchange work in IE6 and IE7
    $('select').bind('onChange',function(){$(this).blur()});

   $("a#showRotateParams").click(function(event){
   event.preventDefault();   
   $("td#rotateParams").removeClass("display_none");
   $("td#rotateParams").addClass("display_block");
   });
   $("a#showScaleParams").click(function(event){
   event.preventDefault();   
   $("td#scaleParams").removeClass("display_none");
   $("td#scaleParams").addClass("display_block");
   });
// upload form: scale options
   $("input#upload_scale").change(function(event){
        if ($("input#upload_scale").is(":checked")){
           $("td#imagescale_options").removeClass("display_none");
        }
        else{
           $("td#imagescale_options").addClass("display_none");
           $(this).blur();
        }
   });
   $("input#upload_rotate").change(function(event){
        if ($("input#upload_rotate").is(":checked")){
           $("td#imagerotate_options").removeClass("display_none");
        }
        else{
           $("td#imagerotate_options").addClass("display_none");
           $(this).blur();
        }
   });




<?lassoscript
  if(variable_defined('editfields'));
      JH_ajaxvalidatejs($editfields);
  /if;
?>


   // next line, to make it work in IE6 and IE7
   $('select').bind('onChange',function(){$(this).blur()});

});

function popitup(url) {
  newwindow=window.open(url,'name','scrollbars=yes,width=450');
  if (window.focus) {newwindow.focus()}
    return false;
} 
        
</script>
<!--
<script type="text/javascript" src="inc/fsmenu.js"></script>
-->
</HEAD>

<?LassoScript
var('showlogin' = action_param('showlogin')); // yes = show  the login form
var('loginform_submitted'= Action_Param('loginform_submitted')); 
var('un_input'=string);
var('pw_input'=string);
var('hash') = LB_createCookiehash;

// check what user input we have 
If( $valid_ses != 1);
  include('inc/auth_define.inc');
/If; 			// end if valid_ses != 1

// check if the user is valid
If( $un_input && $pw_input); // now check the input
  include('inc/auth_check.inc');
/If;          /* end check username & password */

If( $valid_ses != 1 && $must_authenticate == 'yes');
  $showlogin = 'yes'; // show login form
/If;
If( $valid_ses != 1 && $loginform_submitted == 'yes');
  $showlogin = 'yes'; // show login form
/If;


'<BODY>\n';

'<div id="page">\r';
'<div id="header">\r';
Include( $header);
'</div>\r';

'<div ID="wrap">\n';

'<table><tr>\n';
'<td id="leftcol">';
  '<div id="mainmenu">\n';
      $navig -> get_mainmenu(-realmenu='yes');
  '</div>\n'; // end div mainmenu
'</td>\n'; // end leftcol

'<td id="middlecol">\n';
        // category-specific submenu
    	// $navig -> get_submenu(-catid=($navig->get_catid));

    if($showlogin == 'yes');
        include('inc/loginform.inc');

    // check userlevel
    else( $userlevel_ses < Integer($pageSecuritylevel) );
      /* userlevel is too low for access */
      $speakBubble=str('You do not have enough privileges to access this page.');
      $speakBubble+= (' (Your level is ' + $userlevel_ses
          + ', page requires ' + $pagesecuritylevel + '.)');
      MO_speakBubble($speakBubble); 
    else; // show content
      if($navig->get_heading);
        '<h2 class="Admin">' + $navig -> get_heading + '</h2>\n';
      /if;
      Include( $contents);
    /if;
      Include( $footer);
'</td>\n';
'</tr></table>\n';
'</div>\n'; // <!-- end wrap -->
'</div>\n'; // <!-- end page -->
'</BODY>'
'</HTML>';
?>
