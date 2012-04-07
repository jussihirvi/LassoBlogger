<?LassoScript

If: $action == 'add_action' && $pos_status == 'draft' && $dbChange_error == '';

    // notify administrator

    Variable:'emailsubject' = ($basicOptions -> 'blogname') + ': Draft waiting for moderation';
    Variable:'emailbody' = 'A post draft has been added to your blog at';
    $emailbody += '\n\n' + ($basicOptions -> 'siteurl') + '?p=' + $keyValue;
    $emailbody += '\n\n' + 'You may edit or moderate it here:';
    $emailbody += '\n\n' + ($basicOptions -> 'siteurl') + 'admin/man-postedit.lasso?id=' + $keyValue;
    Email_Send: -From=($basicOptions -> 'admin_email'),
	    -To=($basicOptions -> 'admin_email'),
	    -Subject=$emailsubject,
	    -Body=$emailbody;

    $speakBubble += 'The moderator has been notified of your new draft.';
			    If: $speakBubble != '';
			    Include: $fromMetoRoot + $myInc + 'speakBubble.inc';
			    /If;
/If;



?>
