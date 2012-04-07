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

var('aMonths')=Array('Januari','Februari','Maart','April','Mei','Juni','Juli','Augustus','September','Oktober','November','December');

// #2. day names - not used in 0.7, but may be used later

// replace the day names below with those in the target language

var('aDays')=Array('Maandag','Dinsdag','Woensdag','Donderdag','Vrijdag','Zaterdag','Zondag');
var('aDays_short')=Array('Maa','Din','Woe','Don','Vrij','Zat','Zon');

// #3. "emptyString": optional; strings to be used in places where there is no equivalent phrase in English. The string will correspond to the portion in the explanation text that is in BLOCK LETTERS

var('mEmptyString') = Map(
1 = '', // [blogname] uses LassoBlogger BLOGWARE (see footer)
2 = '', // IT WERE FOUND [NN] records (see naviglinks)
);

// #4. other texts

/* 
* Below, leave the English texts EXACTLY as they are now (they are used as search keys), 
* and write the target language versions after them, for example:
* 'Categories'	= 'Kategoriat',
* follow English punctuation etc. 
* see languages/strings.txt for information about where each string is used
*/

var('mTranslations') = Map(
'A comment has been added to your blog at'	= 'Nieuwe commentaar is toegevoegd aan uw blog op', // used in email message
'and start writing posts (if your initial permissions allow it)' = 'en schrijf een artikel (indien u voldoende rechten hebt)', // use-registered.inc
'and your password' =  'en uw passwoord',	// use-registered.inc
'Archives' 	= 'Archief',		// headline
'at the admin pages' = 'op de admin pagina',	// use-registered.inc ("You can now login...")
'Author'	= 'Auteur', 		// headline
'Authors'	= 'Auteurs', 		// headline (for multi-author blog)
'Back to blog' 	= 'Terug naar de blog',	// link text
'Be sure to remember these values.' = 'Onthou zeker deze gegevens.',
'Be the first to comment this page'	= 'Wees de eerste om hier commentaar te geven op deze pagina',
'Be the first to comment this post'	= 'Wees de eerste om hier commentaar te geven op deze post',
'Blacklisted strings were found' = 'Verboden teksten gevonden', // shown to spam commentor; blacklisted means forbidden; blacklists can be composed in Discussion Options
'Categories'	= 'Categoriëen',   // headline
'Comment added'	= 'Commentaar toegevoegd',   // short notice used in email subject
'Comment Title'	= 'Commentaar Titel',
'comment'	= 'commentaar', // There exists 1 COMMENT to the post/page [XX]
'Commenting is turned off for this blog' = 'Commentaar geven is uitgeschakeld voor deze blog',
'comments'	= 'commentaren', // [NN] COMMENTS (were found)
'Comments'	= 'Commentaren', // link text
'Could not identify user. ' = 'Kan de gebruiker niet identificeren',
'Description'	= 'Omschrijving',
'Duplicate value in the field'	= 'Dubbele waarde in het veld', 		// (followed by name of field) 
'Edit'		= 'Aanpassen', 			// link text
'Email (will not be published)'	= 'Email (zal niet getoond worden)',
'email address' = 'email adres',
'Entries'	= 'Entries', // link to RSS feed, see footer
'Fields marked with asterisk (*) are required'	= 'Velden met een sterretje (*) zijn verplicht',
'File Vault' = 'File Vault',
'Fill in to register. '	= 'In te vullen om te registreren.',
'First Name'	= 'Voornaam',
'First page'	= 'Eerste pagina',
'Illegal input for this page' = 'Foutieve ingave voor deze pagina',   // shown to bad spammers who try to load the comment response page without supplying accurate form data; after this text, the page is aborted
'Integer field'	= 'Nummer veld',
'is not valid.'	= 'is niet geldig',
'is powered by'	= 'is gemaakt met', // [blogname] IS POWEDED BY [link to LassoBlogger]
'It didn\'t pass test number' = 'Raakte niet voorbij test nr', // (followed by number) 
'It has to be moderated by administrator before it can be shown.' = 'Het moet goedgekeurd worden door de admin vooraleer het kan getoond worden.',
'Last Name'	= 'Achternaam',
'Last page'	= 'Laatste pagina',
'Leave a Comment' = 'Laat commentaar na',
'Login'		= 'Inloggen',
'Logout'	= 'Uitloggen',
'Login again'	= 'Log opnieuw in',
'Login Name'	= 'Login naam',
'May be your real name, or a nickname' = 'Mag uw echte naam of een schuilnaam zijn',
'Meta'		= 'Meta',	// title for links to admin pages and register page
'More info' = 'Meer info',
'More than one records were found - check your permalink' = 'Meer dan een record gevonden - gelieve uw permalink na te kijken',
'Name' = 'Naam',
'Next page'	= 'Volgende pagina',
'No records were found'	= 'Geen records gevonden',
'Only alphanumeric characters (a-z, A-Z and 0-9) are allowed in the field'	= 'Enkel alfanumerieke tekens (a-z, A-Z) zijn toegelaten in dit veld',
'or edit the post here:' = 'of pas hier uw posting aan:',
'Password'	= 'Paswoord',
'Permanent link to'	= 'Permanente link naar', // (followed by post title)
'Please give'	= 'Geef aub',
'Please login'	= 'Inloggen aub',
'post settings' = 'post instellingen', // meaning a toggle in post settings to either allow or disallow commenting 
'Posted in'	= 'Gepost in',		// (followed by category name)
'Previous page'	= 'Vorige pagina',
'Public Name'	= 'Publieke Naam',
// next two are special; they belong to phrase "NN records were found"
// there is available a text slot *before* NN
// see above, variable 'mEmptyString, line 2
'record was found'	= 'record werd gevonden', // (when 1 was found)..
'records were found'	= 'records werden gevonden', // (when more than 1 were found)
'required'	= 'verplicht',
'Read the whole post' = 'Lees meer', // link text
'Register'	= 'Registreer',
'Registration'	= 'Registratie',
'says'	= 'zegt',		// (preceded by commentor name)
'Search!'	= 'Zoek!',   // button text
'See the site'	= 'Bekijk de site',
'See www site'	= 'Bekijk website',   // tooltip text in link to commentors www site
'Site Admin'	= 'Site Admin',
'Sorry, commenting is not allowed for this item' = 'Sorry, commentaar geven op dit item is niet mogelijk',
'Sorry, your comment was <b>NOT</b> saved.' = 'Sorry, uw commentaar werd <b>NIET</b> opgeslaan.',
'Special Pages'	= 'Speciale Pagina\'s', // headline; aka "static" pages
'Submit Comment'	= 'Verzend commentaar', // button text
'Thanks for your comment!'	= 'Bedankt voor uw commentaar!',
'Thank you for your blog posting' = 'Bedankt voor uw blog post',
'The add operation FAILED. More info:' = 'Het toevoegen is MISLUKT. Meer info:',
'The administrator will be notified of your comment.' = 'De administrator zal verwittigd worden van uw commentaar.',
'The blog posting that you sent by email has just been successfully received. ' = 'De blog post die u doorstuurde via email werd goed ontvangen',
'The email address in field'	= 'Het emailadres in veld',		// (followed by field name)
'The operation will be continued after reauthentication.' = 'De afhandeling zal verder gaan na her-authenticatie',
'The registration has succeeded.' =  'De registratie was successvol', // use-registered.inc 
'The tests are used to control commenting and to prevent spam comments.' = 'De tests worden gebruikt om spam in commentaren te voorkomen',
'The writer of the post will be notified of your comment.' = 'De auteur van deze post zal verwittigd worden over uw commentaar',
'There are limitations to using links in comments' = 'Er zijn beperkingen in het gebruik van links in commentaren',
'to the page'	= 'naar de pagina', // (comments) TO THE PAGE
'to the post'	= 'naar de post', // (comments) TO THE POST
'Too many trials. Please try again later!' = 'Teveel fouten. Gelieve het later nog eens te proberen!',
'uploaded'	= 'upgeloaded', // followed by date
'User Level'	= 'Gebruikersniveau',
'Username'	= 'Gebruikersnaam',
'View all posts filed under'	= 'Bekijk alle posts in categorie', // category link
'View all files in'	= 'Bekijk alle bestanden in', // (followed by file category name)
'View all posts in'	= 'Bekijk alle posts in', // (followed by category name)
'Website'	= 'Website',
'Without the "http://"'	= 'Zonder de "http://"',
'You can now log in' =  'U kan nu inloggen', // use-registered.inc 
'You can try again or contact the administrator at' = 'U kan opnieuw proberen of contacteer de administrator via',
'You do not have enough privileges to access this page.' = 'U heeft niet genoeg rechten om deze pagina te bekijken.',
'You have logged out of the site.' = 'U bent uitgelogd ui deze site.',
'You have to be registered and logged in to be able to comment'  = 'U moet geregistreerd en ingelogd zijn om commentaar te kunnen geven',
'You may browse your post here:' = 'U kan uw post hier bekijken:',
'You may edit or moderate the comment here:'	= 'U kan de commentaar hier bekijken of aanpasen:', // used in email message
'your name' = 'uw naam',
'Your username is' =  'Uw gebruikersnaam is', // use-registered.inc 
);
?>


