<?LassoScript

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
	Local:'use_level' = (Integer);
	Local:'user_id' = (Integer);
	Local:'cusername' = '';
	Local:'cpassword' = '';
	Local:'err' = '';
    Define_Tag:'onCreate';
	Local:'hash' = (LB_createCookiehash);
	If: (Cookie:('bloguser_' + #hash)) && (Cookie:('blogpass_' + #hash));
	    (Self -> 'cusername') = (Cookie:('bloguser_' + #hash));
	    (Self -> 'cpassword') = (Cookie:('blogpass_' + #hash));

	    // verify that cookie data
	    Inline: -search, 
		-database = $myDb, 
		-table = 'users',
		-username = $dbUsername,
		-password = $dbPassword,
		-operator = 'eq',
		'use_login' = (Self-> 'cusername'), 
	    ;
		If: (Error_CurrentError)==(Error_NoError)
		    && (Found_Count) == 1 
		    && (Self -> 'cusername') == (Field:'use_login')
		    && (Self -> 'cpassword') == (Field:'use_pass');

			(Self -> 'use_level') = (Integer:(Field:'use_level'));
			(Self -> 'user_id') = (Field:'id');
		Else;
		    Else: (Self -> 'use_level') = (-2); // fake cookies?
		/If;
	(Self->'err') = (Error_CurrentError);
	    /Inline;
	Else;
	    (Self -> 'use_level') = (-1);
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
	Local:'basicOptions'=(Array:'siteurl','blogname','admin_email','date_format','time_format');
	// NOTE: you have to enter optnames manually anyway 
	// to the select statement below

	// init vars & make inlinestuff
	Local:'inlinestuff' = (Array);
	Iterate:#basicOptions,(Local:'i');
	Local: #i = '';
	#inlinestuff -> (Insert:'opt_name' = #i);
	/Iterate;

    Define_Tag:'onCreate';
    Inline: -search, 
	-database = $myDb, 
	-table = 'options',
	-username = $dbUsername,
	-password = $dbPassword,
	-KeyField='id',
	-LogicalOp = 'or',
	-operator = 'eq',
	-responseField = 'opt_name',
	-responseField = 'opt_value',
	(Self -> 'inlinestuff')
    ;
	If: (Error_CurrentError)==(Error_NoError)
	    && (Found_Count) > 0; 
	    Records;
	    Select: (Field:'opt_name');
	    Case:'siteurl';	(Self -> 'siteurl') = (Field:'opt_value');
	    Case:'blogname';	(Self -> 'blogname') = (Field:'opt_value');
	    Case:'admin_email';	(Self -> 'admin_email') = (Field:'opt_value');
	    Case:'date_format';	(Self -> 'date_format') = (Field:'opt_value');
	    Case:'time_format';	(Self -> 'time_format') = (Field:'opt_value');
	    /Select;
	    /Records;
	Else;
	/If;
    (Self->'err') = (Found_Count);
    /Inline;
    /Define_Tag;

    Define_Tag:'LB_getBlogname';
	Return: (Self -> 'blogname');
    /Define_Tag;
    Define_Tag:'LB_getErr';
	Return: (Self -> 'err');
    /Define_Tag;
/Define_Type;

// get option by opt_name

Define_Tag: 'LB_getOption',-Required='optionName';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,-Database=$myDb,-Table='options',
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
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
            Inline:-search,-Database=$myDb,-Table='users',
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
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
            Inline:-search,-Database=$myDb,-Table='categories',
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
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
            Inline:-search,-Database=$myDb,-Table='comments',
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
                'com_post_id'=#id,
		-ReturnField='id';
		If: (Found_Count) > 0;
		#output = (Found_Count) + ' comment';
		    If: (Found_Count) > 1;
		    #output += 's';
		    /If;
		#output += '  &#187;';
		Else;
		#output = 'Be the first to comment this post &#187;';
		/If;
                #errorCode = (Error_CurrentError:-ErrorCode);
            /Inline;
            Return:#output;
/Define_Tag;

// get number of posts in cat

Define_Tag: 'LB_getNrofPosts',-Required='id';
            Local:'output' = '';
            Local:'errorCode' = '';
            Inline:-search,-Database=$myDb,-Table='posts',
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
                'pos_category'=#id,
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

Define_Tag: 'LB_commentSearch',-Optional='searchArgs';
    If: !(Local_Defined:'searchArgs');
	Local:'searchArgs' = (Array);
   /If;
    Local:'error' = '';

    Inline:-search,-Database=$myDb,-Table='comments',
	-UserName=$dbUsername,
	-PassWord=$dbPassword,
	-MaxRecords='all',
	-SortField='com_date',
	-SortOrder='ascending',
	#searchArgs,
	-InlineName='commentSearch';

	    If: (Found_Count) == 0;
	    // #error = 'No records were found'; // comment out for silent mode
	    // #error += 'with ' + #searchArgs;
	    Else: (Error_CurrentError) != (Error_NoError);
	    #error = (Error_CurrentError);
	    Else;
	    // #error = 'No error';
	    /If;
    /Inline;
    Return: #error;
/Define_Tag;

?>

