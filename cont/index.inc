<?LassoScript

// #0 gather data for cats and filecats

// #0.1 file cats
Inline:-Search,$dbConfig,-Table=$table_prefix+'uploadcats',
	-SortField='upc_name',
	-SortOrder='ascending',
// 	-Op='eq',
// 	'upc_parent' = '',
// even children are searched (needed in case filecat, 
// if selected cat is a child
	-MaxRecords='all',
	-ReturnField='id',
	-ReturnField='upc_parent',
	-ReturnField='upc_name',
	-ReturnField='upc_nicename',
	-ReturnField='upc_description',
	-ReturnField='upc_sortorder',
	-InlineName='filecats_parents';
Var:'aFilecats_parents' = Array;
Records;
If: (Field:'upc_parent') == 0; // child-cats are not inserted
    $aFilecats_parents -> (Insert: (Array:(Field:'id'), '', (Field:'upc_nicename'), (Field:'upc_description'), (Field:'upc_sortorder')));
/If;
/Records;
/Inline;

// #0.2 file subcats
Inline:-Search,$dbConfig,-Table=$table_prefix+'uploadcats',
	-SortField='upc_name',
	-SortOrder='ascending',
	-Op='neq',
	'upc_parent' = '',
	-MaxRecords='all',
	-ReturnField='id',
	-ReturnField='upc_parent',
	-ReturnField='upc_name',
	-ReturnField='upc_nicename',
	-ReturnField='upc_description',
	-ReturnField='upc_sortorder',
	-InlineName='filecats_children';
Var:'aFilecats_children' = Array;
Var:'aFilecats_children_parents' = Array;
Records;
    $aFilecats_children -> 
      (Insert: (Array:(Field:'id'), (Field:'upc_parent'), (Field:'upc_nicename'), (Field:'upc_description'), (Field:'upc_sortorder')));
    $aFilecats_children_parents -> (Insert: (Field:'upc_parent'));
/Records;
/Inline;

// #0.3 post cats
Inline:-Search,$dbConfig,-Table=$table_prefix+'categories',
	-SortField='cat_name',
	-SortOrder='ascending',
// 	-Op='eq',
// 	'cat_parent' = '',
// even children are searched (needed in case cat, 
// if selected cat is a child)
	-MaxRecords='all',
	-ReturnField='id',
	-ReturnField='cat_parent',
	-ReturnField='cat_name',
	-ReturnField='cat_nicename',
	-ReturnField='cat_description',
	-ReturnField='cat_sortorder',
	-InlineName='cats_parents';
Var:'aCats_parents' = array,
    'aCats_parents_ids' = array;
Records;
If: (Field:'cat_parent') == 0; // child-cats are not inserted
    $aCats_parents -> (Insert: (Array:(Field:'id'), '', (Field:'cat_nicename'), (Field:'cat_description'), (Field:'cat_sortorder')));
    $aCats_parents_ids -> (Insert: (Field:'id'));
/If;
/Records;
/Inline;

// #0.4 post subcats
Inline:-Search,$dbConfig,-Table=$table_prefix+'categories',
	-SortField='cat_name',
	-SortOrder='ascending',
	-Op='neq',
	'cat_parent' = '',
	-MaxRecords='all',
	-ReturnField='id',
	-ReturnField='cat_parent',
	-ReturnField='cat_name',
	-ReturnField='cat_nicename',
	-ReturnField='cat_description',
	-ReturnField='cat_sortorder',
	-InlineName='cats_children';
Var:'aCats_children' = array,
    'aCats_children_parents' = array,
    'aCats_children_ids' = array;
Records;
    $aCats_children -> 
      (Insert: (Array:(Field:'id'), (Field:'cat_parent'), (Field:'cat_nicename'), (Field:'cat_description'), (Field:'cat_sortorder')));
    $aCats_children_parents -> (Insert: (Field:'cat_parent'));
    $aCats_children_ids -> (Insert: (Field:'id'));
/Records;
/Inline;

// #1 preparation

Variable: 'searchArgs' = (Array),
		'skip' = (Action_Param:'skip'),
		'case' = '',
	   'linkstuff' = '',
	      'postID' = '';

// capture action params: month, post, searchphrase, category, page

