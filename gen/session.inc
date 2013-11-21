<?LassoScript
Session_Start( -Name='blog', -Expires=Var('sessionlength'), -Path='/', -UseCookie);
Session_AddVariable( -Name='blog', 'user_ses');
Session_AddVariable( -Name='blog', 'valid_ses');
Session_AddVariable( -Name='blog', 'userlevel_ses');
Session_AddVariable( -Name='blog', 'logincount_ses');


If( Variable_Defined('user_ses')==false );
  var('user_ses')= string;
/If;
If( Variable_Defined('valid_ses')==false );
  var('valid_ses')= string;
/If;

If( Variable_Defined('userlevel_ses')==false );
  var('userlevel_ses')= string;
/If;

If( Variable_Defined('logincount_ses')==false );
  var('logincount_ses')= Integer;
/If;
?>


