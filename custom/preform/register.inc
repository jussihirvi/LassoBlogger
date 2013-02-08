<?lassoScript
If( $action == 'add' && ! $dbChange_error );
    '<div style="margin:0 auto 0 auto;width:60%;font-size:1.4em;">\n';
      '<p>';
        str('The registration has succeeded.'); 
      '</p>';
      '<p>';
        str('Your username is'); 
        ' <strong>'; $inputvars -> find('use_login'); '</strong> ';
        str('and your password'); 
        ' <strong>'; $inputvars -> find('use_pass'); '</strong>.<br />';
        str('Be sure to remember these values.');
      '</p>';
      '<p>';
        str('You can now log in at');
        ' <a href="admin/">'; str('the admin pages'); '</a> ';
        str('and start writing posts (if your initial permissions allow it)');
      '!</p>';
    '</div>';
    // this will prevent the form from showing again
    $buttonlist = map('add' = 'no','update' = 'no', 'delete' = 'no');
/If;
?>

