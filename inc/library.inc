<?LassoScript
Variable:'lb_version' = '0.9.1'; // June 19 2009

// $lang = 'fi'; // for testing

// string handler, the most important function
// it searches for translations, if lang not English

// but first include the lang file

    Include: $fromMetoRoot + 'languages/' + $lang + '.inc';

Define_Tag: 'str',-Required='str';
    Local:'output' = '';
	If: $lang == 'en';
	    #output = #str;
	Else;
	    if: $mTranslations -> (Find:#str);
		#output = $mTranslations -> (Find:#str); // see language file
		else;
		#output = #str;
	    /if;
       /If;
    Return:#output;
/Define_Tag;

// special treatment for strings that are empty in english, but not in some other language

Define_Tag: 'emptystrID',-Required='strID';
    Local:'output' = '';
	If: $lang == 'en';
	    #output = '';
	Else;
	    #output = $mEmptyString -> (Get:#strID) -> Second;
       /If;
    Return:#output;
/Define_Tag;

Define_Tag: 'LB_titleCase',-Required='str';
    Local:'output' = '';
    Local:'aStr' = #str -> (Split:' ');
    Local:'aStr2' = (Array);
    Local:'i' = '';
    Local:'iFirst' = '';
    Local:'iRest' = ''; 
    Iterate:#aStr,#i;
        #iFirst = #i -> (Get:1);
        #iFirst -> Uppercase;
        #iRest  = #i;
        #iRest -> (Remove:1,1);
        #aStr2 -> (Insert: (#iFirst + #iRest));
    /Iterate;
        #output = #aStr2 -> (Join:' ');
    Return:#output;
/Define_Tag;


// format dates

Define_Tag: 'LB_formatDateTime',-Optional='datetime';
    Local:'output' = '';
    Local:'date_format' = (LB_basicOptions -> 'date_format');
    If: #date_format == '';
    #date_format = '%m/%d/%y';
    /If;
    Local:'time_format' = (LB_basicOptions -> 'time_format');
    If: #time_format == '';
    #time_format = '%H:%M:%S';
    /If;
	    If: #datetime != '0000-00-00 00:00:00' && #datetime != '';
 	    #output = (Date_Format:(Date:#datetime,-Format=$date_dbformat),
		      -Format=(#date_format + ' ' + #time_format));
Else;
#output = 'No date';
 	       /If;
            Return:#output;
/Define_Tag;

// make hash for cookies

Define_Tag:'LB_createCookiehash';
    Local:'siteurl' =  (LB_basicOptions -> 'siteurl');
    If: #siteurl == '';
        Local:'pathForCookie' = $pathTome;
        Else;
        Local:'pathForCookie' = #siteurl;
    /If;
    Return:(Encrypt_MD5:#pathForCookie);
/Define_Tag;

// recognize user

Define_Type:'LB_Userinfo';
	Local:'inlinestuff' = (Array);
	Local:'use_level' = '';
	Local:'user_id' = '';
	Local:'cusername' = '';
	Local:'cpassword' = '';
	Local:'err' = '';
	Local:'found' = (Integer);
    Define_Tag:'onCreate';
	Local:'hash' = (LB_createCookiehash);
	If: (Cookie:('bloguser_' + #hash)) && (Cookie:('blogpass_' + #hash));
	// use cookies
	    (Self -> 'inlinestuff') -> (Insert:-operator = 'eq');
	    (Self -> 'inlinestuff') -> 
	      (Insert:'use_login' = (Cookie:('bloguser_' + #hash)));
	    (Self -> 'inlinestuff') -> (Insert:-operator = 'eq');
	    (Self -> 'inlinestuff') -> 
	      (Insert:'use_pass' = (Cookie:('blogpass_' + #hash)));
	Else: (Variable_Defined:'user_ses');  
	// use session vars (probably never used, as public pages
	// don't use session)
	    (Self -> 'inlinestuff') -> (Insert:-operator = 'eq');
	    (Self -> 'inlinestuff') -> (Insert:'id' = $user_ses);
	/If;

	If: (Self -> 'inlinestuff') != (Array);
	// verify the user data
	    Inline: -search,$dbConfig, 
		-table = $table_prefix+'users',
		(Self -> 'inlinestuff');

		If: (Error_CurrentError)==(Error_NoError)
		    && (Found_Count) == 1;
		    (Self -> 'cusername') = (Field:'use_login');
		    (Self -> 'cpassword') = (Field:'use_pass');
		    (Self -> 'use_level') = (Field:'use_level');
		    (Self -> 'user_id')   = (Field:'id');
		Else;
		    (Self -> 'use_level') = (-2); // no match?
		/If;
		(Self->'err') = (Field:'use_login') + ', ' + (Field:'use_pass');
		(Self->'found') = (Found_Count);
	    /Inline;
	Else;				
	    (Self -> 'use_level') = (-1); 	// no cookies, no session
	    (Self->'err') = (Cookie:('bloguser_' + #hash));
	/If;
    /Define_Tag;

    Define_Tag:'LB_getUse_level';
	Return: (Self -> 'use_level');
    /Define_Tag;
    Define_Tag:'LB_getUser_id';
	Return: (Self -> 'user_id');
    /Define_Tag;
    Define_Tag:'LB_getErr';
	Return: (Self -> 'err');
    /Define_Tag;

/Define_Type;

// get the basic options

Define_Type:'LB_basicOptions';
	Local:'err' = '';
	Local:'basicOptions'=(Array:
	    'siteurl',
	    'blogname',
	    'admin_email',
	    'date_format',
	    'time_format',
	    'blogdescription',
	    'permalink_structure',
	    'use_fileupload',
	    'www_use_summary',
	    'use_smilies',
	    'fileupload_realpath'
	    );
	// NOTE: you have to enter optnames manually anyway 
	// to the select statement below

	// init vars & make inlinestuff
	Local:'inlinestuff' = (Array);
	Iterate:#basicOptions,(Local:'i');
	Local: #i = '';
	#inlinestuff -> (Insert:'-Op' = 'eq');
	#inlinestuff -> (Insert:'opt_name' = #i);
	/Iterate;

    Define_Tag:'onCreate';
    Inline: -search, $dbConfig,
	-table = $table_prefix+'options',
	-KeyField='id',
	-responseField = 'opt_name',
	-responseField = 'opt_value',
	-LogicalOp = 'or',
	(Self -> 'inlinestuff')
    ;
	If: (Error_CurrentError)==(Error_NoError)
	    && (Found_Count) > 0; 
	    Records;
	    Select: (Field:'opt_name');
	    Case:'siteurl';	(Self -> 'siteurl') = (Field:'opt_value');
	    Case:'blogname';	(Self -> 'blogname') = (Field:'opt_value');
	    Case:'blogdescription';(Self->'blogdescription')=(Field:'opt_value');
	    Case:'admin_email';	(Self -> 'admin_email') = (Field:'opt_value');
	    Case:'date_format';	(Self -> 'date_format') = (Field:'opt_value');
	    Case:'time_format';	(Self -> 'time_format') = (Field:'opt_value');
	    Case:'permalink_structure';	(Self -> 'permalink_structure') = (Field:'opt_value');
	    Case:'use_fileupload';	(Self -> 'use_fileupload') = (Field:'opt_value');
	    Case:'www_use_summary';	(Self -> 'www_use_summary') = (Field:'opt_value');
	    Case:'use_smilies';	(Self -> 'use_smilies') = (Field:'opt_value');
	    Case:'fileupload_realpath';	(Self -> 'fileupload_realpath') = (Field:'opt_value');
	    /Select;
	    (Self -> 'err')  += (Field:'opt_name');
	    /Records;
	Else;
	/If;
    // (Self->'err') = (Found_Count);
    /Inline;
    /Define_Tag;

    Define_Tag:'LB_getBlogname';
	Return: (Self -> 'blogname');
    /Define_Tag;
    Define_Tag:'LB_getErr';
	Return: (Self -> 'err');
    /Define_Tag;
/Define_Type;

// get absolute path based on 'siteurl' option

Define_Tag: 'LB_getAbsolutepath';
    Local:'siteurl' = LB_Basicoptions -> 'siteurl';
    Local:'aPath' = #siteurl -> (Split:'/');
    Local:'sPath' = string;
    If: #aPath -> Size > 4;
	Loop: (#aPath -> Size) - 4;
	    #sPath += '/' + (#aPath -> (Get:(3 +(Loop_Count))));
	/Loop;
	    #sPath += '/';
    Else;
	#sPath = '/';
    /If;
    Return: #sPath;
/Define_Tag;

// get option by opt_name

Define_Tag: 'LB_getOption',-Required='optionName';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,$dbConfig,
		-Table=$table_prefix+'options',
                'opt_name'=#optionName;
		#output = (Field:'opt_value');
                #errorCode = (Error_CurrentError:-ErrorCode);
            /Inline;
            Return:#output;
/Define_Tag;

// get author by ID

Define_Tag: 'LB_getAuthor',-Required='id';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,$dbConfig,
		-Table=$table_prefix+'users',
                'id'=#id;
		#output = (Field:'use_nickname');
                #errorCode = (Error_CurrentError:-ErrorCode);
            /Inline;
            Return:#output;
/Define_Tag;

// get cat by ID

Define_Tag: 'LB_getCategory',-Required='id';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,$dbConfig,-Table=$table_prefix+'categories',
                'id'=#id;
		#output = (Field:'cat_name');
                #errorCode = (Error_CurrentError:-ErrorCode);
            /Inline;
            Return:#output;
/Define_Tag;

// get commentlink

Define_Tag: 'LB_getCommentlink',-Required='id';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,$dbConfig,
		-Table=$table_prefix+'comments',
                'com_post_id'=#id,
		'com_approved'='1',
		-ReturnField='id';
		If: (Found_Count) > 0;
		#output = (Found_Count) + ' ';
		    If: (Found_Count) == 1;
			#output += (str:'comment');
			Else: (Found_Count) > 1;
			#output += (str:'comments');
		    /If;
		#output += '  &#187;';
		Else;
		    If: $pos_status == 'publish';
			#output = (str:'Be the first to comment this post');
		    Else: $pos_status == 'static'; 
			#output = (str:'Be the first to comment this page');
		    /If;
		    #output += ' &#187;';
		/If;
                #errorCode = (Error_CurrentError:-ErrorCode);
            /Inline;
            Return:#output;
/Define_Tag;

// get number of posts in cat

Define_Tag: 'LB_getNrofPosts',-Required='id';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,$dbConfig,
		-Table=$table_prefix+'posts',
                'pos_category'=#id,
		'pos_status' = 'publish',
		-ReturnField='id';
		#output = (Found_Count);
                #errorCode = (Error_CurrentError:-ErrorCode);
            /Inline;
            Return:#output;
/Define_Tag;

Define_Type:'LB_getFieldnames';
 	If: (Params) -> (Size) > 0;
	    Local:'fieldNames' = (Params) -> (Get:1);
	    Else;
	    Local:'fieldNames' = 'Empty';
 	/If;
	Local:'error' = (Error_CurrentError);

    Define_Tag:'giveFieldnames';
	Return: (Self -> 'fieldNames');
    /Define_Tag;

    Define_Tag:'error';
	Return: 'x' + (Self -> 'error');
    /Define_Tag;
    
    Define_Tag:'giveFieldvalues';
	Return: 'x' + (Self -> 'error');
    /Define_Tag;

/Define_Type;

// ** searches **

// make linkstuff from searchargs

Define_Tag: 'LB_makeLinkstuff',-Required='searchArgs';
    Local:'linkStuff';
// 	#linkstuff += '?';
    Iterate:#searchArgs,(Local:'i');
	#linkstuff += #i -> (Get:1) + '=';
	#linkstuff += #i -> (Get:2) + '&';
    /Iterate;
	If: #linkstuff != '';
	#linkstuff -> (RemoveTrailing:'&');
	/If;
    Return:#linkstuff;
/Define_Tag;
// get comments


// ### deprecated 051209 *** //
// Define_Tag: 'LB_commentSearch',-Optional='searchArgs';
//     If: !(Local_Defined:'searchArgs');
// 	Local:'searchArgs' = (Array);
//    /If;
//     Local:'error' = '';
// 
//     Inline:-search,$dbConfig,
//	-Table=$table_prefix+'comments',
// 	-MaxRecords='all',
// 	#searchArgs,
// 	-InlineName='commentSearch';
// 
// 	    If: (Found_Count) == 0;
// 	    // #error = 'No records were found'; // comment out for silent mode
// 	    // #error += 'with ' + #searchArgs;
// 	    Else: (Error_CurrentError) != (Error_NoError);
// 	    #error = (Error_CurrentError);
// 	    Else;
// 	    // #error = 'No error';
// 	    /If;
//     /Inline;
//     Return: #error;
// /Define_Tag;

?>

