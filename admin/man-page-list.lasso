<?LassoScript//>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 2;

var('myTable'='posts');
var('rectype' = 'page');
var('rectype_pl' = 'pages');
var('rectype_fi' = 'sivu');
var('rectype_fi_part' = 'sivuja');

If( $lang == 'fi');
    var('page_title'='Erikoissivut');
    Else;
    var('page_title'='Special pages');
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
// Array('pos_date','Created','text','','10','','','date','','nobreak'),
// difficulty: how to search with bare date in DATETIME field?
Array('pos_author','Author','select','',';'+$auth1,'--'+str('ANY')+';'+$auth2,'','','','nobreak','eq'),
Array('pos_title','Title','text','','50px','','','','','nobreak','ft'),
Array('pos_content','Main text','text','','50px','','','','','nobreak','ft'),
));

var('auth_temp' =
"[
MO_getNameByID(-table='users',-idvalue=$fieldvalue,-namefield='use_nickname',-debug='')
]");
var('cat_temp' =
"[
MO_getNameByID(-table='categories',-idvalue=$fieldvalue,-namefield='cat_name',-debug='')
]");
var('editlink_temp' = "<a href=\"man-page-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

// edit form
var('editFields')=Array(
array('id','ID','replacecode','','10','','click to edit','',$editlink_temp),
Array('pos_date','Created','noentry','','10','','','date',''),
Array('pos_author','Author','replacecode','','10','','','',$auth_temp),
Array('pos_title','Title','noentry','','10','','','',''),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'pos_date',
'-sortorder' = 'descending'
));

// constant values that will be added to each search; pair array
var('defaultSearch' = array('pos_status'='static'));	

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

