<?LassoScript
If( $opts -> find('use_fileupload') == '1' ); // otherwise do nothing

Var('catid' = String,
    'upc_name' = String,
    'upc_nicename' = String,
    'upc_description' = String,
    'upc_sortorder' = String 
);
'<div>';
    var('i') = array;
    iterate($catz->fileparents,$i);
        // what $i contains:
        //  1 field('id'),
        //  2 '',
        //  3 field('upc_nicename'),
        //  4 field('upc_description'),
        //  5 field('upc_sortorder')));

    if( $case == 'files' || ($case == 'filecat' && $i->get(1) == $filecat) );	
    // case is FILES or this FILECAT has been selected

    '<div class="maincat" style="clear:both;">';

    $catid = $i->get(1);
    $upc_nicename = $i->get(3);
    $upc_description = $i->get(4);
    $upc_sortorder = $i->get(5);

    include( $myinc + 'module_filevault.inc');

	    // any child cats?
	    Var('parent_catid') = $catid;
	    '<div class="subcats" style="margin-left:30px;clear:both;">';
	    If( $catz->childparents -> Find($parent_catid) -> Size );
		    // this maincat has children
		var('j') = String;
		iterate( $catz->children, $j); // being a little lazy
		    If( $j -> get(2) == $parent_catid );
		        $catid = $j -> Get(1);
			$upc_nicename = $j -> Get(3);
			$upc_description = $j -> Get(4);
			$upc_sortorder = $j -> Get(5);
			Include( $myinc + 'module_filevault.inc');
		    /If;
		/iterate;
	    /If;
	    '</div>\r'; 


    '</div>\r';  // end div maincat

    /If; // end if case == files OR this was the selected filecat

    /iterate;

    '</div>\r';

/If;
?>

