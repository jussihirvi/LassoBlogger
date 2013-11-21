<?LassoScript//>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 5;

var('myTable'='categories');
var('rectype' = 'category');
var('rectype_pl' = 'categories');
var('rectype_fi' = 'kategoria');
var('rectype_fi_part' = 'kategorioita');

If( $lang == 'fi');
    var('page_title'='Kategoriat');
    Else;
    var('page_title'='Categories');
/If;
// categories
var('i' = MO_composeValuelist(-table='categories',
                              -showfield='cat_name',
                              -sortfield='id'));
var('cat1' = ($i -> first));
var('cat2' = ($i -> second));

// authors
var('i' = MO_composeValuelist(-table='users',
                              -showfield='use_nickname',
                              -sortfield='use_nickname'));
var('auth1' = ($i -> first));
var('auth2' = ($i -> second));


var('searchFields'=Array(
Array('ID','ID','hidden','','','','','','','nobreak','eq'),
Array('cat_name','Name','text','','100px','','','','','nobreak'),
));

var('posts_temp' = '[var(\'catid\'=Field(\'id\'))]
    [Inline(-search,$dbConfig, -table=$table_prefix + \'posts\',
    -KeyField=\'id\',\'pos_category\'=Field(\'id\'))]
    <A HREF="man-post-list.lasso?pos_category=[$catid]">
    [Found_Count]</A>
    [/Inline]');
var('categ_temp') =
'[Var(\'myid\'=Field(\'id\'))][Inline(-findall,$dbConfig, 
     -table=$table_prefix + \'categories\',
    -Op=\'eq\', \'cat_parent\'=\'\',
    -Sortfield=\'cat_name\')]
	<select name="cat_parent" size="1">
		<option value="">-- None selected</option>
	    [Records]
	    [If( Field(\'id\') != $myid)]
		<option value="[Field(\'id\')]"[If( $fieldvalue == 
		  Field(\'id\'))] SELECTED[/If]>[Field(\'cat_name\')]</option>
	    [/If]
	    [/Records]
	</select>
[/Inline]
';
var('editlink_temp' = "<a href=\"man-page-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

// edit form
var('editFields')=Array(
array('id','ID','noentry','','10','','','',''),
Array('cat_name','Name for Admins','text','req','150px','','','',''),
Array('cat_nicename','Public Name','text','','150px','','','',''),
Array('cat_description','Decription','textarea','','150px','3','','',''),
Array('cat_sortorder','Sorting of Posts','select','','descending;ascending','newest first;oldest first','','',''),
Array('id','Posts','replacecode','','','','','',$posts_temp),
Array('cat_parent','Parent category','replacecode','','8','','','',$categ_temp),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'id',
'-sortorder' = 'descending'
));

// constant values that will be added to each search; pair array
var('defaultSearch' = array);	

// buttonlist- say 'yes' or 'no' to each item
var('buttonlist' = map('add'='yes','update'='yes','delete'='yes'));

// var('formTargetpage' = '');   // if target=self, comment this out
var('customValidations' = 'yes'); // delete validation 
	// if yes, the associated include file must be in place
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');

    Include( 'gen/frame_admin.inc');
?>

