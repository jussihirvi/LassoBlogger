<?LassoScript //>
var('fromMetoRoot' = '../');
Include( 'inc/defaults.inc');
var('pageSecuritylevel') = 3;

var('myTable'='uploads');

var('rectype' = 'file');
var('rectype_pl' = 'files');
var('rectype_fi' = 'tiedosto');
var('rectype_fi_part' = 'tiedostoja');

If( $lang == 'fi');
    var('page_title'='Files - form');
    Else;
    var('page_title'='Files - form');
/If;

// for how to use $searchFields and $editFields, 
// see the beginning of adminpage_monster.inc

// search form

var('searchFields')=Array(
Array('id','ID','hidden','','','','','','','','eq'),
Array('upl_catid','catID','hidden','','','','','','','','eq'),
);

// following code throws error "Recursion depth limit (10) exceeded" at LB_getOption('default_category') (noticed by Doug Gentry)
// trying to go round it (see below)

// var('categ_temp') = '[If( $emptyform == \'yes\')][Var(\'comparisonValue\'= LB_getOption(\'default_category\'))][Else][Var(\'comparisonvalue\' = $fieldValue)][/If][Inline(-FindAll,$dbConfig,-table=$table_prefix + \'categories\',
//     -KeyField=\'id\',-SortField=\'cat_name\')]
// 	<select name="pos_category" size="1">
// 	    [Records]
// 		<option value="[Field(\'id\')]"[If( $comparisonValue == 
// 		  Field(\'id\'))] SELECTED[/If]>[Field(\'cat_name\')]</option>
// 	    [/Records]
// 	</select>
//     [/Inline]';


var('cat_temp') = "
    [If( $emptyform == 'yes')]Post-related
    <input type=\"hidden\" name=\"upl_catid\" value=\"-1\">
    [else]
    [inline(-findall,$dbconfig,-table=$table_prefix + 'uploadcats',
      -keyfield='id',-sortfield='upc_name',-maxrecords='all')]
	<select name=\"upl_catid\" size=\"1\">
		<option value=\"-1\"[If( $fieldvalue == \"-1\")] SELECTED[/If]>
                None (post-related file)</option>
	    [Records]
		<option value=\"[Field('id')]\"[if( integer($fieldvalue) ==
		  Field('id') )] SELECTED[/if]>[field('upc_name')]
                </option>
	    [/Records]
	</select>
    [/Inline]
    [/If]";

// author
var('auth' =
"[
($emptyform ? $username_ses | MO_getNameByID(-table='users',-idvalue=$fieldvalue,-namefield='use_nickname',-debug=''))  + '<input type=\"hidden\" name=\"upl_userid\" value=\"'+$userid_ses+'\">'
]");


var('postid_temp') = "
    [If( Field('upl_catid') == '-1')]
    [Inline(-search,$dbConfig, -table=$table_prefix + 'posts',
    -KeyField='id','id'=$fieldvalue)]
    <A HREF=\"man-postedit.lasso?id=[$fieldvalue]\" TITLE=\"See the post\">
    [Field('pos_title')]</A>, id = 
    <input type=\"text\" name=\"upl_postid\" value=\"[$fieldvalue]\" style=\"width:50px;\">
    [/Inline]
    [Else]
    (not relevant for this file)
    [/If]";

// for file_exists, username and pw necessary for L8
inline(-nothing,-username=$filetagsusername,-password=$filetagspassword);

// this is used twice: 
var('pth' = "
[var('basepath') = ($upload_path+'uploads/'+(integer(field('upl_catid'))>0 ? 'filevault' | 'post-related'))]
[var('imagelogic') = MO_imagelogic(field('upl_path'),field('upl_imagedisplay'))]
[var('subpath') = string]
[if($imagelogic->find('linked_size'))]
  [$subpath = $imagelogic->find('linked_size')]
  [$subpath += '/']
[else($imagelogic->find('display_size'))]
  [$subpath = $imagelogic->find('display_size')]
  [$subpath += '/']
[/if]
[var('linkpath') = $basepath + '/' + $subpath + field('upl_name')]
");

var('id_temp' = "
[process($pth)] 
[var('fn') = field('upl_name') -> split('.')]
[var('fbody') = $fn -> get(1)] 
[var('sx') = $fn -> get(2)]
[var('thumb_sx') = $fn -> get(2)]
[if($sx == 'png')][$thumb_sx = 'jpg'][/if]
[var('thumbpath')= $basepath + '/thumbnails/'+$fbody + '.' + $thumb_sx]
[if(file_exists($thumbpath))]
<a href=\"[$linkpath]\" target=\"_blank\">
<img src=\"[$thumbpath]\" alt=\"\">
</a>
[else][/if]
<span style=\"font-size:0.9em;\"> ID = <b>[field('id')]</b>, [str('Created')] <b>[field('upl_created')]</b>, [str('modified')] <b>[field('upl_modified')]</b></span>
");
/inline;

var('name_temp') = "
[process($pth)] 
<b>Caution:</b> the database entry (modified here) must match the actual filename!<br><A HREF=\"[$linkpath]\" TARGET=\"_blank\">Show the file (in a new window)!</A>.<br>For testing: imagelogic=[$imagelogic]
";

// edit form

var('editFields')=Array(
array('id','','replacecode','','','','','',$id_temp),
Array('upl_catid','File Category','replacecode','','','','','',$cat_temp),
Array('upl_postid','To Post','noentry','','','','','',$postid_temp),
Array('upl_name','Filename','text','req','','',$name_temp,'',''),
 Array('upl_path','Paths','noentry','','','','','',''),
Array('upl_description','Description','text','','','','','',''),
Array('upl_customsortcode','Custom sort code','text','','40px','','','',''),
// Array('upl_userid','Uploaded by','replacecode','','','','','',$auth),
Array('upl_modified','Last modified','moddate','','','','','date',''),
Array('upl_imagedisplay','Image Display','select','','right;left;block;link;none','display: right side of post;display: left side of post;display: center of post;no display: show link to file; no display','Relevant only for image files','',''),
Array('upl_linkurl','Link URI','text','','','','Use this to make a post-related image into a link (typically it would point to a big version of an image). If the value does not begin with "http://" or a slash (/), it is relative to the	path that is <a href="opt-uploads.lasso">set in the option</a> <code>fileupload_path</code> (current value: <b>[$opts->find(\'fileupload_path\')]</b>)','',''),
);
// define sorting for the main search inline
var('sortstuff') = Array(
'-sortfield' = 'id',
'-sortorder' = 'descending'
);


var('publicPage'=''); // don't comment out (yet)

var('defaultSearch' = array);


// var('formTargetpage' = '');   // if target=self, comment this out
var('buttonlist' = map('add'='no','update'='yes','delete'='yes'));
var('customSearchvalidations' 	= 'no');
var('customValidations' 		= 'no'); 
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'yes');   

include('gen/frame_admin.inc');

?>

