<table>[Search_Arguments]
<TR><TD valign="top"><FONT SIZE=-1><B> [Search_FieldItem]</B> [Search_OperatorItem]: </FONT></TD>
	<TD></TD>
	<TD valign="top"><FONT SIZE=-1>[Search_ValueItem]</FONT></TD>
</TR>[/Search_Arguments]
[Sort_Arguments]<TR><TD valign="top"><FONT SIZE=-1><B>[Sort_FieldItem]</B></FONT></TD>
	<TD></TD>
	<TD valign="top"><FONT SIZE=-1>[Sort_Orderitem]</FONT></TD>
</TR>[/Sort_Arguments]
</TABLE>

<?LassoScript

// explicit error messages. Use when developing

    If: (Error_CurrentError: -ErrorCode) == '-1278'
	|| ((Error_CurrentError: -ErrorCode) == 0 && (Found_Count) == 0);
    // first condition for FMP, second for MySQL

    $speakBubble='No records were found.';
    Include: $fromMetoRoot + $myInc + 'speakBubble.inc';

    Else: (Error_CurrentError)!=(Error_NoError);
    $speakBubble= 'An error occurred. More info: ' +
	    (Error_CurrentError:-ErrorCode)+', ' + (Error_CurrentError);
    Include: $fromMetoRoot + $myInc + 'speakBubble.inc';

    Else;
    'No error<br>';
    /If;
'inlinestuff = ' + (Var:'inlinestuff_readable') + '<br>';
'hakuinlinestuff = ' + (Var:'hakuinlinestuff') + '<br>';
?>
