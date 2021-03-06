<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 5;

var('myTable'='linkcategories');
var('rectype' = 'link category');
var('rectype_pl' = 'link categories');
var('rectype_fi' = 'linkkikategoria');
var('rectype_fi_part' = 'linkkikategorioita');

If( $lang == 'fi');
    var('page_title'='Linkkikategoriat');
    Else;
    var('page_title'='Link categories');
/If;

var('searchFields'=Array(
Array('ID','ID','hidden','','','','','','','nobreak','eq'),
));

var('editlink_temp' = "<a href=\"man-page-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

// edit form
var('editFields')=Array(
Array('id','ID','replacecode','','','','','[$fieldvalue]',''),
Array('cat_name','Name','text','','100px','','','',''),
Array('auto_toggle','Toggle','select','','Y;N','Yes;No','','',''),
Array('show_images','Images','select','','Y;N','Yes;No','','',''),
Array('show_description','Descr.','select','','Y;N','Yes;No','','',''),
Array('show_rating','Rating','select','','Y;N','Yes;No','','',''),
Array('show_updated','Updated','select','','Y;N','Yes;No','','',''),
Array('sort_order','Sort Field','select','','id;lin_name;lin_url;lin_rating;lin_updated;rand','ID;Link Text;URL;Rating;Mod. Date;Random','','',''),
Array('sort_desc','Sort Order','select','','Y;N','Desc.;Asc.','','',''),
Array('text_before_link','Before','text','','70px','','','',''),
Array('text_after_link','Between','text','','70px','','','',''),
Array('text_after_all','After','text','','70px','','','',''),
Array('list_limit','Limit','text','','70px','','','','')
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
var('customValidations' = 'no'); // delete validation 
	// if yes, the associated include file must be in place
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');

    Include( 'gen/frame_admin.inc');
?>

