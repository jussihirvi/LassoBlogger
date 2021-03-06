<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 8;

var('myTable'='options');
var('rectype' = 'option');
var('rectype_pl' = 'options');
var('rectype_fi' = 'asetus');
var('rectype_fi_part' = 'asetuksia');

If( $lang == 'fi');
    var('page_title'='Asetukset');
    Else;
    var('page_title'='Options');
/If;

var('searchFields'=Array(
array('ID','ID','hidden','','','','','','','','eq'),
));

// SPECIAL code to be inserted to editFields & processed when showin recs

// edit form
var('editFields')=Array(
Array('id','ID','noentry','','10','','','',''),
Array('opt_name','Name','noentry','','','','','',''),
Array('opt_value','Value','text','','150px','','','',''),
Array('opt_description','Description','noentry','','','','','',''),
Array('opt_status','Status','noentry','','','','','',''),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'id',
'-sortorder' = 'ascending'
));

// constant values that will be added to each search; pair array
var( 'defaultSearch') = array('opt_group'='perm');	

// buttonlist- say 'yes' or 'no' to each item
var('buttonlist' = map('add'='no','update'='yes','delete'='no'));

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

