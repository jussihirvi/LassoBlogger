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

Variable:'aMonths'=(Array:'Januari','Februari','Mars','April','Maj','Juni',' Juli','Augusti','September','Oktober','November','December');

// #2. day names - not used in 0.7, but may be used later

// replace the day names below with those in the target language

Variable:'aDays'=(Array:'Måndag','Tisdag','Onsdag','Torsdag','Fredag',' Lördag','Söndag');
Variable:'aDays_short'=(Array:'Mån','Tis','Ons','Tors','Fre','Lö','Sö');

// #3. "emptyString": optional; strings to be used in places where there is no equivalent phrase in English. The string will correspond to the portion in the explanation text that is in BLOCK LETTERS

Variable:'mEmptyString' = (Map:
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

Variable:'mTranslations' = (Map:
'A comment has been added to your blog at'	= 'En komment har tillagts till din blogg, se', // used in email message
'and start writing posts (if your initial permissions allow it)' 
= 'och börja skriva blogginlägg (ifall rättigheterna för en ny användare tillåter detta)', // use-registered.inc
'and your password' =  'och ditt lösenord',	// use-registered.inc
'Archives' 	= 'Arkiv',		// headline
'at the admin pages' = 'till underhållssidorna',    // use-registered.inc ("You can now login...")
'Authentication required' = 'Autenticering krävs',
'Author'	= 'Författare', 		// headline
'Authors'	= 'Flera författare', 		// headline (for multi-author blog)
'back to blog' 	= 'tillbaka till webbsidorna',	// link text
'Be sure to remember these values.'=  'Lägg dem omsorgsfullt i minnet.', // use-registered.inc 
'Be the first to comment this page'	= 'Var den första som kommenterar denna sida',
'Be the first to comment this post'	= 'Var den första som kommenterar denna artikel',
'Blacklisted strings were found' = 'Förbjudna teckenserier har använts', // shown to spam commentor; blacklisted means forbidden; blacklists can be composed in Discussion Options
'Categories'	= 'Kategorier',   // headline
'Comment added'	= 'En kommentar har tillagts',   // short notice used in email subject
'Comment Title'	= 'Kommentarrubrik',
'comment'	= 'kommentar', // There exists 1 COMMENT to the post/page [XX]
'Commenting is turned off for this blog' = 'Kommentarer är ej tillåtna i denna blogg',
'comments'	= 'kommentarer', // [NN] COMMENTS (were found)
'Comments'	= 'Kommentarer', // link text
'Competition overview'	= 'Presentation av tävlingar',
'Competitions'	= 'Tävlingar',
'Current competitions'	= 'Aktuella tävlingar',
'Description'	= 'Beskrivning',
'Duplicate value in the field'	= 'Reserverat värde i fältet', 		// (followed by name of field) 
'Edit'		= 'Redigera', 			// link text
'Email (will not be published; required)'	= 'E-post (publiceras inte; fordras)',
'email address' == 'E-postadress',
'Entries'	= 'Artiklar', // link to RSS feed, see footer
'Fields marked with asterisk (*) are required'	= 'Uppgifterna i fälten utmärkta med asterisk (*) fordras',
'File Vault' = 'Filerna',
'Fill in to register. '	= 'Fyll i uppgifterna och sänd dem in. ',
'First Name'	= 'Förnamn',
'First page'	= 'Första sidan',
'Illegal input for this page' = 'Orättmätiga inmatade uppgifter för denna sida',   // shown to bad spammers who try to load the comment response page without supplying accurate form data; after this text , the page is aborted
'Integer field'	= 'Nummerfält',
'is not valid.'	= 'inte giltigt',
'is powered by'	= 'använder', // [blogname] IS POWEDED BY [link to LassoBlogger]
'It didn\'t pass test number' = 'Den klarade inte testet nummer', // (followed by number) 
'It has to be moderated by administrator before it can be shown.' = 'Administratorn skall godkänna den innan den kan publiceras',
'Last Name'	= 'Efternamn',
'Last page'	= 'Sista sidan',
'Leave a Comment' = 'Lämna in en kommentar',
'Login'		= 'Logga in',
'Login again'	= 'Logga in igen',
'Logout'	= 'Logga ut',
'Login Name'	= 'Användarnamn',
'May be your real name, or a nickname' == 'Det kan vara ditt eget namn eller en signatur',
'Meta'		= 'Meta',	// title for links to admin pages and register page
'More info' = 'Mer uppgifter',
'More than one records were found - check your permalink' = 'Fler än en artikel blev funna - granska din permalänk',
'Name (required)' = 'Namn (fordras)',
'Next page'	= 'Följande sida',
'No records were found'	= 'Inga artiklar funna',
'Only alphanumeric characters (a-z, A-Z and 0-9) are allowed in the field'	
= 'Ändast bokstäverna och numren (a-z, A-Z och 0-9) är tillåtna i fältet',
'Open competitions'	= 'Allmänna tävlingar',
'Password'	= 'Lösenord',
'Permanent link to'	= 'En permanent länk till artikeln', // (followed by post title)
'Please give'	= 'Var vänlig och ange',
'Please login'	= 'Var vänlig och logga in',
'post settings' = 'artikel inmatad data', // meaning a toggle in post settings to either allow or disallow commenting 
'Posted in'	= 'Hör till kategoriet',		// (followed by category name)
'Previous page'	= 'Den föregående sidan',
'Public Name'	= 'Offentligt namn',
// next two are special; they belong to phrase "NN records were found"
// there is available a text slot *before* NN
// see above, variable 'mEmptyString, line 2
'record was found'	= 'textfil hittades', // (when 1 was found)..
'records were found'	= 'textfiler hittades', // (when more than 1 were found)
'required'	= 'fordras',
'Read more' = 'Läs mera', // link text
'Read the whole post' = 'Läs hela texten', // link text
'Register'	= 'Inskriv',
'Registration'	= 'Inskrivning',
'Limited competitions'   = 'Inbjudna tävlingar',
'says'	= 'skriver',		// (preceded by commentor name)
'Search!'	= 'Sök!',   // button text
'See the site'	= 'Se webbplatsen',   // tooltip text in link to commentors www site
'See www site'	= 'Se webbplatsen',   // tooltip text in link to commentors www site
'Site Admin'	= 'Adminsidorna',
'Sorry, commenting is not allowed for this item' = 'Inga kommentarer är tyvärr tillåtna till denna punkt',
'Sorry, your comment was <b>NOT</b> saved.' = 'Din kommentar har tyvärr <b>INTE</b> sparats',
'Special Pages'	= 'Speciella sidor', // headline; aka "static" pages
'Submit Comment'	= 'Skicka kommentar', // button text
'Thanks for your comment!'	= 'Tack för din kommentar!',
'The add operation FAILED. More info:' == 'Funktionen LYCKADES INTE. Se tilläggsinformation:',
'The administrator will be notified of your comment.' = 'Din kommentar meddelas till administratorn',
'The email address in field'	= 'E-postadressen i fältet',		// (followed by field name)
'The registration has succeeded.' =  'Inskrivningen lyckades.', // use-registered.inc 
'The tests are used to control commenting and to prevent spam comments.' = 'Testerna används till att kontrollera kommentarerna och förhindra skräppostkommentarer',
'The writer of the post will be notified of your comment.' = 'Artikelskrivaren meddelas om din kommentar',
'There are limitations to using links in comments' = 'Användandet av länkar i kommentarerna är begränsat',
'to the page'	= 'till sidan', // (comments) TO THE PAGE
'to the post'	= 'till artikeln', // (comments) TO THE POST
'uploaded'	= 'publicerad', // followed by date
'Username'	= 'Användarnamn',
'User Level'	= 'Användarnivå',
'View all posts filed under'	= 'Sök alla artiklar i kategorin', // category link
'View all posts in'	= 'Se alla artiklar i kategorin', // (followed by category name)
'Website'	= 'Webbplats',
'Without the "http://"'	= 'Utan "http://" -prefixet',
'You can now log in' =  'Du kan nu logga in', // use-registered.inc 
'You can try again or contact the administrator at' = 'Du kan prova pånytt eller ta kontakt med administratorn',
'You have logged out of the site.' = 'Du har loggat ut.',
'You have to be registered and logged in to be able to comment'  = 'Endast inskrivna och inloggade användare kan kommentera',
'You may edit or moderate the comment here:'	= 'Du kan redigera din kommentar på denna sida:', // used in email message
'your name' = 'ditt namn',
'Your username is' =  'Ditt användarnamn är', // use-registered.inc 
);
?>

