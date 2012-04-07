
		    #post -> Insert: 'author' = 
		    (LB_searchByID:(Field:'pos_author'),'users','use_nickname');
		    #post -> Insert:'title' = (Field:'pos_title');
		    #post -> Insert:'excerpt' = (Field:'pos_excerpt');
		    #post -> Insert:'content' = (Field:'pos_content');
		    #post -> Insert:'category' = (Field:'pos_category');
		    // dump to output
		    #output -> Insert:#post;
		    #post = (Array); // reset




// getPosts NOT USED
Define_Type:'LB_getPosts';
	Local:'output' = (Array);
	Local:'post'   = (Array);
Define_Tag: 'test_tag';
            Inline:-search,-Database=$myDb,-Table=$table_prefix+'posts',
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
                -SortField='pos_date',
		-SortOrder='descending',
		-MaxRecords='10';

                If: (Error_CurrentError) == (Error_NoError)
                && (Found_Count) > 0;
		
		Records;
		    (Self -> 'post') -> Insert: 'date' = 
		    (LB_formatDateTime:(Field:'pos_date'));
		/Records;
                Else: (Found_Count) == 0;
                    #output = 'No posts were found';
                Else;
                    #output = 'Error while fetching posts' +
                    ': ' + (Error_CurrentError);
                /If;
            /Inline;
/Define_Tag;
/Define_Type;




Define_Tag: 'LB_searchByID',
	-Required='id',-Required='tablename',-Required='fieldname';
            Local:'output' = '';
            Inline:-search,-Database=$myDb,-Table=$table_prefix+#tablename,
                -UserName=$dbUsername,
                -PassWord=$dbPassword,
		'id' = #id;

                If: (Error_CurrentError) == (Error_NoError)
                && (Found_Count) == 1;
		    #output = (Field:#fieldname);
                Else: (Found_Count) == 0;
                    #output = (Tag_Name) + 'No records were found';
                Else: (Found_Count) > 1;
                    #output = (Tag_Name) + 'More than one recs were found';
                Else;
                    #output = (Tag_Name) + 'Error:' + (Error_CurrentError);
                /If;
            /Inline;
            Return:#output;
/Define_Tag;



Define_Type:'LB_getFieldvalues','LB_getFieldnames';
 	If: (Params) -> (Size) > 0;
	    Local:'fieldValues' = (Params) -> (Get:1);
	    Else;
	    Local:'fieldValues' = 'Empty';
 	/If;
	Local:'id' = '';
//	Local:'pos_title' = (Parent -> fieldNames -> (Get:1));
Define_Tag:'giveFieldvalues';
 	Loop: Self -> Parent -> fieldNames -> Size;
 	Local:'i' = (Self -> Parent -> fieldNames -> (Get:(Loop_Count)));
  	Local:(Local:'i') = '';
	If: (Loop_Count) == 1;
	Local:'test' = (Self -> Parent -> fieldNames -> (Get:(Loop_Count)));
	/If;
 	/Loop;
Return: (Self -> Parent -> fieldNames ) +','+ #test;
/Define_Tag;

// Define_Tag:'defineVariables';
//  	If: (Params) -> (Size) > 0;
// 	Loop: (Self -> Parent -> 'fieldNames') -> Size;
// 	Local:(Self -> Parent -> 'fieldNames') -> (Get:(Loop_Count)) 
// 	= #recordData -> (Get:(Loop_Count));
// 	/Loop;
//  	/If;
// 	Return: 'otsikko = ';
// /Define_Tag;
/Define_Type;





// general code for capturing search data
// 	Loop:(Field_Name:-Count);
// 	(Self -> 'aOptions') -> (Insert:(Field_Name:(Loop_Count)) = 
// 	(Field:(Field_Name:(Loop_Count))));
// 	/Loop;
// (Self -> 'aOptions') -> (Insert:'opt_name' = (Field:'opt_name'));


// ctype for fetching & storing all options - this works!!
// first calling code:

// get options from db and store them to ctype
Variable:'optionsData' = (LB_optionsData);	// create object
$optionsData -> setOptions;			// do query
$optionsData -> (getValue:'siteurl');		// get option

// then the ctype code: 

Define_Type:'LB_optionsData';
Local:'errorcode' = 0;
Local:'aOptions' = (Array);
Local:'optionName' = '';
Local:'optionValue' = '';
Define_Tag: 'setOptions';
    Inline:-findAll,-Database=$myDb,-Table=$table_prefix+'options',
	-UserName=$dbUsername,
	-PassWord=$dbPassword,
	-InlineName='setOptions';
	    (Self -> 'errorcode') = (Error_CurrentError:-ErrorCode);
    Records;
    (Self -> 'aOptions') -> (Insert:(Field:'opt_name') = (Field:'opt_value'));
    /Records;
            /Inline;
/Define_Tag;
Define_Tag:'getValue',-Required='optionName';
    (Self -> 'optionValue') = 
	    (Self -> 'aOptions') -> (Find:#optionName) -> (Get:1) -> (Get:2);
    Return: #optionName + '=' + (Self -> 'optionValue');

/Define_Tag;
/Define_Type;



Define_tag:'LB_postsearch',-Optional='searchArgs';
    Local:'error' = '';
    If: !(Local_Defined:'searchArgs');
	Local:'searchArgs' = (Array);
    /If;

    Inline:-search,-Database=$myDb,-Table=$table_prefix+'posts',
	-UserName=$dbUsername,
	-PassWord=$dbPassword,
	-SortField='pos_date',
	-SortOrder='descending',
	-MaxRecords='1',
	#searchArgs,
	-InlineName='postsearch';
	    If: (Found_Count) == 0;
	    #error = 'No records were found';
	    Else: (Error_CurrentError) != (Error_NoError);
	    #error = (Error_CurrentError);
	    Else;
	    // #error = 'No error';
	    /If;
	    Return: #error;
    /Inline;
/Define_Tag;
