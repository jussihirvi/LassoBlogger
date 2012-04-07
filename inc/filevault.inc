<?LassoScript
If: LB_Basicoptions -> 'use_fileupload' == '1'; // otherwise do nothing

Var:'catid' = String,
    'upc_name' = String,
    'upc_nicename' = String,
    'upc_description' = String,
    'upc_sortorder' = String;

'<div>';

    Records: -InlineName='filecats_parents';
    If: ($case == 'files' && (Field:'upc_parent') == 0) || ($case == 'filecat' && (Field:'id') == $filecat);	
//     'test: ' + (Field:'id') + '=' + (Field:'upc_parent') + '<br>';

    '<div class="maincat" style="clear:both;">';

    $catid = (Field:'id');
//    $upc_name = (Field:'upc_name');
    $upc_nicename = (Field:'upc_nicename');
    $upc_description = (Field:'upc_description');
    $upc_sortorder = (Field:'upc_sortorder');

    Include: $myinc + 'module_filevault.inc';

	    // any child cats?
	    Var:'parent_catid' = $catid;
	    '<div class="subcats" style="margin-left:30px;clear:both;">';
	    If: $aFilecats_children_parents -> (Find:$parent_catid) -> Size;
		    // this maincat has children
		Var:'i' = String;
		Iterate: $aFilecats_children, $i; // being a little lazy
		    If: $i -> (get:2) == $parent_catid;
		        $catid = $i -> (Get:1);
			$upc_nicename = $i -> (Get:3);
			$upc_description = $i -> (Get:4);
			$upc_sortorder = $i -> (Get:5);
			Include: $myinc + 'module_filevault.inc';
		    /If;
		/iterate;
	    /If;
	    '</div>\r'; 


    '</div>\r';  // end div maincat

    /If; // end if case == files OR this was the selected filecat

    /Records;

    '</div>\r';

/If;
?>

