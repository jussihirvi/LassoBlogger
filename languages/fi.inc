<?LassoScript


// #1. month names - used for archive page listing

// replace the month names with those in the target language

var('aMonths')=Array('Tammikuu','Helmikuu','Maaliskuu','Huhtikuu','Toukokuu','Kes�kuu','Hein�kuu','Elokuu','Syyskuu','Lokakuu','Marraskuu','Joulukuu');

// #2. day names - not used in 0.7, but may be used later

// replace the day names below with those in the target language

var('aDays')=Array('Maanantai','Tiistai','Keskiviikko','Torstai','Perjantai','Lauantai','Sunnuntai');
var('aDays_short')=Array('Ma','Ti','Ke','To','Pe','La','Su');

// #3. "emptyString": strings to places where there is no English equivalent

var('mEmptyString') = Map(
1 = '-blogisovellusta', // BLOG ENGINE: [blogname] (uses LassoBlogger) ... 
2 = 'L�ytyi',		// IT WERE FOUND: ... [NN] (records)
);

// #4. other texts. Leave the English texts EXACTLY as they are now, 
// write the target language versions after them, for example:
// 'Categories'	= 'Kategoriat',
// follow English punctuation etc. 

var('mTranslations') = Map(
'A comment has been added to your blog at'	= 'Blogiisi on lis�tty kommentti, ks.', // used in email message
'and start writing posts (if your initial permissions allow it)' = 'ja ryhty�
kirjoittamaan blogiviestej� (jos uudelle k�ytt�j�lle my�nnetyt oikeudet
sallivat t�m�n)', // use-registered.inc
'and your password' =  'ja salasanasi', // use-registered.inc 
'Archives' = 'Arkisto', // headline
'at the admin pages' =  'yll�pitosivustolle', // use-registered.inc 
'Author' = 'Kirjoittaja', // headline
'Authors' = 'Kirjoittajat', // headline (for multi-author blog)
'Back to blog'	= 'Takaisin blogiin',	// link text
'Blacklisted strings were found' = 'L�ytyi kiellettyj� merkkijonoja',
'Be sure to remember these values.' =  'Pane ne huolellisesti muistiin.', // use-registered.inc 
'Be the first to comment this page'	= 'Kommentoi ensimm�isen� t�t� sivua',
'Be the first to comment this post'	= 'Kommentoi ensimm�isen� t�t� blogiviesti�',
'Categories'	= 'Kategoriat',
'Comment added'	= 'Kommentti lis�tty',   // short notice used in email subject
'Comment Title'	= 'Kommentin otsikko',
'comment'	= 'kommentti', // (1) comment (to the post/page XX)
'Commenting is turned off for this blog' = 'Kommentointi ei ole sallittua t�ss� blogissa',
'comments '	= 'kommenttia', // (NN) comments (were found)
'Comments'	= 'Kommentit', // link text
'Could not identify user. ' = 'K�ytt�j�n tunnistus ei onnistunut. ',
'Description'	= 'Kuvaus',
'Duplicate value in the field'	= 'Varattu arvo kent�ss�', 		// name of field follows
'Edit'		= 'Editointi', 			// link text
'Email (will not be published)'	= 'Email (ei julkaista)',
'email address' = 'email-osoite',
'Entries'	= 'Blogiviestit', // link  to RSS feed
'Fields marked with asterisk (*) are required'	= 'Asteriskilla (*) merkityt kent�t ovat pakollisia',
'File Vault' = 'Tiedostot',
'Fill in to register. '	= 'T�yt� kent�t ja l�het� tiedot',
'First Name'	= 'Etunimi',
'First page'	= 'Ensimm�inen sivu',
'Illegal input for this page' = 'Laiton sy�te t�t� sivua varten',	// shown to bad spammers who try to load the comment response page without supplying accurate form data; after this text, abort
'Integer field'	= 'Numerokentt�',
'is not valid.'	= 'ei ole validi.',
'is powered by'	= 'k�ytt��', // [blogname] is powered by [link to LassoBlogger]
'It didn\'t pass test number' = 'Se ei l�p�issyt testi� numero', // [number following]
'It has to be moderated by administrator before it can be shown.' = 'Se voidaan n�ytt�� vasta kun yll�pit�j� on hyv�ksynyt sen.',
'Last Name'	= 'Sukunimi',
'Last page'	= 'Viimeinen sivu',
'Leave a Comment' = 'Kommentoi',
'Login'		= 'Kirjaudu',
'Login again'	= 'Kirjaudu uudelleen sis��n',
'Logout'	= 'Kirjaudu ulos',
'Login Name'	= 'K�ytt�j�tunnus',
'May be your real name, or a nickname' = 'Voi olla oikea nimesi tai nimimerkki',
'Meta'		= 'Meta',	// title for links to admin pages and register page
'More info' = 'Lis�tietoja',
'More than one records were found - check your permalink' = 'L�ytyi enemm�n kuin yksi blogiviesti - tarkista permalinkki',
'Name' = 'Nimi',
'Next page'	= 'Seuraava sivu',
'No records were found'	= 'Yht��n tietuetta ei l�ytynyt',
'Only alphanumeric characters (a-z, A-Z and 0-9) are allowed in the field'	= 'vain kirjaimia ja numeroita (a-z, A-Z, 0-9) saa k�ytt�� kent�ss�',
'or edit the post here:' = 'tai editoida sit� t��ll�:',
'Page was not found'	= 'Sivua ei l�ytynyt',
'Password'	= 'Salasana',
'Permanent link to'	= 'Pysyv� linkki blogiviestiin', // ...[post title]
'Please give'	= 'Ole hyv� ja anna',
'Please login'	= 'Kirjaudu palveluun',
'post settings' = 'blogiviestin asetukset', //meaning a toggle in post settings to either allow or disallow commenting 
'Posted in'	= 'Kuuluu kategoriaan',		// ...[category name]
'Previous page'	= 'Edellinen sivu',
'Public Name'	= 'Julkinen nimi',
// next two are special; they belong to phrase "NN records were found"
// there is available a text slot *before* NN
// see top of this page, variable 'mEmptyString, row 2
'record was found'	= 'tietue', // (1 was found)..
'records were found'	= 'tietuetta', // (more than 1 were found)
'required'	= 'pakollinen',
'Read the whole post' = 'Lue koko juttu', // link text
'Register'	= 'Rekister�idy',
'Registration'	= 'Rekister�ityminen',
'says'	= 'kirjoittaa',		// [commentor name] ...
'Search!'	= 'Hae!', // button text
'See the site'	= 'Takaisin sivustolle',
'See www site'	= 'Katso www-sivuja', // tooltip text in link to commenters www site
'Site Admin'	= 'Sivuston hallinta',
'Sorry, commenting is not allowed for this item' = 'T�m�n kohteen kommentointi ei valitettavasti ole sallittua',
'Sorry, no such category!' = 'Haettua kategoriaa ei l�ydy',
'Sorry, your comment was <b>NOT</b> saved.' = 'Kommenttiasi EI valitettavasti tallennettu.',
'Special Pages'	= 'Erikoissivut', // aka static pages
'Submit Comment'	= 'L�het� kommentti', // button text
'Thanks for your comment!'	= 'Kiitos kommentista!',
'Thank you for your blog posting' = 'Kiitos blogiviestist�si',
'The add operation FAILED. More info:' = 'Toiminto EI ONNISTUNUT. Lis�tietoja:',
'The administrator will be notified of your comment.' = 'Yll�pit�j�lle ilmoitetaan kommentistasi.',
'The blog posting that you sent by email has just been successfully received. ' = 'Emailitse l�hett�m�si blogiviesti on otettu onnistuneesti vastaan. ',
'The email address in field'	= 'Email-osoite kent�ss�',		// name of field follows
'The operation will be continued after reauthentication.' = 'Operaatio
suoritetaan, kunhan tunnukset on ensin tarkistettu.',
'The registration has succeeded.' =  'Rekister�inti onnistui.', // use-registered.inc 	
'The tests are used to control commenting and to prevent spam comments.' = 'Testien tarkoitus on hallita kommentointia ja est�� roskapostikommentit.',
'The writer of the post will be notified of your comment.' = 'Blogiviestin kirjoittajalle ilmoitetaan kommentistasi.',
'There are limitations to using links' = 'Linkkien k�ytt�� kommenteissa on rajoitettu',
'to the page'	= 'sivuun', // (comments) to the page
'to the post'	= 'blogiviestiin', // (comments) to the post
'Too many trials. Please try again later!' = 'Liian monta yrityst�. Ole hyv�
ja yrit� my�hemmin uudelleen.',
'uploaded'	= 'julkaistu', // followed by date
'Username'	= 'K�ytt�j�tunnus',
'User Level'	= 'K�ytt�j�taso',
'View all posts filed under'	= 'Hae kaikki blogiviestit kategoriassa', // category link
'View all files in'	= 'Hae kaikki tiedostot kategoriassa', // ...[category name]
'View all posts in'	= 'Hae kaikki blogiviestit kategoriassa', // ...[category name]
'Website'	= 'Www-sivut',
'Without the "http://"'	= 'Ilman "http://"-etuliitett�',
'You can now log in' =  'Nyt voit kirjautua', // use-registered.inc 
'You can try again or contact the administrator at' = 'Voit yritt�� uudelleen tai ottaa yhteytt� yll�pit�j��n osoitteessa',
'You do not have enough privileges to access this page.'    = 'Oikeutesi
eiv�t riit� t�lle sivulle.',
'You have logged out of the site.' = 'Olet kirjautunut ulos sivustolta.',
'You have to be registered and logged in to be able to comment' = 'Vain rekister�ityneet ja sis��nkirjautuneet k�ytt�j�t saavat kirjoittaa kommentteja', 
'You may browse your post here:' = 'Voit selata blogiviesti�si t��ll�:',
'You may edit or moderate the comment here:'	= 'Voit editoida kommenttia sivulla', // used in email message
'your name' = 'nimesi',
'Your username is' =  'K�ytt�j�tunnuksesi on', // use-registered.inc 

);
?>

