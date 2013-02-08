<?LassoScript

if( $action == 'add' || $action == 'update');

    // use_url must not contain http

      If( $inputvars -> find('use_url') -> BeginsWith('http') );
          $speakBubble += (str('Please give')+' Website'+str(' without the "http://"!') + '<br />');
      /If;
/If;

// speakBubble handler is in the monster file


?>
