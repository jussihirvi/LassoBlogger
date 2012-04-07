<?LassoScript

If: (Error_CurrentError: -ErrorCode) == '-1278'
 || ((Error_CurrentError: -ErrorCode) == 0 && (Found_Count) == 0);

// first condition for FMP, second for MySQL

$speakBubble='No records were found.';

Else: (Error_CurrentError)!=(Error_NoError);

$speakBubble= 'An error occurred. More info: ' +
	(Error_CurrentError:-ErrorCode)+', ' + (Error_CurrentError);
/If;
If: $speakBubble;
Include: $fromMetoRoot + $myinc + 'speakBubble.inc';
/If;
?>
