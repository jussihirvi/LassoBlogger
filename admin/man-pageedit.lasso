<?LassoScript //>Variable:'fromMetoRoot' = '../';Include:'../config.lasso';Include:'../gen/session.inc';Library:'../inc/library.inc';Variable:'myStylesheet' = 'admin.css';Variable:'myInc' = 'inc/';Variable:'myImages' = 'images/';// init the basic options datatypeVariable:'basicOptions' = LB_basicOptions;Variable:'menu'  = $fromMetoRoot + $myInc + 'adminmenu.lasso';Variable:'footer'= $fromMetoRoot + $myInc + 'adminfooter.inc';// some values here override those in config.lassoVariable:'contents'= $fromMetoRoot + 'gen/adminpage_monster.inc';If: $lang == 'fi';Variable:'page_title'='Staattiset sivut';Else;Variable:'page_title'='Static pages';/If;Variable:'pageSecuritylevel' = 2; Variable:'rectype_en' = 'page';Variable:'rectype_en_part' = 'pages';Variable:'rectype_fi' = 'sivu';Variable:'rectype_fi_part' = 'sivuja';Variable:'viewtype' = 'formview'; // formview, listviewVariable:'maxrecords_formview' = 1;   Variable:'maxrecords_listview' = 25;   Variable:'dbtype' = 'mysql';   // fmp, mysql (vaikuttaa pvm-k�sittelyynVariable:'updatebehavior' = 1;   // 1 or 2 (see below)// behavior #1: search kept, updated rec shown only if search allows this// 		(keyvalue nulled after update)//		(good for listview)// behavior #2: search bypassed, only updated rec shown// 		(keyvalue kept, hakulomake emptied)//		(sopii formview-n�kym��n)Variable:'searchBehavior' = 1;   // 1 or 2 (see below)// searchBehavior #1: if searchform is empty, then find all// searchBehavior #2: if searchform is empty, then find nothingVariable:'action' = '';Variable:'speakBubble' = '';Variable:'keyvalue'=(Action_Param:'keyvalue');Variable:'skip'=(Action_Param:'skip');Variable:'haku'=(Action_Param:'haku');Variable:'size1default'='400px'; // default (text: size, textarea: cols)Variable:'size2default'='4';  // default (textarea: rows)Variable:'myTable'='posts';Variable:'publicPage'=''; // don't comment outVariable:'defaultClass'='pieni'; // normaalisti "pieni" // for how to use $searchFields and $editFields, // see the beginning of adminpage_monster.inc// search formVariable:'searchFields'=(Array:(Array:'id','ID','hidden','','','','','','','','eq'),);Variable:'parent_temp' = '[Inline:-Search,$dbConfig,    -table=$table_prefix + \'posts\',    -KeyField=\'id\',\'pos_status\'=\'static\',-Op=\'neq\',\'id\'=(Field:\'id\'),-SortField=\'pos_title\']	<select name="pos_parent" size="1">		<option value=""[If: $fieldValue == \'\'] SELECTED[/If]>		-- None</option>	    [Records]		<option value="[Field:\'id\']"[If: $fieldValue == 		  (Field:\'id\')] SELECTED[/If]>[Field:\'pos_title\']</option>	    [/Records]	</select>    [/Inline]';Variable:'user_temp' = '    [Inline:-search,$dbConfig, -table=$table_prefix + \'users\',    -KeyField=\'id\',\'id\'=($fieldvalue)]    <A HREF="use-edit.lasso?id=[Field:\'id\']" TITLE="See user data">    [Field:\'use_nickname\']</A>    [/Inline]    <input type="hidden" name="pos_author" value="[$fieldvalue]">';// edit formVariable:'editFields'=(Array:(Array:'id','ID','nosave','','','','','',''),(Array:'pos_author','Author','override','','','','','',$user_temp),(Array:'pos_title','Page Title','text','req','','','','',''),(Array:'pos_parent','Page Parent','override','','','','if this is a subpage','',$parent_temp),(Array:'pos_name','Permalink Name','text','unique','200px','','This will be part of permalink, if you include template tag /%postname*/ in your permalink_structure option; use only alphanumeric chars (a-z, A-Z, 0-9) and separate words with dashes (-) or underlines (_), if you like; <b>don\'t</b> use spaces','',''),(Array:'pos_excerpt','Summary','textarea','','','2','','',''),(Array:'pos_content','Content','textarea','req','',(LB_getOption:'default_post_edit_rows'),'','',''),(Array:'pos_comment_status','Allow Comments','checkbox','','open','','','','','',''),(Array:'pos_menu_order','Sorting code','text','','60px','','If you have several pages, links to them will appear sorted according to this field (values are treated as numbers)','','','',''),(Array:'pos_status','Status','hidden','','','','','','static'),(Array:'pos_date','Created','creationDate','','12','','','date',''),(Array:'pos_modified','Modified','modDate','','12','','','date',''),);// add formVariable:'addFields'=(Array:(Array:'id','ID','noentry','','','','','',''),(Array:'pos_author','Author','hidden','','','','','','[Var:\'user_ses\']'),(Array:'pos_title','Title','text','req','','','','',''),(Array:'pos_parent','Page Parent','override','','','','if this will be a subpage','',$parent_temp),(Array:'pos_name','Permalink Name','text','unique','200px','','This will be part of permalink, if you include template tag /%postname*/ in your permalink_structure option; use only alphanumeric chars (a-z, A-Z, 0-9) and separate words with dashes (-) or underlines (_), if you like; <b>don\'t</b> use spaces','',''),(Array:'pos_excerpt','Summary','textarea','','','2','','',''),(Array:'pos_content','Content','textarea','req','',(LB_getOption:'default_post_edit_rows'),'','',''),(Array:'pos_comment_status','Allow Comments','checkbox','','open','','','','open','',''),(Array:'pos_menu_order','Sorting code','text','','60px','','If you have several pages, links to them will appear sorted according to this field (values are treated as numbers)','','','',''),(Array:'pos_status','Status','hidden','','','','','','static'),);// define sorting for the main search inlineVariable:'sortstuff' = (Array:'-sortfield' = 'pos_date','-sortorder' = 'descending');// userlevel adjustmentsIf: $userlevel_ses < 8;Variable: 'defaultSearch' = (Array:'pos_author'=$user_ses);	Else;Variable: 'defaultSearch' = (Array:'pos_status'='static');	/If;//  buttonlist (say 'yes' or 'no' to each item)Variable:'buttonlist' = (Array:'add'='yes','update'='yes','delete'='yes');// Variable:'formTargetpage' = '';   // if target=self, comment this outVariable:'customValidations' = 'no'; 	// if yes, the associated include file must be in placeVariable:'customPostprocess' = 'yes';		    	// if not needed, comment this outInclude:'../gen/frame_admin.inc';?>