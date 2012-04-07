<?LassoScript
// remember - var 'i' is reserved (in the enclosing file inc/filevault.inc)!!
	'<h3>'; $upc_nicename; 
	' <span STYLE="font-size:80%;font-weight:normal;">&nbsp;&nbsp;'; 
	$upc_description; '</span>';
	'</h3>\r';

        // sortstuff
        var('sortstuff') = Array;
        if( $upc_sortorder == 'alphabetical' );
                $sortstuff -> insert( -sortfield = 'upl_name');
                $sortstuff -> insert( -sortorder = 'ascending');
            else( $upc_sortorder == 'oldestfirst');
                $sortstuff -> insert( -sortfield = 'id');
                $sortstuff -> insert( -sortorder = 'ascending');
            else( $upc_sortorder == 'newestfirst');
                $sortstuff -> insert( -sortfield = 'id');
                $sortstuff -> insert( -sortorder = 'descending');
            else; 	// the db default is "descending"
                $sortstuff -> insert( -sortfield = 'id');
                $sortstuff -> insert( -sortorder = 'descending');
        /if;

        // get file list
        var('files') = MO_collectUploadData(-filecat=integer($catid),-table='',-sortstuff=$sortstuff);

        // display
        $files -> get(1); // shown files
        $files -> get(2); // linked files

?>

