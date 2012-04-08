<div id="dashboard-strip">
<h3 style="margin-top:0px;">[$opts -> find('blogname')]<br>
<span style="font-size:80%;">- Latest Activity</span></h3>


<?LassoScript
'<div id="posts">\n';
'<h3>Posts <a href="man-post-list.lasso" title="More posts...">&raquo;</a></h3>\n';

    var('searchargs') = array;
    var('skip') = integer;
    var('use_level') = (-1);
    var('speakBubble') = '';

    // posts search

     Inline(-search,$dbConfig,-Table=$table_prefix + 'posts',
        -SortField='pos_date',
        -SortOrder='descending',
        -MaxRecords='7',
        $searchArgs,
	-SkipRecords=$skip,
	-ReturnField='id',
	-ReturnField='pos_title',	
        -InlineName='postsearch'
        );
            If( ! Found_Count );
            $speakBubble = 'No records were found';
            Else( Error_CurrentError != Error_NoError );
            $speakBubble = Error_CurrentError;
            Else;
            // $speakBubble = 'No error';
            /If;

//    var('linkstuff') = LB_makeLinkstuff($searchArgs);
//    Include( $myInc + 'snip_naviglinks.inc');
    '<ul>';
    Records(-InlineName='postSearch');
    '<li><a href="man-post-form.lasso?id=' + Field('id') + '">';
    Field('pos_title');
    '</a></li>\r';
    /Records;
    '</ul>';
    /Inline;
'</div>\n'; // end posts
?>



<?LassoScript
'<div id="comments">\n';
'<h3>Comments <a href="man-comm-list.lasso" title="More comments...">&raquo;</a></h3>\n';
    // comments search

     Inline(-search,$dbConfig,-Table=$table_prefix + 'comments',
        -SortField='com_date',
        -SortOrder='descending',
        -MaxRecords='7',
        $searchArgs,
	-SkipRecords=$skip,
	-ReturnField='id',
	-ReturnField='com_title'
	);
            If( ! Found_Count );
            $speakBubble = 'No records were found';
            Else( Error_CurrentError != Error_NoError );
            $speakBubble = Error_CurrentError;
            Else;
            // $speakBubble = 'No error';
            /If;

    '<ul>';
    Records;
    '<li><a href="man-comm-form.lasso?id=' + Field('id') +'">';
	If( Field('com_title') != '');
	    Field('com_title');
	    Else;
	    '(No title)';
	/If;
    '</a></li>\r';
    /Records;
    '</ul>';
    /Inline;
'</div>\n'; // end comments
  '<div><h3>LassoBlogger info</h3>\n';
  '<p>You have LassoBlogger v. <b>'; $lb_version; '</b>.</p>\n';
  '</div>\n';
'</div>\n'; // end dashboard-strip


'<div id="dashboard-iframe">\n';
'<iframe src="http://www.greenspot.fi/LassoBlogger/dashboard_include.lasso"></iframe>\n';
'<p>&nbsp;</p>\n';
'</div>\n';

// old code (problems with proxies in intranet environs)
// var('pagecontent') = Include_URL('http://www.greenspot.fi/LassoBlogger/dashboard_include.lasso');
// $pagecontent;
?>
