<?LassoScript


/*

last edited: 24th June 2008

* start by copying this file, and name the copy "lg.inc", where "lg" is 
* replaced by a code that identifies
* your target language. "lg" can be two-char code like "en", "de", or longer, 
* like "en_US", "en_UK". You have to write the same code to config.lasso
* (variable 'lang' there) for your new translation file to work
*/

// #1. month names - used for archive page listing

// replace the month names below with those in the target language

Variable:'aMonths'=(Array:'January','February','March','April','May','June','July','August','September','October','November','December');

// #2. day names - not used in 0.7, but may be used later

// replace the day names below with those in the target language

Variable:'aDays'=(Array:'Monday','Tuesday','Wednesday','Thursday','Friday','Saturday','Sunday');
Variable:'aDays_short'=(Array:'Mon','Tue','Wed','Thu','Fri','Sat','Sun');

// #3. "emptyString": optional; strings to be used in places where there is no equivalent phrase in English. The string will correspond to the portion in the explanation text that is in BLOCK LETTERS

Variable:'mEmptyString' = (Map:
1 = '*** replace this ***', // [blogname] uses LassoBlogger BLOGWARE (see footer)
2 = '*** replace this ***', // IT WERE FOUND [NN] records (see naviglinks)
);

// #4. other texts

/* 
* Below, leave the English texts EXACTLY as they are now (they are used as search keys), 
* and write the target language versions after them, for example:
* 'Categories'	= 'Kategoriat',
* follow English punctuation etc. 
* see languages/strings.txt for information about where each string is used
*/

Variable:'mTranslations' = (Map:
'A comment has been added to your blog at'	= '', // used in email message
'and start writing posts (if your initial permissions allow it)' = '', //
use-registered.inc
'and your password' =  '',	// use-registered.inc
'Archives' 	= '',		// headline
'at the admin pages' = '',	// use-registered.inc ("You can now login...")
'Author'	= '', 		// headline
'Authors'	= '', 		// headline (for multi-author blog)
'Back to blog' 	= '',	// link text
'Be sure to remember these values.'
'Be the first to comment this page'	= '',
'Be the first to comment this post'	= '',
'Blacklisted strings were found' = '', // shown to spam commentor; blacklisted means forbidden; blacklists can be composed in Discussion Options
'Categories'	= '',   // headline
'Comment added'	= '',   // short notice used in email subject
'Comment Title'	= '',
'comment'	= '', // There exists 1 COMMENT to the post/page [XX]
'Commenting is turned off for this blog' = '',
'comments'	= '', // [NN] COMMENTS (were found)
'Comments'	= '', // link text
'Could not identify user. ' = '',
'Description'	= '',
'Duplicate value in the field'	= '', 		// (followed by name of field) 
'Edit'		= '', 			// link text
'Email (will not be published; required)'	= '',
'email address' = '',
'Entries'	= '', // link to RSS feed, see footer
'Fields marked with asterisk (*) are required'	= '',
'File Vault' = '',
'Fill in to register. '	= '',
'First Name'	= '',
'First page'	= '',
'Illegal input for this page' = '',   // shown to bad spammers who try to load the comment response page without supplying accurate form data; after this text, the page is aborted
'Integer field'	= '',
'is not valid.'	= '',
'is powered by'	= '', // [blogname] IS POWEDED BY [link to LassoBlogger]
'It didn\'t pass test number' = '', // (followed by number) 
'It has to be moderated by administrator before it can be shown.' = '',
'Last Name'	= '',
'Last page'	= '',
'Leave a Comment' = '',
'Login'		= '',
'Logout'	= '',
'Login again'	= '',
'Login Name'	= '',
'May be your real name, or a nickname' = '',
'Meta'		= '',	// title for links to admin pages and register page
'More info' = '',
'More than one records were found - check your permalink' = '',
'Name' = '',
'Next page'	= '',
'No records were found'	= '',
'Only alphanumeric characters (a-z, A-Z and 0-9) are allowed in the field'	= '',
'or edit the post here:' = '',
'Password'	= '',
'Permanent link to'	= '', // (followed by post title)
'Please give'	= '',
'Please login'	= '',
'post settings' = '', // meaning a toggle in post settings to either allow or disallow commenting 
'Posted in'	= '',		// (followed by category name)
'Previous page'	= '',
'Public Name'	= '',
// next two are special; they belong to phrase "NN records were found"
// there is available a text slot *before* NN
// see above, variable 'mEmptyString, line 2
'record was found'	= '', // (when 1 was found)..
'records were found'	= '', // (when more than 1 were found)
'required'	= '',
'Read the whole post' = '', // link text
'Register'	= '',
'Registration'	= '',
'says'	= '',		// (preceded by commentor name)
'Search!'	= '',   // button text
'See the site'	= '',
'See www site'	= '',   // tooltip text in link to commentors www site
'Site Admin'	= '',
'Sorry, commenting is not allowed for this item' = '',
'Sorry, no such category!' = '',
'Sorry, your comment was <b>NOT</b> saved.' = '',
'Special Pages'	= '', // headline; aka "static" pages
'Submit Comment'	= '', // button text
'Thanks for your comment!'	= '',
'Thank you for your blog posting' = '',
'The add operation FAILED. More info:' = '',
'The administrator will be notified of your comment.' = '',
'The blog posting that you sent by email has just been successfully received. ' = '',
'The email address in field'	= '',		// (followed by field name)
'The operation will be continued after reauthentication.' = '',
'The registration has succeeded.' =  '', // use-registered.inc 
'The tests are used to control commenting and to prevent spam comments.' = '',
'The writer of the post will be notified of your comment.' = '',
'There are limitations to using links in comments' = '',
'to the page'	= '', // (comments) TO THE PAGE
'to the post'	= '', // (comments) TO THE POST
'Too many trials. Please try again later!' = '',
'uploaded'	= '', // followed by date
'User Level'	= '',
'Username'	= '',
'View all posts filed under'	= '', // category link
'View all files in'	= '', // (followed by file category name)
'View all posts in'	= '', // (followed by category name)
'Website'	= '',
'Without the "http://"'	= '',
'You can now log in' =  '', // use-registered.inc 
'You can try again or contact the administrator at' = '',
'You do not have enough privileges to access this page.' = '',
'You have logged out of the site.' = '',
'You have to be registered and logged in to be able to comment'  = '',
'You may browse your post here:' = '',
'You may edit or moderate the comment here:'	= '', // used in email message
'your name' = '',
'Your username is' =  '', // use-registered.inc 
);
?>

