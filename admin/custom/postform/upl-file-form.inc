<?LassoScript

If( $action == 'delet2_action' && ! $dbChange_error );

    // ask about removing the file, too

// 'test: file = ' + $file;
?>

<p class="center"><b>Do you want to move the actual file <br>"[$file]" <br>to the trash folder? (Recommended.)</b></p>
<p class="center"> <A HREF="upl-removefile.lasso?file=[$file]">Ok &raquo;</A>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

<A HREF="[$myfilename]?
[$linkstuff]skip=[$skip]">[If( $lang=='fi' )]Peru[Else]Cancel[/If] &raquo;</A></p>
[Abort]

[/If]
