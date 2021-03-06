<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 3;

var('myTable'='uploads');
var('rectype' = 'file');
var('rectype_pl' = 'files');
var('rectype_fi' = 'tiedosto');
var('rectype_fi_part' = 'tiedostoja');

If( $lang == 'fi');
    var('page_title'='Files - list');
    Else;
    var('page_title'='Files - list');
/If;


// cats 

var('i'=JH_getValuelist(-format='twoarrays',-mytable='uploadcats',-myfield='upc_name',-sortfield='upc_name',-inlinestuff=(array(-op='eq','upc_parent'=''))));
var('cats_field') = ($i -> first  -> join(';'));
var('cats_label') = ($i -> second -> join(';'));

// author
var('auth' =
"[
($emptyform ? $username_ses | MO_getNameByID(-table='users',-idvalue=$fieldvalue,-namefield='use_nickname',-debug=''))  + '<input type=\"hidden\" name=\"upl_userid\" value=\"'+$userid_ses+'\">'
]");


// search form

var('searchFields')=Array(
Array('id','ID','hidden','','','','','','','','eq'),
Array('upl_catid','File Category','select','',';'+$cats_field,'--ANY;'+$cats_label,'','','',''),
Array('upl_name','Filename','text','','15','','','','','nobreak'),
);

var('editlink_temp' = "<a href=\"upl-file-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");
// for thumbnails, username and pw necessary for L8
inline(-nothing,-username=$filetagsusername,-password=$filetagspassword);
var('thumbnail') = "
[var('fn') = field('upl_name') -> split('.')]
[var('fbody') = $fn -> get(1)] 
[var('sx') = $fn -> get(2)]
[var('thumb_sx') = $fn -> get(2)]
[if($sx == 'png')][$thumb_sx = 'jpg'][/if]
[if(file_exists($upload_path+'uploads/'+(field('upload_catid') ? field('upload_catid') | 'post-related/') + 'thumbnails/'+ $fbody + '.' + $thumb_sx) )]
<img src=\"[$upload_path]uploads/[field('upload_catid') ? field('upload_catid') | 'post-related/']thumbnails/[$fbody].[$thumb_sx]\" alt=\"\">
[else] [/if]
";
/inline;

// edit form

var('editFields')=array(
array('id','ID','replacecode','','10','','click to edit','',$editlink_temp),
array('upl_catid','File Category','select','','-1;'+$cats_field,'None (post-related);'+$cats_label,'','','',''),
// array('upl_postid','To Post','noentry','','','','','',$postid_temp),
array('upl_name','Filename','noentry','','','','','',''),
array('upl_customsortcode','Custom sort code','text','','40px','','','',''),
array('upl_userid','Thumbnail','replacecode','','','','','',$thumbnail),
);


// define sorting for the main search inline
var('sortstuff' = Array(
'-sortfield' = 'upl_customsortcode',
'-sortorder' = 'ascending',
'-sortfield' = 'id',
'-sortorder' = 'descending'
));

// constant values that will be added to each search; pair array
var('defaultSearch' = array);	

// buttonlist- say 'yes' or 'no' to each item
var('buttonlist' = map('add'='no','update'='yes','delete'='yes'));

// var('formTargetpage' = '');   // if target=self, comment this out
var('customValidations' = 'no'); // delete validation 
	// if yes, the associated include file must be in place
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'yes');   
var('allowed_file_suffixes' = 'gif jpg png doc pdf ppt');
var('fileupload_maxk' = '10000');

    Include( 'gen/frame_admin.inc');
?>

