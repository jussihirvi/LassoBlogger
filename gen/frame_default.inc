<?LassoScript
loop(1); // hack to mimic abort, which seems to be broken in Lasso 9
// preparation

var('searchArgs' = array,
          'skip' = integer(Action_Param('skip')),
          'case' = string,
     'linkstuff' = string,
        'postID' = string,
      'abort_it' = string,
         'title' = string,
      'og_title' = $opts->find('blogname'),        // default
'og_description' = $opts->find('blogdescription'), // default
      'og_image' = $opts->find('siteurl')+'images/LassoBloggerLogo30x30.png'
);

// capture action params - month, post, searchphrase, category, page

var('y'           = action_param('y'), 
    'm'           = action_param('m'), 
    'p'           = action_param('p'), 
    's'           = action_param('s'), 
    'cat'         = integer(action_param('cat')),
    'page'        = integer(action_param('page')),
    'files'       = action_param('files'),
    'filecat'     = integer(action_param('filecat')),
    'com_post_id' = action_param('com_post_id'),
    'perma'       = action_param('perma'),
    'speakBubble' = string);

// check if database structure is up-to-date

if($lb_version == '0.9.2');
  inline(-search,$dbConfig,-table=$table_prefix+'uploads','id'='xx');
  if(! (field_names -> find('upl_posttable') -> size) );
    MO_speakBubble(str('You are running 0.9.2 code, but your database structure does not seem to support it. Please run the relevant upgrade script(s) in your admin folder.'));
    $abort_it = 'yes';
  /if;
  /inline;
/if;
$abort_it ? loop_continue; // loop_continue mimics abort, necessary in l9

// check the action params, define case

if( $com_post_id ); 		// comment submitted
    include( $myinc + 'com-process.inc');
    if(action_param('oldcase') == 'p');
      $p = $com_post_id;
    else(action_param('oldcase') == 'perma');
      $p = $com_post_id;
    else(action_param('oldcase') == 'page');
      $page = $com_post_id;
    /if;
/if;

if( $y );		// year
    $linkstuff = 'y=' + $y;
    if( $y -> Size != 4 || integer($y) < 1900 || integer($y) > 3000);
	$speakBubble += 'Cannot interpret the specified year ' + $y + '.';
    else;
        $case = 'y';
        var('year')      = integer($y);
	var('startDate') = $y+'-01-01 00:00:00';
	var('endDate')   = $y+'-12-31 23:59:59';
	$searchArgs = array(-gte, 'pos_date' = $startDate,
                            -lte, 'pos_date' = $endDate,
                            'pos_status' = 'publish'
        );
        $title = $year;
    /if;
else( $m );		// month
    $linkstuff = 'm=' + $m;
    if( $m -> Size != 6 || integer($m) < 100000 || integer($m) > 300000);
	$speakBubble += 'Cannot interpret the specified month ' + $m + '.';
    else;
        $case = 'm';
	var('year'       = integer($m -> substring(1,4)));
	var('month'      = integer($m -> substring(5,2)));
        var('year_s')    = string($year);
        var('month_s')   = string($month);
	var('startDate') = $year_s+'-'+$month_s+'-01 00:00:00';
	if( $month == 12);
          var('enddate') = string($year + 1) + '-01-01 00:00:00';
	else;
          var('endDate') = 
            $year_s + '-' + string($month + 1) + '-01 00:00:00';
	/if;
	$searchArgs = array(-gte, 'pos_date' = $startDate,
                            -lt,  'pos_date' = $endDate,
                            'pos_status' = 'publish',
        );
        $title = $aMonths -> get($month) + ' ' + $year;
    /if;
else( $p ); 		// post (and comments)
    $case = 'p';
    $linkstuff = 'p=' + $p;
    $searchArgs = Array('id' = $p, 'pos_status' = 'publish');
else( $page ); 		// static page (and comments)
    $case = 'page';
    $searchArgs = array('id' = string($page), 'pos_status' = 'static');
else( $s ); 		// search form processing
    $case = 's';
    $linkstuff = 's=' + $s;
	$s = (String_ReplaceRegExp( $s, -Find='\\s+', -replace=' '));
	$s -> (Trim);
	$searchArgs -> (Insert( '-OperatorBegin' = 'or'));

