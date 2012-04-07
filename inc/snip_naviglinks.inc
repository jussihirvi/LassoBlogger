<?LassoScript
// old code, sorry
// you are free to modernize it...

// Tätä tiedostoa varten on oltava määriteltyinä muuttujat skip, naviglinks,lang

'<div STYLE="text-align:center;padding-top:10px;">';

// don't show, if action=add

If: !(Variable_Defined:'showNaviglinks');
Variable:'showNaviglinks' = '';
    If: (Variable_Defined:'action');
	If: $action == 'add_action';
	$showNaviglinks = 'no';
	/If;
    /If;
    If: (Found_Count) < 1;
	$showNaviglinks = 'no';
    /If;
/If;

// #1  "n records found"

If: $showNaviglinks != 'no';

'<div ID="navlinks_upper" CLASS="small" STYLE="margin:0 auto;">';

emptyStrID:'2'; // "Es wurde gefunden" or something (if needed)

' <strong>' + (found_count) + '</strong> ';
If: (Found_Count) == 1;
str:'record was found';
Else;
str:'records were found';
/If;
'.'; 
    // Tällä sivulla [If: (Shown_Count)==1]tietue[Else]tietueet[/If] 
    // <B><FONT COLOR="#CC0000">[If: (Shown_Count)==1][shown_first][Else]
    // [shown_first]-[shown_last][/If]</FONT></B>.

'</div>'; // end of upper row


// #2 links to prev & next page

If: ((Shown_First) > 1 || (Shown_Last) != (Found_Count)); // if naviglinkit

'<div id="navlinks_lower" class="small" STYLE="margin:0 auto;text-align:center;">';

// arrow(s) to left
?>
<span STYLE="margin-right:0;">
[Variable:'mathsub'=(Math_Sub:(shown_first),(maxrecords_value),1)]

[if:(shown_first) > 1]
[If: (shown_first) > ((MaxRecords_Value) * 6)]
<a href="[$myfilename]?[$linkstuff]&amp;search=1&amp;skip=0"><IMG SRC="[$fromMetoRoot][$myImages]arrowleft_end.gif" ALT="[str:'First page']" ALIGN="Middle" BORDER="0" WIDTH="17" HEIGHT="15"></a>&nbsp;&nbsp; 
[/If]
<a href="[$myfilename]?[$linkstuff]&amp;search=1&amp;skip=[$mathsub]"><IMG SRC="[$fromMetoRoot][$myImages]arrowleft.gif" ALT="[str:'Previous page']" ALIGN="Middle" BORDER="0" WIDTH="17" HEIGHT="15"></a>
[/if]
&nbsp;&nbsp;
</span>

<?LassoScript

// number links

'<span STYLE="margin:0 auto;">';

Loop:(Found_Count),-LoopIncrement=(MaxRecords_Value);
//    't=' + (Shown_First) + ',' + (((Shown_First) - (5 * (MaxRecords_Value)))) + ':';
    // näytetään 5 ennen ja 5 jälkeen
    If: ((Loop_Count) >= (Shown_First) - (5 * (MaxRecords_Value)))
     && ((Loop_Count) <= (Shown_First) + (5 * (MaxRecords_Value)));
	If: (Loop_Count) != (Shown_First);
	'<A HREF=\"' + $myfilename + '?' + $linkstuff + '&amp;search=1&amp;';
	'skip=' + (((Loop_Count) - 1)) + '\">' + (Loop_Count) + '</A>';
	Else;
	Loop_Count;
	/If;
	If: (Loop_Count) != (Found_Count); // jos ei vika
	'&nbsp;&#124;&nbsp;';
	/If;
    /If;
/Loop;
'</span>';

// arrow right

?>
<span STYLE="margin-left:0;">
&nbsp;&nbsp;
[if:(shown_last)!=(found_count)]
<a href="[$myfilename]?[$linkstuff]&amp;search=1&amp;skip=[shown_last]"><IMG SRC="[$fromMetoRoot][$myImages]arrowright.gif" ALT="[str:'Next page']" ALIGN="Middle" BORDER="0" WIDTH="17" HEIGHT="15"></a>&nbsp;&nbsp;
    <?LassoScript
    Variable:'skipTemp' = (Integer);
    If: Found_Count % MaxRecords_value == 0;
	$skipTemp = Found_Count - MaxRecords_Value;
	Else;
	$skipTemp = (Found_Count - (Found_Count % MaxRecords_Value));
    /If;
    If: $skip < $skipTemp - (5 * (MaxRecords_Value));
    ?>
<a href="[$myfilename]?[$linkstuff]&amp;search=1&amp;skip=[$skipTemp]"><IMG SRC="[$fromMetoRoot][$myImages]arrowright_end.gif" ALT="[str:'Last page']" ALIGN="Middle" BORDER="0" WIDTH="17" HEIGHT="15"></a>
<?LassoScript
    /if;
/if; 			// end if shown_last != found_count
    '</span>';
    '</div>';

    Else; // found n recs is shown but no links -> draw a break
    /If; // end if found_count
/If; // end if naviglinks
'</div>';
?>
