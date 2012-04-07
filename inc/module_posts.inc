<?LassoScript
loop(1); // hack to mimic abort on L9

    // posts module

        var('comment_reg'	= $opts -> find('comment_registration'));
	var('siteurl'	        = $opts -> find('siteurl'));

	var('showcatlink'       = string);
	var('showeditlink'      = string);
	var('showcommentlink'   = string);
        var('showcommentmodule' = string); 
        var('postid'		= string); // necessary when case=perma
     	var('pos_status'	= string); // needed when case=perma
        var('pos_comment_status'= string); // commenting allowed for post? 
	var('url_topost'	= string);

     inline(-search,$dbConfig,-table=$table_prefix+'posts',
        -sortField='pos_date',
        -sortOrder='descending',
        -maxRecords=$opts->find('posts_per_page'),
        $searchArgs,
	-skipRecords=$skip,
        -inlineName='postsearch');
            if( (found_count) == 0);
		$speakBubble = (str('No records were found'));
	    else( (found_count) > 1 && $case == 'perma');
		MO_speakBubble(str('Tried to interpret the address as a permalink, but failed. Please check the address!'));
		'searchArgs = '; $searchArgs;
		abort;
            else( error_currenterror != error_noerror );
		MO_speakBubble(Error_CurrentError);
            /if;

    if( $case != 'p' && $case != 'page' && $case != 'perma');
        MO_naviglinks(-rectype=$rectype,-rectype_pl=$rectype_plural);
    /if;

    records(-inlineName='postSearch');

	$showCatLink       = string;
	$showEditLink      = string;
	$showCommentLink   = string;
        $showCommentModule = string;
	$postID            = string(field('id'));
	$pos_status        = field('pos_status');
	$pos_comment_status= field('pos_comment_status');

        // compose link to post
        $url_topost = $siteurl;
          if( ! $opts->find('permalink_structure') ); // permalinks switched off
            $url_topost += '?p=' + $postID;  
          else;	// use permalink structure
            // available - /%year%/%monthnum%/%day%/%postname%/%postid%  
            var('year'      = date(field('pos_date')) -> Year);
            var('month'     = date(field('pos_date')) -> Month);
            var('day'       = date(field('pos_date')) -> Day);
            var('postname'  = field('pos_name')); 
            // string casting below is mandatory 
            // otherwise L9 does not behave
            // it modifies $opts->find('perma
            // but for some reason only on first round
            var('linkstuff' = string($opts->find('permalink_structure')));
            $linkstuff -> removeLeading('/');
            $linkstuff -> replace('%year%',string($year));
            $linkstuff -> replace('%monthnum%',string($month));
            $linkstuff -> replace('%day%',string($day));
            $linkstuff -> replace('%postname%',string($postname));
            $linkstuff -> replace('%postid%',$postID);
            $url_topost += encode_url($linkstuff);
          /if;

	'<div class="post" id="post-'; (Loop_Count); '">\r';
	  '<h3>';
            // show link
	        '<a href="'; $url_topost; 
		if( $opts -> find('permalink_structure') ); 
		  '" TITLE="'; (str('Permanent link to')); ' &#34;'; 
		  field('pos_title');'&#34;';
		/if;
		'">';
		field('pos_title'); '</A>';
	  '</h3>\r';
	  if( $pos_status != 'static');
	    '<small>'; 

