<?LassoScript //>
var('fromMetoRoot' = '../');
Include( 'inc/monsterdefaults.inc');

var('myTable'='sitecats_admin');
var('rectype' = 'category');
var('rectype_pl' = 'categories');
var('rectype_fi' = 'kategoria');
var('rectype_fi_part' = 'kategorioita');

If( $lang == 'fi');
    var('page_title'='Sivuston p‰‰kategoriat');
Else;
    var('page_title'='Site Main Categories');
/If;

var('searchFields'=Array(
array('ID','ID','hidden','','','','','','','','eq'),
));

// SPECIAL code to be inserted to editFields & processed when showin recs

//var('files_temp') = "[var('catid'=Field('id'))]
//    [Inline(-search,$dbConfig, -table=$table_prefix + 'uploads',
//    -KeyField='id','upl_catid'=(Field('id')))]
//    <A HREF=\"upl-fileedit.lasso?upl_catid=[$catid]\">
//    [Found_Count]</A>
//    [/Inline]";
//
var('categ_temp' = "[Var('myid'=Field('id'))][Inline(-search,$dbConfig, 
     -table=$table_prefix + 'sitecats',
    -Op='eq',
    'parentcat'='',
    -Sortfield='catname')]
	<select name=\"parentcat\" size=\"1\">
		<option value=\"\">-- None</option>
	    [Records]
	    [If( (Field:'id') != $myid)]
		<option value=\"[Field('id')]\"[If( $fieldvalue == 
		  (Field('id')))] SELECTED[/If]>[Field('catname')]</option>
	    [/If]
	    [/Records]
	</select>
[/Inline]
");

// edit form
var('editFields'=Array(
array('id','ID','replacecode','','10','',' ','',''),
array('catname','Kategoria','text','req,unique','120px','','','',''),
array('catname_lang2','Kategoria (en)','text','unique','120px','','','',''),
array('sortcode','lajittelukoodi','text','','50px','','','',''),
// array('parentcat','Yl‰kategoria','addcode','','8','','','',$categ_temp),
));

// define sorting for the main search inline
var('sortstuff' = Array(
'-sortfield' = 'sortcode',
'-sortorder' = 'ascending'
));

// constant values that will be added to each search; pair array
var( 'defaultSearch' = array);	

// buttonlist: say 'yes' or 'no' to each item
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

