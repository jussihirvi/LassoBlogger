[
loop(1); // hack to mimic abort on L9
    // CONF SECTION
	var('path' = $upload_path + $upload_subpath);
    // END CONF SECTION

    // set username & password
      Inline( -nothing,
              -username=$filetagsUsername,
              -password=$filetagsPassword,
      );

// validate

    $uploadErr = integer;

    // has the user defined a file to upload?
    if( ! File_Uploads -> Size );
	$speakBubble += 'Please give a file to upload!<br>';
    /if;

    if( ! $upload_filename );
        $speakBubble += 'Please give a filename.<br>';
        $uploadErr = 31;
    else;
    // throw away the path, if(any);/if; 
        if( $upload_filename >> '/');       // Mac
          var('temp' = $upload_filename -> Split('/'));
          $temp = $temp -> get($temp -> size);
          $upload_filename = string($temp);
        else( $upload_filename >> '\\');  // Windows
          var('temp' = $upload_filename -> Split('\\'));
          $temp = $temp -> get($temp -> size);
          $upload_filename = string($temp);
        /if;

    // strip illegal chars
        $upload_filename = string_replaceregexp( $upload_filename,
          -find="[^a-z0-9\\!\\@\\#\\$\\%\\(\\)\\{\\}\\-\\_\\+\\~\\,\\.]", 
          -replace='',
          -ignorecase);

//    // check for illegal chars
//        if( string_findregexp( $upload_filename,
//          -find="[^a-z0-9\\!\\@\\#\\$\\%\\(\\)\\{\\}\\-\\_\\+\\~\\,\\.]", 
//          -ignorecase)->size > 0 );
//        $speakBubble += 'Please don\'t use special characters or spaces in the filename.<br>';
//        $uploadErr = 32;
//        /if;

        // dot(s) and suffixes
        var('aName' = $upload_filename -> split('.'));
        if( $aName -> Size == 1);
            $speakBubble += ('Please give the file suffix ' +
                '(for example ".jpg") in the filename.<br>');
            $uploadErr = 33;
        else( $aName -> Size > 2);
            $speakBubble += 'Please use only one dot (.) in the filename.<br>';
            $uploadErr = 34;
        else( $aName -> Size == 2); // ok case
            $suffix = $aName -> get(2);
            Var('allowedSx' = $allowed_file_suffixes -> Split(' '));
            if( $allowedSx -> find($suffix) == array );
                $speakBubble += ('The file suffix .' + $suffix +
                        ' is not allowed.<br>');
                $speakBubble += 'Allowed suffices are '+$allowedSx+'.';
                $uploadErr = 35;
            /if;
        /if;
    /if;
    // 'test - filename = '; $upload_filename; '<br>';
    // 'test - cont path = '; $upload_path; '<br>';
    // 'test - upload path = '; $upload_subpath; '<br>';


    // validate - validate folder names & create folders

    var( 'subpath_temp' = $upload_subpath -> split('/'));
    $subpath_temp -> remove;     // remove empty element
    // 'test - ' + $subpath_temp + '<br>';
    var('j') = $upload_path;
    // fail(-1,'path is ' + $upload_path + ', subpath is ' + $subpath_temp);
    iterate( $subpath_temp, var('i') );
      $j += $i + '/';
      $speakBubble += MO_createdir(string($j));
      // if(! file_exists($j));
      //     $speakBubble += 'The directory '+$j+' could not be created! ';
      // /if;
    /iterate;

    // validate: end validate folder names & create folders

    // validate: are there files specified?

    if( file_uploads == array);
        $speakBubble += 'There seems to be no file to upload!';
        MO_speakBubble($speakBubble);
	abort;
    /if;
    // no support for multiple uploads (to get it, see Sokkeli project
    var('mFile'     = File_Uploads -> get(1));
      // item names have changed between L8.5 and L9
      // L8.5        L9 
      // upload.size filesize
      // upload.name tmpfilename filename
      // mFile->type (L9) is staticarray
      // mFIle->type (L8) is probably map
      if( integer(lasso_version(-lassoversion)) >= 9 );
        var('mFile_size') = $mFile -> find('filesize')   ->get(1)->second;
        var('mFile_name') = $mFile -> find('tmpfilename')->get(1)->second;
      else;
        var('mFile_size') = $mFile -> find('Upload.size');
        var('mFile_name') = $mFile -> find('upload.name');
      /if;


    // validate: conflicting or missing selections
    // a) scaling
      var('upload_scale') = string;
      iterate($upload_imagesizes, $i);
          if(var('upload_' + ($i -> get(1))) == 'yes'); // do that image
              $upload_scale = 'yes';
          /if;
      /iterate;
    // b) rotate
      var('upload_rotate') = string;
      if($upload_rotateangle); 
          $upload_rotate = 'yes';
      /if;

    // validate: file size 

    If( $mFile_size == 0);
        $speakBubble += 'The file size seems to be 0. ' +
                'Does the given file really exist?<br>';
        $uploadErr = 41;
    Else( $mFile_size > (1000 * integer($upload_maxk)));
        $speakBubble += 'The file size (' + $mFile_size +
                ') exceeds the max size set in Options!<br>';
        $uploadErr = 42;
    /If;

    // validate: check for duplicate filename

    If( $uploadErr == 0);
	var('testpath') = string;
	var('exists_path') = string;

	$testpath = $path + $upload_filename;
	if(file_exists($testpath));
	    $uploadErr = 51;
	    $exists_path += ('<span style="padding-left:20px;">' 
		+ $testpath + '</span><br>');
	/if;
	$testpath = $path + 'thumbnails/' + $upload_filename;
	if(file_exists($testpath));
	    $uploadErr = 51;
	    $exists_path += ('<span style="padding-left:20px;">' 
		+ $testpath + '</span><br>');
	/if;
	iterate($upload_imagesizes, $i);
	    $testpath = $path + ($i -> get(1)) + '/' + $upload_filename;
	    if(file_exists($testpath));
		$uploadErr = 51;
		$exists_path += ('<span style="padding-left:20px;">'
		    + $testpath + '</span><br>');
	    /if;
	/iterate;
	if($uploadErr == 51);
	$speakBubble += ('In the following directories there already exists a file with this name. Delete those files first, or upload again with another filename!<br>');
	$speakBubble += '<font size="-1">' + $exists_path + '</font>';
	/if;
    /If;

// validate: END

// do the upload

    var('path_description') = array; // list of paths for db-field upl_path

    If( $uploadErr == 0);
	File_Copy( $mFile_name, $path + $upload_filename, -FileOverwrite);
	    If( error_code == 0);
		$path_description -> insert('original');
		$speakBubble += str('File transfer') + ': <font color="green">[ OK ]</font> <br>';
		$speakBubble += str('The path is ') + $path + $upload_filename + '.<br>';
	    Else;
		$uploadErr = 91;
		$speakBubble += str('Errors were reported in file upload.');
		$speakBubble += str('More info: ')  + File_CurrentError + '<br>';
	    /If;
    Else; // case uploadErr
	    $speakBubble += 'Upload was not executed. ' + 
		'Error code was ' + $uploadErr + '.<br>';
    /If;
/Inline;

  // show speakbubble
    if( $uploadErr );
      MO_speakBubble($speakBubble,-style='color:#c00;font-size:1.4em;');
    else;
      MO_speakBubble($speakBubble);
    /if;

// end upload


// image manipulations
if(! $uploadErr );
// set username & password
  Inline( -username=$filetagsUsername,-password=$filetagsPassword,
		-Nothing);

if($suffix == 'jpg' || $suffix == 'gif' || $suffix == 'png');

  if(file_exists($path + $upload_filename));
    // fail(-1,'path was ' + $path + $upload_filename);
    var('myimg' = image($path + $upload_filename));

    // rotate
    if($upload_rotate == 'yes');
	$myimg -> rotate($upload_rotateangle);
    /if;

    // make thumbnail 
    $speakBubble += MO_createdir($path + 'thumbnails' + '/');
    $myimg ->	  save($path + 'thumbnails/' + $upload_filename);
    var('myimg' = image($path + 'thumbnails/' + $upload_filename));

    if(($myimg -> height) > 2 * ($myimg -> width)); // if it is high and narrow 
      if($myimg-> height > 80);                     // does it really need scaling?
	$myimg -> scale(-height='80');
      /if;
    else;
      if($myimg-> width > 80);                      // does it really need scaling?
	$myimg -> scale(-width='80');
      /if;
    /if;
    if($suffix == 'jpg');
	$myimg -> save($path + 'thumbnails/' + $upload_filename,
	    -quality='40');
    else; // gif
	$myimg -> save($path + 'thumbnails/' + $upload_filename);
    /if;
    $path_description -> insert('thumbnail');
    // end make thumbnail

    // make other sizes
    if($upload_scale == 'yes');
	var('iterate_abort' = 'no');
	var('i') = array;
	iterate($upload_imagesizes, $i);
	    if(($iterate_abort == 'no') 
	      && (var('upload_' + ($i -> get(1))) == 'yes')); // do that image
              // create dirs
              $speakBubble += MO_createdir($path+($i->get(1))+'/');
		var('myimg' = image($path + $upload_filename));
		    if($upload_rotate == 'yes'); // rotate
			$myimg -> rotate($upload_rotateangle);
		    /if;
//		    'test - orig = '; $myimg -> width;
//		    ', target = '; $i->get(2); 
//		    ' (';  $i->get(1); ') ';  
//		    '<br>';
		if(($myimg -> width) < $i->get(2)); // image too narrow
		    $speakBubble += 'The original image is narrower '
			+ 'than the specified target size "' + $i->get(1) 
			+ '". The original image will be used. ';
		    $myimg -> save($path + ($i -> get(1)) + '/' + $upload_filename);
		    $path_description -> insert($i->get(1));
		    $iterate_abort = 'yes';
		else;
		    $myimg -> scale(-width=($i -> get(2)));
		    $myimg -> save($path + ($i -> get(1)) + '/' + $upload_filename);
		    $path_description -> insert($i->get(1));
		    // 'TEST - done size '; ($i -> get(1)); 
		    // '<br>';
		/if;
	    /if;
	/iterate;

	// delete original
	file_delete($path + $upload_filename);
	// 'test - remove error'; File_CurrentError; '<br>';
	var('temp') = $path_description -> findIndex('original') -> get(1);
	$path_description -> remove($temp); 

    else($upload_rotate == 'yes'); 
	// if image was rotated, but not scaled, save it over the original
	$myimg -> save($path + $upload_filename);
    /if;
    // end make other sizes
    // 'test area: '; 
    //     '<br>';
  /if;  // end if file exists
/if;    // end if jpg etc. 
/inline;

MO_speakBubble($speakBubble);

/if;    // end image manipulations


// now handle database entries
    If( ! $upload_catid ); $upload_catid = '-1'; /If;
    If( $uploadErr == 0); // if everything is ok so far
	inline( $dbConfig, -table=$table_prefix+'uploads',
	    'upl_posttable'= $upload_posttable,
	    'upl_postid'= $upload_postid,
	    'upl_catid'= $upload_catid,
	    'upl_name'=$upload_filename,
	    'upl_path'=$path_description -> join(', '),
	    'upl_description'=$upload_description,
	    'upl_created'=$nowMySQL,
	    'upl_modified'=$nowMySQL,
	    'upl_userid'=$user_ses,
	    -Add);

	/Inline;
	If( error_msg!= Error_NoError );
            if(error_code == -3000);
              $speakBubble += 'This site seems not to be designed to keep a database of uploads. This is probably ok.';
            else;
              $dbChange_error = error_msg;
              $speakBubble += str('The add operation FAILED. ')+str('More info: ');
              $speakBubble += error_code + ', ' + error_msg;
            /if;
	Else;
            $speakBubble += str('Database write: ') + '<font color="green">[ OK ]</font> <br>';
	/If;
    /If; // end if uploadErr == 0

// END handling database entries

    MO_speakBubble($speakBubble);

/loop; // see first line
]
