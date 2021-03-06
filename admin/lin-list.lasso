<?LassoScript//>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 5;

var('myTable'='links');
var('rectype' = 'link');
var('rectype_pl' = 'link');
var('rectype_fi' = 'linkki');
var('rectype_fi_part' = 'linkkej�');

If( $lang == 'fi');
    var('page_title'='Linkit');
    Else;
    var('page_title'='Links');
/If;

var('searchFields'=Array(
Array('ID','ID','hidden','','','','','','','nobreak','eq'),
));

// author
var('auth' =
"<a href=\"use-form.lasso?id=[$fieldvalue]\">[
($emptyform ? $username_ses | MO_getNameByID(-table='users',-idvalue=$fieldvalue,-namefield='use_nickname',-debug='',-fail_gracefully='yes'))
]</a>");

// categories
var('i' = MO_composeValuelist(-table='linkcategories',
                              -showfield='cat_name',
                              -sortfield='id'));
var('cat1' = ($i -> first));
var('cat2' = ($i -> second));

var('editlink_temp' = "<a href=\"man-page-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

// edit form
var('editFields')=Array(
Array('id','ID','replacecode','','','','','[$fieldvalue]',''),
Array('lin_name','Link Text','text','req','100px','','','',''),
// SPECIAL: link to target
Array('lin_url','Link URL','text','req','100px','','<A HREF="[$fieldvalue]" TARG
ET="_blank" TITLE="Visit [Field(\'lin_name\')]">Go!</A>','',''),
Array('lin_description','Description','text','','100px','','','',''),
Array('lin_category','Link Category','select','',$cat1,$cat2,'','',''),
Array('lin_rating','Rating','text','int','20px','','','',''),
Array('lin_owner','Owner','replacecode','','','','','',$auth),
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

