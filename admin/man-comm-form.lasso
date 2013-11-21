<?LassoScript//>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 4;

var('myTable'='comments');
var('rectype' = 'comment');
var('rectype_pl' = 'comments');
var('rectype_fi' = 'comment');
var('rectype_fi_part' = 'comments');

If( $lang == 'fi');
    var('page_title'='Comments');
    Else;
    var('page_title'='Comments');
/If;
// x categories
var('i' = MO_composeValuelist(-table='categories',
                              -showfield='cat_name',
                              -sortfield='id'));
var('cat1' = ($i -> first ));
var('cat2' = ($i -> second));

// authors
var('i' = MO_composeValuelist(-table='users',
                              -showfield='use_nickname',
                              -sortfield='use_nickname'));
var('auth1' = ($i -> first));
var('auth2' = ($i -> second));


var('searchFields'=Array(
array('id','ID','hidden','','','','','','','','eq'),
array('com_approved','Status','radio','',';1;0;spam','Any;Approved;Waiting for moderation;Spam','','','','nobreak'),
));

var('auth_temp' =
"<a href=\"use-form.lasso?id=[$fieldvalue]\">[
MO_getNameByID(-table='users',-idvalue=$fieldvalue,-namefield='use_nickname',-debug='',-fail_gracefully='yes')
]</a><input type=\"hidden\" name=\"com_user_id\" value=\"[$fieldvalue]\">");

// post
var('post_temp' = 
'<A HREF="man-post-form.lasso?id=[$fieldvalue]" TITLE="See the post">' +  
"[MO_getNameByID(-table='posts',-idvalue=$fieldvalue,-namefield='pos_title',-debug='')]</a><input type=\"hidden\" name=\"com_post_id\" value=\"[$fieldvalue]\">"
);


var('id_temp' = "<span style=\"font-size:0.9em;\">ID = <b>[field('id')]</b>, [str('Created')] <b>[field('com_date')]</b>, [str('modified')] <b>[field('com_modified')]</b></span>
");
// edit form
var('editFields')=Array(
array('id','ID','replacecode','','','','','',$id_temp),
Array('com_date','Created','creationdate','datetime','','','','',''),
Array('com_user_id','Author','hidden','','','','','',$auth_temp),
Array('com_author','Author','noentry','','','','','',''),
Array('com_post_id','To Post (or page)','replacecode','','','','','',$post_temp),
Array('com_author_ip','IP','noentry','','','','','',''),
Array('com_author_email','Email','text','','','','','',''),
Array('com_author_url','URL','replacecode','','','','','','<input type="text" style="width:[$size1default];" name="com_author_url" value="[$fieldvalue]"><A HREF="http://[$fieldvalue]" TITLE="Try the URL">Go!</A>'),
Array('com_title','Comment Title','text','','','','','',''),
Array('com_content','Comment','textarea','','','','','',''),
Array('com_approved','Status','radio','','1;0;spam','Approved;Waiting for moderation;Spam','','',''),
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

