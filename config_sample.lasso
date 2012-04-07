<?LassoScript// ** MySQL settings ** //    // The name of the database:	// You can change this if you like.	// Must begin with letter and contain	// only letters, numbers and underscores	Var:'myDb' = 'LassoBlogger';    // Database user 	// for Lasso Security; not needed, if you decide to	// specify -host in inlines instead 	// (see the "dbConfig" variable below for that option)	// *** CHANGE THESE VALUES: ***	Var:'dbUsername' = 'myDbusername'; 	Var:'dbPassword' = 'myDbpassword';	// These are NOT your login username & password!	// They represent your Lasso Security user.	// For security reasons you should change the default values. 	// When you run install.lasso, you will be prompted to create this user	// in SiteAdmin (called Lasso Admin in Lasso 6), so you'd better 	// write those username & password down    // dbconfig array	// this is what will actually be inserted to each inline	// it will contain db name, username and password 	// (the values you have set above)	// but you can also add -host specification, if you like	var:'dbConfig' = (array:		-database=$myDb,		-username=$dbUsername,		-password=$dbPassword		);    // table prefix	// fill this, if you want to store several blogs	// into a single database; otherwise, you	// can leave it empty	// keep it short - example: 'abc_'	Var:'table_prefix' = '';// ** END MySQL settings - please keep reading! ** //// the following is about multilang support     // Change $lang to localize LB.  A corresponding .inc file for the    // chosen language must be installed to the languages folder.    // For example, check that fi.inc is in the languages folder     // and set $lang to 'fi'    // to enable Finnish language support. :-)    // You can make your own translation - see readme.html    // for guidelines, or ask me!    // available languages (as of 0.9.0): Dutch, Finnish, Swedish    Var:'lang' = 'en'; 			// change this if needed/* END EDITING HERE - and save this file as 'config.lasso'! */// (it's still worthwile to read the rest of this file, too)// the rest are variables that you can use on ANY LB page    Var:'sessionlength'	= 120;     /* minutes; sessions are only used on admin pages */      var:'myDomain' = Server_Name;////  old version of myDomain://    Var:'domain'		= (Server_Name) -> (Split:'.');//	Var:'myDomain'	= ''; 		// needed for cookie writing//	    Var:'i'	= $domain -> Size;//	    $myDomain	= $domain -> (Get:($i - 1));//	    $myDomain	+= '.';//	    $myDomain	+= $domain -> (Get:$i);    Var:'temp_URLList' 	= ((response_filepath) -> (Split:'/'));    Var:'i'		    = ($temp_URLList -> Size);    Var:'myfilename'	    = ($temp_URLList -> (Get:$i));    var:'myfoldername'	    = ($temp_URLList-> (Get:(Math_Sub:$i, 1)));    Var:'i'		    = ($myfilename -> (Split:'.'));    Var:'myfileBody'	    = ($i -> (Get:1));    Var:'pathtome'	    = 'http://' + (Server_Name) + (response_path);    Var:'nowMySQL'	    = (Date_Format:(Date),-Format='%Q %T');    Var:'date_dbformat' = '%Q %T'; 	// date format used in db    // used to calculate page-load times in milliseconds (see /inc/footer.inc)    Var:'start' = (_date_msec);?>