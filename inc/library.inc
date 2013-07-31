<?LassoScript

// this file is sitewide, put adminpage stuff in admin/inc/libmonster.inc
// except maybe strings?? 
// versioning
// 2011-11-23 MO_navig - added support for subfolders
//   requires field 'subdir' in table sitepages, and subdir support in
//   apache rewrite rule
// 2010-11-19 most of the colon syntax removed (for app rahtikirjanumerot)
// 2010-11-29 bug in get_thismainmenu corrected (for menu sub-items,
//   values for 1st item were inserted for all other items, too

// var('lb_version' = '0.9.0'); // July 28 2008
// var('lb_version') = '0.9.1'; // June 19 2009
var('lb_version' = '0.9.3'); // November 15 2011

// hack for (older) $basicoptions system
// from now on, use $opts instead 2011-10

if( variable_defined('basicOptions') );
  var('opts') = $basicOptions;
/if;

// string handler, the most important function
// it searches for translations, if lang not English

// but first include the lang file

    if($lang != 'en');
      include( $fromMetoRoot + 'languages/' + $lang + '.inc');
      include( $fromMetoRoot + 'languages/' + $lang + '_local.inc');
    else;
      include( $fromMetoRoot + 'languages/en.inc');
    /if;

Define_Tag( 'str',-Required='str',-encodenone ); // universal strings (not site-specific)
    local('out' = #str);
	If( $lang != 'en');
	    if( $mTranslations -> find(#str) );
		#out= $mTranslations -> find(#str); // see language file
	    else( $mTransLocal -> find(#str) );
		#out= $mTransLocal -> find(#str); // see local language file
	    /if;
       /If;
    return(#out);
/Define_Tag;
// stl is DEPRECATED 2010-12
Define_Tag( 'stl',-Required='str' ); // site-specific strings
    local('out' = #str);
        If( $lang != 'en');
            if( $mTransLocal -> find(#str) );
                #out= $mTransLocal -> find(#str); // see language file
            /if;
       /If;
    return(#out);
/Define_Tag;

// special treatment for strings that are empty in english, but not in some other language

Define_Tag( 'emptystrID',-Required='strID');
    Local('output' = '');
	If( $lang == 'en');
	    #output = '';
	Else;
	    #output = $mEmptyString -> Get(#strID) -> Second;
       /If;
    Return(#output);
/Define_Tag;

Define_Tag( 'LB_titleCase',-Required='str');
    Local('output' = '');
    Local('aStr' = #str -> Split(' '));
    Local('aStr2' = Array);
    Local('i' = '');
    Local('iFirst' = '');
    Local('iRest' = ''); 
    Iterate(#aStr,#i);
        #iFirst = #i -> Get(1);
        #iFirst -> Uppercase;
        #iRest  = #i;
        #iRest -> Remove(1,1);
        #aStr2 -> Insert(#iFirst + #iRest);
    /Iterate;
        #output = #aStr2 -> Join(' ');
    Return(#output);
/Define_Tag;


// format dates

Define_Tag( 'LB_formatDateTime',-Optional='datetime');
    Local('output' = string);
    Local('date_showformat' = $opts -> find('date_showformat'));
      If( ! #date_showformat );
        #date_showformat = '%m/%d/%y';
      /If;
    Local('time_showformat' = $opts -> find('time_showformat'));
      If( #time_showformat == '');
        #time_showformat = '%H:%M:%S';
      /If;
	    If( #datetime != '0000-00-00 00:00:00' && #datetime != '');
 	    #output = Date_Format(Date(#datetime,-Format='%Q %T'),
		      -Format=(#date_showformat + ' ' + #time_showformat));
Else;
#output = 'No date';
 	       /If;
            Return(#output);
/Define_Tag;

// make hash for cookies

Define_Tag('LB_createCookiehash');
    local('siteurl' =  $opts -> find('siteurl'));
    if( ! #siteurl );
        local('pathForCookie' = $pathTome); // from siteconfig
    else;
        local('pathForCookie' = #siteurl);
    /if;
    return(encrypt_MD5(#pathForCookie));
/Define_Tag;

// recognize user

Define_Type('LB_Userinfo');
	Local('inlinestuff' = Array);
	Local('use_level' = '');
	Local('user_id' = '');
	Local('username' = '');
	Local('password' = '');
	Local('testing' = '');
	Local('found' = Integer);
    Define_Tag('onCreate',-optional='username',-optional='password');
        ! local_defined('username') ? local('username') = string;
        ! local_defined('password') ? local('password') = string;
        self->'testing' = #username;
        local('hash' = LB_createCookiehash);
        if(#username && #password);
          (self->'inlinestuff')->insert(-eq);
          (self->'inlinestuff')->insert('use_login' = #username);
          (self->'inlinestuff')->insert(-eq);
          (self->'inlinestuff')->insert(encrypt_md5('use_pass' = #password));
	else( cookie('user_' + #hash) && cookie('pass_' + #hash));
          // use cookies
          (self -> 'inlinestuff') -> insert(-operator = 'eq');
          (self -> 'inlinestuff') -> 
            insert('use_login' = cookie('user_' + #hash));
          (self -> 'inlinestuff') -> insert(-operator = 'eq');
          (self -> 'inlinestuff') -> 
	      insert('use_pass' = cookie('pass_' + #hash));
	else( variable_defined('user_ses'));  
	// use session vars (probably never used, as public pages
	// don't use session)
	    (self -> 'inlinestuff') -> Insert(-eq);
	    (self -> 'inlinestuff') -> Insert('id' = $user_ses);
	/If;
        If( self -> 'inlinestuff' -> size );
	// verify the user data
	    Inline( -search,$dbConfig, 
		-table = $table_prefix+'users',
		self -> 'inlinestuff');

		if( error_msg == error_noerror && found_count == 1);
                  (self -> 'username') = field('use_login');
                  (Self -> 'password') = Field('use_pass');
                  (Self -> 'use_level') = Field('use_level');
                  (Self -> 'user_id')   = Field('id');
                  // write/refresh cookies
                  var('path') = response_path;
                  $path -> RemoveTrailing('admin/');
                  Cookie_Set( ('user_' + #hash)
                  = #username,-Expires=432000,-Domain=$myDomain,-Path=$path);
                  Cookie_Set( ('pass_' + #hash)
                  = #password,-Expires=432000,-Domain=$myDomain,-Path=$path);
                  // remember that password is now md5-encrypted

		Else;
		    (Self -> 'use_level') = (-2); // no match?
		/If;
		(self->'testing') = field('use_login')+', '+field('use_pass');
		(self->'found') = found_count;
	    /Inline;
	Else;				
	    self -> 'use_level' = (-1); 	// no cookies, no session
	    self->'testing'     = cookie('user_' + #hash);
	/if;
    /Define_Tag;

    define_tag('lb_getuse_level'); return( self -> 'use_level'); /define_tag;
    define_tag('lb_getuser_id');   return( self -> 'user_id');   /define_tag;
    define_tag('testing');         return( self -> 'testing');   /define_tag;
/define_type;



// tag to get options
Define_Tag('siteoptions',-namespace='LB');
  /* usage var('opts') = LB_siteoptions;
     $opts -> find('siteurl');
  */
    local('err' = string);
    local('out' = map);
      inline( -findall, $dbConfig,
        -table = $table_prefix+'options',
        -maxrecords='all');
        records;
          #out -> insert(field('opt_name') = field('opt_value'));
        /records;
      /inline;
      if(error_msg != 'No error');
        fail(-1, 'Tag LB_siteoptions returned "' + error_msg + '".');
      else;
        return(#out);
      /if;
/define_tag;

// get absolute path based on 'siteurl' option

// Define_Tag( 'LB_getAbsolutepath');
//     Local('siteurl' = $opts -> find('siteurl'));
//     Local('aPath' = #siteurl -> Split('/'));
//     Local('sPath' = string);
//     If( #aPath -> Size > 4);
// 	Loop( (#aPath -> Size) - 4);
// 	    #sPath += ('/' + (#aPath -> Get(3 + Loop_Count)));
// 	/Loop;
// 	    #sPath += '/';
//     Else;
// 	#sPath = '/';
//     /If;
//     Return( #sPath);
// /Define_Tag;

// get option by opt_name

// get author by ID

Define_Tag( 'LB_getAuthor',-Required='id');
            Local('output' = '');
            Local('errorCode' = '');
            Inline(-search,$dbConfig,
		-Table=$table_prefix+'users',
                'id'=#id);
		#output = Field('use_nickname');
                #errorCode = error_code;
            /Inline;
            Return(#output);
/Define_Tag;

// get cat by ID

Define_Tag( 'LB_getCategory',-Required='id');
            Local('output' = '');
            Local('errorCode' = '');
            Inline(-search,$dbConfig,-Table=$table_prefix+'categories',
                'id'=#id);
		#output = Field('cat_name');
                #errorCode = error_code;
            /Inline;
            Return(#output);
/Define_Tag;

// get commentlink

Define_Tag( 'LB_getCommentlink',-Required='id',-encodeNone);
            Local('output' = '');
            Local('errorCode' = '');
            Inline(-search,$dbConfig,
		-Table=$table_prefix+'comments',
                'com_post_id'=#id,
		'com_approved'='1',
		-ReturnField='id');
		If( Found_Count > 0 );
		#output = Found_Count + ' ';
		    If( Found_Count == 1);
			#output += str('comment');
			Else( Found_Count > 1);
			#output += str('comments');
		    /If;
		#output += '  &#187;';
		Else;
		    If( $pos_status == 'publish');
			#output = str('Be the first to comment this post');
		    Else( $pos_status == 'static'); 
			#output = str('Be the first to comment this page');
		    /If;
		    #output += ' &#187;';
		/If;
                #errorCode = error_code;
            /Inline;
            Return(#output);
/Define_Tag;

// get number of posts in cat

Define_Tag( 'LB_getNrofPosts',-Required='id');
            Local('output' = '');
            Local('errorCode' = '');
            Inline(-search,$dbConfig,
		-Table=$table_prefix+'posts',
                'pos_category'=#id,
		'pos_status' = 'publish',
		-ReturnField='id');
		#output = Found_Count;
                #errorCode = error_code;
            /Inline;
            Return(#output);
/Define_Tag;

Define_Type('LB_getFieldnames');
 	If( (Params) -> Size > 0);
	    Local('fieldNames' = Params -> Get(1));
	    Else;
	    Local('fieldNames' = 'Empty');
 	/If;
	Local('error' = error_msg);

    Define_Tag('giveFieldnames');
	Return( (Self -> 'fieldNames'));
    /Define_Tag;

    Define_Tag('error');
	Return( 'x' + (Self -> 'error'));
    /Define_Tag;
    
    Define_Tag('giveFieldvalues');
	Return( 'x' + (Self -> 'error'));
    /Define_Tag;

/Define_Type;

// ** searches **

// make linkstuff from searchargs

Define_Tag( 'LB_makeLinkstuff',-Required='searchArgs');
    Local('linkStuff');
// 	#linkstuff += '?';
    Iterate(#searchArgs,Local('i'));
	#linkstuff += #i -> Get(1) + '=';
	#linkstuff += #i -> Get(2) + '&';
    /Iterate;
	If( #linkstuff != '');
	#linkstuff -> RemoveTrailing('&');
	/If;
    Return(#linkstuff);
/Define_Tag;

define_type('cats',-namespace='LB');
 /* gather data for filecats and postcats
    example var('catz') = LB_cats; 
 */
  local('fileparents'   = array,
      'file'            = array,
      'filechildren'    = array,
      'filechildparents'= array,
      'parents'          = array,
      'parentids'        = array,
      'children'         = array,
      'childparents'     = array,
      'childids'         = array); 
 define_tag('onCreate');

  // #0.1 file cats
  Inline(-Search,$dbConfig,-Table=$table_prefix+'uploadcats',
          -SortField='upc_name',
          -SortOrder='ascending',
  // 	-Op='eq',
  // 	'upc_parent' = '',
  // even children are searched - needed in case filecat, 
  // if selected cat is a child
          -MaxRecords='all',
          -ReturnField='id',
          -ReturnField='upc_parent',
          -ReturnField='upc_name',
          -ReturnField='upc_nicename',
          -ReturnField='upc_description',
          -ReturnField='upc_sortorder');
  local('aFile_parents'     = array,
        'aFile'             = array,
        'aFile_children'    = array,
        'aFile_childparents'= array);
  Records;
  If( Field('upc_parent') == 0); // child-cats are not inserted
      #aFile_parents -> Insert( Array(
        Field('id'), 
        '', 
        Field('upc_nicename'), 
        Field('upc_description'), 
        Field('upc_sortorder')));
  else;
      #aFile_children -> Insert( Array(Field('id'), Field('upc_parent'), Field('upc_nicename'), Field('upc_description'), Field('upc_sortorder')));
      #aFile_childparents -> Insert( Field('upc_parent'));
  /If;
      #aFile -> Insert( Array(
        Field('id'), 
        '', 
        Field('upc_nicename'), 
        Field('upc_description'), 
        Field('upc_sortorder'),
        Field('upc_parent')
        ));
  /Records;
  /Inline;

  // #0.2 post cats
  Inline(-Search,$dbConfig,-Table=$table_prefix+'categories',
          -SortField='cat_name',
          -SortOrder='ascending',
  // 	-Op='eq',
  // 	'cat_parent' = '',
  // even children are searched (needed in case cat, 
  // if selected cat is a child)
          -MaxRecords='all',
          -ReturnField='id',
          -ReturnField='cat_parent',
          -ReturnField='cat_name',
          -ReturnField='cat_nicename',
          -ReturnField='cat_description',
          -ReturnField='cat_sortorder');
  local('aParents'      = array,
        'aParentids'    = array,
        'aChildren'     = array,
        'aChildparents' = array,
        'aChildids'     = array);
  Records;
  If( Field('cat_parent') == 0); // child-cats are not inserted
      #aParents -> Insert( Array(Field('id'), '', Field('cat_nicename'), Field('cat_description'), Field('cat_sortorder')));
      #aParentids -> Insert(field('id'));
  else;
      #aChildren -> 
        Insert( Array(Field('id'), Field('cat_parent'), Field('cat_nicename'), Field('cat_description'), Field('cat_sortorder')));
      #aChildparents -> Insert( Field('cat_parent'));
      #aChildids -> Insert( Field('id'));
  /If;
  /Records;
  /Inline;


  self -> 'fileparents'      = #aFile_parents;
  self -> 'file'             = #aFile;
  self -> 'filechildren'     = #aFile_children;
  self -> 'filechildparents' = #aFile_childparents;
  self -> 'parents'          = #aparents;
  self -> 'parentids'        = #aparentids;
  self -> 'children'         = #achildren;
  self -> 'childparents'     = #achildparents;
  self -> 'childids'         = #achildids;
 /define_tag;

 define_tag('detect_this_type'); return(string); /define_tag;
/define_type;

// rest not essential for blog

Define_Tag('SC_bankReference',-Required='basis');
    Local(  'basis_string'	= string(#basis),
	    'kertoimet'		= array(7,3,1),
	    'kerroin' 		= integer,
	    'numero'		= integer,
	    'tulo' 		= integer,
	    'tulo_cumulative'	= integer,
	    'viitenumero'	= string,
	    'i'			= 2, // jotta voidaan jakaa kolmella
	    'tarkiste'		= integer,
	    'testvar'		= string); 
    loop( -loopFrom=(#basis_string -> size), -loopTo=1, -loopIncrement=-1);
	#i += 1;
	#kerroin = #kertoimet -> get((#i % 3) + 1);
// antaa ekalla kierroksella 1
// 'test - kerroin = ' + #kerroin + ', ';
	#numero= #basis_string -> Get(loop_count);
	#tulo= #kerroin * #numero;
	#tulo_cumulative += #tulo;
    /loop;
    // 'test: tulo_cumulative = ' + #tulo_cumulative + ', ';
    #tarkiste = (10 - (#tulo_cumulative % 10)) % 10;
    // 'test - tarkiste = ' + #tarkiste;
    #viitenumero = #basis_string + #tarkiste;
    Return( #viitenumero);
/Define_Tag;

// for hyotykasviyhdistys 0911

Define_Tag('MO_langSuffix');
              // define suffix for field names in the navig type
              local('out' = string);
              if($lang == $langs -> get(1));
                // no lang suffix
              else($langs -> size > 2); // 3 langs
                if($lang == $langs -> get(2));
                  #out = '_lang2'; 
                else($lang == $langs -> get(3));
                  #out = '_lang3'; 
                /if;
              else($langs -> size > 1); // 2 langs
                if($lang == $langs -> get(2));
                  #out = '_lang2'; 
                /if;
              /if;
            return(#out); // this tag deprecated for Rahtikirjanumerot
/Define_Tag;

// commented out for LassoBlogger

// Define_Type('MO_navig');
//     local('test'=integer);
//     local('mypage'=string);
//     local('mycatid'=integer);
//     local('sitecats'=array);
//     local('sitecatids'=array);
//     // set page contents (under constr.)
//     local('myaddress'  = string);
//     local('mymenuname'     = string);
//     local('myheading'  = string);
//     local('mydomain'   = string);
//     local('mycontent'  = string);
//     local('myform'     = string);
//     local('mycustomdisplay' = string);
//     local('contentfile'  = string);
//     local('sitepath'  = string);
// 
// 
//     // define methods
//     Define_Tag('onCreate',-optional='page',-optional='sitepath');
// 	// which page?
// 	if(!(local_defined('page')));
//           local('page') = string;
//         /if;
//         // use sitepath if the whole site is in a subdirectory
// 	if(!(local_defined('sitepath')));
//           local('sitepath') = string;
//         /if;
//           self -> 'mypage' = #page;
//           self -> 'sitepath' = #sitepath;
//         inline( -search, $dbConfig, -table=$table_prefix + 'sitepages',
//                -op = 'eq',
//                'address' = self -> 'mypage');
//          if(found_count == 1);
//             self->'myheading'	    = field('heading' + MO_langSuffix);
//             self->'myaddress'	    = field('address');
//             self->'mymenuname'	    = field('menuname' + MO_langSuffix);
//             self->'mydomain'	    = field('domain');
//             self->'mycontent'	    = field('content' + MO_langSuffix);
//             self->'myform'	    = field('form');
//             self->'mycustomdisplay' = field('customdisplay');
//             self -> 'mycatid'	    = field('catid');
//           else; // error
//             self->'myheading'	    = 'Page not found!<br>';
//          /if;
//         /inline;
// 	// define contentfile
// 	if((self->'mycustomdisplay') == 'on');
//           self->'contentfile'	    = '/cont/' + (self->'myaddress') + '.inc';
// 	else((self->'myaddress') -> contains('.html'));
//           self->'contentfile'	  = '/cont/' 
//                                   + (self->'myaddress') -> split('.') -> get(1)
//                                   + '.inc';
// 	else;
// 	    self->'contentfile'	    = '/cont/cont.inc';
// 	/if;
// 
// 	// compose list of sitecats
// 	inline( -findall, $dbConfig, -table=$table_prefix + 'sitecats',
// 	    -sortfield='sortcode');
//           records;
//             (self -> 'sitecatids') -> insert( field('id'));
//             (self -> 'sitecats') -> insert( field('catname' + MO_langSuffix));
//           /records;
// 	/inline;
//     /Define_Tag;
// 
//     // getters
// 
//     Define_Tag('get_test');	return(self -> 'test'); /Define_Tag;
//     Define_Tag('get_catid');	return(self -> 'mycatid'); /Define_Tag;
//     Define_Tag('get_heading');	return(self -> 'myheading'); /Define_Tag;
//     Define_Tag('get_address');	return(self -> 'myaddress'); /Define_Tag;
//     Define_Tag('get_menuname');	return(self -> 'mymenuname'); /Define_Tag;
//     Define_Tag('get_domain');	return(self -> 'mydomain'); /Define_Tag;
//     Define_Tag('get_content');	return(self -> 'mycontent'); /Define_Tag;
//     Define_Tag('get_form'); return(self -> 'myform'); /Define_Tag;
//     Define_Tag('get_contentfile'); return(self -> 'contentfile'); /Define_Tag;
// 
//     Define_Tag('get_mainmenu',-optional='realmenu',-optional='theme');
// 	if (!(local_defined('realmenu')));
// 	    local('realmenu' = 'no');
// 	/if;
// 	if (!(local_defined('theme')));
// 	    local('theme' = 'wide');
//             // vertical menu with unfolding sub-items on hover
// 	/if;
//         local('test'=string);
// 	local('i'=integer);
// 	local('lcount'=integer);
// 	local('output' = string);
// 
//         // wide theme
// 
//         if(#theme == 'wide');
// 	    // #output += '<div id="mainmenu">\n';
// 	    if(#realmenu=='yes');
// 		#output += '<ul class="menulist" id="listMenuRoot">\n';
// 		else;
// 		#output += '<ul>\n';
// 	    /if;
// 	iterate( (self->'sitecatids'),#i);
// 	    #lcount += 1;
// 	    local('addresses'  = array);
// 	    local('names'      = array);
// 	    local('tooltips'   = array);
// 	    local('domains'    = array);
// 	    local('subdirs'    = array);
// 	    // get pages for each cat
// 	    inline( -search, $dbConfig, -table=$table_prefix + 'sitepages',
// 		    'catid' = #i,
// 		   -op='neq',
// 		   'draft' = 'on',
// 		    -sortfield = 'sortcode',
// 		    -maxrecords = all);
// 		if( found_count == 0); // if category is empty
//                   #addresses -> insert('#');
//                   #names -> insert(str('NO PAGES IN CATEGORY'));
//                   #tooltips -> insert(str('NO PAGES IN CATEGORY'));
//                   #domains -> insert($basicoptions -> (find('sitename')));
// 		else;
//                   records;
//                     #addresses -> insert(field('address'));
//                     #names -> insert(field('menuname' + MO_langSuffix));
//                     #tooltips -> insert(field('tooltip' + MO_langSuffix));
//                     if( field('domain') == 'on'); // use secure server
//                         #domains -> insert($secureServer);
//                     else;
//                         #domains -> insert($myServer);
//                     /if;
//                     if(self->'sitepath');
//                         #subdirs -> insert(self->'sitepath');
//                     else( field('subdir') ); 
//                         #subdirs -> insert(field('subdir'));
//                     else;
//                         #subdirs -> insert(string);
//                     /if;
//                   /records;
// 		/if;
// 	    /inline;
// 	    #output += $navig -> get_thismainmenu(
// 		-addresses  =#addresses,
// 		-names	    =#names,
// 		-tooltips   =#tooltips,
// 		-domains    =#domains,
// 		-subdirs   = #subdirs,
// 		-catid	    =#i,
// 		-lcount  = #lcount
// 		);
//             #test += #names;
//           /iterate;
//           #output += '</ul>\n';
//   //	#output += '<div style="clear:both;"></div>\n';
//   //	#output += '</div>\n';
// 
//         else(#theme == 'bottomfixed'); // used in hmtarkkitehdit.fi
// 
//         #output += '<ul class="menu_bottomfixed">';
// 	iterate( (self->'sitecatids'),#i);
// 	    #lcount += 1;
// 	    local('addresses'  = array);
// 	    local('names'      = array);
// 	    local('domains'    = array);
// 	    local('subdirs'    = array);
// 	    // get pages for each cat
// 	    inline( -search, $dbConfig, -table=$table_prefix + 'sitepages',
// 		    'catid' = #i,
// 		   -op='neq',
// 		   'draft' = 'on',
// 		    -sortfield = 'sortcode',
// 		    -maxrecords = 1); // NO subitems for this menu
// 		if( found_count == 0); // if category is empty
//                   #addresses -> insert('#');
//                   #names -> insert(str('NO PAGES IN CATEGORY'));
//                   #domains -> insert($basicoptions -> (find('sitename')));
// 		else;
//                   records;
//                     #addresses -> insert(field('address'));
//                     if(field('tooltip' + MO_langSuffix));
//                       //#names -> insert(field('tooltip' + MO_langSuffix));
//                       #names -> insert('test');
// 
//                     else;
//                       //#names -> insert(field('menuname' + MO_langSuffix));
//                       #names -> insert('test2');
//                     /if;
//                     if( (field('domain')) == 'on'); // use secure server
//                         #domains -> insert($secureServer);
//                     else;
//                         #domains -> insert($myServer);
//                     /if;
//                     if(self->'sitepath');
//                         #subdirs -> insert(self->'sitepath');
//                     else( field('subdir') ); 
//                         #subdirs -> insert(field('subdir'));
//                     else;
//                         #subdirs -> insert(string);
//                     /if;
//                   /records;
// 		/if;
// 	    /inline;
// 	    #output += $navig -> get_thismainmenu(
// 		-addresses = #addresses,
// 		-names	   = #names,
// 		-domains   = #domains,
// 		-subdirs   = #subdirs,
// 		-catid	   = #i,
// 		-lcount    = #lcount
// 		);
//             #test += #names;
//             if((#lcount <  self -> 'sitecatids' -> size)
//               && (#addresses -> last != '#')); // not empty cats
// //              #output += '&nbsp;&nbsp;|&nbsp;&nbsp;';
//             /if;
//           /iterate;
//           #output += '</ul>\n';
// 
//         /if;
// 
//  	return(#output);
//     /Define_Tag;
// 
//     Define_Tag('get_thismainmenu', 
// 	-required='addresses', 
// 	-required='names', 
// 	-optional='tooltips', 
// 	-required='domains', 
// 	-optional='subdirs', 
// 	-required='catid',
// 	-required='lcount');
// 	local('output'  =string);
//         local('liclass' =string);
// 
//         if(! local_defined('tooltips') );
//           local('tooltips') = array;
//         /if;
//         if(! local_defined('subdirs') );
//           local('subdirs') = array;
//         /if;
// 	if( #addresses != array('#') ); // if cat is not empty
// //	    if( #catid == $navig -> get_catid );
// //                #liclass += "selected ";
// //	    /if;
// //            if(#lcount <  self -> 'sitecatids' -> size);
// //                #liclass += "borderright ";
// //            /if;
// //            if(#liclass);
// //              #output += '<li class="' + #liclass + '">';
// //            else;
// //              #output += '<li>';
// //            /if;
//           // next if is for the menu-drawing js (folding) 
//           if(#addresses -> size == 1); // if cat is not empty
//             // for these items, make link
// 	    if(#catid == ($navig -> get_catid));
// 		#output += '<li class="selected">';
// 	    else;
// 		#output += '<li>';
// 	    /if;
// 	    #output += '<a href="' + #domains->get(1); 
//             if(#subdirs -> size);
//               if(#subdirs -> get(1));
//                 #output += '/' + #subdirs -> get(1); 
//               /if;
//             /if;
// 	    #output += '/' +  #addresses->get(1); 
//             if(#tooltips);
//               #output +=  '" title="' + #tooltips->get(1);  
//             /if;
//             #output +=  '" id="' + #catid + '">';
//             if( (self ->'sitecats') != array);
//                 #output += (self -> 'sitecats')->get(#lcount); 
//             /if;
// 	    #output += '</a>\n';
//           else; // there are subitems
// 	    if(#catid == ($navig -> get_catid));
// 		#output += '<li class="selected menucat">';
// 	    else;
// 		#output += '<li class="menucat">';
// 	    /if;
// 
// //	    // main level item
// //	    #output += '<a href="' + #domains->get(1); 
// //            if( #addresses -> get(1) -> contains('.html') );
// //              #output += '/' +  #addresses->get(1); 
// //              #output +=  '" title="' + #names->get(1) +  '">';
// //            else;
// //              #output += '/?p=' +  #addresses->get(1); 
// //              #output +=  '" title="' + #names->get(1) +  '">';
// //            /if;
// //	    #output +=   '<span class="menu-title">'; 
// //	       if( (self ->'sitecats') != array);
// //		   #output += (self -> 'sitecats')->get(#lcount); 
// //	       /if;
// //	       #output += '</span>';
// //	    #output += '</a>\n';
// 
// 	    // main level item
//             // edit 2011-05-15 nice urls with mod_rewrite
//             // edit 2011-05-15 tooltips added
// 		   #output += self -> 'sitecats' -> get(#lcount); 
// 
//             // sub-items
// //	    if( #addresses->size > 1);
// //		#output += '  <ul>\n';
// //		loop( #addresses->size);
// //		    #output += '    <li>';
// //		    #output += '<a href="' + #domains->get(loop_count); 
// //                      if( #addresses -> get(loop_count) -> contains('.html') );
// //                        #output += '/' +  #addresses->get(loop_count); 
// //                        #output += '" title="'+#names->get(loop_count)+'">';
// //                      else;
// //                        #output += '/?p=' +  #addresses->get(loop_count); 
// //                        #output += '" title="'+#names->get(loop_count)+'">';
// //                      /if;
// //                      #output += '<span class="submenuitem">'; 
// //                      if( #names != array);
// //                          #output += #names->get(loop_count); 
// //                      /if;
// //                      #output += '</span>';
// //		    #output += '</a>';
// //		    #output += '</li>\n';
// //		/loop;
// //		#output += '</ul>\n';
// //	    /if;
// 
//             // sub-items
// 	    if( #addresses->size > 1);
//               // a hack for folding js (use classes)
//               if(#catid == ($navig -> get_catid));
// 		#output += '\n  <ul>\n';
//               else;
// 		#output += '\n  <ul class="hidechildren">\n';
//               /if;
// 		loop( #addresses->size);
//                     if($myfilename == #addresses -> get(loop_count));
//                         #output += '    <li class="selected">';
//                     else;
//                         #output += '    <li>';
//                     /if;
// 		    #output += '<a href="' + #domains->get(loop_count); 
//                     if( #subdirs -> size >= loop_count );
//                       if( #subdirs -> get(loop_count) );
//                         #output += '/' + #subdirs -> get(loop_count); 
//                       /if;
//                     /if;
//                     if( #addresses -> get(loop_count) -> contains('.html') );
//                       #output += '/' +  #addresses->get(loop_count); 
//                       #output += '" title="'+#tooltips->get(loop_count)
//                         +'" id="' + #catid + '">';
//                     else;
//                       #output += '/' + #addresses->get(loop_count); 
//                       #output += '" title="x'+#tooltips->get(loop_count)+'">';
//                     /if;
//                     if( #names != array);
//                         #output += #names->get(loop_count); 
//                     /if;
// 		    #output += '</a>';
// 		    #output += '</li>\n';
// 		/loop;
// 		#output += '</ul>\n';
// 	    /if;
// 
// 	    #output += '</li>\n';
//           /if;
// 	/if; // end if cat is not empty
// 	return(#output);
//     /Define_Tag;
// 
//     Define_Tag('get_submenu',-required='catid',-optional='skip',-type='array');
// 	local('address'  = string);
// 	local('name'	    = string);
// 	local('domain'    = string);
// 	local('mypage' = (self -> 'mypage'));
// 	local('output'    = string);
// 	if (!(local_defined('skip')));
// 	    local('skip' = array);
// 	/if;
// 	    inline( -search, $dbConfig, -table=$table_prefix + 'sitepages',
// 		    -op='eq',
// 		    'catid' = #catid,
//                      -neq,
//                      'draft' = 'on',
// 		    -sortfield = 'sortcode');
// 		if( found_count > 1);
// 		    records;
//                       if( ! #skip -> find(loop_count) );
// 			#address    = Field('address');
// 			#name	    = Field('menuname' + MO_langSuffix);
// 			#domain	    = Field('domain');
// 			#output += $navig -> get_submenuitem(
// 			    -address	= #address,
// 			    -name	= #name,
// 			    -domain	= #domain,
// 			    -mypage	= #mypage
// 			    );	
//                       /if;
// 		    /records;
// 		/if;
// 	    /inline;
// 		return(#output);
// 		// return('jee');
//     /Define_Tag;
// 
//     Define_Tag('get_submenuitem',
// 	-req='address',
// 	-req='name',
// 	-req='domain',
// 	-req='mypage'
// 	);
// 	local('output' = string);
// 	If( #mypage == #address);
// 	    #output += '<p class="L1_selected">';
// 		    #output += #name;
// 	Else;
// 	    #output += '<p class="L1">';
// 	    #output += '<A HREF="index.html?p=' + #address + '">';
// 		    #output += #name;
// 	    #output += '</A>';
// 	/If;
// 	#output += '</p>\r';
// 	return(#output);
//     /Define_Tag;
// 
// /Define_Type;

// there is MO_composevaluelist in libmonster.inc
// what is the difference?
Define_Tag( 'JH_getValueList',
	-Required='mytable',
	-Required='myfield',
	-Optional='sortfield',
	-Optional='inlinestuff',-type='array',
	-Optional='format',     // pairs (= default),array,twoarrays,string 
	-Optional='separator',  // for strings
        -Optional='prependString'); // for pairs, for 1st element      
    // outputs a value list, which can be used elsewhere
    // usage: MO_getValueList($mytable,'myfieldname','sortfieldname',
    //    -format='pairs');

    local('error' = string);
    local('nfound' = integer);
    if (local_defined('format'));
      if(  #format == 'array');    local('out' = array);
      else(#format == 'twoarrays');local('out' = array);
      else(#format == 'pairs');    local('out' = array);
      else(#format == 'string');   local('out' = string);
      else;
           return('illegal input for format');
      /if;
    else; // use defaults
       local('format' = 'pairs'); // default value
       local('out'    = array);
    /if;
    if (! local_defined('separator'));
        local('separator' = ',');
    /if;
    if (! local_defined('sortfield'));
        local('sortfield' = #myfield);
    /if;
    if (! local_defined('prependString'));
        local('prependString' = string);
    /if;
    inline( -search, -database=$myDb, -table=$table_prefix + #myTable,
                -UserName=$dbUsername,-password=$dbPassword,
                -maxrecords='all',
                -KeyField='id',
                -Distinct,
                local('inlinestuff'),
                -sortfield=#sortfield);
	    #nfound = found_count;
	    if( error_msg != Error_NoError );
		#error = error_msg;
	    /if;

	local('subhea'=string);
	local('temp1'=array);
	local('temp2'=array);
	records;
	    if(#subhea != field(#myfield));
                if(#format == 'array');
                  #out-> insert(field(#myfield));
                else(#format == 'twoarrays');
                  #temp1-> insert(field('id'));
                  #temp2-> insert(field(#myfield));
                else(#format == 'pairs');
                  // check for prependString
                  local('i' = #prependString);
                  if(#i);
                    #out -> insert(#i+field('id') = field(#myfield));
                  else;
                    #out->  insert(field('id') = field(#myfield));
                  /if;
                else(#format == 'string');
                  #out+= field(#myfield);
                  #out+= #separator;
                /if;
	    /if;
	    #subhea=(field(#myfield));
	/records;
    /inline;
    if(#format == 'twoarrays');
      #out = array(#temp1,#temp2);
    /if;
    if(#out -> type == 'string');
      #out -> RemoveTrailing(#separator);
    /if;
    if(#error);
	Return('ERROR IN FUNCTION:' + #error);
//    else(#nfound == 0);
//	Return:'Nothing found!' 
//	    + ', Tbl: ' + #mytable
//	    + ', Sort: ' + #sortfield;
    else;
          Return(#out);
    /if;
/Define_Tag;

Define_Tag( 'JH_pairsToSelect',
	-Required='name',
	-Required='pairs',
	-Optional='currentid',
        -Optional='style',
        -encodeNone );
    local('output' = string);
    if (! local_defined('currentid') );
	local('currentid' = '');
    /if;
    // "pairs" should be a pair array
    // usage: 
    // MO_pairsToSelect('fieldname',$pairarray,'2');
    #output += '\r<select name=\"' + #name + '\" size=\"1\"';
    
    if(local_defined('style'));
      #output += ' ' + #style;
    /if; 
    #output += '>\n';
	local('i'=pair, 'lcount' = integer);
	iterate(#pairs,#i);
	    #lcount += 1;
	    #output += '<option value=\"' + #i->first + '\"';
	    If( #currentid == #i->first); 
		#output += ' SELECTED';
	    /If;
	    #output += '>';
	    #output += #i->second;
	    #output += '</option>\r';
	/iterate;
    #output += '</select>\r';
    Return(#output);
/Define_Tag;

// new version of order data

// Define_Type('SC_order');
//   local('err' = string);
//   local('test' = string);
// 
//   // orders table
//   local('ID' = string ); 
//   local('etunimi' = string ); 
//   local('sukunimi' = string ); 
//   local('yritys' = string ); 
//   local('katuosoite' = string ); 
//   local('postinumero' = string ); 
//   local('postitoimipaikka' = string ); 
//   local('maa' = string ); 
//   local('email' = string ); 
//   local('puh' = string ); 
//   local('matkapuh' = string ); 
//   local('perm' = string ); 
//   local('selain' = string ); 
//   local('luontipvm' = string ); 
//   local('komm' = string ); 
//   local('muokkauspvm' = string ); 
//   local('maksutapa' = string ); 
//   local('asiakasid' = string ); 
//   local('hetu' = string ); 
//   local('sivusto' = string ); 
//   local('status' = string ); 
//   local('verkkomaksuaika' = string ); 
//   local('verkkomaksuid' = string ); 
//   local('verkkomaksutapa' = string ); 
// 
//   // prod info
//   local('tuotenumerot' = array ); 
//   local('tuotenimet' = array ); 
//   local('tuotemaarat' = array ); 
//   local('tuotetarjoukset' = array ); 
//   local('tuotehinnat' = array ); 
// 
//   local('paino' = decimal ); 
//   local('toimituskulut' = decimal ); 
//   local('kokonaishinta' = decimal ); 
//   local('maksutapaviesti' = string ); 
// 
//   // define methods
//   Define_Tag('onCreate',-required='orderid');
//           inline( -search, $dbConfig, -table='orders',
//                    -op = 'eq',
//                    'id' = #orderid);
//                self -> 'test' = #orderid;
//                if(found_count == 1);
//                   self->'ID'      = field('ID'); 
//                   self->'etunimi' = field('etunimi'); 
//                   self->'sukunimi'= field('sukunimi'); 
//                   self->'yritys'      = field('yritys'); 
//                   self->'katuosoite'      = field('katuosoite'); 
//                   self->'postinumero'     = field('postinumero'); 
//                   self->'postitoimipaikka'= field('postitoimipaikka'); 
//                   self->'maa'     = field('maa'); 
//                   self->'email'       = field('email'); 
//                   self->'puh'     = field('puh'); 
//                   self->'matkapuh'     = field('matkapuh'); 
//                   self->'perm'        = field('perm'); 
//                   self->'selain'      = field('selain'); 
//                   self->'luontipvm'       = field('luontipvm'); 
//                   self->'komm'        = field('komm'); 
//                   self->'muokkauspvm'     = field('muokkauspvm'); 
//                   self->'maksutapa'       = field('maksutapa'); 
//                   self->'asiakasid'       = field('asiakasid'); 
//                   self->'hetu'        = field('hetu'); 
//                   self->'sivusto'     = field('sivusto'); 
//                   self->'status'      = field('status'); 
//                   self->'verkkomaksuaika'      = field('verkkomaksuaika'); 
//                   self->'verkkomaksuid'      = field('verkkomaksuid'); 
//                   self->'verkkomaksutapa'      = field('verkkomaksutapa');
//                /if;
//           /inline;
// 
//       // compose list of ordered items
//       inline( -search, $dbConfig, 
//           -table='orderitems',
//           'tilausid'=#orderid);
//           records;
//             (self -> 'tuotenumerot') -> insert( field('tuoteid'));
//             (self -> 'tuotemaarat') -> insert( field('tuotekpl'));
//           /records;
//       /inline;
// 
//       // get item info
//       local('rivihinta' = decimal);
//       local('kokonaishinta' = decimal);
//       Loop(self -> 'tuotenumerot' -> size);
//           inline( $dbConfig, -table = $myProdTable,
//                 -Op = 'eq',
//                 $myProdidField = (self -> 'tuotenumerot') -> Get(Loop_Count),
//                 -search);
//               // calculate paino
//                 if( field('annosyksikko') == 'g' );
//                   self -> 'paino' += decimal(field('annos'))
//                     * (self -> tuotemaarat -> get(loop_count));
//                 /if;
//               // product hame
//                 self -> 'tuotenimet' -> insert(field('nimi'));
//               // get the right price
//               local('tarj_apvm'= Date(Field('tarj_apvm')));
//               local('tarj_lpvm'= Date(Field('tarj_lpvm'))); 
//               If( #tarj_apvm <= Date && #tarj_lpvm >= Date);
//                   // tarjoushinta: kaikille
//                   (self -> 'tuotetarjoukset') -> insert(1);
//                   (self -> 'tuotehinnat')	 -> insert(Field('tarj_hinta'));
//                   #rivihinta = decimal(field('tarj_hinta')) 
//                     * (self -> tuotemaarat -> get(loop_count));
//               Else;
//                   (self -> 'tuotetarjoukset') -> insert(0);
//                   (self -> 'tuotehinnat')	 -> insert(Field('hinta'));
//                   #rivihinta = decimal(field('hinta')) 
//                     * (self -> tuotemaarat -> get(loop_count));
//               /If;
//               local('testvalue' = error_msg);
// 
//               #kokonaishinta = #kokonaishinta + #rivihinta;
//           /inline;
//       /loop;
//       self->'paino' -> SetFormat(-Precision=0,-DecimalChar=',');
// 
//       // set shipping & payment parameters
//       // self -> 'toimituskulut' = 5.00;  // default value
//       if(   self -> 'paino' <= 80);
//             self -> 'toimituskulut' = 4.50;  
//       else( self -> 'paino' <= 220);
//             self -> 'toimituskulut' = 6.00;  
//       else( self -> 'paino' <= 460);
//             self -> 'toimituskulut' = 8.00;  
//       else( self -> 'paino' <= 1700);
//             self -> 'toimituskulut' = 12.00;  
//       else( self -> 'paino' <= 4600);
//             self -> 'toimituskulut' = 14.00;  
//       else( self -> 'paino' <= 14500);
//             self -> 'toimituskulut' = 16.00;  
//       else;
//             self -> 'toimituskulut' = 18.00;  
//       /if; 
//       if( self -> 'maksutapa' == 'postiennakko' );
//             self -> 'toimituskulut' += 5.00;  
//       /if;
// 
//       If (self -> 'maksutapa' == 'ennakkolasku');
//         self -> 'maksutapaviesti' = 'postitse, ennakkolasku';
//       Else (self -> 'maksutapa' == 'postiennakko');
//         self -> 'maksutapaviesti' = 'postitse, maksetaan noudettaessa (postiennakko)';
//       Else( (self -> 'maksutapa') == 'verkkomaksu');
//         self -> 'maksutapaviesti' = 'postitse, verkkomaksu tilattaessa';
//       /If;
//     
//       #kokonaishinta += (self -> 'toimituskulut'); 
//       #kokonaishinta -> (SetFormat(-Precision=2,-DecimalChar=','));
//       self -> 'kokonaishinta' = #kokonaishinta;
//   /Define_Tag;
// 
//   // getters
//   // not needed!
//   //  Define_Tag('test');	Return( Self -> 'test'); /Define_Tag;
// 
//   Define_Tag( 'orderConfirmEmail',-Optional='receiver' );
//       local('out'	      = string);
//       local('kappalehinta'    = decimal);
//       local('rivihinta'	      = decimal);
//       local('kokonaishinta'   = decimal);
// 
//       local('toimituskulut'   = self -> 'toimituskulut');
//       #toimituskulut  -> SetFormat(-Precision=2,-DecimalChar=',');
// 
//       // receiver, allowed values: customer, vendor
//       if( ! Local_Defined:'receiver' );
//           local('receiver'='customer');
//       /if;
//       if( #receiver != 'customer' && #receiver != 'vendor');
//           #receiver == 'customer'; // illegal input, use default value
//       /if;
// 
//       if( #receiver == 'customer'); // not to vendor
//           #out = 'Kiitos tilauksestasi. Olet tilannut Hyötykasviyhdistykseltä '
//               + 'tuotteita seuraavasti:\n\n';
//       /if;
//       #out += 'Tilauksen numero: ' + (self -> 'id');
//       #out += '\n\nMaksutapa: ' + (self -> 'maksutapaviesti');
// 
//       #out += '\n\nTilaajan nimi: ' + (self -> 'etunimi') 
//               + ' ' + (self -> 'sukunimi');
//       // replace tag: \r removed from replace-string, 080602 jh
//       #out += '\nOsoite: ' 
//               + self -> 'katuosoite' + ', '
//               + self -> 'postinumero' + ' '
//               + self -> 'postitoimipaikka';
//       #out += '\nEmail: ' + (self -> 'email');
//       #out += '\nPuh: ' + (self -> 'puh');
//       #out += '\nKommentit: ' + (String_Replace: -Find='\r', -Replace='\n           ',(self -> 'komm'));
//       #out += '\n\nTilatut tuotteet:\n';
// 
//       // tilatut tuotteet
//       loop( (self -> 'tuotenumerot') -> size );
//           #out += '\n' + (self -> 'tuotenumerot') -> get(loop_count);
//           If((self -> 'tuotenimet') -> get(loop_count));
//             #out += ' (' + (self -> 'tuotenimet') -> get(loop_count) + ')';
//           /If;
//           If((self -> 'tuotetarjoukset') -> get(loop_count));
//               #out += ' (TARJOUSHINTA!)';
//           /If;
//           #out += '. ' + self -> 'tuotemaarat' -> get(loop_count) + ' kpl, ';
//               #kappalehinta = decimal(self -> 'tuotehinnat' -> get(loop_count));
//               #kappalehinta -> (SetFormat(-Precision=2,-DecimalChar=','));
//           #out += #kappalehinta;
//           #out += ' e/kpl, ';
//               #rivihinta = (decimal(self -> 'tuotehinnat' -> get(loop_count))) 
//                   * (self -> 'tuotemaarat' -> get(loop_count));
//               #rivihinta = decimal(#rivihinta);
//               #rivihinta -> SetFormat(-Precision=2,-DecimalChar=',');
//               #kokonaishinta = #kokonaishinta + #rivihinta;
//               #kokonaishinta = Decimal(#kokonaishinta);
//               #kokonaishinta -> SetFormat(-Precision=2,-DecimalChar=',');
//           #out += 'yht. ' + #rivihinta + ' e\n';
//       /loop;
// 
//       #out += '\nTuotteiden paino noin:: '; 
//       #out += self -> 'paino' + ' g'; 
//       #out += '\nToimituskulut: '; 
//       #out += #toimituskulut;
//           #kokonaishinta += self -> 'toimituskulut'; 
//           #kokonaishinta -> SetFormat(-Precision=2,-DecimalChar=',');
//       #out += '\n\nKaikki yhteensä: ' + #kokonaishinta + ' e';
// 
//       If( (self -> 'maksutapa') == 'l');
//           if( #receiver == 'customer');
//               #out += '\n\nTuotteet maksetaan mukana seuraavalla laskulla.';
//           else; // receiver is vendor
//               #out += '\n\nLaskun tiedot:\n';
//               #out += 'Summa ' + #kokonaishinta + ' e\n'; 
//               #out += 'Tilinumero Nordea 101530-30267\n';
//               #out += 'Viite ' + 
//                 (SC_bankReference(-basis = self -> 'id')) + '\n\n';
//           /if;
//       else( (self -> 'maksutapa') == 'ennakkolasku');
//           #out += '\n\nEnnakkolasku:\n';
//           #out += 'Summa ' + #kokonaishinta + ' e\n'; 
//           #out += 'Tilinumero MEIDÄN_PANKKI 123456-123456\n';
//           #out += 'Viite ' + 
//                 (SC_bankReference(-basis = self -> 'id')) + '\n\n';
//           #out += 'Toimitamme tuotteet postitse kun suoritus näkyy Hyötykasviyhdistyksen tilillä.';
//       else( (self -> 'maksutapa') == 'postiennakko');
//           #out += 'Toimitamme tuotteet postitse. ';
//           #out += '\n\nTuotteet maksetaan noudettaessa (postiennakko).';
//       else( (self -> 'maksutapa') == 'verkkomaksu');
//           #out += '\n\nTuotteet on maksettu verkkomaksuna. ';
//           #out += 'Toimitamme tuotteet postitse.';
//       /If;
//       Return(#out);
//   /Define_Tag;
// 
//   Define_Tag( 'orderDisplay');
//       local('out' = string);
//       local('kokonaishinta' = self -> 'kokonaishinta');
// 
//       #out += '<table BORDER="0" CELLPADDING="0" CELLSPACING="0" style="margin-left:auto;margin-right:auto;border:0px solid black;">\n';
//       #out += '<tr>\n';
//       #out += '<td>\n';
// 
//       // emb table tilaaja
//       #out += '<table id="tilaajantiedot" class="containstext" cellpadding="0" cellspacing="4" width="100%">\n';
//       #out += '<tr>\n';
//       #out += '<td colspan="2"><h2>Tilaajan tiedot</h2></td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td><b>Tilausnumero</b></td>\n';
//       #out += '<td>' + (self -> 'id') +  ' (';
//       // #out += (Date_Format((self -> 'luontipvm'),-Format="%d.%m.%Y"));
//       #out += self -> 'luontipvm';
//       #out += ')</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td><b>Maksutapa</b></td>\n';
//       #out += '<td>' + (self -> 'maksutapaviesti') + '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td><b>Tilaajan nimi</b></td>\n';
//       #out += '<td>' + (self -> 'etunimi') + ' ' + (self -> 'sukunimi') + '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td valign="top"><b>Osoite</b></td>\n';
//       #out += '<td>';
//       #out += (self -> 'katuosoite') + '<br>';
//       #out += (self -> 'postinumero') + ' ' + (self -> 'postitoimipaikka');
//       #out += '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td><b>Email</b></td>\n';
//       #out += '<td>'+ (self -> 'email') + '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td><b>Puh.</b></td>\n';
//       #out += '<td>' + (self -> 'puh') + '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td valign="top"><b>Tilaajan kommentit</b></td>\n';
//       #out += '<td>';
//       If((self -> 'komm'));
//           local('komm_temp' = self -> 'komm');
//           #komm_temp -> (replace('\r','<br>'));
//           #out += encode_smart(#komm_temp);
//       Else;
//           #out += '--';
//       /If;
//       #out += '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td>&nbsp;</td>\n';
//       #out += '<td>&nbsp;</td>\n';
//       #out += '</tr>\n';
//       #out += '</table>\n'; // end emb table tilaaja
// 
//       #out += '<!-- emb table tilatut -->\n';
//       #out += '<table class="containstext" cellpadding="0" cellspacing="4" width="100%">\n';
//       #out += '<tr>\n';
//       #out += '<td colspan="4"><h2>Tilatut tuotteet</h2></td>\n';
//       #out += '</tr>\n';
// 
//       // tilatut tuotteet
//       local('prodlines' = self -> orderDisplayProdlines);
// 
//       #out += #prodlines;
//       #out += '<tr>\n';
//       #out += '<td colspan="4" style="padding:10px 0 0 5px;">\n';
//       if( (self -> 'maksutapa') == 'l');
//           #out += 'Tuotteet maksetaan mukana seuraavalla laskulla.\n';
//       else( (self -> 'maksutapa') == 'ennakkolasku');
// 	#out += '<h2>Ennakkolasku</h2>\n';
//           #kokonaishinta -> (SetFormat(-Precision=2,-DecimalChar=','));
// 	#out += '<p>Summa ' + #kokonaishinta + ' €<br>\n';
// 	#out += 'Tilinumero Nordea 101530-30267<br>\n';
// 	#out += 'Viitenumero '+(SC_bankReference(-basis = self->'id'))+'<br>\n';
// 	#out += '&nbsp;<br>\n';
// 	#out += 'Toimitamme tuotteet postitse kun suoritus näkyy Hyötykasviyhdistyksen tilillä.</p>\n';
//       Else( (self -> 'maksutapa') == 'postiennakko');
//               #out += '<p>Toimitamme tuotteet postitse.\n';
//               #out += 'Tuotteet maksetaan noudettaessa (postiennakko).</p>\n';
//       Else( (self -> 'maksutapa') == 'verkkomaksu');
//           if (self->'status' == 'order_ok_waiting_for_payment');// ennen maksua
//               #out += '<p>Maksu suoritetaan verkkomaksuna tilauksen '
//                       + 'teon yhteydessä.';
//               #out += '&nbsp;<br>\n';
//               #out += 'Toimitamme tuotteet postitse kun suoritus 
//                 on hyväksytty.</p>\n';
//           else(self->'status' == 'order_ok');
//               #out += '<p>Verkkkomaksu on suoritettu. ';
//               #out += 'Toimitamme tuotteet postitse.</p>\n';
//           /if;
//       /If;
//       #out += '</td>\n';
//       #out += '</tr>\n';
//       #out += '</table>\n';
//       Return(#out);
//   /Define_Tag;
// 
// 
//   Define_Tag( 'orderDisplayProdlines', -Optional='admin'
//       );
//       local('out' = string);
//       local('kappalehinta' = decimal);
//       local('rivihinta' = decimal);
//       local('kokonaishinta' = decimal);
// 
//       local('toimituskulut'   = self -> 'toimituskulut');
//       #toimituskulut  -> SetFormat(-Precision=2,-DecimalChar=',');
// 
//       if(! local_defined('admin') );
//           local('admin' = 'no');
//       /if;
// 
//       loop( self -> 'tuotenumerot' -> size);
//           #out += '<tr>\n';
//           #out += '<td valign="top">\n';
//                   
//           if(#admin=='yes');
//               #out += '<a href="siepussit-form.html?tilnro=' 
//                   + (self -> 'tuotenumerot') -> get(loop_count) + '">';
//               #out += (self -> 'tuotenumerot') -> get(loop_count);
//               #out += '</a>';
//           else;
//               #out += (self -> 'tuotenumerot') -> get(loop_count);
//           /if;
//           #out += ' (';
//           #out += self -> 'tuotenimet' -> get(loop_count);
//           #out += ')';
//           If( self -> 'tuotetarjoukset' -> get(loop_count));
//               #out += ' (TARJOUSHINTA!)';
//           /If;
//           #out += '</td>\n';
//           #out += '<td valign="top" style="text-align:right;">\n';
//           #out += self -> 'tuotemaarat' -> get(loop_count);
//           #out += '&nbsp;kpl</td>\n';
//           #out += '<td valign="top" style="text-align:right;">\n';
//             #kappalehinta = decimal(self -> 'tuotehinnat' -> get(loop_count));
//             #kappalehinta -> SetFormat(-Precision=2,-DecimalChar=',');
//           #out += #kappalehinta;
//           #out += '&nbsp;€/kpl</td>\n';
//           #out += '<td valign="top" style="text-align:right;">';
//               #rivihinta = decimal(self -> 'tuotehinnat' -> get(loop_count)) 
//                   * (self -> 'tuotemaarat' -> get(loop_count));
//               #rivihinta = decimal(#rivihinta);
//               #rivihinta -> SetFormat(-Precision=2,-DecimalChar=',');
//               #kokonaishinta = #kokonaishinta + #rivihinta;
//               #kokonaishinta = Decimal(#kokonaishinta);
//               #kokonaishinta -> SetFormat(-Precision=2,-DecimalChar=',');
//           #out += 'Yht.&nbsp;' + #rivihinta + '&nbsp;€</td>\n';
//           #out += '</tr>\n';
//       /loop;
//       #out += '<tr>\n';
//       #out += '<td colspan="3">Tuotteiden paino (noin)</td>\n';
//       #out += '<td style="text-align:right;"> \n';
//       #out += self -> 'paino' + '&nbsp;g';
//       #out += '</td>\n';
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td colspan="3">Toimituskulut</td>\n';
//       #out += '<td style="text-align:right;"> \n';
//       #out += #toimituskulut + '&nbsp;€';
//       #out += '</td>\n';
//           #kokonaishinta += self -> 'toimituskulut'; 
//           #kokonaishinta -> SetFormat(-Precision=2,-DecimalChar=',');
//       #out += '</tr>\n';
//       #out += '<tr>\n';
//       #out += '<td colspan="3"><b>Kaikki yhteensä</b></td>\n';
//       #out += '<td style="text-align:right;"> \n';
//       #out += '<b>' + #kokonaishinta + '&nbsp;€</b></td>\n';
//       #out += '</tr>\n';
//       #out += '</table>\n'; // end emb table tilatut
//       #out += '</td></tr>\n';
//       Return(#out);
//   /Define_Tag;
// 
// /Define_Type;

Define_Tag('MO_speakBubble',-required='speakBubble',-optional='style',-encodeNone);
    local('output' = string);
    local('fromMetoRoot' = string);
    if(variable_defined('fromMetoRoot'));
      #fromMetoRoot = $fromMetoRoot;
    /if;
    local('speakimage' = $opts->find('siteurl') + '/images/speak.gif');
    if(! local_defined('style') );
      local('style' = string);
    /if;
    If( #speakBubble);
	#output += '<div class="speakBubble">\r';
	    #output += '<div class="speakBubbleImage">' + '<img src="' + #speakimage; 
	    #output += '" alt="speak" width="32" height="32"></div>\r';
	    #output += '<div class="speakBubbleText" style="'+#style+'">'; 
	    #output += #speakBubble; 
	    #output += '</div>\r';
	#output += '</div>\r';
	#output += '<div style="clear:both;">';
	#output += '</div>';
    /If;			// end if #speakBubble
    var('speakBubble' = '');
    Return( #output);
/Define_Tag;

// commented out for LassoBlogger
// 
// define_tag('horizontalmenu',-namespace='JH',
//                             -required='pairs',
//                             -required='current',-type='string',
//                             -encodeNone);
// /* example use
// JH_horizontalmenu(-pairs=('logout.html'=str('Logout')),-current=$myfilename);
// */
//   local('out' = string);
//   local('i' = array);
//   iterate(#pairs,#i);
//     if(#i->first == #current);
//         #out += '<b>' + #i->second + '</b>';
//     else;
//       #out += '<a href="' + #i->first + '">';
//         #out += #i->second;
//       #out += '</a>';
//     /if;
//     if(loop_count < #pairs -> size);
//       #out += '&nbsp;&nbsp;|&nbsp;&nbsp;';
//     /if;
//   /iterate;
//   return(#out);
// /define_tag;
// define_tag('horizontalmenuv2',-namespace='JH',
//                             -required='in',-type='array',
//                             -required='current',-type='string',
//                             -optional='separator',-type='string',
//                             -encodeNone);
// /* example use
//   JH_horizontalmenuv2(-in=('logout.html'=str('Logout')),-current=$myfilename);
// */
//   local('out' = string);
//   local('i'   = array);
//   if(! local_defined('separator'));
//     local('separator' = '&nbsp;|&nbsp;');
//   /if;
// 
//   iterate(#in,#i);
//       // zap the locals
//       local('href'        = string); // required
//       local('text'        = string); // required
//       local('href_addons' = string);
//       local('title'       = string);
//       local('id'          = string);
//       local('li_style'    = string);
//       local('href_style'  = string);
//       local('href_total'  = string);
//       local('target'      = string);
// 
//     // define locals
//     if(#i -> find('href'));
//       #href       = #i -> find('href') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('href_addons'));
//       #href_addons= #i -> find('href_addons') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('title'));
//       #title      = #i -> find('title') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('id'));
//       #id       = #i -> find('id') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('li_style'));
//       #li_style    = #i -> find('li_style') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('href_style'));
//       #href_style    = #i -> find('href_style') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('text'));
//       #text    = #i -> find('text') -> get(1) -> get(2);
//     /if;
//     if(#i -> find('target'));
//       #target    = #i -> find('target') -> get(1) -> get(2);
//     /if;
// 
//     // define href_total
//     #href_total = #href;
//     if(#href_addons);
//       #href_total += '?' + #href_addons;
//     /if;
// 
//     if(#href_total == #current); // no link needed
//         #out += '<li><b>' + #text + '</b></li>';
//     else;
//       #out += '<li';
//       if(#li_style);
//         #out += ' style="' + #li_style + '"';
//       /if;
//         #out += '><a href="' + #href;
//         if(#href_addons); #out += '?' + #href_addons;         /if;
//       #out += '" ';
//         if(#id);        #out += 'id="' + #id + '" ';      /if;
//         if(#href_style);     #out += 'style="' + #href_style + '" '; /if;
//         if(#title);      #out += 'title="'  + #title + '" ';  /if;
//         if(#target);     #out += 'target="' + #target + '" '; /if;
//       #out += '>';
//         #out += #text;
//       #out += '</a></li>';
//     /if;
//     if(loop_count < #in -> size);
//       #out += #separator;
//     /if;
//   /iterate;
//   return(#out);
// /define_tag;

define_tag('ajaxvalidatejs',-namespace='JH',
                            -required='editfields',-type='array',
                            -encodeNone);
/* example use
  JH_ajaxvalidate_js($editfields);
*/
  local('out' = string);
  local('responseFilepath' = 'ajax_validate.lasso');
  local('i'   = array);
    iterate(#editfields,#i);
      local('fname'    = #i -> get(1));
      local('valtypes' = #i -> get(4));
      local('helptext' = #i -> get(7));

      if(#valtypes -> find('companyid') 
        || #valtypes -> find('email') 
        || #valtypes -> find('password')
        || #valtypes -> find('unique')
        || #valtypes -> find('date')
        || #valtypes -> find('datetime')
        || #valtypes -> find('time')
        || #valtypes -> find('int')
      );
      #out += ("$('.input-" + #fname + "').bind('change', function() {validateit" + loop_count + "($(this).val())});\n"); 
      #out += "function validateit" + loop_count + "(myvalue) {\n"; 
        #out += ("$('#help-" + #fname + "').load('"
          + #responseFilePath + "',{fname:'"
          + #fname + "',value:myvalue,label:'"
          + #i->get(2) + "',valtypes:'" 
          + #valtypes + "',helptext:'" 
          + #helptext + "'} );\n"); 
      #out += "}\n"; 
      /if;
    /iterate;
  return(#out);
/define_tag;

define_tag('readable',-namespace='MO',-required='array',-encodenone);
/*
usage - MO_readable(action_params);
input has to be pair array or a map
*/
    local('out' = string);
    local('i' = array);
    iterate(#array, #i); 
        #out += (#i -> First);
        #out += ' = "'; 
        #out += (#i -> Second);
        #out += '"<br>';
    /iterate;

  return(#out);
/define_tag;

define_tag('shorten',-namespace='MO',
  -required='string',-type='string',
  -required='maxlength',-type='integer',
  -encodenone);
/*
usage - MO_shorten('mytext',10);
*/
    local('out' = string);
    if(#string -> size > (#maxlength - 3));
      #out = #string -> substring(1,#maxlength-3) + '...';
    else;
      #out = #string;
    /if;
  return(#out);
/define_tag;

Define_tag('naviglinks',-namespace='MO',
  -optional='linkstuff',
  -optional='textclass',
  -optional='fromMetoRoot',
  -optional='rectype',
  -optional='rectype_pl',
  -optional='found',
  -optional='shown',
  -optional='max',
  -optional='shownfirst',
  -optional='shownlast',
  -encodeNone);
  local('out' = string);

/*
example usage
MO_naviglinks(
  -frommetoroot=$fromMetoRoot,
  -linkstuff=$linkstuff,
  -textclass=$textclass);
*/
  ! local_defined('linkstuff')    ? local('linkstuff' = string);
  ! local_defined('textclass')    ? local('textclass' = string);
  ! local_defined('fromMetoRoot') ? local('fromMetoRoot' = string);
  ! local_defined('rectype')      ? local('rectype' = string);
  ! local_defined('rectype_pl')   ? local('rectype_pl' = string);
  ! local_defined('found')        ? local('found' = found_count);
  ! local_defined('shown')        ? local('shown' = shown_count);
  ! local_defined('max')          ? local('max' = maxrecords_value);
  ! local_defined('shownfirst')   ? local('shownfirst' = shown_first);
  ! local_defined('shownlast')    ? local('shownlast' = shown_last);

  local('mathsub'=Math_Sub(shown_first,maxrecords_value,1));
  local('arrowleft' = '<IMG SRC="' + $fromMetoRoot + 'images/arrowleft.gif" '
      + 'ALT="'+str('Previous page')
      + '" BORDER="0" align="middle" style="width:17px;height:15px;">');
  local('arrowright' = '<IMG SRC="' + $fromMetoRoot 
      + 'images/arrowright.gif" ALT="' + str('Next page') + '"'
      + ' BORDER="0" style="width:17px;height:15px;">');

  #out += '<div id="naviglinks" class="' + #textClass + '">\n';

  // #1 // "n records found"

  #out += '<div id="naviglinks_upper">\n';
  //       If($lang == 'fi');
  //         #out += 'Löytyi ';
  //         #out += '<em>' + string(found_count) + '</em> ';
  //         #out += 'tietue'; 
  //         #out += (found_count > 1 ? 'tta.');
  //       Else;
  //         #out += '<em>' + string(found_count) + '</em> ';
  //         #out += (found_count > 1 ? #rectype_pl+' were' | #rectype+' was');
  //         #out += ' found.';
  //       /If;
  #out += '</div>\n';


  // #2 links to prev & next page

  If( Shown_First > 1 || Shown_count < Found_Count );  // some links are needed

  #out += '<div id="naviglinks_lower">';

  // #2-1 Left cell

  #out += '<div id="naviglinks_lowleft">\n';
    if(shown_first > 1);
      #out += #arrowleft;
      #out += ' <a href="' + $myfilename + '?ref=naviglinks&amp;';
        #out += $linkstuff + '&amp;skip=' + #mathsub+'">\n';
        #out += str('Previous page');
      #out += '</a>';
    /if;
  #out += '&nbsp;</div>\n';

  // #2-2 Middle cell

  #out += '<div id="naviglinks_lowmid">\n';
  //   Loop(-to=Found_Count,-by=MaxRecords_Value);
  //     // show 5 before ja 5 after
  //     If( (Loop_Count >= Shown_First - (5 * MaxRecords_Value))
  //      && (Loop_Count <= Shown_First + (5 * MaxRecords_Value)));
  //         If( Loop_Count != Shown_First);
  //           #out += '<A HREF=\"' + $myfilename + '?ref=naviglinks&amp;' + $linkstuff;
  //           #out += 'skip=' + (loop_count - 1) + '\">' + loop_count + '</A>';
  //         Else;
  //           #out += '<em>' + string(loop_count) + '</em>';
  //         /If;
  //         If( loop_count != found_count); // if not last
  //           #out += '&nbsp;&#124;&nbsp;';
  //         /If;
  //     /If;
  //   /Loop;
  #out += '</div>\n';

  // #2-3 Right cell

  #out += '<div id="naviglinks_lowright">\n&nbsp;';
    if( shown_last != found_count );
      #out += ('<a href="' + $myfilename + '?ref=naviglinks&amp;' + $linkstuff);
      #out += ('&amp;skip=' + shown_last + '">');
      #out += str('Next page');
      #out += (#arrowright + '</a> ');
    /if;
  #out += '</div>\n'; // end naviglinks_lowright
  #out += '</div>\n'; // end naviglinks_low

  Else; //  found n recs is shown but no links -> draw a line maybe?
  /If;  //  end if found_count

  #out += '</div>'; // end naviglinks
  #out += '<div style="clear:both;">\n';
  #out += '</div>\n';
  #out += '<div STYLE="clear:both;padding-bottom:3px;margin:0 auto 6px auto;width:100%; height:3px;border-bottom:1px solid #999;">';
  #out += '</div>\n';
  return(#out);
/Define_Tag;

Define_tag('collectUploaddata',-namespace='MO',
  -optional='table',-type='string',
  -optional='postid',-type='integer',
  -optional='filecat',-type='integer',
  -optional='sortstuff',-type='array',
  -encodeNone);
  local('out'      = string);
  local('divclass' = string);
  local('last_divclass' = string);
/*
old tag name collectImageData
example usage
  var('files') = MO_collectUploadData(-table='events',
                       -postid=integer(field('id')));
*/
  if(! local_defined('table'));
    local('table' = string);
  /if;
  if(! local_defined('postid'));
    local('postid' = integer);
  /if;
  if(! local_defined('filecat'));
    local('filecat' = integer);
  /if;
  if(! local_defined('sortstuff'));
    local('sortstuff' = array);
  /if;

  if(#table);
    local('stuff') = array('-eq','upl_posttable'=#table);
  else; // find filevault files /non-post-related
    local('stuff') = array('-gt','upl_catid'='0');
  /if;
  if(#postid);
    #stuff -> insert('-eq');
    #stuff -> insert('upl_postid' = #postid);
  /if;
  if(#filecat);
    #stuff -> insert('-eq');
    #stuff -> insert('upl_catid' = #filecat);
  /if;
       // collect image data
        local('filedata' = Array);
          inline( $dbConfig, -table=$table_prefix+'uploads',
                #stuff,
                #sortstuff,
                -search);
// return('diagnostics ' + error_currenterror 
//   + ', ' + found_count
//   + ', table is ' + #table
//   + ', postid is ' + #postid
//   );
          records;
              // collect image data
              #filedata -> insert(array(
                field('upl_path'),
                field('upl_name'),
                field('upl_description'),
                field('upl_imagedisplay'),
                field('upl_linkurl')
              ));
          /records;
          /inline;

          // prepare for iterating filedata
          local('suffix' 	    = string);
          local('linked_files'= '<p>');
          local('shown_files' = string);
          local('shown_addrs' = array);
          local('i' 	    = array);
          local('basepath'    = $upload_path + $upload_subpath );
          if(#postid);
            #basepath += 'post-related/';
          else;
            #basepath += 'filevault/';
          /if;

          Iterate( #filedata, #i);
              local('paths'= #i -> get(1)); // is array
              local('name' = #i -> get(2));
              local('desc' = #i -> get(3));
              local('disp' = #i -> get(4));
              local('url'  = #i -> get(5));
              local('display_size'  = string);
              local('linked_size'   = string);
              local('atag'          = string);
              #suffix = #name  -> split('.') -> Last;

              // get image logic
              local('imagelogic') = MO_imagelogic(#paths,#disp);
              // return('imagelogic is ' + #imagelogic);
              #display_size = #imagelogic->find('display_size');
              #divclass     = #imagelogic->find('divclass');
              #linked_size  = #imagelogic->find('linked_size');

              if(  #disp == 'none'); // just skip the file
              else( (#suffix == 'jpg' 
                      ||  #suffix == 'jpeg'
                      ||  #suffix == 'gif'
                      ||  #suffix == 'png') 
                      &&  #disp != 'link'); // display that img

                  // start handling div
                  if( ! #shown_files ); // first image to be shown
                    #shown_files += '<div class="' + #divclass + '">\n';
                  else( #divclass != #last_divclass ); // switch div
                    #shown_files += '</div>\n<!--' + #last_divclass + '-->';
                    #shown_files += '<div class="' + #divclass + '">\n';
                    // the (last) div will be closed after iterate
                  /if;
                  // end handling div

                  // compose a tag, if any
                  if( #url );
                      #atag += '<a href="';
                      if(   #url  -> beginsWith('http'));
                          #atag += #url + '">';
                      else( #url  -> beginsWith('/'));
                          #atag += #url + '">';
                      /if;
                  else(#linked_size);
                      #atag += '<a href="' + #basepath + #linked_size;
                      #atag += '/' + #name + '">';
                  /if;
                  #shown_files += #atag;
                  // end compose a a tag

                  local('subdir') = string;
                    // orig-size pics are saved at root level
                  #subdir = #display_size;
                  #subdir ? #subdir += '/';

                  #shown_files += '<img src="';
                  #shown_files += #basepath + #subdir + #name + '" alt="';
                  #shown_addrs -> insert(#basepath+#subdir+#name);
                  #shown_files += #desc + '" title="';
                  #shown_files += #desc + '">\r';
                 // close the a tag?
                  if( #url || #linked_size);
                      #shown_files += '</a>';                
                  /if;
                  // memorize divclass; string ensures that var is copied in L9
                  #last_divclass = string(#divclass); 
              else; 	  // just display link
                  #linked_files += ('<A HREF="' +  #basepath 
                    + #linked_size + '/' + #name);
                  #linked_files += '" TITLE="' +  #desc;
                  #linked_files += '">' + #name + '</A><br>\r';
              /if;
          /iterate;
          if(#shown_files);
            #shown_files += '</div>\n'; // close the (last) div
          /if;
  return(array(#shown_files,#linked_files,#shown_addrs));
/define_tag;
define_tag('imagelogic',-namespace='MO',
        -required='paths',
        -required='display_on_page');
  local('out' = map);
  if( #display_on_page == 'right');
      if(#paths >> 'halfcolumn');
          #out -> insert('display_size' = 'halfcolumn');
          #out -> insert('divclass' = 'alignright');
          if(#paths >> 'fullscreen');
              #out -> insert('linked_size' = 'fullscreen');
          else(#paths >> 'fullcolumn');
              #out -> insert('linked_size' = 'fullcolumn');
          /if;
      else(#paths >> 'original');
          #out -> insert('display_size' = ''); // file is saved at root level
          #out -> insert('divclass' = 'alignright');
      else(#paths >> 'fullcolumn');
              #out -> insert('display_size' = 'fullcolumn');
              #out -> insert('divclass' = 'centered');
      else; // could be image from an earlier LB version
          #out -> insert('display_size' = '');
          #out -> insert('divclass' = 'alignright');
      /if;
  else( #display_on_page == 'left');
      if(#paths >> 'halfcolumn');
          #out -> insert('display_size' = 'halfcolumn');
          #out -> insert('divclass' = 'alignleft');
          if(#paths >> 'fullscreen');
              #out -> insert('linked_size' = 'fullscreen');
          else(#paths >> 'fullcolumn');
              #out -> insert('linked_size' = 'fullcolumn');
          /if;
      else(#paths >> 'original');
          #out -> insert('display_size' = '');
          #out -> insert('divclass' = 'alignright');
      else(#paths >> 'fullcolumn');
              #out -> insert('display_size' = 'fullcolumn');
              #out -> insert('divclass' = 'centered');
      else; // could be image from an earlier LB version
            #out -> insert('display_size' = '');
            #out -> insert('divclass' = 'alignleft');
      /if;
  else( #display_on_page == 'block');
      if(#paths >> 'original');
          #out -> insert('display_size' = '');
      else(#paths >> 'fullcolumn');
          #out -> insert('display_size' = 'fullcolumn');
      /if;
      #out -> insert('divclass' = 'centered');
  else( #display_on_page == 'link');
      if(#paths >> 'original');
          #out -> insert('linked_size' = '');
      else;
          #out -> insert('linked_size' = #paths->split(', ')->last);
      /if;
  /if;
  return(#out);
/define_tag;


// // tag to write options
Define_Tag('LB_writeOptions');
    local('err' = string);
    local('out' = string);
    local('optfile') = 'inc/options.inc';
      #out += '[\nDefine_type(\'LB_myoptions\');\n';
        inline( -findall, $dbConfig,
          -table = $table_prefix+'options');
          records;
            #out += '  define_tag(\'' + field('opt_name')  + '\');\n';
            #out += '    return(\''   + field('opt_value') + '\');\n';
            #out += '  /define_tag;\n';
          /records;
        /inline;
      #out += '/define_type;\n]\n';
      file_write(#optfile,#out,-fileoverwrite);
      if(file_currenterror != 'Noerror');
        return(file_currenterror);
      else;
        return(string);
      /if;
/define_tag;

Define_Tag( 'MO_errorhandler',-Optional='level',-encodeNone);
    // level - normal (= default) or debug
    Local('output'       = string,
	   'speakbubble' = string
    );
    If( !(Local_Defined('level')));
	Local('level' = 'normal');
    /If;
    if(error_code);
	If( error_msg != (Error_NoError));
	    If(  error_code == '-1278'
	      || (error_code == 0 && found_count == 0)
	      || (error_code == '26279936'));

	      // no records error
	      // first condition above is for FMP, second for MySQL

		#speakBubble += 'No records were found.';
	    Else;
		#speakBubble += 'An error occurred. More info: "' +
		    error_code + '", ' + error_msg;
		// other errors
	    /If;
	/If;
    else(error_code);
        #speakBubble += 'File tag error: ' + (File_CurrentError);
    /if;
    #output += MO_speakBubble(#speakBubble);
    If( #level == 'debug');
      #output += '<b>inputvars</b> = ' + $inputvars + '<br>';
      #output += '<b>inlinestuff</b> = ' + $inlinestuff + '<br>';
      #output += '<b>searchstuff_temp</b> = ' + $searchstuff_temp + '<br>';
      #output += '<b>sortstuff</b> = ' + $sortstuff + '<br>';
      #output += 'found = ' + found_count + '<br>';
      #output += '<b>db</b> = ' + $myDb + '<br>';
      #output += '<b>table</b> = ' + $myTable + '<br>';
      #output += '<b>action</b> = ' + $action + '<br>';
      #output += '<b>action params</b> = ' + action_params + '<br>';

    /If;
    Return(local('output'));
/Define_Tag;

Define_Tag( 'LB_sidebarItem',-required='href',-required='label',-optional='afterlink',-encodeNone);
  local('out') = string;
  ! local_defined('afterlink') ? local('afterlink') = string;
          #out += '<li>';
          #out += '<a href="'+#href+'" title="'+#label+'">'+#label+'</a>';
          #out += #afterlink;
          #out += '</li>\n';
  return(#out);
/Define_Tag;






?>

