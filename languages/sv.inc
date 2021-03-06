<?LassoScript


/*
* start by copying this file, and name the copy "lg.inc", where "lg" is 
* replaced by a code that identifies
* your target language. "lg" can be two-char code like "en", "de", or longer, 
* like "en_US", "en_UK". You have to write the same code to config.lasso
* (variable 'lang' there) for your new translation file to work
*/

// #1. month names - used for archive page listing

// replace the month names below with those in the target language

var('aMonths')=Array('Januari','Februari','Mars','April','Maj','Juni',' Juli','Augusti','September','Oktober','November','December');

// #2. day names - not used in 0.7, but may be used later

// replace the day names below with those in the target language

var('aDays')=Array('M�ndag','Tisdag','Onsdag','Torsdag','Fredag',' L�rdag','S�ndag');
var('aDays_short')=Array('M�n','Tis','Ons','Tors','Fre','L�','S�');

// #3. "emptyString": optional; strings to be used in places where there is no equivalent phrase in English. The string will correspond to the portion in the explanation text that is in BLOCK LETTERS

var('mEmptyString') = Map(
1 = '', // [blogname] uses LassoBlogger BLOGWARE (see footer)
2 = '', // IT WERE FOUND [NN] records (see naviglinks)
);

// #4. other texts

/* 
* Below, leave the English texts EXACTLY as they are now (they are used as search keys), 
* and write the target language versions after them, for example:
* 'Categories'	= 'Kategorier',
* follow English punctuation etc. 
* see languages/strings.txt for information about where each string is used
*/

