<?LassoScript 
// ** MySQL settings ** //

    // The name of the database:

	// You can change this if you like.
	// Must begin with letter and contain
	// only letters, numbers and underscores

	var('myDb' = 'LassoBlogger');

    // Database user (for Lasso Security):

	// for Lasso Security; not needed, if you decide to
	// specify -host in inlines instead 
	// (see the "dbConfig" variable below for that option)

	// *** CHANGE THESE VALUES: ***

	var('dbUsername' = 'my-db-username'); 
	var('dbPassword' = 'my-db-password');

	// These are NOT your login username & password!
	// They represent your Lasso Security user.
	// For security reasons you should change the default values. 
	// When you run install.lasso, you will be prompted to create this user
	// in SiteAdmin (called Lasso Admin in Lasso 6), so you'd better 
	// write those username & password down

    // dbconfig array

	// this is what will actually be inserted to each inline
	// it will contain db name, username and password 
	// (the values you have set above)
	// but you can also add -host specification, if you like

	var('dbConfig' = array(
		-database=$myDb,
		-username=$dbUsername,
		-password=$dbPassword
		));

    // table prefix

	// fill this, if you want to store several blogs
	// into a single database; otherwise, you
	// can leave it empty

	var('table_prefix' = string);

// ** END MySQL settings - please keep reading! ** //


// the following is about multilang support 
    // Change $lang to localize LB.  A corresponding .inc file for the
    // chosen language must be installed to the languages folder.
    // For example, check that fi.inc is in the languages folder 
    // and set $lang to 'fi'
    // to enable Finnish language support. :-)
    // You can make your own translation - see readme.html
    // for guidelines, or ask me!
    // available languages (as of 0.9.0): Dutch, Finnish, Swedish

    var('lang' = 'en'); 			// change this if needed


/* END EDITING HERE - and save this file as 'config.lasso'! */

// (it's still worthwile to read the rest of this file, too)

// the rest are vars that you can use on ANY LB page

    var('sessionlength'	= 120); 
    /* minutes; sessions are only used on admin pages */

    // some of these are never used, 2011-10
    var('myDomain' = Server_Name);
    var('temp_URLList' 	    = response_filepath -> Split('/'));
    var('temp_URLListSize'  = $temp_URLList -> Size);
    var('myfilename'	    = $temp_URLList -> Get($temp_URLListSize));
    var('temp_myfileList'   = $myfilename -> Split('.'));
    var('myfileBody' 	    = $temp_myfileList -> Get(1));
    var('myfoldername'	    = $temp_URLList-> get(Math_Sub($temp_URLListSize, 1)));
    var('pathtome'	    = 'http://' + Server_Name + response_path);
    var('nowMySQL' 	    = Date_Format(date,-format='%Q %T'));
    var('date_dbformat' = '%Q %T'); 	// date format used in db

    // used to calculate page-load times in milliseconds (see /inc/footer.inc)
    var('start' = _date_msec);
?>

