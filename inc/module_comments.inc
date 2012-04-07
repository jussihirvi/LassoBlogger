<?LassoScript

// comments module

// var postID comes from inc/module_posts.inc

Inline:-search,$dbConfig,
    -Table=$table_prefix+'comments',
    'com_post_id' = $postID,
    'com_approved' = '1',
    -SortField = 'com_date',
    -MaxRecords='all',
    -InlineName='commentSearch';

    Records;
	If: (Loop_Count) == 1;
	    '<h3 id="comments">' + (Found_Count) + ' ';
		    If: (Found_Count) == 1;
			(str:'comment');
			Else: (Found_Count) > 1;
			(str:'comments');
		    /If;
	    ' ';  
	    If: $case == 'p'; 
		str:'to the post'; 
		Else; // page, not post
		str:'to the page'; 
	    /If;
	    ' &#8220;' + $pos_title + '&#8221;';
	    '</h3>'; 	
	    '<ol class="commentlist">';
	/If;

// show the comment

'<li class="comment" id="comment-'; loop_count; '">';
'<span class="commentauthor">';
    If: (Field:'com_author_url');
	'<a href="http://' + (Field:'com_author_url');
	'" TITLE="'; str:'See www site'; '"';
	'" rel="external nofollow">';
	(Field:'com_author') + '</a>';
    Else;
	(Field:'com_author');
    /If;
'</span> '; str:'says'; 

'<span class="commentmetadata" style="font-size:0.9em;">';
    ' <a href="#comment-1" title="">';
	'('; 
	    LB_formatDateTime:(Field:'com_date'); 
	')';
    '</a>';
'</span>';
':<br />\r';

if: (field:'com_title');
    '<div class="commenttitle">'; 
	(Field:'com_title',-encodehtml); 
    '</div>';
/if;

Variable:'content' = (Field:'com_content');
	$content -> (Replace:'\n','\r');
	$content -> (Replace:'\r\r','</p><p>');
	$content -> (Replace:'\r','<br>');
	$content -> (Replace:'<p>','<p>\r'); // for neater html source
'<div class="commentcontent"><p>'; Var:'content',-encodeSmart; '</p></div>';
'</li>';
    
// end showing comment

	If: (Loop_Count) == (Found_Count);
	'</ol>\r';
	/If;
    /Records;
/Inline;

	// show error, if any
	If: $speakBubble != '';
	Include: $myInc + 'speakBubble.inc';
	/If;

// leave a comment
// first, check cookie and search db for user data
// and compute the encrypted string containing the date

Variable:'hash' = (LB_createCookiehash);
If: (Cookie:'comment_author_' + $hash);
    Variable:'com_author' = (Cookie:('comment_author_' + $hash));
    Else;
    Variable:'com_author' = '';
/If;
If: (Cookie:'comment_author_email_' + $hash);
    Variable:'com_author_email' = (Cookie:('comment_author_email_' + $hash));
    Else;
    Variable:'com_author_email' = '';
/If;
If: (Cookie:'comment_author_url_' + $hash);
    Variable:'com_author_url' = (Cookie:('comment_author_url_' + $hash));
    Else;
    Variable:'com_author_url' = '';
/If;
var:'temp'=(date->format('%q')) + (math_random:-lower=1000,-upper=9999);
var:'spamcontrol' = encode_base64(encrypt_blowfish2($temp, -seed=$hash));
'<!-- ';
// test area
'-->';
?>

<h3 id="respond">[str:'Leave a Comment']</h3>

<form action="com-posted.lasso" method="post" id="commentform">
<input type="hidden" name="action" value="add">
<input type="hidden" name="case" value="[$case]">
<input type="hidden" name="com_post_ID" value="[$postID]">
<input type="hidden" name="spamcontrol" value="[$spamcontrol]">

<p>
<input type="text" name="com_author" id="author" value="[$com_author]" size="22" tabindex="1">
<label for="author"><small>[str:'Name']
[If: (LB_getOption:'require_name_email') == 1]([str:'required'])[/If]
</small></label>
</p>

<p>
<input type="text" name="com_author_email" id="com_author_email" value="[$com_author_email]" size="22" tabindex="2">
<label for="com_author_email"><small>[str:'Email (will not be published)']
[If: (LB_getOption:'require_name_email') == 1]([str:'required'])[/If]
</small></label>
</p>

<p>
<input type="text" name="com_author_url" id="com_author_url" value="[$com_author_url]" size="22" tabindex="3">
<label for="com_author_url"><small>[str:'Website']</small></label></p>

<p>
<input type="text" name="com_title" id="com_title" value="" size="44" tabindex="3">
<label for="com_title"><small>[str:'Comment Title']</small></label></p>

<!--<p><small><strong>XHTML:</strong> You can use these tags: &lt;a href=&quot;&quot; title=&quot;&quot;&gt; &lt;abbr title=&quot;&quot;&gt; &lt;acronym title=&quot;&quot;&gt; &lt;b&gt; &lt;blockquote cite=&quot;&quot;&gt; &lt;code&gt; &lt;em&gt; &lt;i&gt; &lt;strike&gt; &lt;strong&gt; </small></p>-->

<p><textarea name="com_content" id="com_content" rows="10" tabindex="4" style="width:400px;"></textarea></p>

<p><input name="submit" type="submit" id="submit" tabindex="5" value="[str:'Submit Comment']">
</p>

</form>
