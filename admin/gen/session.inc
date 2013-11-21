<?LassoScript

var('sesname' = 'blogadmin'); // session name

// don't include siteconfig before session

Session_Start( -Name=$sesname, -Expires=2880 );
// 'ses result is '; session_result; '<br>';

    Session_AddVariable( -Name=$sesname, 'userid_ses');
    Session_AddVariable( -Name=$sesname, 'username_ses');
    Session_AddVariable( -Name=$sesname, 'username_human_ses');
    Session_AddVariable( -Name=$sesname, 'valid_ses');
    Session_AddVariable( -Name=$sesname, 'userlevel_ses');
    Session_AddVariable( -Name=$sesname, 'logincount_ses');
    Session_AddVariable( -Name=$sesname, 'searchVars_ses');
    Session_AddVariable( -Name=$sesname, 'searchstuff_ses');

If( Variable_Defined('user_ses')==false );
var('user_ses')='';
/If;
If( Variable_Defined('userid_ses')==false );
var('userid_ses')='';
/If;
If( Variable_Defined('username_ses')==false );
var('username_ses')='';
/If;
If( Variable_Defined('username_human_ses')==false );
var('username_human_ses')='';
/If;
If( Variable_Defined('valid_ses')==false );
var('valid_ses')='';
/If;

If( ! Variable_Defined('userlevel_ses') );
var('userlevel_ses')=integer;
/If;

If( Variable_Defined('logincount_ses')==false );
var('logincount_ses')=Integer;
/If;

If( Variable_Defined('searchVars_ses')==false );
var('searchVars_ses')=array;
/If;

If( Variable_Defined('searchstuff_ses')==false );
var('searchstuff_ses')=array;
/If;
?>


