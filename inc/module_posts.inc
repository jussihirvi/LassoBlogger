<?LassoScript

    // posts module

        Var:'postid'		= string; // necessary when case=perma
     	Var:'pos_status'	= string; // needed when case=perma
        Var:'pos_comment_status'= string; // commenting allowed for post? 
        Var:'comment_reg'	= (LB_getOption:'comment_registration');

	Var:'showCatLink'       = string; // postmetadata line
	Var:'showEditLink'      = string; // postmetadata line
	Var:'showCommentLink'   = string; // postmetadata line
        Var:'showCommentModule' = string; 
	Var:'url_topost'	= string;
	Var:'siteurl'	        = $basicOptions -> 'siteurl';

     Inline:-search,$dbConfig,-Table=$table_prefix+'posts',
        -SortField='pos_date',
        -SortOrder='descending',
        -MaxRecords=(LB_getOption:'posts_per_page'),
        $searchArgs,
	-SkipRecords=$skip,
        -InlineName='postsearch';
            If: (Found_Count) == 0;
		$speakBubble = (str:'No records were found');
	    Else: (Found_Count) > 1 && $case == 'perma';
		$speakBubble = (str:'Tried to interpret the address as a permalink, but failed. Please check the address!');
		Include: $myInc + 'speakBubble.inc';
		'searchArgs = '; $searchArgs;
//		Include: $myInc + 'errorhandler_debug.inc';
		Abort;
            Else: (Error_CurrentError) != (Error_NoError);
		$speakBubble = (Error_CurrentError);
            // Else;
		// $speakBubble = 'No error';
            /If;
// 	Include: $myInc + 'errorhandler_debug.inc';

    If: $case != 'p' && $case != 'page' && $case != 'perma';
	// Var linkstuff (used in naviglinks) has been defined in cont/index.inc
	Include: $myInc + 'snip_naviglinks.inc';
    /If;

    Records:-InlineName='postSearch';

	Var:'showCatLink'       = string; // postmetadata line
	Var:'showEditLink'      = string; // postmetadata line
	Var:'showCommentLink'   = string; // postmetadata line
        Var:'showCommentModule' = string; 
	Var:'url_topost'	= string;

	$postID = (Field:'id');
	$pos_status = (Field:'pos_status');
	$pos_comment_status = (Field:'pos_comment_status');

	'<div class="post" id="post-'; (Loop_Count); '">\r';
	  '<h3>';
	  // compose link to post
	      $url_topost = $siteurl;
		If: $basicOptions -> 'permalink_structure' == ''; 
		  // if option value is empty: no permalinks
		$url_topost += '?p=' + $postID;  
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
		  $url_topost += (var:'linkstuff',-encodeURL);
		/if;

	  If: (Found_Count) > 1; // show link
	        '<a href="'; $url_topost; 
		If: $basicOptions -> 'permalink_structure' != ''; 
		  '" TITLE="'; (str:'Permanent link to'); ' &#34;'; 
		  Field:'pos_title';'&#34;';
		/If;
		'">';
		Field:'pos_title'; '</A>';
	  Else; 	// case found_count == 1 (-> no link needed)
	      Field:'pos_title'; 
	  /If;
	  '</h3>\r';
	  If: $pos_status != 'static';
	    '<small>'; 

// for multi-user blogs, just comment out the author display 

	    LB_getAuthor:(Field:'pos_author'); ', ';

	    LB_formatDateTime:(Field:'pos_date');

	    '</small>\r';
	  /If;

	// any uploaded files for this post?

	Var:'aFiles' = (Array);
	If: LB_Basicoptions -> 'use_fileupload' == '1';
	  inline: $dbConfig, -table=$table_prefix+'uploads',
		-Op='eq',
		'upl_catid'= '-1',
		-Op='eq',
		'upl_postid'= $postid,
		-Search;
	  Records;
	      // collect file data
	      $aFiles -> (Insert:(Array:(Field:'upl_path'),(Field:'upl_name'),
	      (Field:'upl_description'), (Field:'upl_imagedisplay'), 
	      (field:'upl_linkurl')));
	  /Records;
	  /Inline;
	/If;
	    '<div class="entry">';
