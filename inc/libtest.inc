[
Define_Type('LB_Userinfo');
	Local('inlinestuff' = Array);
	Local('use_level' = integer);
	Local('user_id' = integer);
	Local('cusername' = '');
	Local('cpassword' = '');
	Local('tst' = '');
	Local('found' = Integer);
    Define_Tag('onCreate',-optional='username',-optional='password');
        if(! local_defined('username'));
          local('username') = string;
        /if;
        if(! local_defined('password'));
          local('password') = string;
        /if;
        local('hash' = LB_createCookiehash);
        if(#username && #password);
          self->'tst' += 'case 1, ';
          (self->'inlinestuff')->insert(-operator = 'eq');
          (self->'inlinestuff')->insert('use_login' = #username);
          (self->'inlinestuff')->insert(-operator = 'eq');
          (self->'inlinestuff')->insert('use_pass'=encrypt_md5(#password));
	else( Cookie('siteuser_' + #hash) && Cookie('sitepass_' + #hash));
          self->'tst' += 'case 2, ';
          // use cookies
          (self -> 'inlinestuff') -> Insert(-operator = 'eq');
          (self -> 'inlinestuff') -> 
            Insert('use_login' = Cookie('siteuser_' + #hash));
          (self -> 'inlinestuff') -> Insert(-operator = 'eq');
          (self -> 'inlinestuff') -> 
	      Insert('use_pass' = Cookie('sitepass_' + #hash));
	Else( Variable_Defined('user_ses'));  
          self->'tst' += 'case 3, ';
	// use session vars (probably never used, as public pages
	// don't use session)
	    (self -> 'inlinestuff') -> Insert(-operator = 'eq');
	    (self -> 'inlinestuff') -> Insert('id' = $user_ses);
	/If;
        If( (Self -> 'inlinestuff') != Array);
	// verify the user data
	    Inline( -search,$dbConfig, 
		-table = $table_prefix+'users',
		Self -> 'inlinestuff');

		If( error_msg == Error_NoError && Found_Count == 1);
                  (Self -> 'cusername') = Field('use_login');
                  (Self -> 'cpassword') = Field('use_pass');
                  (Self -> 'use_level') = integer(Field('use_level'));
                  (Self -> 'user_id')   = integer(Field('id'));
                  // write/refresh cookies
                  local('path') = response_path;
                  #path -> RemoveTrailing('admin/');
                  Cookie_Set( ('siteuser_' + #hash)
                  = self->'cusername',-Expires=432000,-Domain=$myDomain,-Path=#path);
                  Cookie_Set( ('sitepass_' + #hash)
                  = self->'cpassword',-Expires=432000,-Domain=$myDomain,-Path=#path);
                  // remember that cpassword is now md5-encrypted
		Else;
		    (Self -> 'use_level') = (-2); // no match?
		/If;
		(Self->'found') = Found_Count;
	    /Inline;
	Else;				
	    (Self -> 'use_level') = (-1); 	// no cookies, no session
	    // (Self->'tst') = Cookie('siteuser_' + #hash);
	/If;
		(Self->'tst') += 'mydom = ' + $mydomain;
    /Define_Tag;

    Define_Tag('LB_getUse_level');
	Return( Self -> 'use_level');
    /Define_Tag;
    Define_Tag('LB_getUser_id');
	Return( Self -> 'user_id');
    /Define_Tag;
    Define_Tag('tst');
	Return( Self -> 'tst');
    /Define_Tag;
/Define_Type;
]
