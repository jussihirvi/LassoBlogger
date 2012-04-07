<?LassoScript
// colon (:) = &#58;
// level can be 'normal' or 'debug'


// namespaces 
// MO = monster
// SC = shopping cart


Define_Tag('MO_htmltest');
    Return('<b>test</b>');
/Define_Tag;

Define_Tag( 'MO_titleCase',-Required='str');
    Local('output' = '');
    Local('aStr' = #str -> (Split(' ')));
    Local('aStr2' = array);
    Local('i' = '');
    Local('iFirst' = '');
    Local('iRest' = ''); 
    Iterate(#aStr,#i);
        #iFirst = #i -> (Get(1));
        #iFirst -> Uppercase;
        #iRest  = #i;
        #iRest -> (Remove(1,1));
        #aStr2 -> (Insert( (#iFirst + #iRest)));
    /Iterate;
        #output = #aStr2 -> (Join(' '));
    Return(#output);
/Define_Tag;

Define_Tag( 'MO_getCategoryName',-Required='int');
    Local('output' = '');
	inline( -search, $dbConfig, -table= $table_prefix + 'sitecats',
              -op='eq',
              'id' = #int);
	    #output = field('catname');
	/inline;
    Return(#output);
/Define_Tag;

// FORM PROCESSOR TAGS 
// MO_customforms - see liblocal


Define_Tag('MO_tablerow',-Required='cell1',
    -Optional='cell2',
    -Optional='cell3',
    -Optional='cell4',
    -Optional='cell5',
    -Optional='cell6',
    -Optional='style',
    -encodeNone
    );
   local('result' = string); 
    If( !(Local_Defined('cell2'))); Local('cell2' = ''); /If;
    If( !(Local_Defined('cell3'))); Local('cell3' = ''); /If;
    If( !(Local_Defined('cell4'))); Local('cell4' = ''); /If;
    If( !(Local_Defined('cell5'))); Local('cell5' = ''); /If;
    If( !(Local_Defined('cell6'))); Local('cell6' = ''); /If;
    If( !(Local_Defined('style'))); Local('style' = 'padding-right:10px;');/If;
    #result =  '<tr>';
    #result += '<td';
    if(#style);
	#result += ' style="' + #style + '"';
    /if;
    #result += '>' + #cell1 + '</td>';;
    if(#cell2);
	#result += '<td';
	if(#style);
	    #result += ' style="' + #style + '"';
	/if;
	#result += '>' + #cell2 + '</td>';;
    /if;
    if(#cell3);
	#result += '<td';
	if(#style);
	    #result += ' style="' + #style + '"';
	/if;
	#result += '>' + #cell3 + '</td>';;
    /if;
    if(#cell4);
	#result += '<td';
	if(#style);
	    #result += ' style="' + #style + '"';
	/if;
	#result += '>' + #cell4 + '</td>';;
    /if;
    if(#cell5);
	#result += '<td';
	if(#style);
	    #result += ' style="' + #style + '"';
	/if;
	#result += '>' + #cell5 + '</td>';;
    /if;
    if(#cell6);
	#result += '<td';
	if(#style);
	    #result += ' style="' + #style + '"';
	/if;
	#result += '>' + #cell6 + '</td>';;
    /if;
    #result += '</tr>\n';
    return(#result);
/Define_Tag;


define_tag('MO_processSearchresText',-required='text',-required='s');
  // process searchresult text - used on page cont/searchres.lasso

  // set vars
    local('out' = string);
    local('testvalue' = string);
    local('instance' = string);
    local('position'    = integer);
    local('start'       = integer, 'end'=integer);
    local('startIsCut'  = string);
    local('endIsCut'    = string);

// prepare
#position = integer(#text -> find(#s));
#instance = #text -> subString(#position,(#s -> Size));

// define start & end points
If( (#text -> Size) < 150 );
    // use whole text
    // 'TEST:' + (Loop_count) + 'no cut <br>';
    #out = #text;

else( (#text -> Size) >= 150 );
    If( #position < 100);
	#start = 1;
    Else; 
	#start = (#position - 90);
	#startIsCut='yes';
    /If; 
    If( #position > ((#text -> Size) - 100));
	#end = (#text -> Size);
    Else;
	#end = (#position + 90);
	#endIsCut='yes';
    /If; 

    // make the cut
    #out = (#text -> (SubString(#start, (#end - #start))));

    // trim, if necessary
    If( #startIsCut == 'yes');
	// trim beginning (remove the first, proabably partial word)
        if(#out -> contains(' '));
          #out = #out -> split(' ');
          #out -> remove(1);
          #out = #out -> join(' '); 
        /if;
    /If;
    If( #endIsCut == 'yes');
	// trim end (remove the last, proabably partial word)
        if(#out -> contains(' '));
          #out = #out -> split(' ');
          #out -> removelast;
          #out = #out -> join(' '); 
        /if;
    /If; 
/If; 

// if trimming went wrong, give up
if(! #out);
  return(string);
/if;

// bold the search string
    #position = integer(#out -> find(#s)); // redefine
    #out = String_Replace(#out,-find=#s,-replace=('<b>' + #instance + '</b>'));

// add ... when necessary
    If( #startIsCut == 'yes'); 
	#out = '...' + #out; 
    /If;
    If( #endIsCut == 'yes');
	#out += '...';
    /If; 

    return(#out);
    // return(#testvalue);

/define_tag;

define_tag('MO_striphtml',-required='in');
    local('out' = string);
    #out = string_replaceRegExp(#in,-find='<.*?>',-replace='');
    return(#out);
/define_tag;

/* monster validation tags 
- first the master tags, then individual validations */ 

define_tag('MO_val',-required='editfields',-required='inputvars',-required='action');
  local('err' = string );
  local('this' = array );
  // do basic val for all fields??
  iterate ( #editfields, #this );
    local('fname'   = #this -> get(1));
    local('flabel'  = #this -> get(2));
    local('valtypes'= #this -> get(4) -> split(','));
    local('date'   = #this -> get(8));    // deprecated 2010-12-03
    local('value'   = string(#inputvars -> find(#fname)));
    if( #valtypes -> find('req') != array );
      #err += MO_val_req(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( ( !(#valtypes -> find('allowhtml')) != array) && (#inputvars->find(#fname)) != array );
      #err += MO_val_html(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( #valtypes -> find('unique') != array );
      #err += MO_val_unique(-fname = #fname, -label = #flabel, -value = #value, -action = #action);
    /if;
   If( (#valtypes -> find('date') != array) && #value && #value != '0000-00-00' ); // if not empty
      #err += MO_val_date(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( (#valtypes -> find('time') != array) && #value );
      #err += MO_val_time(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( (#valtypes -> find('datetime') != array) && #value );
      #err += MO_val_datetime(-fname = #fname, -label = #flabel, -value = #value);
    /if;
   If( (#valtypes -> find('email') != array) && #value );
      #err += MO_val_email(-fname = #fname, -label = #flabel, -value = #value);
   /if;
   If( (#valtypes -> find('password')  != array) && #value );
      #err += MO_val_password(-fname = #fname, -label = #flabel, -value = #value);
   /if;
    If( (#valtypes -> find('alphanum') != array) && #value );
      #err += MO_val_alphanum(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( (#valtypes -> find('int') != array) && #value );
      #err += MO_val_int(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( (#valtypes -> find('float') != array) && #value );
      #err += MO_val_float(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( (#valtypes -> find('companyid') != array) && #value );
      #err += MO_val_companyid(-fname = #fname, -label = #flabel, -value = #value);
    /if;
    If( (#valtypes -> find('personid') != array) && #value );
      #err += MO_val_personid(-fname = #fname, -label = #flabel, -value = #value);
    /if;
  /iterate;
  return(#err);
/define_tag;

Define_Tag('MO_valBasic',-Required='input',-encodeNone);
    local('output' = string);
    if  (!(#input));
    else(#input -> size > 32000);
    #output = 'liian pitk‰.';
    else(#input -> (Contains('<?'))	// kielletyt merkit
     || #input -> (Contains('?>'))
     || #input -> (Contains('['))
     || #input -> (Contains(']'))
    );
    #output = 'sis‰lt‰‰ kiellettyj‰ merkkej‰.';
    /if;
    return(#output);
/Define_Tag;

Define_Tag('MO_val_req',-required='label',-required='value',-encodeNone);
    local('err' = string );
    If( !(#value) );
      #err = MO_valStr(-fLabel=#label,-valName='req');
    /If;
    return(#err);
/Define_Tag;
Define_Tag('MO_val_unique',-required='fname',-required='label',-required='value',-required='action',-optional='mytable',-optional='short_output',-encodeNone);
    local('errshort' = string );
    local('errlong'  = string );
    if(!(local_defined('mytable')));
      local('mytable' = $mytable);
    /if;
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
      inline( $dbConfig,  -table = $table_prefix + #myTable,
//              -neq, 'id'   = $keyvalue,
                -eq,  #fname = #value,
              -search);
        If( #action == 'add' && found_count > 0 );
          #errshort = str('This value is reserved');
          #errlong = MO_valStr(-fLabel=#label,-valName='unique');
        else( #action == 'update' && found_count > 1 );
          #errshort = str('This value is reserved');
          #errlong = MO_valStr(-fLabel=#label,-valName='unique');
        /If; 
      /Inline;
    if(#short_output == 'yes');
      return(#errshort);
    else;
      return(#errlong);
    /if;
/Define_Tag;
Define_Tag('MO_val_date',-required='fname',-required='label',-required='value',-optional='short_output',-encodeNone);
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
    local('errshort' = string );
    local('errlong' = string );
      if(#value);
        If( date(#value,-Format=($opts -> find('date_showformat'))) -> type != 'Date'); 
            #errshort = str('Not valid');
            #errlong = MO_valStr(-fLabel=#label,-valName='dateValid');
        Else( date(#value,-Format=($opts -> find('date_showformat')))->year < '1000' ); 
                          // date is valid, but may be senseless
            #errshort = str('Not valid');
            #errlong = MO_valStr(-fLabel=#label,-valName='dateOld');
        /if;
      /if;
    if(#short_output == 'yes');
      return(#errshort);
    else;
      return(#errlong);
    /if;
/Define_Tag;
Define_Tag('MO_val_time',-required='fname',-required='label',-required='value',-optional='short_output',-encodeNone);
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
    local('errshort' = string );
    local('errlong' = string );
      if( date(#value,-Format=($opts -> find('time_showformat'))) -> Type != 'Date' ); 
          #errshort = str('Not valid');
          #errlong = MO_valStr(-fLabel=#label,-valName='time');
     /if;
    if(#short_output == 'yes');
      return(#errshort);
    else;
      return(#errlong);
    /if;
/Define_Tag;
Define_Tag('MO_val_datetime',-required='fname',-required='label',-required='value',-optional='short_output',-encodeNone);
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
    local('errshort' = string );
    local('errlong' = string );
      If( date(#value,-Format=($opts -> find('datetime_showformat'))) -> type != 'Date'); 
          #errshort = str('Not valid');
          #errlong = MO_valStr(-fLabel=#label,-valName='dateValid');
      Else( date(#value,-Format=($opts -> find('datetime_showformat')))->year < '1000' ); 
                        // date is valid, but may be senseless
          #errshort = str('Not valid');
          #errlong = MO_valStr(-fLabel=#label,-valName='dateOld');
      /if;
    if(#short_output == 'yes');
      return(#errshort);
    else;
      return(#errlong);
    /if;
/Define_Tag;
Define_Tag('MO_val_email',
      -required='fname',
      -required='label',
      -required='value',
      -optional='short_output',
      -encodeNone);
    local('errshort' = string );
    local('errlong' = string );
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
      If( !(Valid_Email(#value)) );
          #errshort = str('Not valid');
          #errlong = MO_valStr(-fLabel=#label,-valName='email');
      /if;
    if(#short_output == 'yes');
      return(#errshort);
    else;
      return(#errlong);
    /if;
/Define_Tag;
Define_Tag('MO_val_password',
      -required='fname',
      -required='label',
      -required='value',
      -optional='short_output',
      -encodeNone);
    local('errshort' = string );
    local('errlong' = string );
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
       if( String_FindRegExp( #value,-find='[^0-9^\\w\-\_]' ) != array );
          #errshort = str('Only a-z, A-Z, 0-9, -, _ are allowed');
          #errlong += MO_valStr(-fLabel=#label,-valName='password_forbidden');
       /if;
       if( #value -> Size < 7);
          #errshort = str('Too short');
          #errlong += MO_valStr(-fLabel=#label,-valName='password_short');
       else( #value -> Size > 12);
          #errshort = str('Too long');
          #errlong += MO_valStr(-fLabel=#label,-valName='password_long');
       /if;
           if(#short_output == 'yes');
             return(#errshort);
           else;
             return(#errlong);
           /if;
/Define_Tag;
Define_Tag('MO_val_alphanum',-required='fname',-required='label',-required='value',-encodeNone);
    local('err' = string );
        If( !(String_isAlphaNumeric(#value)) );
          #err = MO_valStr(-fLabel=#label,-valName='alphanum');
        /if;
    return(#err);
/Define_Tag;
Define_Tag('MO_val_int',-required='fname',-required='label',-required='value',-optional='short_output',-encodeNone);
    local('errshort' = string );
    local('errlong' = string );
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;
        If( !(String_isDigit(#value)) );
          #errshort = str('Must be digit');
          #errlong = MO_valStr(-fLabel=#label,-valName='int');
        /if;
   if(#short_output == 'yes');
     return(#errshort);
   else;
     return(#errlong);
   /if;
/Define_Tag;
Define_Tag('MO_val_float',-required='fname',-required='label',-required='value',-encodeNone);
    local('err' = string );
        #value -> replace($opts->find('floatseparator_show'),'.');
        If( !(String_isNumeric(#value)) );
          #err = MO_valStr(-fLabel=#label,-valName='float');
        /if;
    return(#err);
/Define_Tag;
Define_Tag('MO_val_html',-required='fname',-required='label',-required='value',-encodeNone);
    local('err' = string );
        local('test' = array);
        local('err' = string);
        local('offendingChars' = string);
        #test = (String_FindRegExp( #value, -find='[^0-9 \\w\\sÂ‰ˆ≈ƒ÷¸‹\\-\\.\\,\\:\\;\\!\\?\\*\\&\\^\\%\\$\\#\\"\'\\@\\/\\=\\(\\)\\[\\]\\<\\>]')); // allowed chars
        #test -> merge(string_FindRegExp(#value,-find='\\<\\?')); // forbidden char run(s)
        If( #test != array);
          #offendingChars += (#test -> (Join(' '))); // can be used for debug 
          #err = MO_valStr(-fLabel=#label,-valName='html');
          // #err += ('Offending chars: ' + #offendingChars);
        /if;
    return(#err);
/Define_Tag;
Define_Tag('MO_val_companyid',
  -required='fname',
  -required='label',
  -required='value',
  -optional='onlyFinnishFormat',
  -optional='short_output',
  -encodeNone);
  /* validate Finnish Business ID
  - for valid company ID, no output
  - for invalid input, error text
  - by default accepts both Finnish and intl format 
    123456-7
    1234567-8
    FI1234567
    FI12345678
  - default is long output
  - short output is good when validation result is shown next to field
  */
  local('errshort' = string );
  local('errlong'  = string );
  local('errcode'  = integer );
  local('firstpart' = integer );
  local('num' = string );
  local('control' = integer );
  local('controlsum' = integer );
  if( !(local_defined('short_output')));
    local('short_output' = 'no');
  /if;
      if(#value -> size < 8);
        #errshort = str('Too short');
        #errlong  = MO_valStr(-fLabel=#label,-valName='companyid_short');
      else(7 < #value->size && #value->size < 10 // size 8 or 9
        && string_findRegExp(#value,-find="^[0-9]{6,7}-[0-9]{1}$") != array);
          // check for Finnish format
          #num     = #value -> split('-') -> get(1);
          #control = #value -> split('-') -> get(2);
          #num -> padLeading(7,0);
            #controlSum += integer(#num -> substring(1, 1))*7;
            #controlSum += integer(#num -> substring(2, 1))*9;
            #controlSum += integer(#num -> substring(3, 1))*10;
            #controlSum += integer(#num -> substring(4, 1))*5;
            #controlSum += integer(#num -> substring(5, 1))*8;
            #controlSum += integer(#num -> substring(6, 1))*4;
            #controlSum += integer(#num -> substring(7, 1))*2;
            #controlSum  = #controlSum % 11;
            if (#controlsum == 0 && #control != 0); 
                #errshort = str('Not valid');
                #errcode = 1;
                #errlong  = MO_valStr(-fLabel=#label,
                    -valName='companyid_notvalid');
            else( #controlsum >= 2 && #controlsum <= 10 ); 
              if(11 - #controlsum != #control);
                #errshort = str('Not valid');
                #errcode = 2;
                #errlong = MO_valStr(-fLabel=#label,
                    -valName='companyid_notvalid');
              /if;
            /if;
      else(8 < #value->size && #value->size < 11 // size 9 or 10
        && string_findRegExp(#value,-find="^FI[0-9]{7,8}$") != array);
        // check for international format, Finnish company
          #num = #value; 
          #num -> remove(1,2);
          #control = #num -> get(#num->size);
          #num -> remove(#num->size,1);
          #num -> padLeading(7,0);
            #controlSum += integer(#num -> substring(1, 1))*7;
            #controlSum += integer(#num -> substring(2, 1))*9;
            #controlSum += integer(#num -> substring(3, 1))*10;
            #controlSum += integer(#num -> substring(4, 1))*5;
            #controlSum += integer(#num -> substring(5, 1))*8;
            #controlSum += integer(#num -> substring(6, 1))*4;
            #controlSum += integer(#num -> substring(7, 1))*2;
            #controlSum  = #controlSum % 11;
            if (#controlsum == 0 && #control); 
                #errshort = str('Not valid');
                #errcode = 3;
                #errlong  = MO_valStr(-fLabel=#label,
                    -valName='companyid_notvalid');
            else( #controlsum >= 2 && #controlsum <= 10 ); 
              if(11 - #controlsum != #control);
                #errshort = str('Not valid');
                #errcode = 4;
                #errlong = MO_valStr(-fLabel=#label,
                    -valName='companyid_notvalid');
              /if;
            /if;

      else(8 < #value->size && #value->size < 11 // size 9 or 10
        && string_findRegExp(#value,-find="^[a-z A-Z]{2}[0-9]{7,8}$") != array);
        // check for international format
        // passes the text, though not sure if it really is valid
        // we could add validation for Finnish ID, beginning with FI...
      else(#value->size > 10);
        #errshort = str('Too long');
        #errlong = MO_valStr(-fLabel=#label,-valName='companyid_long');
      else;
          #errshort = str('Not valid');
          #errcode = 5;
          #errlong = MO_valStr(-fLabel=#label,-valName='companyid_notvalid');
      /if;
        if(#short_output == 'yes');
          return(#errshort);
        else;
          return(#errlong);
        /if;
/Define_Tag;
Define_Tag('MO_val_personid',-required='fname',-required='label',-required='value',-optional='short_output',-encodeNone);
    local('err'	    = integer);
    local('errshort'= string);
    local('errlong' = string);
    if( !(local_defined('short_output')));
      local('short_output' = 'no');
    /if;

    #value -> Trim; 		// siistit‰‰n
    If( #value -> Size != 11);	// #1. pituus oltava 11
	#err = 1;
    /If;
    If( !(#err));			// create substrings
	local('sPaiva'		= #value -> substring(1,2));
	local('sKuukausi'	= #value -> substring(3,2));
	local('sVuosi2'	    	= #value -> substring(5,2));
	local('sPvm'	    	= #value -> substring(1,6));
	local('sValimerkki'   	= #value -> substring(7,1));
	local('sYksilonumero' 	= #value -> substring(8,3));
	local('sTarkistusmerkki'= #value -> substring(11,1));
	local('s9string'	= $sPvm + $sYksilonumero);
    /If;

    If(! #err );			// #2. numeroita, ei kirjaimia tms.
	If( !(Integer(#s9string)));
	#err = 21;
	/If;
    /If;

    If(! #err );		    // #3. valimerkki, sallitut -,+,A
				    // jos ok, m‰‰rit‰ vuosisata
	local('nVuosisata' = (Integer));
	    If( #sValimerkki == '-');
              local('nVuosisata' = 19);
	    Else( #sValimerkki == '+');
              local('nVuosisata' = 18);
	    Else( #sValimerkki == 'A');
              local('nVuosisata' = 20);
	    Else;			// valimerkki on virheellinen
              #err = 3;
	    /If;
    /If;

				    // #4. date validation
    If(! #err );		    // vuosiluku

    // koosta vuosiluku
	local('nVuosi4' = (Integer((#nVuosisata + #sVuosi2))));

    // vuosi pit‰‰ olla 1850...nykyhetki
	If( #nVuosi4 < 1850 || #nVuosi4 > ((Date) -> Year));
	#err = 41;
	/If;
    /If;				    // vuosiluku
    If(! #err );			    // onko karkausvuosi?
	local('nHelmikuunPituus' = (Integer));
	if( #nVuosi4 > 1582 &&		    // gregoriaaninen kal.
	    (
	      (#nVuosi4 % 4 == 0 && 
	      #nVuosi4 % 100 != 0	    // joka nelj‰s v paitsi tasavsadat
	      ) || 		
	      #nVuosi4 % 400 == 0			// joka nelj‰ssadas
	    ));
	    #nHelmikuunPituus = 29;			// karkausvuosi
	else( #nVuosi4 % 4 == 0 && #nVuosi4 < 1582); // juliaaninen kalenteri
	    #nHelmikuunPituus = 29;			// karkausvuosi
	else;
	    #nHelmikuunPituus = 28;
	/if;
	
	// kuukausien pituudet ko. vuonna
	local('arKuunPituudet' = 
	    Array(31,#nHelmikuunPituus,31,30,31,30,31,31,30,31,30,31));


	If(  integer(#sKuukausi) < 1 || 	// onko kk v‰lill‰ 1..12
	    (Integer(#sKuukausi) > 12));
	    #err = 42;
	/If; 
    /If;						// end onko karkausvuosi

    If(! #err );  				// p‰iv‰
	If( Integer(#sPaiva) < 1 || 	// onko annettu p olemassa ko. vuonna?
	    Integer(#sPaiva) > 
		(#arKuunPituudet -> Get(#sKuukausi)));
	#err = 43;
	/If;
    /If; 					// end p‰iv‰

    If(! #err );				// #5. tarkistetaan tarkistusmerkki
	local('i' = '');			// tilap‰inen var
	local('sOikeaTarkistusmerkki' = '');
	Select( (#s9string % 31));
	    Case(0); 	#i = 0;
	    Case(1); 	#i = 1;
	    Case(2); 	#i = 2;
	    Case(3); 	#i = 3;
	    Case(4); 	#i = 4;
	    Case(5); 	#i = 5;
	    Case(6); 	#i = 6;
	    Case(7); 	#i = 7;
	    Case(8); 	#i = 8;
	    Case(9); 	#i = 9;
	    Case(10);	#i = 'A';
	    Case(11);	#i = 'B';
	    Case(12);	#i = 'C';
	    Case(13);	#i = 'D';
	    Case(14);	#i = 'E';
	    Case(15);	#i = 'F';
	    Case(16);	#i = 'H';
	    Case(17);	#i = 'J';
	    Case(18);	#i = 'K';
	    Case(19);	#i = 'L';
	    Case(20);	#i = 'M';
	    Case(21);	#i = 'N';
	    Case(22);	#i = 'P';
	    Case(23);	#i = 'R';
	    Case(24);	#i = 'S';
	    Case(25);	#i = 'T';
	    Case(26);	#i = 'U';
	    Case(27);	#i = 'V';
	    Case(28);	#i = 'W';
	    Case(29);	#i = 'X';
	    Case(30);	#i = 'Y';
	/Select;
	#sOikeaTarkistusmerkki = #i;

	if( #sTarkistusmerkki != #sOikeaTarkistusmerkki);
	    #err = 5;
	/If;

    /If;      		// end tarkistetaan tarkistusmerkki
    if(#err);
      #errshort = str('Not valid');
      #errlong = MO_valStr(-fLabel=#label,-valName='personid_notvalid');
    /if;
    if(#short_output == 'yes');
        return(#errshort);
    else;
        return(#errlong);
    /if;
/Define_tag;




/* FORM FIELD PRESENTATION 
// first handler tag, then tags for individual fields
*/ 

Define_Tag('MO_formfieldHandler',-required='this',
                                 -required='fvalue',
                                 -optional='customcode',
                                 -encodeNone);
  local('out' = string);
  local('myWidth' = 'width:');
  local('fname' = #this -> get(1));
  local('ftype' = #this -> get(3));
  if( !(local_defined('customcode')));
    local('customcode' = string);
  /if;
  local('checkedvalue' = string);
      #fvalue = string(#fvalue);
      // define width attribute
      If( #this -> Get(5) ); 
        #myWidth += #this -> get(5);
      else; // use default
        #myWidth += $size1default;   
      /If;
      // for checkbox
      if(#ftype=='checkbox' && #this->get(5));
        #checkedvalue = #this->get(5);
      else;
        #checkedvalue = 'on';
      /if;
  If( #ftype == 'noentry'); 	// case noentry
    #out = MO_formfieldNoentry(-name=#fname,-value=#fvalue);

  Else( #ftype == 'replacecode'); 	// case replacecode
    #out = MO_formfieldReplacecode(-name=#fname,-value=#this->get(9));

  Else( #ftype == 'saveIfNotEmpty'); 	// case saveIfNotEmpty
    #out = MO_formfieldSaveIfNotEmpty(-name=#fname,-value=#fvalue,-styleAttr=#myWidth);

  Else( #ftype == 'hidden'); 	// case hidden
    #out = MO_formfieldHidden(-name=#fname,-value=#fvalue);

  Else( #ftype == 'text'); 	// case text
    #out = MO_formfieldText(
      -name=#fname,
      -value=#fvalue,
      -validation=#this->get(4) -> split(','),
      -styleAttr=#myWidth,
      -type='text');

  Else( #ftype == 'password'); 	// case password
    #out = MO_formfieldText(
      -name=#fname,
      -value=#fvalue,
      -validation=#this->get(4) -> split(','),
      -styleAttr=#myWidth,
      -type='password');

  Else( #ftype == 'textarea');	// case textarea
        // define rows attribute
        local('myRows' = integer);
        If( (#this -> (Get(6)))); 
          #myRows = (#this -> (Get(6)));
        else; // use default
          #myRows = $size2default;   
        /If;
    #out = MO_formfieldTextarea(-name=#fname,-value=#fvalue,-styleAttr=#myWidth,-rows=#myRows);

  Else( #ftype == 'textarea_ckeditor');	// case textarea with ckeditor
        // define rows attribute
        local('myRows' = integer);
        var('myRows' = integer);
        If( (#this -> (Get(6)))); 
          #myRows = (#this -> (Get(6)));
        else; // use default
          #myRows = $size2default;   
        /If;
    #out = MO_formfieldTextarea(-name=#fname,-value=#fvalue,-styleAttr=#myWidth,-rows=#myRows,-editor='ckeditor');

  Else( #ftype == 'select');	// case select
    #out = MO_formfieldSelect(-name=#fname,-value=#fvalue,-values=#this->get(5),-labels=#this->get(6),-customcode=#customcode);

  Else( #ftype >> 'selectMultiple');	// case selectMultiple
    #out = MO_formfieldSelectMultiple(-name=#fname,-value=#fvalue,-values=#this->get(5),-labels=#this->get(6));

  Else( #ftype >> 'checkMultiple');	// case checkMultiple
    #out = MO_formfieldCheckMultiple(-name=#fname,-value=#fvalue,-values=#this->get(5),-labels=#this->get(6));

  Else( #ftype == 'radio');	// case radio
    #out = MO_formfieldRadio(-name=#fname,-value=#fvalue,-values=#this->get(5),-labels=#this->get(6));

  Else( #ftype == 'checkbox');	// case checkbox
    #out = MO_formfieldCheckbox(-name=#fname,-checkedvalue=#checkedvalue,-value=#fvalue);

  Else( #ftype == 'addcode');	// case addcode 
    #out = MO_formfieldAddcode(-name=#fname,-value=#fvalue);
  /If; 					// if type 
  return(#out);
/Define_Tag;

Define_Tag('MO_formfieldNoentry',-required='name',-required='value');
  local('out' = string );
  #out += '<span class="noentry">' + encode_break(#value) + '</span>';
  // url_encode necessary for quote char
  #out += '<input type="hidden" name="' + #name + '" value="' + encode_url(encode_html(#value)) + '">'; 
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldReplacecode',-required='name',-required='value');
  local('out' = string );
  #out += process(#value); 
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldSaveIfNotEmpty',-required='name',-required='value',-optional='styleAttr');
  local('out' = string );
  #out += '<input type="' + 'text' + '" name="' + #name + '"';
  if(local_defined('styleAttr'));
      #out += ' STYLE="' + #styleAttr + ';"';
    /If;
  #out += ' value="">';
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldHidden',-required='name',-required='value');
  local('out' = string );
    // url_encode necessary for quote char
    #out += '<input type="hidden" name="' + #name;
    #out += '" value="' + encode_url(encode_html(string(#value))) + '">';
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldText',
        -required='name',
        -required='value',
        -optional='validation',
        -optional='styleattr',
        -optional='type');
  local('out' = string );
  if( !(local_defined('type')));
    local('type' = 'text'); // possible types: text, password
  /if;
  local('class' = 'input-' + #name);
    if(local_defined('validation'));
      if(#validation -> find('req'));
        #class += ' required';
      /if;
      if(#validation -> find('float'));
        if($opts -> find('floatseparator_show') == ',');
          #class += ' numberDE';
        else;
          #class += ' number';
        /if;
      /if;
      if(#validation -> find('date'));
        #class += ' dateDE';
      /if;
    /if;
    #out += '<input type="';
    #out += (#type == 'password' ? 'password' | 'text');
    #out += '" name="' + #name + '"';
    if(#class);
      #out += ' class="' + #class + '"';
    /if;
    #out += ' id="input-' + #name + '"';
    if(local_defined('styleattr'));
      #out += ' STYLE="' + #styleattr + ';"';
    /if;
    // why is this quote-handling necessary? it seems to be,
    // otherwise strs containing quotes will be clipped
    // in input fields
    #value -> replace('"','&quot;');
    #out += ' value="' + encode_html(#value) + '">'; // enc html 050921
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldTextarea',-required='name',-required='value',-optional='styleattr',-optional='rows',-optional='editor');
  local('out' = string );
  if( !(local_defined('editor')));
    local('editor' = string );
  /if;
    #out += '<Textarea name="' + #name + '"';
        if( local_defined('styleAttr') );
          #out += ' style="' + #styleAttr + ';"';
        /if;
    #out += ' rows="'; 
        if( local_defined('rows') );
          #out += #rows;
        else;
          #out += '3';
        /if;
    #out += '" wrap="soft">';
    #out += encode_html(#value);
    #out += '</Textarea>';
    // ckeditor code
    if(#editor == 'ckeditor');
      #out += '<script type="text/javascript"> CKEDITOR.replace( \'' + #name + '\' );</script>\n';
    /if;
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldSelect',-required='name',-required='value',-required='values',-required='labels',-optional='customcode',-encodenone);
  local('out' = string );
  local('valuesA' = #values -> split(';'));
  local('labelsA' = #labels -> split(';'));
  if( !(local_defined('customcode')));
    local('customcode' = string);
  /if;
    #out += '<select name="' + #name + '" size="1" ';
    #out += 'id="select-' + #name + '" ';
    if(#customcode);
      #out += #customcode;
    /if;
    #out += '>\n';
    local('thisValue' = '');
    Loop( (#valuesA -> size));
      #out += '<option value="' + (#valuesA -> (Get((Loop_Count)))) + '"';
      If( #value == (#valuesA -> (Get((Loop_Count))))); 
      #out += ' SELECTED';
      /If;
      #out += '>';
      #thisValue = #labelsA-> (Get((Loop_Count)));
          If( #thisValue -> Size > 60);		// shorten
              #thisValue = #thisValue -> (SubString(1,50));
              #thisValue += '...';
          /If;
      #out += #thisValue;
      #out += '</option>\r';
    /Loop;
    #out += '</select>\r';
    return(#out);
/Define_Tag;
Define_Tag('MO_formfieldSelectMultiple',-required='name',-required='value',-required='values',-required='labels');
  local('out' = string );
  local('valuesA'   = #values -> Split(';'));
  local('labelsA'   = #labels -> Split(';'));
  local('valueA' = #value  -> Split('\r'));
  #out += '<select name="' + #name + '" size="+ $valuesA->size 
    + " MULTIPLE style="float:left;">\n';
  local('thisLabel' = string);
  local('thisValue' = string);
  Loop( (#valuesA -> size));
    #thisValue = #valuesA -> get(loop_count);
    #thisLabel = #labelsA -> get(Loop_Count);
	If( #thisLabel -> Size > 60);		// shorten
	    #thisLabel = #thisLabel -> (SubString(1,50));
	    #thisLabel += '...';
	/If;
    #out += '<option value="' + #thisValue + '"';
    loop( #valueA -> Size);
      If( #valueA -> Get(Loop_Count) == #thisValue); 
        #out += ' SELECTED';
      /If;
    /loop;
    #out += '>';
    #out += #thisLabel;
    #out += '</option>\r';
  /Loop;
  #out += '</select>\r';
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldCheckmultiple',-required='name',-required='value',-required='values',-required='labels');
  local('out' = string );
  local('valuesA' = #values  -> Split(';'));
  local('labelsA' = #labels  -> Split(';'));
  local('valueA' = #value -> Split('\r'));
  Loop( (#valuesA -> Size));
      local('i' = #valuesA -> get(loop_count));
      #out += '<INPUT TYPE="checkbox" class="checkbox" NAME="';
      #out += #name + '" VALUE="' + #i + '"';
          Loop( #valueA -> Size );
              If( #valueA -> Get(Loop_Count) == #i ); 
                  #out += ' CHECKED';
              /If;
          /Loop;
      #out += '> ' + #labelsA -> get(loop_count) +  '<br>\n';
  /loop;
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldRadio',-required='name',-required='value',-required='values',-required='labels');
  local('out' = string );
  local('valuesA' = #values -> split(';'));
  local('labelsA' = #labels -> split(';'));
  Loop( (#valuesA -> Size));
    #out += '<INPUT TYPE="radio" class="radio" NAME="' + #name + '" VALUE="';
    #out += #valuesA -> Get(Loop_Count) + '"';
        If( #value == #valuesA -> Get(Loop_Count)); 
        #out += ' CHECKED';
        /If;
    #out += '> ' + #labelsA -> Get(Loop_Count) + '&nbsp;\n';
  /loop;
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldCheckbox',-required='name',-required='checkedvalue',-required='value');
  local('out' = string );
  #out += ('<INPUT TYPE="checkbox" class="checkbox" NAME="' + #name + '" ');
  #out += ('VALUE="' + #checkedvalue + '" ');
    If( #value == #checkedvalue); 
      #out += ' CHECKED';
    /If;
  #out += '>\n';
  return(#out);
/Define_Tag;
Define_Tag('MO_formfieldAddcode',-required='name',-required='value');
  // the element to be added is defined in $searchfields, slot9
  // not operative for editform?? 
  local('out' = string );
	#out += '<input type="hidden" name="' + #name;
	#out += '" value="' + #value + '">';
  return(#out);
/Define_Tag;

/* *** FORMATTING *** */

Define_Tag('MO_fixedwidth_columnize',-required='arrays',-optional='margin',-optional='removeHtml');
  // input must consist of arrays, where each tablerow is a sub-array
  local('out' = string );
  local('err' = string );
  local('i' = array );
  local('j' = string );
  local('widths' = map );
  if(!(local_defined('margin')) );
    local('margin' = 4); // default value
  /if;
  if( local_defined('removeHtml') ); // even without value
    local('removeHtml' = 'yes');
    else;
    local('removeHtml' = 'no');
  /if;
  // if input is map, convert to array
  if(#arrays -> type != array);
    #err='must be array';
  /if;
  local('colcount' = #arrays -> get(1) -> size);
  if( #colcount == 1 );
    #err='Tag input contains only one column - please check!';
  /if;

  // define width of cols
    loop(#arrays -> first -> size);
        #widths -> insert(loop_count = 0);
    /loop;
  iterate( #arrays, #i );
    // initialize widths
    iterate(#i, #j); // get cell sizes
      if( (#j -> size) > (#widths -> find(loop_count)) );
        #widths -> insert(loop_count = #j -> size);
      /if;
    /iterate;
  /iterate;
  // add margins
  iterate( #widths, #i );
    #widths -> insert(loop_count = #i->second + #margin );
  /iterate;
  // TEST: #err = 'widths = ' #widths;
  // compose output
  iterate( #arrays, #i );
    #out += '\n';
    iterate( #i, #j );
      if(#j && #removeHtml == 'yes');
        #j -> replace('&euro;','e');
      /if;
      #out += #j;   
      loop((#widths -> get(loop_count) -> second) - (#j -> size));
        #out += ' ';
      /loop;
    /iterate;
  /iterate;
    // output
    if(#err); return(#err);
    else;     return(#out);
    /if;
/Define_Tag;

Define_Tag('MO_langSuffix');
              // define suffix for field names in the navig type
              local('out' = string);
              if($lang == $langs -> get(1));
                // no lang suffix
              else($langs -> size > 1); // lang2
                if($lang == $langs -> get(2));
                  #out = '_lang2'; 
                /if;
              /if;
            return(#out);
/Define_Tag;

Define_Type('MO_adminnavig');
    local('test'=string);
    local('mypage'=string);
    local('mycatid'=integer);
    local('sitecats'=array);
    local('sitecatids'=array);
    // set page contents (under constr.)
    local('myaddress'  = string);
    local('mymenuname'     = string);
    local('mymenuname_lang2'= string);
    local('myheading'  = string);
    local('mydomain'   = string);
    local('mycontent'  = string);
    local('mycontent_lang2'= string);
    local('myform'     = string);
    local('mycustomdisplay' = string);
    local('contentfile'  = string);
    local('dbConfig'  = array);
    local('table_prefix' = string);
    local('includefile' = string);

    Define_Tag('onCreate',-optional='page',-optional='table_prefix',-optional='includefile');
	// which page?
	if(!(local_defined('page')));
        local('page' = string);
        /if;
	if(!(local_defined('table_prefix')));
        local('table_prefix' = $table_prefix);
        /if;
	if(#page);
	    self->'mypage' = #page;
	    self->'test' = '1' + self ->'mypage';
	else;
	    self->'mypage' = 'etusivu';
	    self->'test' = '2' + self ->'mypage';
	/if;
        self->'table_prefix' = #table_prefix;
        if(local_defined('includefile'));
          self -> 'includefile' = 'yes';
          include(#includefile);
        else;
          local('includefile') = string;
        /if;

        // save dbConfig to the type - it may be overriden 
        // on the page before menu is called
        self -> 'dbConfig' = $dbConfig;
        if(! #includefile );
        inline(-search,self->'dbConfig',-table=#table_prefix+'sitepages_admin',
             -op = 'eq',
             'address' = self -> 'mypage');
             if(found_count == 1);
                self->'myheading'	    = field('heading');
                self->'myaddress'	    = field('address');
                self->'mymenuname'	    = field('menuname');
                self->'mymenuname_lang2'    = field('name_lang2');
                self->'mydomain'	    = field('domain');
                self->'mycontent'	    = field('content');
                self->'mycontent_lang2'     = field('content_lang2');
                self->'myform'	            = field('form');
                self->'mycustomdisplay'     = field('customdisplay');
                self -> 'mycatid'	    = field('catid');
             /if;
        /inline;
        else; // include file
          local('tmp') = $pages->find(#page)->get(1)->second;
          self->'myheading'	   = #tmp->find('heading')->get(1)->second;
          self->'myaddress'	   = #tmp->find('address')->get(1)->second;
          self->'mymenuname'	   = #tmp->find('menuname')->get(1)->second;
          self->'mymenuname_lang2' = #tmp->find('name_lang2')->get(1)->second;
          self->'mydomain'	   = #tmp->find('domain')->get(1)->second;
          self->'mycontent'	   = #tmp->find('content')->get(1)->second;
          self->'mycontent_lang2'  = #tmp->find('content_lang2')->get(1)->second;
          self->'myform'	   = #tmp->find('form')->get(1)->second;
          self->'mycustomdisplay'  = #tmp->find('customdisplay')->get(1)->second;
          self -> 'mycatid'	   = #tmp->find('catid')->get(1)->second; 
        /if;
	// define contentfile
	if((self->'mycustomdisplay') == 'on');
	    self->'contentfile'	    = '/cont/' + (self->'myaddress') + '.inc';
	else;
	    self->'contentfile'	    = '/cont/cont.inc';
	/if;

	// compose list of sitecats
        if(! #includefile );
	inline( -findall, self -> 'dbConfig', -table=#table_prefix + 'sitecats_admin',
	    -sortfield='sortcode');
	    records;
	      self -> 'sitecatids' -> insert( field('id'));
	      self -> 'sitecats'   -> insert( field('catname'));
	    /records;
	/inline;
        else; // use includefile
            loop($sitecats->size);
	      self -> 'sitecatids' -> insert($sitecatids->get(loop_count));
	      self -> 'sitecats'   -> insert($sitecats->get(loop_count));
            /loop;
        /if;
    /Define_Tag;

    // getters

    Define_Tag('get_test');	Return( self -> 'test'); /Define_Tag;
    Define_Tag('get_catid');	Return( Self -> 'mycatid'); /Define_Tag;
    Define_Tag('get_heading');	Return( Self -> 'myheading'); /Define_Tag;
    Define_Tag('get_address');	Return( Self -> 'myaddress'); /Define_Tag;
    Define_Tag('get_menuname');	Return( Self -> 'mymenuname'); /Define_Tag;
    Define_Tag('get_menuname_lang2'); Return( Self -> 'mymenuname_lang2'); /Define_Tag;
    Define_Tag('get_domain');	Return( Self -> 'mydomain'); /Define_Tag;
    Define_Tag('get_content');	Return( Self -> 'mycontent'); /Define_Tag;
    Define_Tag('get_content_lang2'); Return( Self -> 'mycontent_lang2'); /Define_Tag;
    Define_Tag('get_form'); Return( Self -> 'myform'); /Define_Tag;
    Define_Tag('get_contentfile'); Return( Self -> 'contentfile'); /Define_Tag;


    Define_Tag('get_mainmenu',-optional='realmenu');
	if (!(local_defined('realmenu')));
	    local('realmenu' = 'no');
	/if;
	local('i'=integer);
	local('lcount'=integer);
	local('output' = string);
	    // #output += '<div id="mainmenu">\n';
	    if(#realmenu=='yes');
		#output += '<ul class="menulist" id="listMenuRoot">';
		else;
		#output += '<ul>';
	    /if;
        self->'test' += 'scidstype is ' + self->'sitecatids'->type '. ';
        self->'test' += 'scids-get1type is ' + self->'sitecatids'->get(1)->type '. ';
        local('i') = integer;
	iterate( self->'sitecatids',#i);
            self->'test' += ' i is ' + #i + ', t=' #i -> type + ', ';
	    #lcount += 1;
	    local('catpageaddresses'  = array);
	    local('catpagenames'      = array);
	    local('catpagenames_lang2'= array);
	    local('catpagedomains'    = array);
	    // get pages for each cat
            if(! self -> 'includefile' );
              inline( -search, self -> 'dbConfig', 
                    -table = (self -> 'table_prefix') + 'sitepages_admin',
		    'catid' = #i,
		   -neq,
		   'draft' = 'on',
		    -sortfield = 'sortcode',
		    -maxrecords = all
                );
		if( found_count == 0); // if category is empty
			#catpageaddresses -> insert('#');
			#catpagenames -> insert('NO PAGES IN CATEGORY');
			#catpagenames_lang2 -> insert('NO PAGES IN CATEGORY');
			#catpagedomains -> insert($opts -> (find('sitename')));
		else;
		    records;
			#catpageaddresses -> insert(field('address'));
			#catpagenames -> insert(field('menuname'));
			#catpagenames_lang2 -> insert(field('name_lang2'));
			if( field('domain') == 'on'); // use secure server
			    #catpagedomains -> insert($secureServer);
			else;
			    #catpagedomains -> insert($myServer);
			/if;
		    /records;
		/if;
              /inline;
            else; // use includefile
              loop($pages->size);
                local('pg') = $pages->get(loop_count)->second;
              if(#pg->find('catid')->get(1)->second == integer(#i));
                if(#pg->find('draft')->get(1)->second != 'yes');
                  #catpageaddresses -> insert(#pg->find('address')->get(1)->second);
                  #catpagenames     -> insert(#pg->find('menuname')->get(1)->second);
                  #catpagenames_lang2 -> insert(#pg->find('name_lang2')->get(1)->second);
                  if( #pg->find('domain')->get(1)->second == 'on'); // use secure server
                      #catpagedomains -> insert($secureServer);
                  else;
                      #catpagedomains -> insert($myServer);
                  /if;
                /if;
              /if;
              /loop;
              self->'test' += #catpagenames;
            /if; // end includefile
	    #output += $navig -> get_thismainmenu(
		-addresses  =#catpageaddresses,
		-names	    =#catpagenames,
		-names_lang2=#catpagenames_lang2,
		-domains    =#catpagedomains,
		-catid	    =#i,
		-lcount  = #lcount
		);
	/iterate;
	#output += '</ul>\n';
//	#output += '<div style="clear:both;"></div>\n';
//	#output += '</div>\n';
	return(#output);
    /Define_Tag;

    Define_Tag('get_thismainmenu', 
	-required='addresses', 
	-required='names', 
	-required='names_lang2', 
	-required='domains', 
	-required='catid',
	-required='lcount'
        );
	local('output'=string);

        // main category (possibly with link to the first sub-item)
	if(#addresses -> size > 0); // if cat is not empty
          // next if is for the menu-drawing js (folding) 
          if(#addresses -> size == 1); // if cat is not empty
            // for these links, make link
	    if(#catid == ($navig -> get_catid));
		#output += '<li class="selected">';
	    else;
		#output += '<li>';
	    /if;
	    #output += '<a href="' + #domains->get(1); 
            #output += ((#domains->get(1)->endswith('/')) ? '' | '/');
	    #output += $myadminpages + '/' +  #addresses->(get(1)); 
	    #output +=  '" title="' + #domains->get(1) + ' ' + #names->(get(1));  
            #output +=  '" id="' + #catid + '">';
            if( (self ->'sitecats') != array);
                #output += (self -> 'sitecats')->(get(#lcount)); 
            /if;
	    #output += '</a>\n';
          else; // there are subitems
	    if(#catid == ($navig -> get_catid));
		#output += '<li class="selected menucat">';
	    else;
		#output += '<li class="menucat">';
	    /if;
            if( (self ->'sitecats') != array);
                #output += (self -> 'sitecats')->(get(#lcount)); 
            /if;
          /if;
	    // 'test: this='+$thiscatid+', i='+$i;
	    if( #addresses->size > 1);
                // subpages
              // a hack for folding other lists than parent of selected
              if(#catid == ($navig -> get_catid));
		#output += '\n  <ul>\n';
              else;
		#output += '\n  <ul class="hidechildren" ';
                // following not used?
                #output += ('id="ul' + #catid + '">\n');
              /if;
		loop( #addresses->size);
                if($myfilename == #addresses -> get(loop_count));
		    #output += '    <li class="selected">';
                else;
		    #output += '    <li>';
                /if;
		    #output += '<a href="' + #domains->get(loop_count); 
                    #output += ((#domains->get(loop_count)->endswith('/')) ? '' | '/');
                    #output += $myadminpages+'/'+#addresses->get(loop_count); 
                    #output += '" title="' + #names->get(loop_count);
                    #output += '" id="' + #catid + '">';
                    if( #names != array );
                        #output += #names->get(loop_count); 
                    /if;
		    #output += '</a>';
		    #output += '</li>\n';
		/loop;
		#output += '  </ul>\n';
	    /if;
	    #output += '</li>\n';
	/if; // end if cat is not empty
	return(#output);
    /Define_Tag;

    Define_Tag('get_submenu',-req='catid');
	local('address'  = string);
	local('name'	    = string);
	local('name_lang2'= string);
	local('domain'    = string);
	local('mypage' = (self -> 'mypage'));
	local('output'    = string);
          if(! self -> 'includefile' );
	    inline( -search, 
                    self -> 'dbConfig', 
                    -table = (self -> 'table_prefix') + 'sitepages_admin',
		    -op='eq',
		    'catid' = #catid,
		    -sortfield = 'sortcode');
		if( found_count > 1);
                    #output += '<p class="submenu">';
		    records;
			#address    = Field('address');
			#name	    = Field('menuname');
			#name_lang2 = Field('menuname_lang2');
			#domain	    = Field('domain');
			#output += $navig -> get_submenuitem(
			    -address	= #address,
			    -name	= #name,
			    -name_lang2 = #name_lang2,
			    -domain	= #domain,
			    -mypage	= #mypage
			    );	
                        if( loop_count < found_count );
                          #output += ' | ';
                        /if;
		    /records;
                    #output += '</p>';
		/if;
	    /inline;
          else; // use includefile
                  // first assemble category
                  local('catpageaddresses'  = array);
                  local('catpagenames'      = array);
                  local('catpagenames_lang2'= array);
                  local('catpagedomains'    = array);
              loop($pages->size);
                local('pg') = $pages->get(loop_count)->second;
              if(#pg->find('catid')->get(1)->second == integer(#catid));
                #catpageaddresses -> insert(#pg->find('address')->get(1)->second);
                #catpagenames     -> insert(#pg->find('menuname')->get(1)->second);
                #catpagenames_lang2 -> insert(#pg->find('name_lang2')->get(1)->second);
                if( #pg->find('domain')->get(1)->second == 'on'); // use secure server
                    #catpagedomains -> insert($secureServer);
                else;
                    #catpagedomains -> insert($myServer);
                /if;
              /if;
              /loop;
                // now handle the category
                if(#catpagenames -> size);
                  #output += '<p class="submenu">';
                  loop(#catpagenames->size);
                      #output += $navig -> get_submenuitem(
                        -address	= #catpageaddresses->get(loop_count),
                        -name	= #catpagenames->get(loop_count),
                        -name_lang2 = #catpagenames_lang2->get(loop_count),
                        -domain	= #catpagedomains->get(loop_count),
                        -mypage	= #mypage
			    );	
                      if( loop_count < #catpagenames->size );
                        #output += ' | ';
                      /if;
                  /loop;
                  #output += '</p>';
                /if;
          /if; // end includefile
		return(#output);
    /Define_Tag;

    Define_Tag('get_submenuitem',
	-req='address',
	-req='name',
	-req='name_lang2',
	-req='domain',
	-req='mypage'
	);
	local('output' = string);
	If( #mypage == #address);
	    #output += '<span class="L1_selected">';
		If( $lang=='en');
                    if(#name_lang2); #output += #name_lang2; /if;
		Else;
		    #output += #name;
		/If;
	    #output += '</span>';
	Else;
	    #output += '<A HREF="' + #address + '" class="L1">';
		If( $lang=='en');
                    if(#name_lang2); #output += #name_lang2; /if;
		Else;
		    #output += #name;
		/If;
	    #output += '</A>';
	/If;
	return(#output);
    /Define_Tag;

/Define_Type;

/* STRINGS */
define_tag('MO_valStr',-required='fLabel',-type='string',
                       -required='valName',-type='string',
                       -optional='moreinfo',
                       -encodeNone);
  local('valErr' = map);
  local('label' = '<b>' + #fLabel + '</b>');

    #valErr-> insert('req'=str('Please give') + ' ' + #label + '.');
    #valErr-> insert('unique'=str('Duplicate value in field')+' '+#label+'.');
    #valErr-> insert('dateValid'=str('The date ')+str('in field ') + #label 
        + str(' is not valid.'));
    #valErr-> insert('dateOld' = str('The date ') + str('in field ') + #label 
        + str(' does not seem to make sense.')); 
    #valErr-> insert('time'    = str('The time ') + str('in field ') + #label 
        + str(' is not valid.'));
    #valErr-> insert('email'    = str('The email address ') + str('in field ') + #label 
        + str(' is not valid.'));
    #valErr-> insert('password_forbidden'= str('There are forbidden characters ') +
        str('in field ') + #label + '. ' +
        str('Only a-z, A-Z, 0-9, -, _ are allowed.'));
    #valErr-> insert('password_short'    = str('The value ') + str('in field ') + #label 
        + str(' is too short.'));
    #valErr-> insert('password_long'    = str('The value ') + str('in field ') + #label 
        + str(' is too long.'));
    #valErr-> insert('alphanum'= str('Only alphanumeric characters (a-z, A-Z 
        and 0-9) are allowed in the field ')  
        + #label + '.' );
    #valErr-> insert('int'= str('Only numbers are allowed ') 
        + str('in the field ') + #label + '.');
    #valErr-> insert('float'= str('Only numbers (0-9), commas and hyphens are allowed ')
        + str('in the field ') + #label + '.');
    #valErr-> insert('html'= str('There are forbidden characters ')
        + str('in the field ') + #label + '.');
    #valErr-> insert('companyid_short'= str('The company ID ')
        + str('in the field ') + #label + str(' is too short.'));
    #valErr-> insert('companyid_long'= str('The company ID ')
        + str('in the field ') + #label + str(' is too long.'));
    #valErr-> insert('companyid_notvalid'= str('The company ID ')
        + str('in the field ') + #label + str(' is not valid.'));
    #valErr-> insert('personid_notvalid'= str('The person ID ')
        + str('in the field ') + #label + str(' is not valid.'));
    // add html line break
    local('i' = pair);
    iterate(#valErr,#i);
      #valErr -> insert((#i -> first) = (#i -> second + '<br>'));
    /iterate;
  return(#valErr -> find(#valName));
/define_tag;

define_tag('MO_buttonStr',-required='buttonname');
  local('buttonStr' = map);
    #buttonStr -> insert('add' = str('Save a new ') + var('rectype_'+$lang));
    #buttonStr -> insert('update' = str('Save changes'));
    #buttonStr -> insert('delete' = str('Delete'));
    #buttonStr -> insert('quickdelete' = str('Delete')+' ('+str('NO WARNING')+')');
  return(#buttonStr -> find(#buttonname));
/define_tag;

define_tag('showfiles', -namespace='MO',
                        -required='filepath',-type='string',
                        -required='cat_id',-type='integer',
                        -required='inlinestuff',-type='array',
                        -encodenone);
  // process and display files
  local('out'         = string);
  local('aFiles'      = array); 
  local('linked_files'= '');    // for temp storing 
  local('shown_files' = '');	  // for temp storing
  local('error' 	      = string);    
  local('j' 	      = array);    

  // find the files first
	  inline( $dbConfig, -table=$table_prefix+'uploads',
		'upl_catid'=#cat_id,
		#inlinestuff,
		-MaxRecords='all',
		-Search
              );
              MO_errorhandler(-level='normal');
              #error = found_count;
              // return(#error);
            Records;
                // collect file data
                #aFiles -> Insert(Array(
                  Field('upl_path'),
                  Field('upl_name'),
                  Field('upl_description'), 
                  Field('upl_imagedisplay'), 
                  Field('upl_linkurl')));
            /Records;
	  /Inline;

// now iterate through search results

  Iterate( #aFiles, #j);
        // set vars
        local('mysize_shown'  = string);   // reset
        local('mysize_linked' = string);   // reset
        local('myclass'            = string);   // reset
        local('myimagetag'         = string);   // reset

        local('mypath'        = #j -> get(1));
        local('myname'        = #j -> get(2));
        local('mydescription' = #j -> get(3));
        local('myimagedisplay'= #j -> get(4));
        local('mylinkurl'     = #j -> get(5));

        local('mysuffix'        = #myname  -> split('.') -> Last);

        If(  #myimagedisplay == 'none'); // just skip the file
        Else( (#mysuffix == 'jpg' 
                ||  #mysuffix == 'jpeg'
                ||  #mysuffix == 'gif'
                ||  #mysuffix == 'png') 
                &&  #myimagedisplay != 'link'); // display that img

            // define css class
            If( #myimagedisplay == 'right');
                if(#mypath >> 'halfcolumn');
                    #mysize_shown = 'halfcolumn';
                    #myclass = 'alignright';
                    if(#mypath >> 'fullscreen');
                        #mysize_linked = 'fullscreen';
                    else(#mypath >> 'fullcolumn');
                        #mysize_linked = 'fullcolumn';
                    /if;
                else(#mypath >> 'original');
                    #mysize_shown = '';
                    #myclass = 'alignright';
                else(#mypath >> 'fullcolumn');
                        #mysize_shown = 'fullcolumn';
                        #myclass = 'centered';
                else;
                        // show only link!
                /if;
            Else( #myimagedisplay == 'left');
                if(#mypath >> 'halfcolumn');
                    #mysize_shown = 'halfcolumn';
                    #myclass = 'alignleft';
                    if(#mypath >> 'fullscreen');
                        #mysize_linked = 'fullscreen';
                    else(#mypath >> 'fullcolumn');
                        #mysize_linked = 'fullcolumn';
                    /if;
                else(#mypath >> 'original');
                    #mysize_shown = '';
                    #myclass = 'alignleft';
                else(#mypath >> 'fullcolumn');
                        #mysize_shown = 'fullcolumn';
                        #myclass = 'centered';
                else;
                        // show only link!
                /if;
            Else( #myimagedisplay == 'block');
                if(#mypath >> 'original');
                    #mysize_shown = '';
                else(#mypath >> 'fullcolumn');
                    #mysize_shown = 'fullcolumn';
                /if;
                #myclass = 'centered';
            /If;

            // compose imagetag
            #myimagetag += '<IMG class="' + #myclass + '" SRC="';
            #myimagetag += #filepath + #mysize_shown 
                + '/' + #myname + '" alt="';
            #myimagetag += #mydescription + '" title="';
            #myimagetag += #mydescription + '">\r';

            // is the image supposed to act as link?
            // (either to url defined in db, or to a bigger-size img)
            if( #mylinkurl);
              #shown_files += '<a href="' + #mylinkurl + '">';
              #shown_files += #myimagetag + '</a>\n';
            else(#mysize_linked);
              #shown_files += '<a href="'+ #filepath + #mysize_linked;
              #shown_files += '/' + #myname + '">';
              #shown_files += #myimagetag + '</a>\n';
            else;                             // no link
              #shown_files += #myimagetag;
            /if;

        Else; 			 	// just display link
            #linked_files += '<A HREF="' +  #filepath + #mysize_linked 
              + #myname + '" TITLE="' +  #mydescription
              + '">' + #myname + '</A><br>\n';
        /If;
  /Iterate;

  // now display

  #out += #shown_files  + '\n\n';
  #out += #linked_files + '\n\n';
  return(#out);
/define_tag;

define_tag('showfilesmakeSortstuff', -namespace='MO',
                        -required='order',-type='string',
                        -encodenone);
  // put together sortstuff array, based on filecat search
  local('out' = array);
          // assemble sortstuff for filesearch
          If( #order == 'alphabetical');
              #out -> (Insert( -SortField = 'upl_name'));
              #out -> (Insert( -SortOrder = 'ascending'));
          Else( #order == 'oldestfirst');
              #out -> (Insert( -SortField = 'id'));
              #out -> (Insert( -SortOrder = 'ascending'));
          Else( #order == 'newestfirst');
              #out -> (Insert( -SortField = 'id'));
              #out -> (Insert( -SortOrder = 'descending'));
          Else( #order == 'custom');
              #out -> (Insert( -SortField = 'upl_customsortcode'));
              #out -> (Insert( -SortOrder = 'ascending'));
              #out -> (Insert( -SortField = 'upl_name'));
              #out -> (Insert( -SortOrder = 'ascending'));
          Else; 	// the default is "descending"
              #out -> (Insert( -SortField = 'id'));
              #out -> (Insert( -SortOrder = 'descending'));
          /If;
  return(#out);
/define_tag;

define_tag('heading', -namespace='MO',
                        -required='nicename',-type='string',
                        -required='description',-type='string',
                        -encodenone);
  // Print the heading, with description
  local('out' = string);
    #out +='<h3>' + #nicename; 
        #out += '<span STYLE="font-size:80%;font-weight:normal;">&nbsp;&nbsp;'; 
	#out += #description + '</span>';
    #out += '</h3>\n';
  return(#out);
/define_tag;

// there is JH_getValuelist in library.inc
// what is the difference?
define_tag('composeValuelist', -namespace='MO',
                        -required='table',-type='string',
                        -required='showfield',-type='string',
                        -required='sortfield',-type='string',
                        -optional='searchstuff',-type='array',
                        -encodenone);
/* example use
var('i' = MO_composeValuelist(-table='groups',-showfield='name',-sortfield='name'));
var('crtrs1' = $i -> first);
var('crtrs2' = $i -> second);
*/
  local('out'     = pair);
  local('values1' = string);
  local('values2' = string);
  local('err' = string);
  if(!(local_defined('searchstuff')));
    local('searchstuff' = array);
  /if;
    inline(-search,$dbConfig, 
         -table=$table_prefix + #table,
         -maxRecords='all',
         #searchstuff,
        -sortField=#sortfield);
            #err = error_msg;
            Records;
              if(loop_count > 1);
                #values1 += ';';
                #values2 += ';';
              /if;
              #values1 += field('id');
              #values2 += field(#showfield);
            /Records;
    /inline;
  #out = pair(#values1=#values2);
  return(#out);
/define_tag;

Define_tag('valuelistsFromRelatedTable',-namespace='MO',
                        -required='table',-type='string',
                        -required='showfield',-type='string',
                        -required='sortfield',-type='string',
                        -optional='searchstuff',-type='array',
                        -encodenone);
/* 
collect value lists for select menu from related table. example use
var('i' = JH_valuelistsRromRelatedTable(-table='groups',-showfield='name',-sortfield='name'));
var('fieldvalues' = ';'         + $i -> first);
var('showvalues'  ='--VALITSE;' + $i -> second);
*/            
local('a' = string);
local('b' = string);
	inline( $dbConfig,-table=$table_prefix + #table,
	    -sortfield=#sortfield,
            -maxrecords='all',
	    -findall);
	    records;
		#a += field('id');
		#b += field(#showfield);
                if(loop_count < shown_count);
                  #a += ';';
                  #b += ';';
                /if;
	    /records;
	/inline;
  return(pair(#a = #b));
/define_tag;

define_tag('getNameByID', -namespace='MO',
                        -required='table',-type='string',
                        -required='idvalue',
                        -required='namefield',-type='string',
                        -optional='idname',-type='string',
                        -optional='debug',-type='string',
                        -optional='fail_gracefully',-type='string',
                        -encodenone);
/*
example
var('mycompany' = MO_getNameByID(-table='yritykset',-idvalue=$userid_ses,-namefield='yritys'));
*/
  local('out'        = string);
  if( ! local_defined('debug') );
    local('debug' = string);
  /if;
  if( ! local_defined('idname') );
    local('idname' = 'id'); // default id field
  /if;
  if( ! local_defined('fail_gracefully') );
    local('fail_gracefully' = string); 
  /if;
  local('debug_msg' = string);
    inline(-search,$dbConfig, 
               -table=$table_prefix + #table,
               -op='eq',
               #idname=#idvalue);
      if(found_count != 1 && ! #fail_gracefully);
        #out = ('ERROR IN TAG. found_count was ' + found_count 
          + '(not 1). Idvalue was ' + #idvalue + '.');
      else;
        Records;
          #out = field(#namefield);
        /Records;
      /if;
      #debug_msg += 'Table was ' + #table + '.<br>';
      #debug_msg += 'Available field names are ' + field_names + '.<br>';
    /inline;
  if(#debug);
    return(#debug_msg);
  else;
    return(#out);
  /if;
/define_tag;

define_tag('showlinks', -namespace='MO',
                        -required='cat_id',-type='integer',
                        -required='inlinestuff',-type='array',
                        -encodenone);
  // process and display files
  local('out'         = string);
  local('error'         = string);

  // find the links first
	  inline( $dbConfig, -table=$table_prefix+'links',
                -op='eq',
		'category'=#cat_id,
		#inlinestuff,
		-MaxRecords='all',
		-Search);
              MO_errorhandler(-level='normal');
              #error = found_count;
              // return(#error);
            Records;
                    #out += '<p><a href="' + Field('url') + '" title="';
                    #out += Field('description') + '">' + Field('name') + '</a></p>';

            /Records;
	  /Inline;
  return(#out);
/define_tag;


define_tag('fvalues', -namespace='MO',
                        -required='editfields',-type='array',
                        -required='dbChange_error',
                        -required='viewtype',
                        -encodenone);
  local('errors'       = array);
  local('diagn'       = array);
  local('case'       = integer);
  local('this'       = array);
  local('out'         = map);

/* example usage
  var('fvalues' = MO_fvalues(
    -editfields=$editfields,
    -dbChange_error=$dbChange_error,
    -viewtype=$viewtype));
*/
  iterate(#editfields,#this);
    // handle too short arrays
    if(#this -> size < 9);
      $speakBubble += 'The $editFields line ' + #this->get(2) + ' is too short. ' 
        + 'Minimun length is 9 slots.<br>';
      MO_speakBubble($speakBubble,-style='color:#c00;font-size:1.4em;');
      abort;
    /if;

    // helper vars
    local('fName'  = #this -> get(1));
    local('fLabel' = #this -> get(2));
    local('fType'  = #this -> get(3));
    local('valtypes' = #this -> get(4) -> split(','));
    local('fValue'  = string);
    local('saveIfNotEmpty'  = string);
    if(#this->size > 9);
      #saveIfNotEmpty = #this -> get(10);
    /if;

    //      * * * define  fvalue * * *

    if( #saveIfNotEmpty == 'saveIfNotEmpty' );
      // do not output the value
    else( #dbChange_error);
      if(#viewtype == 'listview' && found_count);
        #fvalue = field(#fName);
      else;
        #fvalue = $inputVars -> find(#fName);
      /if;
      #case=1;
    else(#fType == 'heading' || #ftype == 'heading2');
        #fvalue = #this -> get(9);
        #case=3;
    else(  $emptyForm == 'yes');
      if(#this->Size > 11); // if default val (#12) has been defined
        #fvalue = #this -> get(12);
      else;
        #fvalue = string;
      /if;
      #case=4;
    else; 				// use field
        #fvalue = field(#fName);
        #case=5;
    /if;
    #diagn -> insert(#fname = #case);

    // 	* * * reformat fvalue where needed  * * * 

    If( $emptyForm != 'yes');		

        If( #valtypes -> find('datetime') != array); 	// datetime

           // is the input really datetime
           if( string(#fvalue) -> size == 10 );
            #fvalue = string;
            #errors -> insert(#fname = 'field labeled datetime, but input to fvalues is date'); // the input is date, not datetime
           else( #fvalue && (#fvalue != '0000-00-00 00:00:00') ); // if not empty
               #fvalue = 
                 date(#fvalue)->format($opts->find('datetime_showformat'));
             else;
                 #fvalue = string;
           /If;
           
        Else( #valtypes -> find('timestamp') != array ); 	// timestamp

            If( #fvalue != '' && #fvalue != '00000000000000'); // if not empty
                #fvalue = date(#fvalue,-format='%Y%m%d%H%M%S')
                    -> format($opts -> find('datetime_showformat'));
            /If;

        Else( #valtypes -> find('date') != array );

           If( #fvalue && (#fvalue != '0000-00-00' )); // if not empty
               if( #dbChange_error);
               #fvalue = date(#fvalue,-format=($basicOptions -> find('date_showformat'))) -> format($basicOptions -> find('date_showformat')); // 2012-01
               else;
                 #fvalue = 
                   Date_Format(Date(#fvalue,
                   -Format= ($opts -> find('date_dbformat'))), 
                   -Format= ($opts -> find('date_showformat' )));
               /if;
           else; // if empty or 0000-00-00
             #fvalue = string;
           /If;
            
        Else( #valtypes -> find('time') != array );

            If( #fvalue ); // if not empty
                if( #dbChange_error);
                  #fvalue = 
                    date(#fvalue,
                       -format=($opts -> find('time_showformat'))) 
                    ->  format(($opts -> find('time_showformat')));
                else;
                  #fvalue = 
                    date(#fvalue,-Format='%T') 
                    -> format($opts -> find('time_showformat'));
                /if;
            /If; 
        Else( #valtypes -> find('float') != array ); // floating-point number
                if( ! #dbChange_error );
                   #fvalue -> replace('.',$opts -> find('floatseparator_show')); 
                /if;
        /If; 
        // encrypted fields
        If( #this -> size > 10 ); // if encrypt (#11) is defined
            If( #this -> Get(11) == 'blowfish');
                #fvalue = Decrypt_BlowFish(field(#fname),-Seed=$seed);
            /If;
        /If;
    /If; 					// end if emptyform != 1

        // ftype replacecode handled elsewhere
        // see inc/editform and ctag formfield_replacecode

    // insert value to output
    #out -> insert(#fname = string(#fvalue));
  /iterate;
  return(map('fvalues'=#out,'errors'=#errors,'diagn'=#diagn));
/define_tag;

Define_Tag('MO_listview_fieldlabels',-required='editfields',-encodeNone);
  local('out' = string);
  local('this' = array);
  #out += '<tr>';  
  Iterate( #editfields, #this);
      If( (#this -> get(3)) != 'hidden' 
          && #this -> get(3) != 'moddate'
          && #this -> get(3) != 'creationdate' );
          #out += '<th class="listviewlabel" scope="col">';
          #out += #this -> get(2); // nimilappu
              If( (#this -> get(4)) >> 'req');
                $reqFieldsFound = 'yes';
                #out += '*';
              /If; // if pakollinen kentt‰
          #out += '</th>\r';
      /if;
  /Iterate;
  #out += '</tr>\r';
  return(#out);
/Define_Tag;
Define_Tag('MO_listview_helptexts',-required='editfields',-encodeNone);
  local('out' = string);
  local('this' = array);
  #out += '<tr>';  
  Iterate( #editfields, #this);
      If( (#this -> get(3)) != 'hidden' 
          && #this -> get(3) != 'moddate'
          && #this -> get(3) != 'creationdate' );
          #out += '<td class="' + $textClass + '" valign="top" style="padding-bottom:10px;">'; 
            // helptext div, in case there is ajax validation
          #out += '<div class="helptext" id="help-'+ #this->get(1) + '" style="padding-left:0;"> ';
              If( #this -> get(7) != ''); // helppiteksti
                  #out += #this -> get(7); // helppiteksti
              /If;
          #out += '</div>\n';
          #out += '</td>\r';
      /if;
  /Iterate;
  #out += '</tr>\r';
  return(#out);
/Define_Tag;

Define_Tag('MO_listview_emptyform',-required='editfields',-encodeNone);
  // create empty form for listview
  local('out' = string);
  local('this' = array);
  #out += '<form name="editform" class="listview_addform" action="' + $formTargetpage;
      if( $myp);   
        #out += '?p='; $myp;    
      /if;
  #out += '" method="POST" style="padding:10px;border:1px solid black;">\n';
  #out += '<input type="hidden" name="ref" value="editform">\n';
  #out += '<input type="hidden" name="skip" value="' + $skip + '">\n';
  local('labelrow' = string);
  local('datarow' = string);
  local('tdcount' = integer(1));
  var('fieldvalue'=string); 
  iterate(#editfields,#this);
    If(     #this->get(3) != 'hidden' 
         && #this->get(3) != 'moddate' 
         && #this->get(3) != 'creationdate');
      #datarow += '<td valign="top" class="listview_addform" style="';
      #labelrow += '<th class="listviewlabel_addform" style="';
      if(loop_count == 1);
        #datarow += 'border-left-width:1px;';
        #labelrow +='border-left-width:1px;';
      /if;
      #datarow += '">';
      #labelrow += '">' + #this->get(2) + '</th>';
        #datarow += MO_formfieldHandler(-this=#this,-fvalue=string);
        // helptext div, in case there is ajax validation
        #datarow += '<div class="helptext" id="help'+loop_count+'-' + #this -> get(1) +'"> ';
        #datarow += '</div>\n';
      #datarow += '</td>\n';
      #tdcount += 1;
    Else;   // hidden field
        #datarow += '\n';
        #labelrow += '\n';
    /If; 
  /iterate;
  // upper row
  #out += '<tr><td class="listview_addform_upper" colspan="'+#tdcount+'" style="border-bottom-width:0px;">' + str('Add a new ') + var('rectype_'+$lang) + '</td></tr>';
  #out += '<tr>\n';
  #out += #labelrow;
  #out += '<th class="listviewlabel_addform" style="border-right-width:1px;">&nbsp;</th>';
  #out += '</tr>\n';
  #out += '<tr>\n';
  #out += #datarow;
  #out += '<td class="listview_addform" STYLE="border-bottom-width:1px;border-right-width:1px;">';
  #out += '<input type="Submit" class="submit" name="add" value="';
      #out += MO_buttonStr('add');
  #out += '">\n';
  #out += '</td></tr>\n';
  #out += '</FORM>';
  #out += '<tr><td>&nbsp;</td></tr>\n\n';
  return(#out);
/Define_Tag;


Define_Tag('inputVars',-namespace='MO',-required='editfields',-encodeNone);
  /* 
      create variables from editform input
      example use - var('inputVars' = MO_inputVars($editfields);
  */
  local('inputVars' = map);
  local('this' = array);
  Iterate( #editFields,#this );
      local('ftype' = #this -> get(3));
      if(  #ftype == 'heading' || #ftype == 'heading2');
      else( #ftype == 'hidden' || #ftype == 'noentry');
          // helper var - url_encode necessary for quote char
          local('myActpar')  = decode_html(decode_url(action_param(#this->get(1))));
          #myActpar -> trim;
          #inputVars -> insert( #this->get(1) = #myActpar );
      else;
          // helper var
          local('myActpar'  = action_param(#this -> get(1)));
          #myActpar -> trim;
          #inputVars -> insert( #this->get(1) = #myActpar );
      /if;
      // check array length
        if( #this -> size < 9);
            $speakBubble = 'At least one line in editfields array is too '
            + 'short. The line (array) must have at least 10 slots. The first '
            + 'offending line was ' + loop_count + '.';
            fail(-1,$speakBubble);
        /if;
      // capture deprecated code
          if( #ftype == 'nosave' || #ftype == 'override');
                $speakBubble = 'Field handlers "nosave" and "override" have been deprecated. Please correct your searchFields variable. Use "replacecode" (where you can use slot 9 for field presentation, if necessary), or "noentry" (if you want to save but not let the user modify the value).';
            fail(-1,$speakBubble);
          /if;

      // modify editfields if date handling uses deprecated standards
        if(#this->get(8) == 'date'
              || #this->get(8) == 'datetime'
              || #this->get(8) == 'time');
          if(#this->get(4));
            #this->get(4) += ',' + #this->get(8);
          else;
            #this->get(4) = #this -> get(8);
          /if;
          #this -> get(8) = string;
        /if;
        // end capture deprecated code
        $editfields -> remove(loop_count);
        $editfields -> insert(#this,loop_count);

  /Iterate;
  return(#inputVars);
/Define_Tag;

define_tag('update', -namespace='MO',
                        -required='table',-type='string',
                        -optional='idname',-type='string',
                        -required='idvalue',
                        -required='stuff',-type='array',
                        -encodenone);
/*
example
var('mycompany' = MO_update(-table='tilaukset',-idvalue=$tilausid,$stuff));
*/
  if( ! local_defined('idname') );
    local('idname' = 'id'); // default id field
  /if;
    inline(-update,$dbConfig,
               -table=$table_prefix + #table,
               -keyfield=#idname,
               -keyvalue=#idvalue,
               #stuff);
      if(error_msg != error_noError);
        return(str('ERROR IN TAG. ') + str('The update FAILED. '));
      /if;
    /inline;
    return(string);
/define_tag;

define_tag('createdir',-namespace='MO',
  -required='dirname',-type='string',
  -encodenone);
// fail(-1,'input was ' + #dirname + ', apath is ' + $apath);
    /* dirname should end with slash
    */
    local('out') = string;
    if(! (#dirname -> endswith('/')) );
      fail(-1,'Dirname should end with slash. Check your code.');
    /if;
  if( file_exists(#dirname) );
  else;
    if(integer(lasso_version(-lassoversion)) >= 9);
      dir(#dirname) -> create;
    else;
      file_create(#dirname); // does not work for dirs in L9.0-L9.1.3?
    /if;
    if( File_CurrentError == 'No error');
      #out = ('The directory ' + #dirname +' was created! You may have to manually check its permissions; Lasso should have full rights to the directory.<br>');
    else;
      #out = ('Tried to create the directory ' + #dirname + ', but
      encountered an error: ' + File_CurrentError + '.
      Please check a) file permissions and b) the privileges for your db
      user (if Lasso version is < 9). If this is LassoBlogger, see blog
      docs at <code>docs/lb_uploads.txt</code><br>');
    /if;
  /if;
  return(#out);
/define_tag;


?>