//		'test: afiles = ' + $aFiles + '<br>';
		Var:'summary' = '';
		Var:'content' = '';
		// first insert images/files (if any)
		Var:'suffix' 	= '';
		Var:'linked_files' = '<p>';
		Var:'shown_files' = '';
		Var:'i' 	= Array;
		Iterate: $aFiles, $i;
		    $suffix = $i -> (Get: 2)  -> (Split:'.') -> Last;

		    If:  $i -> (Get:4) == 'none'; // just skip the file
		    Else: ($suffix == 'jpg' 
			    ||  $suffix == 'jpeg'
			    ||  $suffix == 'gif'
			    ||  $suffix == 'png') 
			    &&  $i -> (Get:4) != 'link'; // display that img
			// define style class
			Var:'imgclass' = '';
			If: $i -> (Get:4) == 'right';
			    $imgclass = 'alignright';
			Else: $i -> (Get:4) == 'left';
			    $imgclass = 'alignleft';
			Else: $i -> (Get:4) == 'block';
			    $imgclass = 'centered';
			/If;
			// is the image supposed to act as a link?
//			if: $i -> (get:5);
//			    $shown_files += '<a href="';
//			    if: $i -> (get:5) -> (BeginsWith:'http');
//				$shown_files += $i -> (get:5) + '">';
//			    else: ($i -> (get:5) -> (BeginsWith:'/'));
//				$shown_files += $i -> (get:5) + '">';
//			    else;
//				$shown_files += $basicOptions -> 'fileupload_realpath' + '/' + $i -> (get:5) + '">';
//			/if;

			$shown_files += '<IMG class="' + $imgclass + '" SRC="';
			$shown_files += $i -> (Get:1) + $i -> (Get:2) + '" alt="';
			$shown_files += $i -> (Get:3) + '" title="';
			$shown_files += $i -> (Get:3) + '">\r';
		       // close the a tag?
			if: $i -> (get:5);
			    $shown_files += '</a>';                
			/if;
		    Else; 			 	// just display link
			$linked_files += '<A HREF="' +  $i -> (Get:1) + $i -> (Get:2);
			$linked_files += '" TITLE="' +  $i -> (Get:3);
			$linked_files += '">' + $i -> (Get:2) + '</A><br>\r';
		    /If;
		/Iterate;
	        $linked_files += '</p>\r';


		// process excerpt/summary and content
		$summary += (Field:'pos_excerpt');
		$summary -> (Replace:'\r\r','</p><p>');
		$summary -> (Replace:'\r','<br>');
		$summary -> (Replace:'<p>','<p>\r'); // for neater html source

		$content += (Field:'pos_content');
		$content -> (Replace:'\r\r','</p><p>');
		$content -> (Replace:'\r','<br>');
		$content -> (Replace:'<p>','<p>\r'); // for neater html source

		// smilies

		var:'smilies' = (map:
		':)' = 'smile',
		':-)' = 'smile',
		':D' = 'biggrin',
		':-D' = 'biggrin',
		':(' = 'sad',
		':-(' = 'sad',
		':o' = 'eek',
		':-o' = 'eek',
		':0' = 'surprised',
		':-0' = 'surprised',
		'8O' = 'surprised',
		'8-O' = 'surprised',
		':?' = 'question',
		':-?' = 'question',
		'8)' = 'cool',
		'8-)' = 'cool',
		':X' = 'mad',
		':-X' = 'mad',
		':P' = 'razz',
		':-P' = 'razz',
		':|' = 'neutral',
		':-|' = 'neutral',
		';)' = 'wink',
		';-)' = 'wink' 
		);

		if: ($basicOptions -> 'use_smilies') == 1;
		    var:'i' = string;
		    iterate:$smilies, $i;
			$summary -> (replace:($i -> first),
			  '<img src="' + $siteurl + 'images/smilies/icon_' 
			  + ($i -> second) + '.gif" alt="smilie" title="' 
			  + ($i -> second) + '">'); 
			$content -> (replace:($i -> first),
			  '<img src="' + $siteurl + 'images/smilies/icon_' 
			  + ($i -> second) + '.gif" alt="smilie" title="' 
			  + ($i -> second) + '">'); 
		    /iterate;
		/if;


		If: ($basicOptions -> 'www_use_summary') == 1
			&& $summary && ($case != 'p' && $case != 'perma');
		    '<p class="summary">';$shown_files; 
			(Var:'summary',-encodeSmart); 
		    '</p>\r';
		    '<p class="linkfromsummary"><a href="' + $url_topost + '" TITLE="';
		    (str:'Read the whole post');
		    '">';
		    (str:'Read the whole post'); ' &#187;</a></p>\r';
		Else;
		    '<p class="summary">';$shown_files; 
			(Var:'summary',-encodeSmart); 
		    '</p><p>';
			(Var:'content',-encodeSmart); 
		    '</p>\r';
		/If;
		If: $linked_files != '<p></p>\r'; $linked_files; /If;

	    '</div>\r';	// end entry 


	'<p class="postmetadata">';

	// first lay down logic

	    If: $pos_status != 'static';
		    $showCatLink = 'yes';
	    /If;
	    If: $use_level > 2;
		    $showEditLink = 'yes';
	    /If;
	    If: (LB_getOption:'default_comment_status') == 1
	      && (Field:'pos_comment_status') == 'open'
	      && ( ( $comment_reg == 1 && $use_level > 0)
		   || $comment_reg != 1
		 );
		// these will be checked once more on the 
		// comment form response page (to prevent misuse)
		If:$case == 'p'||  $case == 'page';
		    $showCommentModule = 'yes';
		Else;
		    $showCommentLink = 'yes';
		/If;
	    /If;

	// now show

