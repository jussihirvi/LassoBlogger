<?LassoScript

// comments module

// var postID comes from inc/module_posts.inc

inline(-search,$dbConfig,
    -table=$table_prefix+'comments',
    'com_post_id' = $postID,
    'com_approved' = '1',
    -sortField = 'com_date',
    -maxRecords='all',
    -inlineName='commentSearch');

    records;
	if( (Loop_Count) == 1 );
	    '<h3 id="comments">' + Found_Count + ' ';
		    if( found_count == 1 );
			str('comment');
                    else( found_count > 1 );
			str('comments ');
		    /if;
                    ' ';  
                    if( $case == 'p' || $case == 'perma'); 
                        str('to the post'); 
                    else( $case == 'page'); 
                        str('to the page'); 
                    /if;
                    ' &#8220;' + $pos_title + '&#8221;';
	    '</h3>'; 	
	    '<ol class="commentlist">';
	/if;

// show the comment

'<li class="comment" id="comment-'; loop_count; '">';
'<span class="commentauthor">';
    if( field('com_author_url') );
	'<a href="http://' + field('com_author_url');
	'" TITLE="'; str('See www site'); '"';
	'" rel="external nofollow">';
	field('com_author') + '</a>';
    else;
	field('com_author');
    /if;
'</span> '; str('says'); 

'<span class="commentmetadata" style="font-size:0.9em;">';
    ' <a href="#comment-1" title="">';
	'('; 
	    LB_formatDateTime(Field('com_date')); 
	')';
    '</a>';
'</span>';
':<br />\r';

if( field('com_title') );
    '<div class="commenttitle">'; 
	field('com_title'); 
    '</div>';
/if;

var('content') = field('com_content');
	$content -> replace('\n','\r');
	$content -> replace('\r\r','</p><p>');
	$content -> replace('\r','<br>');
	$content -> replace('<p>','<p>\r'); // for neater html source
'<div class="commentcontent"><p>'; encode_smart($content); '</p></div>';
'</li>';
    
// end showing comment

	If( loop_count == found_count );
	'</ol>\r';
	/If;
    /records;
/inline;

	// show error, if any
        MO_speakBubble($speakBubble);

// leave a comment
// first, check cookie and search db for user data
// and compute the encrypted string containing the date

var('hash') = LB_createCookiehash;
var('com_author') = string;
var('com_author_email') = string;
var('com_author_url') = string;
if( cookie('comment_author_' + $hash) );
    $com_author = cookie('comment_author_' + $hash);
/if;
if( cookie('comment_author_email_' + $hash) );
    $com_author_email = cookie('comment_author_email_' + $hash);
/if;
if( cookie('comment_author_url_' + $hash) );
    $com_author_url = cookie('comment_author_url_' + $hash);
/if;
var('temp')=(string(date->format('%q'))) + math_random(-lower=1000,-upper=9999);
var('spamcontrol') = encode_base64(encrypt_blowfish($temp, -seed=$hash));

if( ! variable_defined('com_title'));   var('com_title')   = string; /if;
if( ! variable_defined('com_content')); var('com_content') = string; /if;

// erase input for accepted/moderated comments
if($com_post_id); // comment submitted
  if($result != 'discard');
    $com_title   = string;
    $com_content = string;
  /if;
/if;
?>

<h3 id="respond">[str('Leave a Comment')]</h3>
<form action="[$opts->find('siteurl')]index.lasso" method="post" id="commentform">
<input type="hidden" name="oldcase" value="[$case]">
<input type="hidden" name="com_post_ID" value="[$postID]">
<input type="hidden" name="spamcontrol" value="[$spamcontrol]">

<p>
<input type="text" name="com_author" id="author" value="[$com_author]" size="22" tabindex="1">
<label for="author"><small>[str('Name')]
[If( $opts -> find('require_name_email') == '1')]([str('required')])[/If]
</small></label>
</p>

<p>
<input type="text" name="com_author_email" id="com_author_email" value="[$com_author_email]" size="22" tabindex="2">
<label for="com_author_email"><small>[str('Email (will not be published)')]
[If( $opts -> find('require_name_email') == '1')]([str('required')])[/If]
</small></label>
</p>

<p>
<input type="text" name="com_author_url" id="com_author_url" value="[$com_author_url]" size="22" tabindex="3">
<label for="com_author_url"><small>[str('Website')]</small></label></p>

<p>
<input type="text" name="com_title" id="com_title" value="[$com_title]" size="44" tabindex="3">
<label for="com_title"><small>[str('Comment Title')]</small></label></p>

<!--<p><small><strong>XHTML:</strong> You can use these tags: &lt;a href=&quot;&quot; title=&quot;&quot;&gt; &lt;abbr title=&quot;&quot;&gt; &lt;acronym title=&quot;&quot;&gt; &lt;b&gt; &lt;blockquote cite=&quot;&quot;&gt; &lt;code&gt; &lt;em&gt; &lt;i&gt; &lt;strike&gt; &lt;strong&gt; </small></p>-->

<p><textarea name="com_content" id="com_content" cols="60" rows="10" tabindex="4">[$com_content]</textarea></p>

<p><input name="submit" type="submit" id="submit" tabindex="5" value="[str('Submit Comment')]">
</p>

</form>
