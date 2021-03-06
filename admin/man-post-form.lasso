<?LassoScript//>
var('fromMetoRoot' = '../');
Include( 'inc/defaults.inc');
var('pageSecuritylevel') = 1;

var('myTable'='posts');

var('ckeditor' = 'enable'); // script in frame.inc takes 2 seconds to load?

var('rectype' = 'post');
var('rectype_pl' = 'posts');
var('rectype_fi' = 'artikkeli');
var('rectype_fi_part' = 'artikkeleita');

If( $lang == 'fi');
    var('page_title'='Posts - form');
    Else;
    var('page_title'='Posts - form');
/If;

// for how to use $searchFields and $editFields, 
// see the beginning of monster.inc

// categories (standard code)
// var('i' = MO_composeValuelist(-table='categories',
//                               -showfield='cat_name',
//                               -sortfield='id'));
// var('cat1_s' = ';' + ($i -> first));
// var('cat2_s' = '--ANY;' + ($i -> second));
// var('cat1' = ($i -> first));
// var('cat2' = ($i -> second));

// categories (implement default_category option - ugly!)
var('default_cat') = integer($opts -> find('default_category')); 
var('cat_temp') = "[If( $emptyform == 'yes')][var('comparisonValue'=$default_cat)][Else][Var('comparisonvalue' = $fieldValue)][/If]
[Inline(-FindAll,$dbConfig,-table=$table_prefix + 'categories',
    -KeyField='id',-SortField='cat_name',-maxrecords='all')]
	<select name=\"pos_category\" size=\"1\">
	    [Records]
              <option value=\"[Field('id')]\" [If( $comparisonValue == 
              field('id'))]SELECTED[/If]>[Field('cat_name')]</option>
	    [/Records]
	</select>
    [/Inline]";

// author
var('user_temp' =
"[var('uid') = ($emptyform ? $userid_ses | $fieldvalue )]
<a href=\"use-form.lasso?id=[$uid]\">[MO_getNameByID(-table='users',-idvalue=$uid,-namefield='use_nickname',-debug='',-fail_gracefully='yes')]</a>
<input type=\"hidden\" name=\"pos_author\" value=\"[$uid]\">
");

// status
If( $userlevel_ses == 1);
    var('stat1')='draft';
    var('stat2')='Draft';
    var('stathelp') = 'Level 1 users can write only drafts';
    Else;
    var('stat1')='publish;draft;private';
    var('stat2')='Publish;Draft;Private';
    var('stathelp') = '';
/If;

// search form
var('searchFields')=Array(
array('id','ID','hidden','','','','','','','','eq'),
array('pos_content','Content','text','','150px','','','','','nobreak','cn'),
);

var('id_temp' = "<span style=\"font-size:0.9em;\">ID = <b>[field('id')]</b>, [str('Created')] <b>[field('pos_date')]</b>, [str('modified')] <b>[field('pos_modified')]</b></span>
");

// edit form

var('editFields')=Array(
array('id','','replacecode','','','','','',$id_temp),
Array('pos_date','Creation date','creationdate','datetime','','','','',''),
Array('pos_modified','Mod date','moddate','datetime','','','','',''),
Array('pos_author','Author','replacecode','','10','','','',$user_temp),
//Array('pos_category','Category','select','req',';'+$cat1,'--SELECT;'+$cat2,'','','','',''),
Array('pos_category','Category','replacecode','req','','','','',$cat_temp,'',''),
Array('pos_title','Title','text','req','','','Permalink version: [Field(\'pos_name\')]','',''),
Array('pos_name','Permalink version','hidden','','','','','',''),
// field 'pos_name' value generation will be handled in custom validation
Array('pos_excerpt','Summary','textarea','','','2','','',''),
Array('pos_content','Content','textarea_ckeditor','req,allowhtml','450px',$opts->find('default_post_edit_rows'),'','',''),
Array('pos_comment_status','Allow Comments','checkbox','','OPEN','','','','','','','OPEN'),
Array('pos_status','Status','select','req',$stat1,$stat2,$stathelp,'',''),
);
// define sorting for the main search inline
var('sortstuff') = Array(
'-sortfield' = 'id',
'-sortorder' = 'descending'
);


var('publicPage'=''); // don't comment out (yet)

var( 'defaultSearch') = Array('-op' = 'neq', 'pos_status' = 'static');	
// userlevel adjustments
    If( $userlevel_ses < 8);
    $defaultSearch -> insert('pos_author'=$userid_ses);	
    /If;

// var('formTargetpage' = '');   // if target=self, comment this out
var('buttonlist' = map('add'='yes','update'='yes','delete'='yes'));
var('customSearchvalidations' = 'no');
var('customValidations'       = 'yes');  // !
var('customPreform'           = 'no'); 
var('customPostform'          = 'yes'); // ! 

var('fileupload' = 'yes');   

include('gen/frame_admin.inc');

?>

