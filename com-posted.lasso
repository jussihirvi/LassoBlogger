<?LassoScript //>Variable:'fromMetoRoot' = '';Include:'config.lasso';Library:'inc/library.inc';// get user levelVariable:'userInfo' = LB_Userinfo;Variable:'use_level' = $userInfo -> LB_getUse_level;// init the basic options datatypeVariable:'basicOptions' = LB_basicOptions;    Variable:'page_title'= (str:'Thanks for your comment!');// some values here override those in config.lassoVariable:'contents'='cont/'+ $myfilebody + '.inc';Variable:'pageSecuritylevel' = 1; // available to the users of same level & upVariable:'rectype_en' = 'post';Variable:'rectype_en_part' = 'posts';Variable:'rectype_fi' = 'artikkeli';Variable:'rectype_fi_part' = 'artikkeleita';Variable:'speakBubble' = '';Variable:'myStylesheet' = 'themes/default/style.css';Variable:'header'='inc/header.inc';Variable:'menu'='inc/menu.lasso';Variable:'footer'='inc/footer.inc';Variable:'myInc' = 'inc/';Variable:'myImages' = 'images/';// Variable:'myThemeImages' = 'themes/default/images';Variable:'error' = (Integer);Variable:'errorText' = (String);Include:'gen/frame_default.inc';?>