<?LassoScript
// delete validation

If( $action == 'delete'); 
    // necessary cond., because this file is run also in cases add & update

    Inline(-search,
	$dbConfig, 
	-table=$table_prefix + 'uploads',
	-KeyField='id',
	-op='eq',
	'upl_catid'=$keyvalue
        );

	If(found_count);
	    If( $lang=='fi');
	    $speakBubble +='Varoitus: kategoriassa on j�seni�! <br>Poista ensin j�senet, sitten vasta kategoria. <br>Ole hyv� ja peru.<br>' + $id;
	    Else;
	    $speakBubble +='Warning: the category is not empty! <br>Remove first the members, only then the category. <br>Please cancel.<br>';
	    /If;
	/if;
    /Inline;

/If; // end if action

// speakBubble handler is in the monster file
?>
