<?LassoScript

    // use_url must not contain http

if( $action == 'add' || $action == 'update');
    If( $inputvars -> find('use_url') -> BeginsWith('http') );
        If( $lang=='fi');
        $speakBubble +='Anna Website ilman "http"-alkulitaniaa!<br>';
        Else;
        $speakBubble +='Please give Website without the "http://"!<br>';
        /If;
    /If;
/If;
// speakBubble handler is in the monster file


?>
