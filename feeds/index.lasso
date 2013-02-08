<?LassoScript //>
// cases: feed=rss2, feed=comments-rss2

'<?xml version="1.0" encoding="ISO-8859-1"?>';
var('fromMetoRoot') = '../';
Include('../config.lasso');
Include('../inc/library.inc');

  define_tag( 'LB_codeCleanup');
    local( 'output')=(params)->get(1);
    #output = string_replace( -find=' ', -replace=' ', #output);
    #output = string_replace( -find='<br />', -replace='\r', #output);
    #output = string_replace( -find='<i>', -replace='', #output);
    #output = string_replace( -find='</i>', -replace='', #output);
    #output = string_replace( -find='<b>', -replace='', #output);
    #output = string_replace( -find='</b>', -replace='', #output);
//    #output = string_replace( -find='&lt;', -replace='<', #output);
//     #output = string_replace( -find='&gt;', -replace='>', #output);
    #output = string_replace( -find='<!--', -replace='', #output);
    #output = string_replace( -find='-->', -replace='', #output);
    #output = encode_xml( #output);
    #output = string_replace( -find='\r', -replace='<br />', #output);
    return( #output);
  /define_tag;

content_type('text/xml');
?>
<rss version="2.0" 
	xmlns:content="http://purl.org/rss/1.0/modules/content/"
	xmlns:wfw="http://wellformedweb.org/CommentAPI/"
	xmlns:dc="http://purl.org/dc/elements/1.1/"
	>

  <channel>
    <title>[$opts -> find('blogname')][If( Action_Param('feed') == 'comments-rss2')] - Comments[/If]</title>
    <link>[$opts->find('siteurl')]</link>
    <description>[$opts -> find('blogdescription')]</description>
    <language>[If( $lang == 'fi')]fi-fi[Else]en-us[/If]</language>

    <?LassoScript
    If( Action_Param('feed') == 'rss2' );
     Inline(-search,-Database=$myDb,-Table=$table_prefix+'posts',
        'pos_status'='publish',
        -UserName=$dbUsername,
        -PassWord=$dbPassword,
        -SortField='pos_date',
        -SortOrder='descending',
        -MaxRecords=$opts -> find('posts_per_rss'),
        -InlineName='postsearch'
      );
	Records;
	    If( $opts -> find('rss_use_summary') == '1' );
		var('rss_content') = Field('pos_excerpt');
		Else;
		var('rss_content') = Field('pos_content');
	    /If;
	?>
	<item>
	  <title>[LB_codeCleanup(encode_none(field('pos_title')))]</title>
	  <link>[$opts -> find('siteurl')]?p=[Field('id')]</link>
	  <description>[LB_codeCleanup(encode_none($rss_content))]</description>
	  <dc:creator>[LB_getAuthor(Field('pos_author'))]</dc:creator>
	  <dc:date>[LB_formatDatetime(Field('pos_date'))]</dc:date>    
	</item>
	<?LassoScript
	/Records;
	/Inline;

    Else( Action_Param('feed') == 'comments-rss2' );

    Inline(-findall,-Database=$myDb,-Table=$table_prefix+'comments',
	-UserName=$dbUsername,
	-PassWord=$dbPassword,
	'com_approved' = '1',
        -SortField = 'com_date',
	-SortOrder = 'descending',
        -MaxRecords=$opts -> find('posts_per_rss'),
	-InlineName='commentSearch');

	Records(-InlineName='commentSearch');
	?>
	<item>
	  <title>[LB_codeCleanup(Field('com_title'))]</title>
	  <link>[$opts -> find('siteurl')]?p=[Field('com_post_id')]</link>
	  <description>[LB_codeCleanup(Field('com_content'))]</description>
	  <dc:creator>[Field('com_author')]</dc:creator>
	  <dc:date>[LB_formatDatetime(Field('com_date'))]</dc:date>    
	</item>
	<?LassoScript
	/Records;
    /Inline;

    /If; 				// end if feed
    ?>
  </channel>
</rss> 
