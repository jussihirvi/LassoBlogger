     <div style="margin:0;padding:0; background: transparent url(images/rushes.jpg) repeat 0px -15px;height:53px;">
     <h1>
<?LassoScript
     str($opts -> find('sitename_long'));
     if( $myfoldername = 'admin' );
	 If( $lang == 'fi' );
	     ' &ndash; Ylläpitosivut';
	     Else;
	     ' &ndash; Admin pages';
	 /If;
    /if;
     '</h1>\n';
     '</div>\n';
?>