Variable:'m' = (Action_Param:'m'), 
    'p' = (Action_Param:'p'), 
    's' = (Action_Param:'s'), 
    'cat' = (Action_Param:'cat'),
    'page' = (Action_Param:'page'),
    'files' = (Action_Param:'files'),
    'filecat' = (Action_Param:'filecat'),
    'speakBubble' = (Action_Param:'speakBubble'); // capture speakBubble 
		    // value (may be passed from comment-posted.lasso)

If: $speakBubble != '';
    $speakBubble = $speakBubble;
    Include:$myInc + 'speakBubble.inc';
/If;

// check the action params, define case

If: $m != '';		// month
    $case = 'm';
    $linkstuff = 'm=' + $m;
    If: $m -> Size != 6 || (Integer:$m) -> Type != 'Integer';
	$speakBubble += 'Cannot interpret the specified month "' + $m + '"';
    Else;
	Var:'year' = $m -> (SubString:1,4);
	Var:'month' = $m -> (SubString:5,2);
	Var:'startDate' = (Date:($year + '-' + $month + '-' + '1' ));
	If: $month == '12';
        Var:'endDate' =
	 (Date:(((Integer:$year) + 1) + '-' + '1' + '-' + '1' ));
	Else;
	Variable:'endDate' = 
	  (Date:($year + '-' + ((Integer:$month) + 1) + '-' + '1' ));
	/If;
	$searchArgs = (Array:'-Operator' = 'gte', 
		'pos_date' = (Date_Format:$startDate,-Format='%Q %T'),
		'-Operator' = 'lt', 
		'pos_date' = (Date_Format:$endDate,-Format='%Q %T'),
		'pos_status' = 'publish',
		);
    /If;

Else: $p != ''; 		// post (and comments)
    $case = 'p';
    $linkstuff = 'p=' + $p;
    $searchArgs = (Array:'id' = $p, 'pos_status' = 'publish'
    );

Else: $s != ''; 		// search form processing
    $case = 's';
    $linkstuff = 's=' + $s;
	$s = (String_ReplaceRegExp: $s, -Find='\\s+', -replace=' ');
	$s -> (Trim);
	$searchArgs -> (Insert: '-OperatorBegin' = 'or');

//	$searchArgs -> (Insert: '-Op' = 'ft');
// 	$searchArgs -> (Insert: 'pos_combined' = $s);
// 	$searchArgs -> (Insert: 'pos_title' = $s);
// 	$searchArgs -> (Insert: '-Op' = 'ft');
// 	$searchArgs -> (Insert: 'pos_excerpt' = $s);
// 	$searchArgs -> (Insert: '-Op' = 'ft');
// 	$searchArgs -> (Insert: 'pos_content' = $s);

	$searchArgs -> (Insert: '-Op' = 'cn');
 	$searchArgs -> (Insert: 'pos_title' = $s);
 	$searchArgs -> (Insert: '-Op' = 'cn');
 	$searchArgs -> (Insert: 'pos_excerpt' = $s);
 	$searchArgs -> (Insert: '-Op' = 'cn');
 	$searchArgs -> (Insert: 'pos_content' = $s);

	$searchArgs -> (Insert: '-OperatorEnd' = 'or');
	$searchArgs -> (Insert: 'pos_status' = 'publish');

Else: $cat != '';		// category
    $case = 'cat';
    $linkstuff = 'cat=' + $cat;
    $searchArgs = (Array:'pos_status' = 'publish', 
    			'-OpBegin' = 'or',
			'pos_category' = $cat);

    // subcats should be included, too: search for them

	    If: $aCats_children_parents -> (Find:$cat) -> Size;
		var:'i' =string, 
		    'lc'=integer;
		Iterate:$aCats_children, $i;
		    $lc += 1;
		    If: $aCats_children_parents -> (get:$lc) == $cat;
		    $searchargs -> (Insert: 
			'pos_category'=
			($aCats_children -> (get:$lc) -> (get:1))
		    );
		    /If;
		/Iterate;
	    /If;
