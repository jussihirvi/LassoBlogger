<?LassoScript

// explicit error messages. Use when developing

If: (Error_CurrentError)!=(Error_NoError);

If: (Error_CurrentError: -ErrorCode) == '-1278'
|| ((Error_CurrentError: -ErrorCode) == 0 && (Found_Count) == 0);

// first condition for FMP, second for MySQL

$speakBubble='No records were found.';
Else;
$speakBubble= 'An error occurred. More info: ' +
	(Error_CurrentError:-ErrorCode)+', ' + (Error_CurrentError);
/If;
Include: $fromMetoRoot + $myinc + 'speakBubble.inc';
Else;
/If;
?>
