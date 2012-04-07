<?LassoScript
  $debug ? 'User list is ' + $userlist + '.<br>'; 

  if( $userlist == 'database');

    if($db_pw_encrypt == 'md5' && $auth_source != 'cookie');
      $pw_input = encrypt_md5($pw_input);
    /if;

    inline( -search, 
        $dbConfig,
        -table=$table_prefix + $db_authtable,
        -operator='eq',
        $db_unfield=$un_input, 
        -operator='eq',
        $db_pwfield=$pw_input 
    );
      $debug ? 'action statement is ' + action_statement + $br;
      $debug ? 'found_count was ' + found_count + $br;
      $debug ? 'inline error was ' + error_msg + $br; 

      If( error_msg == Error_NoError
                  && Found_Count == 1 
                  && $un_input == Field($db_unfield)
                  && $pw_input == Field($db_pwfield));
        $valid_ses = 1;
        $userid_ses = integer(field('id'));
        $userlevel_ses = integer(field('use_level'));
        $username_ses = $un_input;
        $username_human_ses = field('etunimi') + ' ' + field('sukunimi');
        $debug ? ('success. Userid=' + field('id') + ', name=' field('etunimi') + ' ' + field('sukunimi') + $br);
      /if;
    /inline;
  else( $userlist == 'adminusername');
      $debug ? 'Checking admin username...<br>'; 
      If( $un_input == $adminUsername && $pw_input == $adminPassword);	
        $debug ? 'Successful identification.<br>'; 
        $valid_ses = 1;
        $username_ses = $adminusername;
        $username_human_ses = $adminusername;
	$userlevel_ses = 10; 		// default =10
      else;
        $debug ? 'Unsuccessful identification.<br>'; 
        $debug ? $un_input + ', ' + $pw_input + '.<br>'; 
      /if;
  /if;
  if($valid_ses == 1);
        if($accept_cookies == 'yes');
          // write cookies
          var('hash' = (LB_createCookiehash));
          Cookie_Set( ('user_' + $hash) 
              = $un_input,-Expires=432000,-Domain=$myDomain,-Path='/');
          Cookie_Set( ('pass_' + $hash) 
              = $pw_input,-Expires=432000,-Domain=$myDomain,-Path='/');
        /if;
  else; // if username & password were NOT ok
          var('valid_ses' = -1);
          $speakBubble = str('The user could not be identified.');
  /If;

// logincount - prevent flooding
// temporarily disabled

?>

    [Output_None][NoProcess]

    If( $logincount_ses == 5);  /* flood attempt - send email */ 
	var('emailbody' = 'A user has tried to login 5 times. ' + 
       'On the last trial, the username was \"' + $un_input + '\"');

	Email_Send( 				// fill in -Host, if needed
	-From = $opts -> Find('admin_email'),
	-To   = $opts -> Find('admin_email'),
	-Subject = ($opts -> Find('sitename')) + ' - flood attempt detected',
	-Body = $emailbody
        );

	Else( $logincount_ses > 7);   // seven trials - say goodbye  
          MO_speakBubble(str('Too many trials. Please try again later!'));
	Abort;
    /If; 			// end prevent flooding 
    [/NoProcess][/Output_None]

<?LassoScript

// /Inline;
?>
