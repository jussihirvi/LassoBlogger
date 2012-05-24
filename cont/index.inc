<?LassoScript
loop(1); // hack to mimic abort, which seems to be broken in Lasso 9
// #1 preparation

var( 'searchArgs' = array,
		'skip' = integer(Action_Param('skip')),
		'case' = string,
	   'linkstuff' = string,
	      'postID' = string,
            'abort_it' = string);

// capture action params - month, post, searchphrase, category, page

var('m'           = action_param('m'), 
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
      $p= $com_post_id;
    else(action_param('oldcase') == 'perma');
      $p = $com_post_id;
    else(action_param('oldcase') == 'page');
      $page = $com_post_id;
    /if;
/if;

if( $m );		// month
    $linkstuff = 'm=' + $m;
    if( $m -> Size != 6 || integer($m) < 100000 || integer($m) > 300000);
	$speakBubble += 'Cannot interpret the specified month ' + $m + '.';
    else;
        $case = 'm';
	var('year'      = integer($m -> substring(1,4)));
	var('month'     = integer($m -> substring(5,2)));
        var('year_s')    = string($year);
        var('month_s')   = string($month);
	var('startDate') = date($year_s+'-'+$month_s+'-01 00:00:00');
	if( $month == 12);
          var('enddate') = date(($year_s + 1) + '-01-01 00:00:00' );
	else;
          var('endDate' = 
            date($year_s + '-' + string($month + 1) + '-01 00:00:00' ));
	/if;
	$searchArgs = array('-Operator' = 'gte', 
		'pos_date' = (Date_Format($startDate,-Format='%Q %T')),
		'-Operator' = 'lt', 
		'pos_date' = (Date_Format($endDate,-Format='%Q %T')),
		'pos_status' = 'publish',
		);
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
	    else( $catz->childids -> findIndex($cat) -> size);
		$ix = $catz->childids -> findIndex($cat) -> get(1);
		$cat_name = $catz->children -> get($ix) -> get(3);
		$cat_description = $catz->children -> get($ix) -> get(4);
		$searchArgs -> 
		  Insert('-SortOrder'= $catz->children->get($ix)->get(5));
	    else;
                $searchArgs = array;
		$speakBubble = str('Sorry, no such category!');
                MO_speakBubble($speakBubble);
	    /if;
		
else( $files ); 		// file vault
    $case = 'files';
else( $filecat ); 		// file vault, only one cat
    $case = 'filecat';
else( $perma ); 
    $case = 'perma';
    var('permaStruct') 	= string($opts -> find('permalink_structure'));
    $permaStruct -> removeLeading('/');

    // if option is not defined
    if( ! $permaStruct );
      MO_speakBubble('The permalink structure has not been defined in blog options. You may have an old link. Go to the <A HREF="' + ($opts -> find('siteurl')) + '">front page</A> and try to search for the post by its title.');
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

// testground
// 	'test - startdate = ' + $startDate + '<br>';
// 	'test - enddate = '   + $endDate   + '<br>';
//	'test - searchArgs = ' + $searchArgs + '<br>';
//	'test - case = ' + $case + '<br>';



// #2 content begins

// #2.1 the left (wide) column

'<table cellspacing="0" cellpadding="0" border="0" style="font-size:100%;">';
'<tr>';
'<td valign="top" id="widecolumn">';
'<div id="title">';
If( $case == 'm'); // monthly
    '<h2>';
    $aMonths -> get($month); ' '; $year;
    '</h2>';
Else( $case == 'cat');
    '<h2>';
    $cat_name; ' <span class="subtitle">'; $cat_description; '</span>';
    '</h2>';
Else( $case == 'files' || $case == 'filecat');
    '<h2>';
    str('File Vault');
    '</h2>';
/If;
'</div>\r';
  if( $case == 'files' || $case == 'filecat');
      include( $myinc + 'filevault.inc');
  else;
      include( $myinc + 'module_posts.inc');
  /if;
'</td>\r'; //  end td widecolumn


// #2.2 sidebar
?>
	<td valign="top" id="sidebar">

<form method="get" id="searchform" action="[$myfilename]">
    <div>
    <input type="text" value="[$s]" name="s" id="s">
    <input type="submit" id="searchsubmit" value="[str('Search!')]">
    </div
</form>		



	<!-- Author information is disabled per default. Uncomment and fill in your details if you want to use it. -->
	<!-- single-author blog -->
<!--	<h2>Author</h2>
	<p>A little something about you, the author. Nothing lengthy, just an overview.</p>
	-->
	<!-- multi-author blog -->
<?LassoScript
// 	'<h2>'; (str('Authors')); '</h2>';
//     Inline(-search,$dbConfig,-Table=$table_prefix+'users',
// 	-Op='gte',
// 	'use_level' =2,
// 	-SortField='use_nickname',
// 	-SortOrder='ascending',
// 	-MaxRecords='all',
// 	-ReturnField='id',
// 	-ReturnField='use_nickname',
// 	-InlineName='getAuthorlist');
// 	'<ul>';
//         Records;
//             '<li>';
// 		// '<a href="?page='; Field('id'); '">';
// 		Field('use_nickname');
// 		// '</a>';
// 	    '</li>\r';
//         /Records;
// 	'</ul>';
// 	/Inline;

//     // begin Special Pages
// 
     Inline(-search,$dbConfig,-Table=$table_prefix+'posts',
 	'pos_status' = 'static',
 	-SortField='pos_menu_order',
 	-SortOrder='ascending',
 	-MaxRecords='all',
 	-ReturnField='id',
 	-ReturnField='pos_title',
 	-ReturnField='pos_date',
 	-ReturnField='pos_name',
 	-InlineName='getStaticPages');
     If( (Found_Count) > 0);
         '<h2>'; str('Special Pages'); '</h2>\r<ul>';
         Records;
 	    Var('permaStruct' = $opts -> find('permalink_structure'));
             '<li><a href="'; 
 	      $opts -> find('siteurl');
 		If( !($permaStruct -> (Find('%postname%')))); 
 			// if option not defined
 		    '?page='; Field('id'); 
  		    '">';
 		Else;	// use permalink structure
 		// available - /%year%/%monthnum%/%day%/%postname%/ /%postid%  
 		  Var('year' = (Date(Field('pos_date'))) -> Year);
 		  Var('month' = (Date(Field('pos_date'))) -> Month);
 		  Var('day' = (Date(Field('pos_date'))) -> Day);
 		  Var('postname' = Field('pos_name')); 
 		  Var('linkstuff' = ($opts -> find('permalink_structure')));
 		  $linkstuff ->  (RemoveLeading('/'));
 		  $linkstuff -> (Replace('%year%',$year));
 		  $linkstuff -> (Replace('%monthnum%',$month));
 		  $linkstuff -> (Replace('%day%',$day));
 		  $linkstuff -> (Replace('%postname%',$postname));
 		  $linkstuff -> (Replace('%postid%',$postID));
 		  encode_url($linkstuff); 
 		  '" TITLE="'; (str('Permanent link to')); ' &#34;'; 
 		  Field('pos_title');'&#34;">';
 		/If;
             Field('pos_title');
             '</a></li>\r';
         /Records;
         '</ul>';
     /If;
     /Inline;

'<h2>' + (str('Categories')) + '</h2>\r';

'<ul>';
// show cat links
// cat lists are compiled at top of this file

var('i' = array);
var('parent_catid' = integer);
Iterate( $catz->parents, $i); // parent cats (= all except children)
    '<li><a href="';
    $opts -> find('siteurl');
    '?cat=';
    $i -> Get(1);
    '" title="';
    (str('View all posts filed under'));
    ' '; 
    $i -> Get(3);
    '">'; $i -> Get(3); '</a>'; 
    ' ('; LB_getNrofPosts(($i -> Get(1))); ')';
    '</li>\r';
    // any child cats?
    $parent_catid = $i -> Get(1);
	If( $catz->childparents -> (Find($parent_catid)) -> (Size));
			// this parent has children
	    var('j' = array);
	    Iterate( $catz->children, $j); // being a little lazy
		If( ($j -> get(2)) == $parent_catid);
		    // child
		    '<li>-- <a href="';
		    $opts -> find('siteurl');
		    '?cat='; 
		    $j -> Get(1);
		    '" title="';
		    (str('View all posts filed under'));
		    ' '; 
		    $j -> Get(3);
		    '">'; $j -> Get(3); '</a>'; 
		    ' ('; LB_getNrofPosts(($j -> Get(1))); ')';
		    '</li>\r';
		/If;
	    /iterate;

	/If;
    /Iterate;

'</ul>';




If( ($opts -> find('use_fileupload')) == '1');
	  inline( -search,$dbConfig, -table=$table_prefix+'uploads',
		    -neq,
		    'upl_catid'= '-1',
		    -MaxRecords='1',
		    -Search);
	      If( Found_Count );
'<h2>'; '<a href="?files=yes">'; str('File Vault'); '</a>';'</h2>\r';

'<ul>';

// show filecat links
// filecat lists are compiled at top of this file

// Var('i' = array, 'j' = array, 'parent_catid' = Integer);
var('i' = array);
var('parent_catid' = integer);
Iterate( $catz -> fileparents, $i);
    // parent cats (= all except children)
    '<li><a href="?filecat=';
    $i -> Get(1);
    '" title="';
    (str('View all files in'));
    ' '; 
    $i -> Get(3);
    '">'; $i -> Get(3); '</a>'; 
    '</li>\r';
    // any child cats?
    $parent_catid = $i -> Get(1);
	If( $catz->filechildparents -> find($parent_catid) -> size);
			// this parent has children
	    var('j' = array);
	    Iterate( $catz->filechildren, $j); // being a little lazy
		If( ($j -> get(2)) == $parent_catid);
		    // child
		    '<li>-- <a href="?filecat=';
		    $j -> Get(1);
		    '" title="';
		    (str('View all files in'));
		    ' '; 
		    $j -> Get(3);
		    '">'; $j -> Get(3); '</a>'; 
		    '</li>\r';
		/If;
	    /iterate;

	/If;
    /Iterate;
    '</ul>';


  /If; // end if found_count
/Inline;
/If;


'<h2>'; Str('Archives'); '</h2>';
'<ul>';
Inline(-findAll,$dbConfig,-Table=$table_prefix+'posts',
    'pos_status' = 'publish',
    -SortField='pos_date',
    -SortOrder='descending',
    -MaxRecords='all',
    -ReturnField='pos_date',
    -InlineName='archiveSearch');

    var(	'tempDate' 	= Date,
		'myMonth' 	= Integer,
		'myMonthname'	= String,
		'myYear' 	= Integer);

    var('subhea' = '');
    Records;
	$tempDate = (Date(Field('pos_date'),-Format='%Q %T'));

	$myMonth 	= $tempDate -> Month;
	// for month names, see config.lasso
	$myMonthname 	= $aMonths -> Get($myMonth);
	$myYear 	= $tempDate -> Year;

	If( $subhea != (String($myYear)) + (String($myMonth)));
	// display
	    '<li><a href="';
	    $opts -> find('siteurl');
	    '?m=' + $myYear + (Date_Format($tempDate,-Format='%m')) + 
	    '" title="' + $myMonthname + 
	     ' ' +  $myYear + '">' + $myMonthname + ' ' + $myYear + '</a></li>';
	/If;
    $subhea = (String($myYear)) + (String($myMonth));
//     $subhea;'<br>';
    /Records;
/Inline;
'</ul>';

// Links (h2) begin

Inline(-findAll,$dbConfig,-Table=$table_prefix+'linkcategories',
	    -SortField='cat_name',
	    -SortOrder='ascending',
	    -ReturnField='id',
	    -ReturnField='cat_name',
	    -InlineName='linkcategories');
    Records;
    var('linkcatID' = Field('id'));	
			    '<h2 ID="linkcat-'; Loop_Count; '">';
			    Field('cat_name'); '</h2>';
			    '<ul>';

	// embedded inline
	Inline(-search,$dbConfig,-Table=$table_prefix+'links',
		    -SortField='lin_name',
		    -SortOrder='ascending',
		    -ReturnField='lin_url',
		    -ReturnField='lin_name',
		    -ReturnField='lin_description',
		    'lin_category'=$linkcatID,
		    -InlineName='links');
	Records;
	'<li><a href="' + Field('lin_url') + '" title="';
	Field('lin_description') + '">' + Field('lin_name') + '</a></li>';
	/Records;
	/Inline;
'</ul>';
/Records;
/Inline;


'<h2>' + (str('Meta')) + '</h2>';
    '<ul>';

    If($use_level > 0);
	'<li><a href="admin/">'; str('Site Admin'); '</a></li>\r';	
    /If;
    If( integer($opts -> find('users_can_register')) == 1);
	'<li><a href="';
	$opts -> find('siteurl');
	'register.lasso">'; str('Register');
	'</a></li>\r';	
    /If;
    if( $use_level < 1);
	'<li><a href="';
	$opts -> find('siteurl');
	'?showlogin=yes">'; str('Login');
	'</a></li>\r';	
    else;
	'<li><a href="';
	'logout.lasso">'; str('Logout');
	'</a></li>\r';	
    /if;
?>
    </ul>

</td> <!-- end sidebar -->
</tr>
</table>
[/loop]






