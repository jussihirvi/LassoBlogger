<?LassoScript
    If( ! $action || ($action == 'add' && $dbchange_error));
	var('emptyform' = 'yes');
        MO_speakBubble(str('Please fill in and submit!'));
    /if;
?>