var('mTranslations') = Map(
'A comment has been added to your blog at'	= 'En komment har tillagts till din blogg, se', // used in email message
'and start writing posts (if your initial permissions allow it)' 
= 'och b�rja skriva blogginl�gg (ifall r�ttigheterna f�r en ny anv�ndare till�ter detta)', // use-registered.inc
'and your password' =  'och ditt l�senord',	// use-registered.inc
'Archives' 	= 'Arkiv',		// headline
'at the admin pages' = 'till underh�llssidorna',    // use-registered.inc ("You can now login...")
'Authentication required' = 'Autenticering kr�vs',
'Author'	= 'F�rfattare', 		// headline
'Authors'	= 'Flera f�rfattare', 		// headline (for multi-author blog)
'back to blog' 	= 'tillbaka till webbsidorna',	// link text
'Be sure to remember these values.'=  'L�gg dem omsorgsfullt i minnet.', // use-registered.inc 
'Be the first to comment this page'	= 'Var den f�rsta som kommenterar denna sida',
'Be the first to comment this post'	= 'Var den f�rsta som kommenterar denna artikel',
'Blacklisted strings were found' = 'F�rbjudna teckenserier har anv�nts', // shown to spam commentor; blacklisted means forbidden; blacklists can be composed in Discussion Options
'Categories'	= 'Kategorier',   // headline
'Comment added'	= 'En kommentar har tillagts',   // short notice used in email subject
'Comment Title'	= 'Kommentarrubrik',
'comment'	= 'kommentar', // There exists 1 COMMENT to the post/page [XX]
'Commenting is turned off for this blog' = 'Kommentarer �r ej till�tna i denna blogg',
'comments'	= 'kommentarer', // [NN] COMMENTS (were found)
'Comments'	= 'Kommentarer', // link text
'Competition overview'	= 'Presentation av t�vlingar',
'Competitions'	= 'T�vlingar',
'Current competitions'	= 'Aktuella t�vlingar',
'Description'	= 'Beskrivning',
'Duplicate value in the field'	= 'Reserverat v�rde i f�ltet', 		// (followed by name of field) 
'Edit'		= 'Redigera', 			// link text
'Email (will not be published; required)'	= 'E-post (publiceras inte; fordras)',
'email address' == 'E-postadress',
'Entries'	= 'Artiklar', // link to RSS feed, see footer
'Fields marked with asterisk (*) are required'	= 'Uppgifterna i f�lten utm�rkta med asterisk (*) fordras',
'File Vault' = 'Filerna',
'Fill in to register. '	= 'Fyll i uppgifterna och s�nd dem in. ',
'First Name'	= 'F�rnamn',
'First page'	= 'F�rsta sidan',
'Illegal input for this page' = 'Or�ttm�tiga inmatade uppgifter f�r denna sida',   // shown to bad spammers who try to load the comment response page without supplying accurate form data; after this text , the page is aborted
'Integer field'	= 'Nummerf�lt',
'is not valid.'	= 'inte giltigt',
'is powered by'	= 'anv�nder', // [blogname] IS POWEDED BY [link to LassoBlogger]
'It didn\'t pass test number' = 'Den klarade inte testet nummer', // (followed by number) 
'It has to be moderated by administrator before it can be shown.' = 'Administratorn skall godk�nna den innan den kan publiceras',
'Last Name'	= 'Efternamn',
'Last page'	= 'Sista sidan',
'Leave a Comment' = 'L�mna in en kommentar',
'Login'		= 'Logga in',
'Login again'	= 'Logga in igen',
'Logout'	= 'Logga ut',
'Login Name'	= 'Anv�ndarnamn',
'May be your real name, or a nickname' == 'Det kan vara ditt eget namn eller en signatur',
'Meta'		= 'Meta',	// title for links to admin pages and register page
'More info' = 'Mer uppgifter',
'More than one records were found - check your permalink' = 'Fler �n en artikel blev funna - granska din permal�nk',
'Name (required)' = 'Namn (fordras)',
'Next page'	= 'F�ljande sida',
'No records were found'	= 'Inga artiklar funna',
'Only alphanumeric characters (a-z, A-Z and 0-9) are allowed in the field'	
= '�ndast bokst�verna och numren (a-z, A-Z och 0-9) �r till�tna i f�ltet',
'Open competitions'	= 'Allm�nna t�vlingar',
'Password'	= 'L�senord',
'Permanent link to'	= 'En permanent l�nk till artikeln', // (followed by post title)
'Please give'	= 'Var v�nlig och ange',
'Please login'	= 'Var v�nlig och logga in',
'post settings' = 'artikel inmatad data', // meaning a toggle in post settings to either allow or disallow commenting 
'Posted in'	= 'H�r till kategoriet',		// (followed by category name)
'Previous page'	= 'Den f�reg�ende sidan',
'Public Name'	= 'Offentligt namn',
// next two are special; they belong to phrase "NN records were found"
// there is available a text slot *before* NN
// see above, variable 'mEmptyString, line 2
'record was found'	= 'textfil hittades', // (when 1 was found)..
'records were found'	= 'textfiler hittades', // (when more than 1 were found)
'required'	= 'fordras',
'Read more' = 'L�s mera', // link text
'Read the whole post' = 'L�s hela texten', // link text
'Register'	= 'Inskriv',
'Registration'	= 'Inskrivning',
'Limited competitions'   = 'Inbjudna t�vlingar',
'says'	= 'skriver',		// (preceded by commentor name)
'Search!'	= 'S�k!',   // button text
'See the site'	= 'Se webbplatsen',   // tooltip text in link to commentors www site
'See www site'	= 'Se webbplatsen',   // tooltip text in link to commentors www site
'Site Admin'	= 'Adminsidorna',
'Sorry, commenting is not allowed for this item' = 'Inga kommentarer �r tyv�rr till�tna till denna punkt',
'Sorry, your comment was <b>NOT</b> saved.' = 'Din kommentar har tyv�rr <b>INTE</b> sparats',
'Special Pages'	= 'Speciella sidor', // headline; aka "static" pages
'Submit Comment'	= 'Skicka kommentar', // button text
'Thanks for your comment!'	= 'Tack f�r din kommentar!',
'The add operation FAILED. More info:' == 'Funktionen LYCKADES INTE. Se till�ggsinformation:',
'The administrator will be notified of your comment.' = 'Din kommentar meddelas till administratorn',
'The email address in field'	= 'E-postadressen i f�ltet',		// (followed by field name)
'The registration has succeeded.' =  'Inskrivningen lyckades.', // use-registered.inc 
'The tests are used to control commenting and to prevent spam comments.' = 'Testerna anv�nds till att kontrollera kommentarerna och f�rhindra skr�ppostkommentarer',
'The writer of the post will be notified of your comment.' = 'Artikelskrivaren meddelas om din kommentar',
'There are limitations to using links in comments' = 'Anv�ndandet av l�nkar i kommentarerna �r begr�nsat',
'to the page'	= 'till sidan', // (comments) TO THE PAGE
'to the post'	= 'till artikeln', // (comments) TO THE POST
'uploaded'	= 'publicerad', // followed by date
'Username'	= 'Anv�ndarnamn',
'User Level'	= 'Anv�ndarniv�',
'View all posts filed under'	= 'S�k alla artiklar i kategorin', // category link
'View all posts in'	= 'Se alla artiklar i kategorin', // (followed by category name)
'Website'	= 'Webbplats',
'Without the "http://"'	= 'Utan "http://" -prefixet',
'You can now log in' =  'Du kan nu logga in', // use-registered.inc 
'You can try again or contact the administrator at' = 'Du kan prova p�nytt eller ta kontakt med administratorn',
'You have logged out of the site.' = 'Du har loggat ut.',
'You have to be registered and logged in to be able to comment'  = 'Endast inskrivna och inloggade anv�ndare kan kommentera',
'You may edit or moderate the comment here:'	= 'Du kan redigera din kommentar p� denna sida:', // used in email message
'your name' = 'ditt namn',
'Your username is' =  'Ditt anv�ndarnamn �r', // use-registered.inc 
);
?>