//	$searchArgs -> (Insert( '-Op' = 'ft'));
// 	$searchArgs -> (Insert( 'pos_combined' = $s));
// 	$searchArgs -> (Insert( 'pos_title' = $s));
// 	$searchArgs -> (Insert( '-Op' = 'ft'));
// 	$searchArgs -> (Insert( 'pos_excerpt' = $s));
// 	$searchArgs -> (Insert( '-Op' = 'ft'));
// 	$searchArgs -> (Insert( 'pos_content' = $s));

	$searchArgs -> (Insert( '-Op' = 'cn'));
 	$searchArgs -> (Insert( 'pos_title' = $s));
 	$searchArgs -> (Insert( '-Op' = 'cn'));
 	$searchArgs -> (Insert( 'pos_excerpt' = $s));
 	$searchArgs -> (Insert( '-Op' = 'cn'));
 	$searchArgs -> (Insert( 'pos_content' = $s));

	$searchArgs -> (Insert( '-OperatorEnd' = 'or'));
	$searchArgs -> (Insert( 'pos_status' = 'publish'));

else( $cat );		// category
    $case = 'cat';
    $linkstuff = 'cat=' + $cat;
    $searchArgs = Array('pos_status' = 'publish', 
    			'-OpBegin' = 'or',
			'pos_category' = $cat);

    // subcats should be included, too( search for them

	    if( $catz->childparents -> find($cat) -> size);
		var('i' =string, 
		    'lc'=integer);
		iterate($catz->children, $i);
		    $lc += 1;
		    if( $catz->childparents -> get($lc) == $cat);
		    $searchargs -> Insert( 
			'pos_category'=
			$catz->children -> get($lc) -> get(1)
		    );
		    /if;
		/iterate;
	    /if;

		    $searchargs -> (Insert( '-OpEnd' = 'or'));

	    // get cat name (to display title)
	    // get cat sorting order (from v0.7.2 up)

	    // maincat or subcat?
	    var('cat_name' = string, 
		'cat_description' = string, 
		'ix' = integer);
	    if( $catz->parentids -> findIndex($cat)->size );
		$ix = $catz->parentids -> findIndex($cat) -> get(1);
		$cat_name = $catz->parents -> get($ix) -> get(3);
		$cat_description = $catz->parents -> get($ix) -> get(4);
		$searchArgs -> 
		  Insert('-SortOrder'= $catz->parents -> get($ix) -> get(5));
                $title = ($cat_name + ' <span class="subtitle">'
                        + $cat_description + '</span>'); 
	    else( $catz->childids -> findIndex($cat) -> size);
		$ix = $catz->childids -> findIndex($cat) -> get(1);
		$cat_name = $catz->children -> get($ix) -> get(3);
		$cat_description = $catz->children -> get($ix) -> get(4);
		$searchArgs -> 
		  Insert('-SortOrder'= $catz->children->get($ix)->get(5));
                $title = ($cat_name + ' <span class="subtitle">'
                        + $cat_description + '</span>'); 
	    else;
                $searchArgs = array;
		$speakBubble = str('Sorry, no such category!');
                MO_speakBubble($speakBubble);
	    /if;
		
else( $files ); 		// file vault
    $case = 'files';
    $title = str('File Vault');
else( $filecat ); 		// file vault, only one cat
    $case = 'filecat';
    $title = str('File Vault');
