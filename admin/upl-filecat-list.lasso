<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 5;

var('myTable'='uploadcats');
var('rectype' = 'category');
var('rectype_pl' = 'categories');
var('rectype_fi' = 'kategoria');
var('rectype_fi_part' = 'kategorioita');

If( $lang == 'fi');
    var('page_title'='Tiedostokategoriat');
    Else;
    var('page_title'='File Categories');
/If;

var('searchFields'=Array(
array('ID','ID','hidden','','','','','','','','eq'),
));

// SPECIAL code to be inserted to editFields & processed when showin recs

// link
var('files_temp') = "[var('catid'=Field('id'))]
    [Inline(-search,$dbConfig, -table=$table_prefix + 'uploads',
    -KeyField='id','upl_catid'=Field('id'))]
    <A HREF=\"upl-files-form.lasso?upl_catid=[$catid]\">
    [Found_Count]</A>
    [/Inline]";

var('categ_temp') = "[Var('myid'=Field('id'))][Inline(-findall,$dbConfig, 
     -table=$table_prefix + 'uploadcats',
    -Op='eq',
    'upc_parent'='',
    -Sortfield='upc_name')]
	<select name=\"upc_parent\" size=\"1\">
		<option value=\"\">-- None</option>
	    [Records]
	    [If( Field('id') != $myid)]
		<option value=\"[Field('id')]\"[If( $fieldvalue == 
		  Field('id'))] SELECTED[/If]>[Field('upc_name')]</option>
	    [/If]
	    [/Records]
	</select>
[/Inline]
";

// edit form
var('editFields')=Array(
Array('id','ID','noentry','','10','',' ','',''),
Array('upc_name','Private Name','textarea','req','100px','3','also sorting key','',''),
Array('upc_nicename','Public Name','textarea','','100px','3','','',''),
Array('upc_description','Decription','textarea','','150px','3','','',''),
Array('upc_sortorder','Sorting of Files','select','','newestfirst;oldestfirst;alphabetical;custom','Newest first;Oldest first;Alphabetical;Custom','','',''),
Array('id','Nr. of Files','replacecode','','','','','',$files_temp),
Array('upc_parent','Parent category','replacecode','','8','','','',$categ_temp),
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