/*  'TEST: level='; $use_level; ' ';
    'showCommentLink='; $showCommentLink; ' ';
    'showCommentModule='; $showCommentModule; ' ';
    'comment_registration='; (LB_getOption:'comment_registration'); ' ';
*/

	If: $showCatLink == 'yes';

	    // maincat or subcat?
	    var:'cat' = string, 'ix' = integer;
	    If: $aCats_parents_ids -> (FindIndex:(Field:'pos_category'))->size;
		$ix = $aCats_parents_ids -> (FindIndex:(Field:'pos_category')) 
		    -> (get:1);
		$cat = $aCats_parents -> (get:$ix) -> (get:3);
	    Else;
		$ix = $aCats_children_ids -> (FindIndex:(Field:'pos_category'))
		    -> (get:1);
		$cat = $aCats_children -> (get:$ix) -> (get:3);
	    /If;

//	    var:'cat' = $aCats_parents 
//		   -> (Get:($aCats_parents_ids 
//			   -> (FindIndex:(Field:'pos_category'))->(Get:1)))
//		   -> (Get:3); // for title
////	    Variable:'cat'= (LB_getCategory:(Field:'pos_category'));
	    (str:'Posted in'); 
	    ' <a href="'; $siteurl; '?cat='; Field:'pos_category'; '"'; 
	    ' title="'; str:'View all posts in'; ' '; $cat;
	    '" rel="category tag">'; $cat; '</a>';  
	    If: $showEditLink == 'yes' || $showCommentLink == 'yes'
	        || $comment_reg == 1;
		' <strong>|</strong> ';  
	    /If;
	/If;

	If: $showEditLink == 'yes';
	    '<a href="admin/';
	    If: $pos_status == 'static';
		'man-pageedit.lasso';
		Else;
		'man-postedit.lasso';
	    /If;
	    '?&amp;id=';
	    $postID;
	    '">'; (str:'Edit'); '</a>';
	    If: $showCommentLink == 'yes' || $comment_reg == 1;
		' <strong>|</strong> ';  
	    /If;
	/If;

	// link to comments / commenting

	If: $showCommentLink == 'yes';
	  /* NOTE: for case=perma we show comment *links*, because
	     if we would show comment *form* on the perma page
	     (as is done on single-post pages normally, 
	     that is, when case=p)
	     the user cookies could not be read, because the "virtual
	     directory" is different from that stored in
	     cookies; :-) if someone knows a way around this,
	     let me know; maybe in Lasso 8...
	  */

	    '<a href="'; $siteurl;
		If: $pos_status == 'static';
		    '?page=';
		Else;
		    '?p=';
		/If;
		$postID; '#comments">'; 
		LB_getCommentLink:$postID,-EncodeNone;
	    '</a>';
	Else: $comment_reg == '1';
	str:'You have to be registered and logged in to be able to comment';
	/If;
	'</p>';

    '</div>\r'; 	// end post


	Variable:'pos_title' = (Field:'pos_title');
    /Records;
    /Inline;
	// show error, if any
	If: $speakBubble != '';
	Include: $myInc + 'speakBubble.inc';
	/If;

// comments module

If: $showCommentModule == 'yes';
    Include: $myinc + 'module_comments.inc';
/If; 
?>
