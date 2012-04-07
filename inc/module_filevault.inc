<?LassoScript
// remember - var 'i' is reserved (in the enclosing file inc/filevault.inc)!!
	'<h3>'; $upc_nicename; 
	' <span STYLE="font-size:80%;font-weight:normal;">&nbsp;&nbsp;'; 
	$upc_description; '</span>';
	'</h3>\r';

        // find files (if there are any)
		// sortstuff
		Var:'sortstuff' = Array;
		If: $upc_sortorder == 'alphabetical';
		    $sortstuff -> (Insert: -SortField = 'upl_name');
		    $sortstuff -> (Insert: -SortOrder = 'ascending');
		Else: $upc_sortorder == 'oldestfirst';
		    $sortstuff -> (Insert: -SortField = 'id');
		    $sortstuff -> (Insert: -SortOrder = 'ascending');
		Else: $upc_sortorder == 'newestfirst';
		    $sortstuff -> (Insert: -SortField = 'id');
		    $sortstuff -> (Insert: -SortOrder = 'descending');
		Else: $upc_sortorder == 'custom';
		    $sortstuff -> (Insert: -SortField = 'upl_customsortcode');
		    $sortstuff -> (Insert: -SortOrder = 'ascending');
		    $sortstuff -> (Insert: -SortField = 'upl_name');
		    $sortstuff -> (Insert: -SortOrder = 'ascending');
		Else; 	// the db default is "descending"
		    $sortstuff -> (Insert: -SortField = 'id');
		    $sortstuff -> (Insert: -SortOrder = 'descending');
		/If;
	  inline: $dbConfig, -table=$table_prefix+'uploads',
		'upl_catid'=$catid,
		$sortstuff,
		-MaxRecords='all',
		-Search;
		// 'test: found ' + (Found_Count) + ' files' + '<br>';
	    	Include: $fromMetoRoot + $myInc + 'errorhandler.inc';
	      Var:'aFiles' = Array;
	  Records;
	      // collect file data
	      $aFiles -> (Insert:(Array:(Field:'upl_path'),(Field:'upl_name'),
	      (Field:'upl_description'), (FIeld:'upl_imagedisplay'), (Field:'upl_created')));
	  /Records;
	  /Inline;

	  // process and display files
		// 'test: afiles ' + $aFiles + '<br>';
		Var:'suffix' 	= '';
		Var:'linked_files' = '';    // var for storing 
		Var:'shown_files' = '';	    // var for storing
		Var:'j' 	= Array;    // (var i is reserved!)
	    Iterate: $aFiles, $j;
		$suffix = $j -> (Get: 2)  -> (Split:'.') -> Last;

		If:  $j -> (Get:4) == 'none'; 	// just skip the file
		Else: ($suffix == 'jpg' 
		||  $suffix == 'jpeg'
		||  $suffix == 'gif'
		||  $suffix == 'png') 
		&&  $j -> (Get:4) != 'link'; 	// that img will be displayed
		$shown_files += '<div class="img" style="clear:both;padding:0px 0 5px 0;"><div><IMG align="left" SRC="';
		$shown_files += $j -> (Get:1) + $j -> (Get:2) + '" alt="' + 
		    $j -> (Get:3) + '" style="' +
		    'padding:0 10px 5px 0;" TITLE="';
		    If: $j -> (Get:5) != '0000-00-00 00:00:00'
		    &&  $j -> (Get:5) != '';
			$shown_files += (str:'uploaded') + ' ' + 
			(LB_formatDateTime:$j -> (Get:5));
		    /If;
		$shown_files += '"></div>\r' + 
		    '<div class="caption">' + 
		    $j -> (Get:3);
		$shown_files += '</div></div>\r';
		Else; 			 	// just display link
		$linked_files += '<div style="clear:both;"><A HREF="' +  $j -> (Get:1) + $j -> (Get:2);
		$linked_files += '" TITLE="';
		    If: $j -> (Get:5) != '0000-00-00 00:00:00'
		    &&  $j -> (Get:5) != '';
			$linked_files += (str:'uploaded') + ' ' + 
			(LB_formatDateTime:$j -> (Get:5));
		    /If;
		$linked_files += '">' + $j -> (Get:2) + '</A>';
		$linked_files += ' (' + $j -> (Get:3) + ')';
		$linked_files += '</div>\r';
		/If;
	    /Iterate;
	        $linked_files += '\r';

	    // now display
	    If: $shown_files != ''; $shown_files; '\r'; /If;
	    If: $linked_files != '\r'; $linked_files; '\r'; /If;

?>

