<?LassoScript //>Variable:'fromMetoRoot' = '../';Include:'../config.lasso';Include:'../gen/session.inc';Library:'../inc/library.inc';Variable:'myStylesheet' = 'admin.css';Variable:'myInc' = 'inc/';Variable:'myImages' = 'images/';// init the basic options datatypeVariable:'basicOptions' = LB_basicOptions;Variable:'menu'  = $fromMetoRoot + $myInc + 'adminmenu.lasso';Variable:'footer'= $fromMetoRoot + $myInc + 'adminfooter.inc';// some values here override those in config.lassoVariable:'contents'= $fromMetoRoot + 'gen/adminpage_monster.inc';If: $lang == 'fi';Variable:'page_title'='Staattiset sivut';Else;Variable:'page_title'='Static pages';/If;Variable:'pageSecuritylevel' = 2; Variable:'rectype_en' = 'page';Variable:'rectype_en_part' = 'pages';Variable:'rectype_fi' = 'sivu';Variable:'rectype_fi_part' = 'sivuja';Variable:'viewtype' = 'listview'; // formview, listviewVariable:'maxrecords_formview' = 1;   Variable:'maxrecords_listview' = 25;   Variable:'dbtype' = 'mysql';   // fmp, mysql (vaikuttaa pvm-k�sittelyynVariable:'updatebehavior' = 1;   // 1 or 2 (see below)// behavior #1: search kept, updated rec shown only if search allows this// 		(keyvalue nulled after update)//		(good for listview)// behavior #2: search bypassed, only updated rec shown// 		(keyvalue kept, hakulomake emptied)//		(sopii formview-n�kym��n)Variable:'searchBehavior' = 1;   // 1 or 2 (see below)// searchBehavior #1: if searchform is empty, then find all// searchBehavior #2: if searchform is empty, then find nothingVariable:'action' = '';Variable:'speakBubble' = '';Variable:'keyvalue'=(Action_Param:'keyvalue');Variable:'skip'=(Action_Param:'skip');Variable:'haku'=(Action_Param:'haku');Variable:'size1default'='50'; // default (text: size, textarea: cols)Variable:'size2default'='4';  // default (textarea: rows)Variable:'myTable'='posts';Variable:'publicPage'=''; // don't comment outVariable:'defaultClass'='pieni'; // normaalisti "pieni" // for how to use $searchFields and $editFields, // see the beginning of adminpage_monster.inc// SPECIAL code to be inserted in $searchfields  // and to be processed when showin recsVariable:'user_temp' = '[Inline:-FindAll,$dbConfig,-table=$table_prefix + \'users\',    -KeyField=\'id\',-SortField=\'use_nickname\']	<select name="[\'searchparam\' + $fieldcounter]" size="1">		<option value="">-- ANY 	    [Records]		<option value="[Field:\'id\']"[If: 		 $fieldvalue == (Field:\'id\')] SELECTED[/If]>		 [Field:\'use_nickname\']</option>	    [/Records]	</select>    [/Inline]';Variable:'searchFields'=(Array:// (Array:'pos_date','Created','text','','10','','','date','','nobreak'),// difficulty: how to search with bare date in DATETIME field?(Array:'pos_author','Author','override','','10','','','',$user_temp,'nobreak'),(Array:'pos_title','Title','text','','10','','','','','nobreak','ft'),(Array:'pos_content','Main text','text','','10','','','','','nobreak','ft'),); // SPECIAL code to be inserted in $editFields // to be processed when showin recsVariable:'user_temp' = '    [Inline:-search,$dbConfig, -table=$table_prefix + \'users\',    -KeyField=\'id\',\'id\'=($fieldvalue)]    <A HREF="use-edit.lasso?id=[Field:\'id\']" TITLE="See user data">    [Field:\'use_nickname\']</A>    [/Inline]    <input type="hidden" name="pos_author" value="[$fieldvalue]">';Variable:'editFields'=(Array:(Array:'id','ID','nosave','','10','','','',''),(Array:'pos_date','Created','noentry','','10','','','date',''),(Array:'pos_author','Author','override','','10','','','',$user_temp),(Array:'pos_title','Page Title','override','','10','','','','<A HREF="man-pageedit.lasso?id=[Field:\'id\']" TITLE="Edit the page">[$fieldvalue]</A><input type="hidden" name="pos_title" value="[$fieldvalue]">'),);Variable:'addFields'=$editFields;// define sorting for the main search inlineVariable:'sortstuff' = (Array:'-sortfield' = 'pos_date','-sortorder' = 'descending');// constant values that will be added to each search; pair arrayVariable: 'defaultSearch' = (Array:'pos_status'='static');	// buttonlist: say 'yes' or 'no' to each itemVariable:'buttonlist' = (Array:'add'='no','update'='no','delete'='no');// Variable:'formTargetpage' = '';   // if target=self, comment this outVariable:'customValidations' = 'no'; 	// if yes, the associated include file must be in placeVariable:'customPostprocess' = 'no';		    	// if not needed, comment this outInclude:'../gen/frame_admin.inc';?>