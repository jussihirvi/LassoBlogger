<?LassoScript
'<div id="footer">';
'<hr style="margin-bottom:8px;">';
    '<p>';
    '<a href="';
    $opts -> find('siteurl');
    '" target="_blank">'; str('See the site'); '</a>';
    if($userlevel_ses);
      '&nbsp;&nbsp;|&nbsp;&nbsp;';
      '<a href="logout.lasso">'; str('Logout'); '</a>\n';
    /if;
//		' &bull; ';
//		(_date_msec) - $start; ' ms';
    '</p>';

'</div>'; //  end footer
?>


