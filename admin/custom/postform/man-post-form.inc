<?LassoScript

If( $action == 'add' && $inputvars->find('pos_status') == 'draft' && ! $dbChange_error);

    // notify administrator

    var('emailsubject') = $opts -> find('blogname') + ': Draft waiting for moderation';
    var('emailbody') = 'A post draft has been added to your blog at';
    $emailbody += '\n\n' + $opts -> find('siteurl') + '?p=' + $keyValue;
    $emailbody += '\n\n' + 'You may edit or moderate it here:';
    $emailbody += '\n\n' + $opts -> find('siteurl') + 'admin/man-postedit.lasso?id=' + $keyValue;
    Email_Send( -From=$opts -> find('admin_email'),
	    -To=$opts -> find('admin_email'),
	    -Subject=$emailsubject,
	    -Body=$emailbody);

    $speakBubble += 'The moderator has been notified of your new draft.';
    MO_speakBubble($speakBubble);
/If;
?>
