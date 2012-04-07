<?LassoScript //>Variable:'fromMetoRoot' = '../';Include:'../config.lasso';Include:'../gen/session.inc';Library:'../inc/library.inc';Variable:'myStylesheet' = 'admin.css';Variable:'myInc' = 'inc/';Variable:'myImages' = 'images/';// init the basic options datatypeVariable:'basicOptions' = LB_basicOptions;Variable:'menu'  = $fromMetoRoot + $myInc + 'adminmenu.lasso';Variable:'footer'= $fromMetoRoot + $myInc + 'adminfooter.inc';// some values here override those in config.lassoVariable:'contents'= $fromMetoRoot + 'gen/adminpage_monster.inc';If: $lang == 'fi';Variable:'page_title'='Kommentit';Else;Variable:'page_title'='Comments';/If;Variable:'pageSecuritylevel' = 2; Variable:'rectype_en' = 'comment';Variable:'rectype_en_part' = 'comments';Variable:'rectype_fi' = 'kommentti';Variable:'rectype_fi_part' = 'kommentteja';Variable:'viewtype' = 'listview'; // formview, listviewVariable:'maxrecords_formview' = 1;   Variable:'maxrecords_listview' = 25;   Variable:'dbtype' = 'mysql';   // fmp, mysql (vaikuttaa pvm-k�sittelyynVariable:'updatebehavior' = 1;   // 1 or 2 (see below)// behavior #1: search kept, updated rec shown only if search allows this// 		(keyvalue nulled after update)//		(good for listview)// behavior #2: search bypassed, only updated rec shown// 		(keyvalue kept, hakulomake emptied)//		(sopii formview-n�kym��n)Variable:'searchBehavior' = 1;   // 1 or 2 (see below)// searchBehavior #1: if searchform is empty, then find all// searchBehavior #2: if searchform is empty, then find nothingVariable:'action' = '';Variable:'speakBubble' = '';Variable:'keyvalue'=(Action_Param:'keyvalue');Variable:'skip'=(Action_Param:'skip');Variable:'haku'=(Action_Param:'haku');Variable:'size1default'='50'; // default (text: size, textarea: cols)Variable:'size2default'='4';  // default (textarea: rows)Variable:'myTable'='comments';Variable:'publicPage'=''; // don't comment outVariable:'defaultClass'='pieni'; // normaalisti "pieni" // for how to use $searchFields and $editFields, // see the beginning of adminpage_monster.inc// search formVariable:'searchFields'=(Array:(Array:'com_approved','Status: ','radio','',';1;0;spam','Any;Approved;Waiting for moderation;Spam','','','','nobreak'),);Variable:'temp_postid' = '    [Inline:-search,$dbConfig, -table=$table_prefix + \'posts\',    -KeyField=\'id\',\'id\'=$fieldvalue]    [If: (Field:\'pos_status\') == \'static\'][Var:\'pagename\'=\'man-pageedit.lasso\'][Else][Var:\'pagename\'=\'man-postedit.lasso\'][/If]    <A HREF="[$pagename]?id=[$fieldvalue]" TITLE="See the post">    [Field:\'pos_title\']</A>    [/Inline]';Variable:'user_temp' = '[If: $fieldvalue != \'0\']    [Inline:-search,$dbConfig, -table=$table_prefix + \'users\',    -KeyField=\'id\',\'id\'=($fieldvalue)]    [Variable:\'myName\' = (Field:\'use_nickname\')]    [/Inline]<A HREF="use-edit.lasso?id=[$fieldvalue]" TITLE="See the author">[$myName]</A>[Else][Field:\'com_author\'][/If]';// edit formVariable:'editFields'=(Array:(Array:'id','ID','nosave','','','','','',''),(Array:'com_approved','Status','override','','','','','','[if:$fieldvalue==\'1\']Appr.[else:$fieldvalue==\'0\']Non-mod.[else:$fieldvalue==\'spam\']Spam[/if]'),(Array:'com_user_id','Author','nosave','','','','','',$user_temp),(Array:'com_author_ip','IP','nosave','','','','','',''),(Array:'com_post_id','To Post (or page)','nosave','','','','','',$temp_postid),(Array:'com_title','Comment Title','nosave','','','','','',''),(Array:'com_content','Excerpt','nosave','','','','','','<A HREF="man-commedit.lasso?id=[Field:\'id\']">[If: $fieldvalue -> (Size) > 60][(Var:\'fieldvalue\',-encodeHTML) -> (SubString:1,40)]...[Else][(Var:\'fieldvalue\',-encodeHTML)][/If]</A>'),);// add form Variable:'addFields'=(Array:(Array:'id','ID','noentry','','10','','','',''),(Array:'com_post_id','To Post','text','','','','','',''),(Array:'com_author','Author','text','req','10','','','',''),(Array:'com_author_email','Email','text','','10','','','',''),(Array:'com_ip','IP','nosave','','10','','','',''),(Array:'com_content','Comment','text','','10','','','',''),);// define sorting for the main search inlineVariable:'sortstuff' = (Array:'-sortfield' = 'id','-sortorder' = 'descending');// constant values that will be added to each search; pair arrayVariable: 'defaultSearch' = (Array);	// buttonlist: say 'yes' or 'no' to each itemVariable:'buttonlist' = (Array:'add'='no','update'='no','delete'='no','quickdelete'='yes');// Variable:'formTargetpage' = '';   // if target=self, comment this outVariable:'customValidations' = 'no'; 	// if yes, the associated include file must be in placeVariable:'customPostprocess' = 'no';		    	// if not needed, comment this outInclude:'../gen/frame_admin.inc';?>