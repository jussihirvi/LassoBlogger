<?LassoScript //>
var('fromMetoRoot' = '../');
Include( 'inc/monsterdefaults.inc');

var('myTable'='sitepages');

var('rectype' = 'page');
var('rectype_pl' = 'pages');
var('rectype_fi' = 'sivu');
var('rectype_fi_part' = 'sivuja');

If( $lang == 'fi');
    var('page_title'='Sivut');
    Else;
    var('page_title'='Pages');
/If;

// SPECIAL code to be inserted to editFields & processed when showin recs

//var('files_temp' = "var('catid'=Field('id'))]
//    [Inline(-search,$dbConfig, -table=$table_prefix + 'uploads',
//    -KeyField='id','upl_catid'=Field('id'))]
//    <A HREF=\"upl-fileedit.lasso?upl_catid=[$catid]\">
//    [Found_Count]</A>
//    [/Inline]");

// for category search
Inline(-findall,$dbConfig, -table=$table_prefix + 'sitecats',-sortfield='sortcode');
     var('cats1' = string);
     var('cats2' = string);
     records;
	$cats1 += ';' + field('id');
	$cats2 += ';' + field('catname');
     /records;
	$cats1 -> RemoveLeading(';');
	$cats2 -> RemoveLeading(';');
/inline;

var('editlink_temp' = "<a href=\"sitepages-form.html?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

var('searchFields'=array(
array('ID','ID','hidden','','','','','','','','eq'),
array('catid','Kategoria','select','',';'+$cats1,'--Mikä tahansa;'+$cats2,'','','','nobreak'),
));

// edit form
var('editFields'=array(
array('id','ID','replacecode','','10','','click to edit','',$editlink_temp),
array('creationdate','Luontipvm','creationdate','','','','','datetime',''),
array('moddate','Muokkauspvm','moddate','','','','','datetime',''),
array('menuname','Valikkonimi','text','req','120px','','','',''),
array('menuname_lang2','valikkonimi (en)','text','','120px','','','',''),
array('menuname_lang3','valikkonimi (se)','text','','120px','','','',''),
array('address','Osoite','text','req,unique','120px','','oltava uniikki, esim. <b>edutjahaitat</b>','',''),
array('catid','Kategoria','select','',';'+$cats1,'?;'+$cats2,'','',''),
array('sortcode','Lajittelukoodi (sivujen järj. kategoriassa)','text','','50px','','','',''),
));

// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'catid',
'-sortfield' = 'sortcode',
));

var( 'defaultSearch' = array);	

// buttonlist - say 'yes' or 'no' to each item
var('buttonlist' = map('add'='yes','update'='yes','delete'='yes'));

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

