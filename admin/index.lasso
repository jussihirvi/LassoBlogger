<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');

var('myTable'='pagecontents');
var('rectype' = 'page');
var('rectype_pl' = 'pages');
var('rectype_fi' = 'sivu');
var('rectype_fi_part' = 'sivuja');

If( $lang == 'fi');
    var('page_title'='Etusivu');
    Else;
    var('page_title'='Front page');
/If;

var('contents'='cont/' + $myfilebody + '.inc');


// for how to use $searchFields and $editFields, 
// see the beginning of adminpage_monster.inc

//	inline( -database=$myDb, -table=$myTable,
//	    -username=$dbUsername,
//	    -password=$dbPassword,
//	    -sortfield='pagename',
//	    -search);
//	    var('pagelist_field' = '');
//	    var('pagelist_show' = '--VALITSE');
//	    records;
//		$pagelist_field += ';' + field('pagename');
//		$pagelist_show  += ';' + field('pagename');
//	    /records;
//	/inline;

// search form
var('searchFields'=Array(
array('ID','ID','hidden','','','','','','',''),
//array('pagename','Sivu','select','',$pagelist_field,$pagelist_show,'','','','nobreak'),
));


// edit form
If( $lang == 'fi');
    var('editFields'=Array(
    array('creation_date','Luontipvm','creationdate','','','','','datetime',''),
    array('mod_date','Muokkauspvm','moddate','','','','','datetime',''),
    array('pagename','Sivun nimi','text','req','','','','','',''),
    array('content_en','Sivun sisältö (en)','textarea','req','','','','','',''),
    array('content_fi','Sivun sisältö (fi)','textarea','req','','','','','',''),
    ));
    
Else; // case lang=en

    var('editFields'=Array(
    array('creation_date','Luontipvm','creationdate','','','','','datetime',''),
    array('mod_date','Muokkauspvm','moddate','','','','','datetime',''),
    array('pagename','Sivun nimi','text','req','','','','','',''),
    array('content_en','Sivun sisältö (en)','textarea','req','','','','','',''),
    array('content_fi','Sivun sisältö (fi)','textarea','req','','','','','',''),
    ));
/If;


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'pagename',
'-sortorder' = 'ascending'
));


var('defaultSearch' = Array);

// var('formTargetpage' = '');   // if target=self, comment this out
var('buttonlist' = map('add'='yes','update'='yes','delete'='yes'));
var('customSearchvalidations' 	= 'no');
var('customValidations' 		= 'no'); 
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');
	include('gen/frame_admin.inc');

?>

