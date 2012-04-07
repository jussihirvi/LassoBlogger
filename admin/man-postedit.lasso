<?LassoScript //>Variable:'fromMetoRoot' = '../';Include:'../config.lasso';Include:'../gen/session.inc';Library:'../inc/library.inc';Include: 'fckeditor/fckeditor.lasso';// review fckeditor.lasso, update var fckeditor_basepathVariable:'myStylesheet' = 'admin.css';Variable:'myInc' = 'inc/';Variable:'myImages' = 'images/';// init the basic options datatypeVariable:'basicOptions' = LB_basicOptions;Variable:'menu'  = $fromMetoRoot + $myInc + 'adminmenu.lasso';Variable:'footer'= $fromMetoRoot + $myInc + 'adminfooter.inc';// some values here override those in config.lassoVariable:'contents'= $fromMetoRoot + 'gen/adminpage_monster.inc';If: $lang == 'fi';Variable:'page_title'='Artikkelit';Else;Variable:'page_title'='Posts';/If;Variable:'pageSecuritylevel' = 1; Variable:'rectype_en' = 'post';Variable:'rectype_en_part' = 'posts';Variable:'rectype_fi' = 'artikkeli';Variable:'rectype_fi_part' = 'artikkeleita';Variable:'viewtype' = 'formview'; // formview, listviewVariable:'maxrecords_formview' = 1;   Variable:'maxrecords_listview' = 25;   Variable:'dbtype' = 'mysql';   // fmp, mysql (vaikuttaa pvm-k�sittelyynVariable:'updatebehavior' = 1;   // 1 or 2 (see below)// behavior #1: search kept, updated rec shown only if search allows this// 		(keyvalue nulled after update)//		(good for listview)// behavior #2: search bypassed, only updated rec shown// 		(keyvalue kept, hakulomake emptied)//		(sopii formview-n�kym��n)Variable:'searchBehavior' = 1;   // 1 or 2 (see below)// searchBehavior #1: if searchform is empty, then find all// searchBehavior #2: if searchform is empty, then find nothingVariable:'action' = '';Variable:'speakBubble' = '';Variable:'keyvalue'=(Action_Param:'keyvalue');Variable:'skip'=(Action_Param:'skip');Variable:'haku'=(Action_Param:'haku');Variable:'size1default'='600px'; // default (text: size, textarea: cols)Variable:'size2default'='4';  // default (textarea: rows)Variable:'myTable'='posts';Variable:'publicPage'=''; // don't comment outVariable:'defaultClass'='pieni'; // normaalisti "pieni" // for how to use $searchFields and $editFields, // see the beginning of adminpage_monster.inc// search formVariable:'searchFields'=(Array:(Array:'id','ID','hidden','','','','','','','','eq'),(Array:'pos_content','Content','text','','20','','','','','nobreak','cn'),);// following code throws error "Recursion depth limit (10) exceeded" at (LB_getOption:'default_category') (noticed by Doug Gentry)// trying to go round it (see below)// Variable:'categ_temp' = '[If: $emptyform == \'yes\'][Var:\'comparisonValue\'= (LB_getOption:\'default_category\')][Else][Var:\'comparisonvalue\' = $fieldValue][/If][Inline:-FindAll,$dbConfig,-table=$table_prefix + \'categories\',//     -KeyField=\'id\',-SortField=\'cat_name\']// 	<select name="pos_category" size="1">// 	    [Records]// 		<option value="[Field:\'id\']"[If: $comparisonValue == // 		  (Field:\'id\')] SELECTED[/If]>[Field:\'cat_name\']</option>// 	    [/Records]// 	</select>//     [/Inline]';Variable:'default_cat' = (LB_getOption:'default_category');Variable:'categ_temp' = '[If: $emptyform == \'yes\'][Var:\'comparisonValue\'= ' + $default_cat + '][Else][Var:\'comparisonvalue\' = $fieldValue][/If][Inline:-FindAll,$dbConfig,-table=$table_prefix + \'categories\',    -KeyField=\'id\',-SortField=\'cat_name\']	<select name="pos_category" size="1">	    [Records]		<option value="[Field:\'id\']"[If: $comparisonValue == 		  (Field:\'id\')] SELECTED[/If]>[Field:\'cat_name\']</option>	    [/Records]	</select>    [/Inline]';Variable:'user_temp' = '    [Inline:-search,$dbConfig, -table=$table_prefix + \'users\',    -KeyField=\'id\',\'id\'=($fieldvalue)]    <A HREF="use-edit.lasso?id=[Field:\'id\']" TITLE="See user data">    [Field:\'use_nickname\']</A>    [/Inline]    <input type="hidden" name="pos_author" value="[$fieldvalue]">';    If: $userlevel_ses == 1;	Variable:'pos_status_values'='draft';	Variable:'pos_status_labels'='Draft';	Else;	Variable:'pos_status_values'='publish;draft;private';	Variable:'pos_status_labels'='Publish;Draft;Private';    /If;// edit formVariable:'editFields'=(Array:(Array:'id','ID','nosave','','','','','',''),(Array:'pos_author','Author','override','','','','','',$user_temp),(Array:'pos_category','Category','override','','','','','',$categ_temp),(Array:'pos_title','Title','text','req','','','Permalink version: [Field:\'pos_name\']','',''),// field 'pos_name' value generation will be handled in custom validation(Array:'pos_excerpt','Summary','textarea','','','2','','',''),(Array:'pos_content','Content','textarea_fckeditor','req','',(LB_getOption:'default_post_edit_rows'),'','',''),(Array:'pos_comment_status','Allow Comments','checkbox','','open','','','','','',''),(Array:'pos_status','Status','select','req',$pos_status_values,$pos_status_labels,'','',''),(Array:'pos_date','Created','creationDate','','12','','','date',''),(Array:'pos_modified','Modified','modDate','','12','','','date',''),);// add formVariable:'addFields'=(Array:// (Array:'id','ID','noentry','','','','','',''),(Array:'pos_author','Author','override','','','','','','<input type="hidden" name="pos_author" value="[Var:\'user_ses\']">[Var:\'user_ses\']'),(Array:'pos_category','Category','override','','','','','',$categ_temp),(Array:'pos_title','Title','text','req','','','','',''),// field 'pos_name' value generation will be handled in custom validation(Array:'pos_excerpt','Summary','textarea','','','2','','',''),(Array:'pos_content','Content','textarea','req','',(LB_getOption:'default_post_edit_rows'),'','',''),(Array:'pos_comment_status','Allow Comments','checkbox','','open','','','','open','',''),(Array:'pos_status','Status','select','req',$pos_status_values,$pos_status_labels,'','',''),);// define sorting for the main search inlineVariable:'sortstuff' = (Array:'-sortfield' = 'pos_date','-sortorder' = 'descending');Variable: 'defaultSearch' = (Array:'-Op' = 'neq', 'pos_status' = 'static');	// userlevel adjustments    If: $userlevel_ses < 8;    $defaultSearch -> (Insert:'pos_author'=$user_ses);	    /If;//  buttonlist (say 'yes' or 'no' to each item)Variable:'buttonlist' = (Array:'add'='yes','update'='yes','delete'='yes');// Variable:'formTargetpage' = '';   // if target=self, comment this outVariable:'customValidations' = 'yes'; 	// if yes, the associated include file must be in placeVariable:'customPostprocess' = 'yes';		    Variable:'allowFileupload' = 'yes';		    Include:'../gen/frame_admin.inc';?>