else( $perma ); 
    $case = 'perma';
    var('permaStruct') 	= string($opts -> find('permalink_structure'));
    $permaStruct -> removeLeading('/');

    // if option is not defined
    if( ! $permaStruct );
      MO_speakBubble('The permalink structure has not been defined in blog options. You may have an old link. Go to the <a href="' + ($opts -> find('siteurl')) + '">front page</a> and try to search for the post by its title.');
        loop_continue; // mimics abort
    /if;

    var('i'= string, 'lc' = integer, 'structItem' = string );
    var('year' = integer, 'monthnum' = integer, 'day' = integer, 'postname' = string, 'postid' = integer);

    iterate( $perma->split('/'), $i);
      $lc += 1;
      if($permastruct->split('/')->size >= $lc);
        $structItem = $permaStruct -> split('/') -> get($lc);
        if( $structItem->beginswith('%') && $structItem->endswith('%')); 
                //otherwise do nothing
                // available /%year%/%monthnum%/%day%/%postname%/%postid%  
            if( $structItem == '%year%');       $year = integer($i);
            else( $structItem == '%monthnum%'); $monthnum = integer($i);
            else( $structItem == '%day%');      $day = integer($i);
            else( $structItem == '%postname%'); $postname = $i;
            else( $structItem == '%postid%');   $postid = integer($i);
            /if;
        /if;
      else; // input does not match permastructure
      MO_speakBubble('Page is not found');
      loop_continue;
      /if;
    /iterate;

    // assemble $searchArgs
    if( $postname );
        $searchArgs -> insert( 'pos_name' = $postname);
    /if;
    if( $postid );
        $searchArgs -> insert( 'id' = $postid);
    /if;
    if( 1900 < $year && $year < 3000);	// we have a date
        // date->set seems to be broken in L9, using workaround
        var('startDate') = date;
        var('endDate') = date;
        if( ! $monthnum ); 	// only year is known
            $startDate = date($year    + '-01-01 00:00:00');
            $endDate = date($startDate);
            $endDate -> add(-year=1);
        else( ! $day );		// year and month are known
                    // capture crazy input
                    if(1 > $monthnum || $monthnum > 12 );
                      MO_speakBubble('The month number is out of range');
                      loop_continue; // mimics abort
                    /if;
            $startDate = date($year+'-' + $monthnum + '-01 00:00:00');
            $endDate = date($startDate);
            $endDate -> add(-month=1);
            $startDate = $startDate -> format('%Q %T');
            $endDate = $endDate -> format('%Q %T');
        else;			// year, monthnum, day are known
                    // capture crazy input
                    if(1 > $monthnum || $monthnum > 12 );
                      MO_speakBubble('The month number is out of range');
                      loop_continue; // mimics abort
                    /if;
                    if(1 > $day || $day > 31 );
                      MO_speakBubble('The day number is out of range');
                      loop_continue; // mimics abort
                    /if;
            $startDate = date($year + '-' + $monthnum + '-' + $day+' 00:00:00');
            $endDate = date($startDate);
            $endDate -> add(-day=1);
            $startDate = $startDate -> format('%Q %T');
            $endDate = $endDate -> format('%Q %T');
        /if;
        $searchArgs -> insert( '-gte');
        $searchArgs -> insert( 'pos_date' = $startDate);
        $searchArgs -> insert( '-lt');
        $searchArgs -> insert( 'pos_date' = $endDate);
        $searchArgs -> insert( '-OpBegin' = 'or');
        $searchArgs -> insert( 'pos_status' = 'publish');
        $searchArgs -> insert( 'pos_status' = 'static');
        $searchArgs -> insert( '-OpEnd' = 'or');
    /if;	// end date
    // searchArgs is ready

else; 		// find the latest
    $case = string;
    $searchArgs = array('pos_status' = 'publish');
/if;

MO_speakBubble($speakBubble);

if( $case != 'files' && $case != 'filecat' );
  // content will be processed in module_posts.inc
  // inline is here bc post title is needed for the attribute og:title
  inline(-search,$dbConfig,-table=$table_prefix+'posts',
            -sortField='pos_date',
            -sortOrder='descending',
            -maxRecords=$opts->find('posts_per_page'),
            $searchArgs,
            -skipRecords=$skip,
            -inlineName='postsearch');
    if( ! found_count );
        $speakBubble = (str('No records were found'));
    else( found_count > 1 && $case == 'perma');
        MO_speakBubble(str('Tried to interpret the address as a permalink, but failed. Please check the address!'));
        'searchArgs = '; $searchArgs;
        abort;
    else( error_currenterror != error_noerror );
        MO_speakBubble(Error_CurrentError);
    /if;
    if(found_count == 1);
      $og_title       = field('pos_title');
      $og_description = field('pos_content') -> split('\r') -> get(1);
      $og_image = ($opts->find('siteurl')
        + (MO_collectUploadData(-table='posts',-postid=integer(field('id')))
          -> get(3) -> get(1)));

    /if;
  /inline;
/if;

?>

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
<meta property="og:title" content="[$og_title]" />
<meta property="og:description" content="[$og_description]" />
<meta property="og:url" content="[client_url]" />
<meta property="og:image" content="[$og_image]" />
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
[/loop]