// for multi-user blogs, just comment out the author display 
	    LB_getAuthor(field('pos_author')); ', ';
	    LB_formatDateTime(field('pos_date'));
	    '</small>\r';
	  /If;

	    '<div class="entry">';
		var('summary'     = string);
		var('content'     = string);

		// first insert uploaded files (if any)
                var('files') = MO_collectUploadData(-table='posts',
                           -postid=integer($postid));
                // returns array consisting of shown_files and linked_files
		// process excerpt/summary and content
                // html_comment;
                // 'test files is '; $files;
                // /html_comment;
                // loop_continue;
		$summary += field('pos_excerpt');
		$summary -> replace('\r\r','</p><p>');
		$summary -> replace('\r','<br>');
		$summary -> replace('<p>','<p>\r'); // for neater html source

		$content += field('pos_content');
		$content -> replace('\r\r','</p><p>');
		$content -> replace('\r','<br>');
		$content -> replace('<p>','<p>\r'); // for neater html source

		// smilies

		var('smilies' = map(
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
		));

		if( ($opts -> find('use_smilies')) == 1);
		    var('i' = string);
		    iterate($smilies, $i);
			$summary -> replace($i -> first,
			  '<img src="' + $siteurl + 'images/smilies/icon_' 
			  + ($i -> second) + '.gif" alt="smilie" title="' 
			  + ($i -> second) + '">'); 
			$content -> replace($i -> first,
			  '<img src="' + $siteurl + 'images/smilies/icon_' 
			  + ($i -> second) + '.gif" alt="smilie" title="' 
			  + ($i -> second) + '">'); 
		    /iterate;
		/if;


		If( $opts -> find('www_use_summary') == '1'
			&& $summary && ($case != 'p' && $case != 'perma'));
		    '<p class="summary">';$shown_files; 
                        encode_smart($summary);
		    '</p>\r';
		    '<p class="linkfromsummary"><a href="' + $url_topost + '" TITLE="';
		    str('Read the whole post');
		    '">';
		    str('Read the whole post'); ' &#187;</a></p>\r';
		Else;
		    '<p class="summary">'; 
                        $files -> get(1); // shown_files
                        encode_smart($summary);
		    '</p><p>';
                        encode_smart($content);
		    '</p>\r';
		/If;
		If( $files->get(2)->size); $files->get(2); /If; //linked_files

	    '</div>\r';	// end entry 


	'<p class="postmetadata">';

	// first lay down logic

	    If( $pos_status != 'static');
		    $showCatLink = 'yes';
	    /If;
	    If( $use_level > 2);
		    $showEditLink = 'yes';
	    /If;
	    If( $opts -> find('default_comment_status') == '1'
	      && field('pos_comment_status') == 'open'
	      && ( ( $comment_reg == 1 && $use_level > 0)
		   || $comment_reg != 1
		 ));
		// these will be checked once more on the 
		// comment form response page (to prevent misuse)

		If($case == 'p'||  $case == 'page' || $case == 'perma' );
		    $showCommentModule = 'yes';
		Else;
		    $showCommentLink = 'yes';
		/If;
	    /If;

	// now show

/*  'TEST - level='; $use_level; ' ';
    'showCommentLink='; $showCommentLink; ' ';
    'showCommentModule='; $showCommentModule; ' ';
    'comment_registration='; $opts -> find('comment_registration'); ' ';
*/

	If( $showCatLink == 'yes');

	    // maincat or subcat?
	    var('cat' = string, 'ix' = integer);
	    If( $catz->parentids -> FindIndex(Field('pos_category'))->size);
		$ix = $catz->parentids -> FindIndex(Field('pos_category')) 
		    -> (get(1));
		$cat = $catz->parents -> get($ix) -> get(3);
	    Else;
		$ix = $catz->childids -> FindIndex(Field('pos_category'))
		    -> get(1);
		$cat = $catz->children -> get($ix) -> get(3);
	    /If;

	    str('Posted in'); 
	    ' <a href="'; $siteurl; '?cat='; Field('pos_category'); '"'; 
	    ' title="'; str('View all posts in'); ' '; $cat;
	    '" rel="category tag">'; $cat; '</a>';  
	    If( $showEditLink == 'yes' || $showCommentLink == 'yes'
	        || $comment_reg == 1 );
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
	    '<a href="'; $siteurl;
		If( $pos_status == 'static');
		    '?page=';
		Else;
		    '?p=';
		/If;
		$postID; '#comments">'; 
		LB_getCommentLink($postID);
	    '</a>';
	Else( $comment_reg == '1');
	str('You have to be registered and logged in to be able to comment');
	/If;
	'</p>';

    '</div>\r'; 	// end post


	var('pos_title' = Field('pos_title'));
    /Records;
    /Inline;
	// show error, if any
	If( $speakBubble);
	MO_speakBubble($speakBubble);
	/If;

// comments module
If( $showCommentModule == 'yes');
    Include( $myinc + 'module_comments.inc');
/If; 

/loop;
?>
