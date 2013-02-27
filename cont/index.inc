<?LassoScript
loop(1); // hack to mimic abort, which seems to be broken in Lasso 9

// testground
// 	'test - startdate = ' + $startDate + '<br />';
// 	'test - enddate = '   + $endDate   + '<br />';
//	'test - searchArgs = ' + $searchArgs + '<br />';
//	'test - case = ' + $case + '<br />';



// #2 content begins

// #2.1 the left (wide) column

'<table id="content">\n';
'<tr>\n';
'<td id="widecolumn">\n';
'<div id="title">';
$title ? '<h2>' + $title +'</h2>\n';
'</div>\n';
  if( $case == 'files' || $case == 'filecat');
      include( $myinc + 'filevault.inc');
  else;
      include( $myinc + 'module_posts.inc');
  /if;
'</td>\n'; //  end td widecolumn


// #2.2 sidebar
?>
	<td id="sidebar">

<form method="get" id="searchform" action="[$myfilename]">
    <div>
    <input type="text" value="[$s]" name="s" id="s">
    <input type="submit" id="searchsubmit" value="[str('Search!')]">
    </div>
</form>		



	<!-- Author information is disabled per default. Uncomment and fill in your details if you want to use it. -->
	<!-- single-author blog -->
<!--	<h2>Author</h2>
	<p>A little something about you, the author. Nothing lengthy, just an overview.</p>
	-->
	<!-- multi-author blog -->
<?LassoScript
// 	'<h2>'; str('Authors'); '</h2>';
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
// 	    '</li>\n';
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
     If( found_count );
         '<h2>'; str('Special Pages'); '</h2>\n<ul>';
         Records;
 	    Var('permaStruct') = $opts -> find('permalink_structure');
             '<li><a href="'; 
 	      $opts -> find('siteurl');
 		If( ! ($permaStruct -> find('%postname%')) ); 
 			// if option not defined
 		    '?page='; Field('id'); 
  		    '">';
 		Else;	// use permalink structure
 		// available - /%year%/%monthnum%/%day%/%postname%/ /%postid%  
 		  Var('year')      = Date(Field('pos_date')) -> Year;
 		  Var('month')     = Date(Field('pos_date')) -> Month;
 		  Var('day')       = Date(Field('pos_date')) -> Day;
 		  Var('postname')  = Field('pos_name'); 
 		  Var('linkstuff') = $opts -> find('permalink_structure');
 		  $linkstuff -> RemoveLeading('/');
 		  $linkstuff -> Replace('%year%',string($year));
 		  $linkstuff -> Replace('%monthnum%',string($month));
 		  $linkstuff -> Replace('%day%',string($day));
 		  $linkstuff -> Replace('%postname%',$postname);
 		  $linkstuff -> Replace('%postid%',$postID);
 		  encode_url($linkstuff); 
 		  '" title="'; str('Permanent link to'); ' &#34;'; 
 		  Field('pos_title');'&#34;">';
 		/If;
             Field('pos_title');
             '</a></li>\n';
         /Records;
         '</ul>';
     /If;
     /Inline;

'<h2>' + (str('Categories')) + '</h2>\n';

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
    '</li>\n';
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
		    '</li>\n';
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
'<h2>'; '<a href="?files=yes">'; str('File Vault'); '</a>';'</h2>\n';

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
    '</li>\n';
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
		    '</li>\n';
		/If;
	    /iterate;

	/If;
    /Iterate;
    '</ul>';


  /If; // end if found_count
/Inline;
/If;


// begin Archives
    var(        'myMonth' 	= string,
		'myYear' 	= string,
                'subheaMonth'   = string,
                'subheaYear'    = string,
                'url'          = $opts->find('siteurl'),
                'yearMode'      = false,
                'postcount'     = integer,
                'postcount_yearly'  = integer,
                'displaycount'  = integer,
                'monthly'       = array);

var('sql') = 
'select concat(year(pos_date),lpad(month(pos_date), 2, \'0\'),count(*)) from 
  (select pos_date from ' + $table_prefix + 'posts WHERE pos_status=\'publish\' order by pos_date DESC
  ) 
  as tmp 
  group by concat(year(pos_date),lpad(month(pos_date), 2, \'0\'))
  order by pos_date desc
';

  // collect monthly data
  inline($dbConfig,-sql=$sql);
      iterate(records_array,local('i'));
        local('tmp') = #i -> get(1);
        $monthly -> insert(
           array(#tmp -> substring(1,4), // year
           #tmp -> substring(5,2), // month
           integer(#tmp -> substring(7)) // postcount
        ));
      /iterate;
  /inline;

  // sort out and display
'<h2>'; Str('Archives'); '</h2>';
'<ul>';
     iterate($monthly,local('i'));
       $myYear  = #i->get(1);
       $myMonth = #i->get(2);
       $postcount = #i->get(3);
       if($displayCount > 6 && $myYear != $subheaYear); // year changed
         if($yearMode);   // ...but not for the first time 
           // show yearly link for previous year
           LB_sidebarItem( $url + '?y=' + $subheaYear,
                           $subheaYear,
                           ' ('+$postcount_yearly+')'
           );
           $displayCount += 1;
           $postcount_yearly = integer; // reset
         else;  // first year-change - start collecting yearly data
           $yearMode = true; 
         /if;
       /if;
       if(! $yearMode ); // show monthly link
         LB_sidebarItem( $url + '?m=' + $myYear + $myMonth,
                         $aMonths->get(integer($myMonth))+' '+$myYear,
                         ' (' + $postcount + ')'
         );
         $displayCount += 1;
       /if;
       if($yearMode);
         $postcount_yearly += $postcount;   // collect yearly data
       /if;
       $subheaYear  = $myYear;
       $subheaMonth = $myMonth;
     /iterate;

     // show the last yearly link, if there is any data
     if($postcount_yearly);
         LB_sidebarItem( $url + '?y=' + $subheaYear,
                         $subheaYear,
                         ' ('+$postcount_yearly+')'
         );
     /if;
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
        '<li><a href="'; $opts -> find('siteurl');
        'admin/">'; str('Site Admin'); '</a></li>\n'; 
    /If;
    If( integer($opts -> find('users_can_register')) == 1);
	'<li><a href="';
	$opts -> find('siteurl');
	'register.lasso">'; str('Register');
	'</a></li>\n';	
    /If;
    if( $use_level < 1);
	'<li><a href="';
	$opts -> find('siteurl');
	'?showlogin=yes">'; str('Login');
	'</a></li>\n';	
    else;
	'<li><a href="';
	'logout.lasso">'; str('Logout');
	'</a></li>\n';	
    /if;
?>
    </ul>

</td> <!-- end sidebar -->
</tr>
</table>
[/loop]