// 'test zone<br>';
// '<b>aCats_parents</b>=' + $aCats_parents + '<br>';
// '<b>aCats_parents_ids</b>=' + $aCats_parents_ids + '<br>';
// '<b>aCats_children</b>=' + $aCats_children + '<br>';
// '<b>aCats_children_parents</b>=' + $aCats_children_parents + '<br>';
// '<b>$cat</b>=' + $cat + '<br>';

		    $searchargs -> (Insert: '-OpEnd' = 'or');

	    // get cat name (to display title)
	    // get cat sorting order (from v0.7.2 up)

	    // maincat or subcat?
	    var:'cat_name' = string, 
		'cat_description' = string, 
		'ix' = integer;
	    If: $aCats_parents_ids -> (FindIndex:$cat)->size;
		$ix = $aCats_parents_ids -> (FindIndex:$cat) -> (get:1);
		$cat_name = $aCats_parents -> (get:$ix) -> (get:3);
		$cat_description = $aCats_parents -> (get:$ix) -> (get:4);
		$searchArgs -> 
		  (Insert:'-SortOrder'= $aCats_parents -> (get:$ix) -> (get:5));
	    Else: $aCats_children_ids -> (FindIndex:$cat)->size;
		$ix = $aCats_children_ids -> (FindIndex:$cat) -> (get:1);
		$cat_name = $aCats_children -> (get:$ix) -> (get:3);
		$cat_description = $aCats_children -> (get:$ix) -> (get:4);
		$searchArgs -> 
		  (Insert:'-SortOrder'= $aCats_children->(get:$ix)->(get:5));
	    Else;
		$speakBubble = (str:'Sorry, no such category!');
		Include: $myinc + 'speakBubble.inc';
	    /If;
		
Else: $page != ''; 		// static page (and comments)
    $case = 'page';
    $searchArgs = (Array:'id' = $page, 'pos_status' = 'static'
    );
Else: $files != ''; 		// file vault
    $case = 'files';
Else: $filecat != ''; 		// file vault, only one cat
    $case = 'filecat';
Else: (Variable_Defined:'perma'); 
    $searchArgs = $permaSearchArgs;
		// this page is loaded by /permalink_handler.lasso
    $case = 'perma';
Else; 		// find the latest
    $case = '';
    $searchArgs = (Array:'pos_status' = 'publish'
    );

/If;

If: $speakBubble != ''; // if errors detected so far, tell them
Include: $myinc + 'speakBubble.inc';
Abort;
/If;

// testground
// 	'test: startdate = ' + $startDate + '<br>';
// 	'test: enddate = '   + $endDate   + '<br>';
//	'test: searchArgs = ' + $searchArgs + '<br>';
//	'test: case = ' + $case + '<br>';



// #2 content begins

// #2.1 the left (wide) column

'<table cellspacing="0" cellpadding="0" border="0">';
'<tr>';
'<td valign="top" id="widecolumn">';
'<div id="title">';
If: $case == 'm'; // monthly
    '<h2>';
    $aMonths -> (get: $month); ' '; $year;
    '</h2>';
Else: $case == 'cat';
    '<h2>';
    $cat_name; ' <span class="subtitle">'; $cat_description; '</span>';
    '</h2>';
Else: $case == 'files' || $case == 'filecat';
    '<h2>';
    (str:'File Vault');
    '</h2>';
// Else: $case == '';
//     Variable: 'blogdesc' = $basicOptions -> 'blogdescription';
//     '<h2>';
// 	$blogdesc -> (Get:1);
// 	'<span class="subtitle">'; 
// 	$blogdesc -> (Substring:2, ($blogdesc -> Size));
// 	'</span>';
//     '</h2>';
/If;
'</div>\r';
If: $case == 'files' || $case == 'filecat';
    Include: $myinc + 'filevault.inc';
// Else: !($case);
//     Include: $myinc + 'postlist.inc';
Else;
    Include: $myinc + 'module_posts.inc';
/If;

'</td>\r'; //  end td widecolumn


// #2.2 sidebar

?>

	<td valign="top" id="sidebar">

<form method="get" id="searchform" action="[$myfilename]">
    <div>
    <input type="text" value="[$s]" name="s" id="s">
    <input type="submit" id="searchsubmit" value="[str:'Search!']">
    </div>
</form>		



	<!-- Author information is disabled per default. Uncomment and fill in your details if you want to use it. -->
	<!-- single-author blog: -->
<!--	<h2>Author</h2>
	<p>A little something about you, the author. Nothing lengthy, just an overview.</p>
	-->
	<!-- multi-author blog: -->
