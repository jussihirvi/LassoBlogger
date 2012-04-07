<?LassoScript
// delete validation

If: $action == 'delete_action'; 
    // necessary cond., because this file is run also in cases add & update

    Inline:-search,
	$dbConfig, 
	-table=$table_prefix + 'posts',
	-KeyField='id',
	-op='eq',
	'pos_category'=$keyvalue;

	If: (found_count);
	    If: $lang=='fi';
	    $speakBubble +='Varoitus: kategoriassa on jäseniä! <br>Poista ensin jäsenet, sitten vasta kategoria. <br>Ole hyvä ja peru.<br>' + $id;
	    Else;
	    $speakBubble +='Warning: the category is not empty! <br>Remove first the members, only then the category. <br>Please cancel.<br>';
	    /If;
	/if;
    /Inline;

/If; // end if action

// speakBubble handler is in the monster file
?>
