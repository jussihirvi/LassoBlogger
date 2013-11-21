<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 1;

var('myTable'='users');
var('rectype' = 'user');
var('rectype_pl' = 'users');
var('rectype_fi' = 'käyttäjä');
var('rectype_fi_part' = 'käyttäjiä');

If( $lang == 'fi');
    var('page_title'='Käyttäjät');
    Else;
    var('page_title'='Users');
/If;

var('searchFields'=Array(
array('ID','ID','hidden','','','','','','','','eq'),
));

// SPECIAL code to be inserted to editFields & processed when showin recs

var('posts_temp') = '
    [var(\'userid\'=Field(\'id\'))]
    [Inline(-search,$dbConfig, -table=$table_prefix + \'posts\',
    -KeyField=\'id\',\'pos_author\'=$userid)]
    [if(found_count)]<A HREF="man-post-list.lasso?pos_author=[$userid]">
    [Found_Count]</A>[else]-[/if]
    [/Inline]';

var('editlink_temp' = "<a href=\"use-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

// edit form
var('editFields')=Array(
Array('id','ID','replacecode','','10','','','',$editlink_temp),
Array('use_nickname','Public Name','noentry','','','','','',''),
Array('use_firstname','Name','noentry','','','','','',''),
Array('use_lastname','','noentry','','','','','',''),
Array('use_level','Level','noentry','','','','','',''),
Array('#posts','Posts','replacecode','','','','','',$posts_temp),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'id',
'-sortorder' = 'ascending'
));

// constant values that will be added to each search; pair array
var('defaultSearch' = array);	

// buttonlist- say 'yes' or 'no' to each item
var('buttonlist' = map('add'='no','update'='no','delete'='no'));

// var('formTargetpage' = '');   // if target=self, comment this out
var('customValidations' = 'no'); // delete validation 
	// if yes, the associated include file must be in place
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');

    Include( 'gen/frame_admin.inc');
?>