<?LassoScript
// 	'<h2>'; (str:'Authors'); '</h2>';
//     Inline:-search,$dbConfig,-Table=$table_prefix+'users',
// 	-Op='gte',
// 	'use_level' =2,
// 	-SortField='use_nickname',
// 	-SortOrder='ascending',
// 	-MaxRecords='all',
// 	-ReturnField='id',
// 	-ReturnField='use_nickname',
// 	-InlineName='getAuthorlist';
// 	'<ul>';
//         Records;
//             '<li>';
// 		// '<a href="?page='; Field:'id'; '">';
// 		Field:'use_nickname';
// 		// '</a>';
// 	    '</li>\r';
//         /Records;
// 	'</ul>';
// 	/Inline;

    // begin Special Pages

    Inline:-search,$dbConfig,-Table=$table_prefix+'posts',
	'pos_status' = 'static',
	-SortField='pos_menu_order',
	-SortOrder='ascending',
	-MaxRecords='all',
	-ReturnField='id',
	-ReturnField='pos_title',
	-ReturnField='pos_date',
	-ReturnField='pos_name',
	-InlineName='getStaticPages';
    If: (Found_Count) > 0;
        '<h2>'; str:'Special Pages'; '</h2>\r<ul>';
        Records;
	    Var:'permaStruct' = $basicOptions -> 'permalink_structure';
            '<li><a href="'; 
	      $basicOptions -> 'siteurl';
		If: !($permaStruct -> (Find:'%postname%')); 
			// if option not defined
		    '?page='; (Field:'id'); 
 		    '">';
		Else;	// use permalink structure
		// available: /%year%/%monthnum%/%day%/%postname%/ /%postid%  
		  Var:'year' = (Date:(Field:'pos_date')) -> Year;
		  Var:'month' = (Date:(Field:'pos_date')) -> Month;
		  Var:'day' = (Date:(Field:'pos_date')) -> Day;
		  Var:'postname' = (Field:'pos_name'); 
		  Var:'linkstuff' = ($basicOptions -> 'permalink_structure');
		  $linkstuff ->  (RemoveLeading:'/');
		  $linkstuff -> (Replace:'%year%',$year);
		  $linkstuff -> (Replace:'%monthnum%',$month);
		  $linkstuff -> (Replace:'%day%',$day);
		  $linkstuff -> (Replace:'%postname%',$postname);
		  $linkstuff -> (Replace:'%postid%',$postID);
		  (Var:'linkstuff',-encodeURL);
		  '" TITLE="'; (str:'Permanent link to'); ' &#34;'; 
		  Field:'pos_title';'&#34;">';
		/If;
            Field:'pos_title';
            '</a></li>\r';
        /Records;
        '</ul>';
    /If;
    /Inline;

'<h2>' + (str:'Categories') + '</h2>\r';

'<ul>';
// show cat links
// cat lists are compiled at top of this file

var:'i' = array;
var:'parent_catid' = integer;
Iterate: $aCats_parents, $i;
    // parent cats (= all except children)
    '<li><a href="';
    $basicOptions -> 'siteurl';
    '?cat=';
    $i -> (Get:1);
    '" title="';
    (str:'View all posts filed under');
    ' '; 
    $i -> (Get:3);
    '">'; $i -> (Get:3); '</a>'; 
    ' ('; LB_getNrofPosts:($i -> (Get:1)); ')';
    '</li>\r';
    // any child cats?
    $parent_catid = $i -> (Get:1);
	If: $aCats_children_parents -> (Find:$parent_catid) -> (Size);
			// this parent has children
	    var:'j' = array;
	    Iterate: $aCats_children, $j; // being a little lazy
		If: ($j -> (get:2)) == $parent_catid;
		    // child
		    '<li>-- <a href="';
		    $basicOptions -> 'siteurl';
		    '?cat='; 
		    $j -> (Get:1);
		    '" title="';
		    (str:'View all posts filed under');
		    ' '; 
		    $j -> (Get:3);
		    '">'; $j -> (Get:3); '</a>'; 
		    ' ('; LB_getNrofPosts:($j -> (Get:1)); ')';
		    '</li>\r';
		/If;
	    /iterate;

	/If;
    /Iterate;

'</ul>';




If: (LB_basicOptions -> 'use_fileupload') == 1;
	  inline: $dbConfig, -table=$table_prefix+'uploads',
		    -Op='neq',
		    'upl_catid'= '-1',
		    -MaxRecords='1',
		    -Search;
	      If: (Found_Count);
'<h2>'; '<a href="?files=yes">'; str:'File Vault'; '</a>';'</h2>\r';

'<ul>';

// show filecat links
// filecat lists are compiled at top of this file

