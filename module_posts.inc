<?LassoScript

    // posts module

        Var('postid'		= string); // necessary when case=perma
     	Var('pos_status'	= string); // needed when case=perma
        Var('pos_comment_status'= string); // commenting allowed for post? 
        Var('comment_reg'	= (LB_getOption('comment_registration')));

	Var('showCatLink'       = string); // postmetadata line
	Var('showEditLink'      = string); // postmetadata line
	Var('showCommentLink'   = string); // postmetadata line
        Var('showCommentModule' = string); 
	Var('url_topost'	= string);
	Var('siteurl'	        = $basicOptions -> find('siteurl'));

     Inline(-search,$dbConfig,-Table=$table_prefix+'jas_posts',
        -SortField='pos_date',
        -SortOrder='descending',
        -MaxRecords='all',
        $searchArgs,
	-SkipRecords=$skip,
        -InlineName='postsearch'
        );
            If( (Found_Count) == 0);
		$speakBubble = (str('No records were found'));
	    Else( (Found_Count) > 1 && $case == 'perma');
		$speakBubble = (str('Tried to interpret the address as a permalink, but failed. Please check the address!'));
                MO_speakBubble($speakBubble);
		'searchArgs = '; $searchArgs;
//                MO_errorhandler(-level='debug');
		Abort;
            Else( (Error_CurrentError) != (Error_NoError));
		$speakBubble = (Error_CurrentError);
            // Else;
		// $speakBubble = 'No error';
            /If;
//                MO_errorhandler(-level='debug');
    If( $case != 'post' && $case != 'page' && $case != 'perma');
	// Var linkstuff (used in naviglinks) has been defined in cont/index.inc
//	Include( 'inc/naviglinks.inc');
        // naviglinks bugittaa 2011-01
    /If;
    Records(-InlineName='postSearch');

	Var('showCatLink'       = string); // postmetadata line
	Var('showEditLink'      = string); // postmetadata line
	Var('showCommentLink'   = string); // postmetadata line
        Var('showCommentModule' = string); 
	Var('url_topost'	= string);

	$postID = (Field('id'));
	$pos_status = (Field('pos_status'));
	$pos_comment_status = (Field('pos_comment_status'));

	'<div class="post" id="post-'; (Loop_Count); '">\r';
	  '<h3>';
	  // compose link to post
	      $url_topost = $siteurl;
		$url_topost += '?p=' + $postID;  

	  If( (Found_Count) > 1); // show link
	        '<a href="'; $url_topost; 
		'">';
		Field('pos_title'); '</A>';
	  Else; 	// case found_count == 1 (-> no link needed)
	      Field('pos_title'); 
	  /If;
	  '</h3>\r';
	  If( $pos_status != 'static');
	    '<small>'; 

// for multi-user blogs, just comment out the author display 

	    Field('pos_author'); ', ';

	    LB_formatDateTime(Field('pos_date'));

	    '</small>\r';
	  /If;

	// any uploaded files for this post?

	    '<div class="entry">';
		Var('summary'     = string);
		Var('content'     = string);
		// first insert images/files (if any)

                var('files') = MO_collectUploadData(-table='jas_posts',
                           -postid=integer($postid));
                // returns array consisting of shown_files and linked_files

		// process excerpt/summary and content
		$summary += (Field('pos_excerpt'));
		$summary -> (Replace('\r\r','</p><p>'));
		$summary -> (Replace('\r','<br>'));
		$summary -> (Replace('<p>','<p>\r')); // for neater html source

		$content += (Field('pos_content'));
		$content -> (removeLeading('<p>')); 
		$content -> (removeTrailing('<p>')); 

		    '<p class="summary">'; 
			(Var('summary',-encodeSmart)); 
		    '</p>';
                    '<p>'; $files -> get(1); // shown_files
			$content;		    
                      '</p>\r';
		If( $files -> get(2) ); $files -> get(2); /If; // linked_files

	    '</div>\r';	// end entry 


	'<p class="postmetadata">';

	// first lay down logic

	    If( $pos_status != 'static');
		    $showCatLink = 'yes';
	    /If;
	    If( $use_level > 2);
		    $showEditLink = 'yes';
	    /If;
	    If: (LB_getOption:'default_comment_status') == 1
	      && (Field('pos_comment_status')) == 'open'
	      && ( ( $comment_reg == 1 && $use_level > 0)
		   || $comment_reg != 1
		 );
		// these will be checked once more on the 
		// comment form response page (to prevent misuse)
		If($case == 'p'||  $case == 'page');
		    $showCommentModule = 'yes';
		Else;
		    $showCommentLink = 'yes';
		/If;
	    /If;

	// now show

/*  'TEST: level='; $use_level; ' ';
    'showCommentLink='; $showCommentLink; ' ';
    'showCommentModule='; $showCommentModule; ' ';
    'comment_registration='; (LB_getOption('comment_registration')); ' ';
*/

	If( $showCatLink == 'yes');

	    // maincat or subcat?
	    var('cat' = string, 'ix' = integer);
	    If( $aCats_parents_ids -> (FindIndex((Field:'pos_category')))->size);
		$ix = $aCats_parents_ids -> (FindIndex((Field('pos_category')))) 
		    -> (get(1));
		$cat = $aCats_parents -> (get($ix)) -> (get(3));
	    Else;
		$ix = $aCats_children_ids -> (FindIndex((Field('pos_category'))))
		    -> (get(1));
		$cat = $aCats_children -> (get($ix)) -> (get(3));
	    /If;

//	    var:'cat' = $aCats_parents 
//		   -> (Get:($aCats_parents_ids 
//			   -> (FindIndex((Field:'pos_category')))->(Get(1))))
//		   -> (Get(3)); // for title
////	    Variable('cat'= (LB_getCategory((Field:'pos_category'))));
	    str('Posted in'); 
	    ' <a href="'; $siteurl; '?cat='; Field('pos_category'); '"'; 
	    ' title="'; str('View all posts in'); ' '; $cat;
	    '" rel="category tag">'; $cat; '</a>';  
	    If: $showEditLink == 'yes' || $showCommentLink == 'yes'
	        || $comment_reg == 1;
		' <strong>|</strong> ';  
	    /If;
	/If;

	If( $showEditLink == 'yes');
	    '<a href="admin/';
	    If( $pos_status == 'static');
		'man-pageedit.lasso';
		Else;
		'man-postedit.lasso';
	    /If;
	    '?&amp;id=';
	    $postID;
	    '">'; (str('Edit')); '</a>';
	    If( $showCommentLink == 'yes' || $comment_reg == 1);
		' <strong>|</strong> ';  
	    /If;
	/If;

	// link to comments / commenting

	If( $showCommentLink == 'yes');
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
		If( $pos_status == 'static');
		    '?page=';
		Else;
		    '?p=';
		/If;
		$postID; '#comments">'; 
		LB_getCommentLink($postID,-EncodeNone);
	    '</a>';
	Else( $comment_reg == '1');
	str('You have to be registered and logged in to be able to comment');
	/If;
	'</p>';

    '</div>\r'; 	// end post


	Variable('pos_title' = (Field('pos_title')));
    /Records;
    /Inline;
	// show error, if any
	If( $speakBubble != '');
	Include( $myInc + 'speakBubble.inc');
	/If;

// comments module

If( $showCommentModule == 'yes');
    Include( $myinc + 'module_comments.inc');
/If; 
?>

