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
array('com_approved','Status','radio','',';1;0;spam','Any;Approved;Waiting for moderation;Spam','','','','nobreak'),
));

var('auth_temp' =
"<a href=\"use-form.lasso?id=[$fieldvalue]\">[
MO_getNameByID(-table='users',-idvalue=$fieldvalue,-namefield='use_nickname',-debug='',-fail_gracefully='yes')
]</a>");

// post
var('post_temp' = 
'<A HREF="man-post-form.lasso?id=[$fieldvalue]" TITLE="See the post">' +  
"[MO_getNameByID(-table='posts',-idvalue=$fieldvalue,-namefield='pos_title',-debug='')]</a>"
);

var('editlink_temp' = "<a href=\"man-comm-form.lasso?id=[$fieldvalue]\" TITLE=\"Edit this item\">[$fieldvalue]</a>
");

// edit form
var('editFields')=Array(
array('id','ID','replacecode','','10','','click to edit','',$editlink_temp),
Array('com_date','Created','noentry','date','','','','',''),
Array('com_approved','Status','replacecode','','','','','','[if($fieldvalue==\'1\')]Appr.[else($fieldvalue==\'0\')]Non-mod.[else($fieldvalue==\'spam\')]Spam[/if]'),
// Array('com_user_id','Author','replacecode','','','','','',$auth_temp),
Array('com_author','Author','noentry','','','','','',''),
// Array('com_author_ip','IP','noentry','','','','','',''),
Array('com_post_id','To Post (or page)','replacecode','','','','','',$post_temp),
Array('com_title','Comment Title','noentry','','','','','',''),
Array('com_content','Excerpt','replacecode','','','','','','<A HREF="man-comm-form.lasso?id=[Field(\'id\')]">[If( $fieldvalue -> Size > 60)][encode_html(Var(\'fieldvalue\') -> SubString(1,40))]...[Else][encode_html(var(\'fieldvalue\'))][/If]</A>'),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'id',
'-sortorder' = 'descending'
));

// constant values that will be added to each search; pair array
var('defaultSearch' = array);	

// buttonlist- say 'yes' or 'no' to each item
var('buttonlist' = map('add'='no','update'='no','delete'='no','quickdelete'='yes'));

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

