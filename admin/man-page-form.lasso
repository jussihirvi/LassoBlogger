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

var('searchFields'=Array(
Array('id','ID','hidden','','','','','','','','eq'),
));

// author
var('user_temp' =
"[var('uid') = ($emptyform ? $userid_ses | $fieldvalue )]
<a href=\"use-form.lasso?id=[$uid]\">[MO_getNameByID(-table='users',-idvalue=$uid,-namefield='use_nickname',-debug='',-fail_gracefully='yes')]</a>
<input type=\"hidden\" name=\"pos_author\" value=\"[$uid]\">
");

// cat
var('cat_temp' =
"[
MO_getNameByID(-table='categories',-idvalue=$fieldvalue,-namefield='cat_name',-debug='')
]");

// parent (if any)
var('parent_temp') = '[Inline(-Search,$dbConfig,
    -table=$table_prefix + \'posts\',
    -KeyField=\'id\',\'pos_status\'=\'static\',-Op=\'neq\',\'id\'=Field(\'id\'),-SortField=\'pos_title\')]
	<select name="pos_parent" size="1">
		<option value=""[If( $fieldValue == \'\')] SELECTED[/If]>
		-- None</option>
	    [Records]
		<option value="[Field(\'id\')]"[If( $fieldValue == 
		  Field(\'id\') )] SELECTED[/If]>[Field(\'pos_title\')]</option>
	    [/Records]
	</select>
    [/Inline]';

var('status') = 'Static page <input type="hidden" name="pos_status" value="static">\n';

var('id_temp' = "<span style=\"font-size:0.9em;\">ID = <b>[field('id')]</b>, [str('Created')] <b>[field('pos_date')]</b>, [str('modified')] <b>[field('pos_modified')]</b></span>
");

// edit form
var('editFields')=Array(
array('id','','replacecode','','10','','','',$id_temp),
Array('pos_date','Creation date','creationdate','datetime','','','','',''),
Array('pos_modified','Mod date','moddate','datetime','','','','',''),
Array('pos_author','Author','replacecode','','10','','','',$user_temp),
Array('pos_title','Title','text','req','','','','',''),
Array('pos_parent','Page Parent','replacecode','','','','if this is a subpage','',$parent_temp),
Array('pos_name','Permalink Name','text','unique','200px','','This will be part of permalink, if you include template tag /%postname*/ in your permalink_structure option; use only alphanumeric chars (a-z, A-Z, 0-9) and separate words with dashes (-) or underlines (_), if you like; <b>don\'t</b> use spaces','',''),
Array('pos_excerpt','Summary','textarea','','','2','','',''),
Array('pos_content','Content','textarea','req','',$opts->find('default_post_edit_rows'),'','',''),
Array('pos_comment_status','Allow Comments','checkbox','','open','','','','','',''),
Array('pos_menu_order','Sorting code','text','int','60px','','If you have several pages, links to them will appear sorted according to this field (values are treated as numbers)','','','',''),
Array('pos_status','Status','replacecode','','','','','',$status,'','',''),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'pos_date',
'-sortorder' = 'descending'
));

// constant values that will be added to each search; pair array
var('defaultSearch' = array('pos_status'='static'));	
If( $userlevel_ses < 8);
  $defaultSearch -> insert('pos_author'=$user_ses);	
/If;

// buttonlist- say 'yes' or 'no' to each item
var('buttonlist' = map('add'='yes','update'='yes','delete'='yes'));

// var('formTargetpage' = '');   // if target=self, comment this out
var('customValidations' = 'no'); // delete validation 
	// if yes, the associated include file must be in place
var('customPreform' = 'yes'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');

    Include( 'gen/frame_admin.inc');
?>