// Var:'i' = array, 'j' = array, 'parent_catid' = Integer;
var:'i' = array;
var:'parent_catid' = integer;
Iterate: $aFilecats_parents, $i;
    // parent cats (= all except children)
    '<li><a href="?filecat=';
    $i -> (Get:1);
    '" title="';
    (str:'View all files in');
    ' '; 
    $i -> (Get:3);
    '">'; $i -> (Get:3); '</a>'; 
    '</li>\r';
    // any child cats?
    $parent_catid = $i -> (Get:1);
	If: $aFilecats_children_parents -> (Find:$parent_catid) -> (Size);
			// this parent has children
	    var:'j' = array;
	    Iterate: $aFilecats_children, $j; // being a little lazy
		If: ($j -> (get:2)) == $parent_catid;
		    // child
		    '<li>-- <a href="?filecat=';
		    $j -> (Get:1);
		    '" title="';
		    (str:'View all files in');
		    ' '; 
		    $j -> (Get:3);
		    '">'; $j -> (Get:3); '</a>'; 
		    '</li>\r';
		/If;
	    /iterate;

	/If;
    /Iterate;
    '</ul>';


  /If; // end if found_count
/Inline;
/If;


'<h2>'; Str:'Archives'; '</h2>';
'<ul>';
Inline:-findAll,$dbConfig,-Table=$table_prefix+'posts',
    'pos_status' = 'publish',
    -SortField='pos_date',
    -SortOrder='descending',
    -MaxRecords='all',
    -ReturnField='pos_date',
    -InlineName='archiveSearch';

    Variable:	'tempDate' 	= (Date),
		'myMonth' 	= (Integer),
		'myMonthname'	= (String),
		'myYear' 	= (Integer);

    Variable:'subhea' = '';
    Records;
	$tempDate = (Date:(Field:'pos_date'),-Format='%Q %T');

	$myMonth 	= $tempDate -> Month;
	// for month names, see config.lasso
	$myMonthname 	= $aMonths -> (Get:$myMonth);
	$myYear 	= $tempDate -> Year;

	If: $subhea != (String:$myYear) + (String:$myMonth);
	// display
	    '<li><a href="';
	    $basicOptions -> 'siteurl';
	    '?m=' + $myYear + (Date_Format:$tempDate,-Format='%m') + 
	    '" title="' + $myMonthname + 
	     ' ' +  $myYear + '">' + $myMonthname + ' ' + $myYear + '</a></li>';
	/If;
    $subhea = (String:$myYear) + (String:$myMonth);
//     $subhea;'<br>';
    /Records;
/Inline;
'</ul>';

// Links (h2) begin

Inline:-findAll,$dbConfig,-Table=$table_prefix+'linkcategories',
	    -SortField='cat_name',
	    -SortOrder='ascending',
	    -ReturnField='id',
	    -ReturnField='cat_name',
	    -InlineName='linkcategories';
    Records;
    Variable:'linkcatID' = (Field:'id');	
			    '<h2 ID="linkcat-'; Loop_Count; '">';
			    Field:'cat_name'; '</h2>';
			    '<ul>';

	// embedded inline
	Inline:-search,$dbConfig,-Table=$table_prefix+'links',
		    -SortField='lin_name',
		    -SortOrder='ascending',
		    -ReturnField='lin_url',
		    -ReturnField='lin_name',
		    -ReturnField='lin_description',
		    'lin_category'=$linkcatID,
		    -InlineName='links';
	Records;
	'<li><a href="' + (Field:'lin_url') + '" title="';
	(Field:'lin_description') + '">' + (Field:'lin_name') + '</a></li>';
	/Records;
	/Inline;
'</ul>';
/Records;
/Inline;


'<h2>' + (str:'Meta') + '</h2>';
    '<ul>';

    If:$use_level > 0;
	'<li><a href="admin/">'; str:'Site Admin'; '</a></li>\r';	
    /If;
    If: (LB_getOption:'users_can_register') == 1;
	'<li><a href="';
	$basicOptions -> 'siteurl';
	'use-register.lasso?emptyForm=yes">'; str:'Register';
	'</a></li>\r';	
    /If;
    if: $use_level < 1;
	'<li><a href="';
	$basicOptions -> 'siteurl';
	'?showlogin=yes">'; str:'Login';
	'</a></li>\r';	
    else;
	'<li><a href="';
	'logout.lasso">'; str:'Logout';
	'</a></li>\r';	
    /if;
?>
    </ul>

</td> <!-- end sidebar -->
</tr>
</table>






