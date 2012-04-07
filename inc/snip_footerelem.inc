<?LassoScript
    If: $myfilename == ($stuff -> (Get:1));
    $stuff -> (Get:2);
    Else;
    '<A HREF=\"' + ($stuff -> (Get:1)) + '\">' + ($stuff -> (Get:2)) + '</A>';
    /If;
?>
