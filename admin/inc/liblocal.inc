<?lassoscript

define_tag('orderCalcFirstNr', -namespace='RA',
                        -required='company_id',-type='integer',
                        -required='series_size',-type='integer',
                        -required='order_nr',-type='integer',
                        -encodenone);
  // rahtikirjanumerot
  local('start' = (_date_msec));

  local('out'           = integer);
  local('debug_msg'     = string);
  local('error_msg'     = string);
  local('customseries'  = array);
  local('usecustom'     = integer);
  local('lowestpublicnr'= 20000000000); // 20 billion, not incl tarkistusnumero
  local('firstnr'       = integer); // 20 billion, not incl tarkistusnumero
  local('firstnr_ok'    = string); 
  local('lastnr'        = integer);

  // 1. should we use custom series?

    inline(-search,$dbconfig, -table=$table_prefix + 'varatutsarjat',
        -op = 'eq',
        'yritys_id' = #company_id,
        -sortfield='numerosarja'
        );
        if(found_count >= 1);
          records;
            #customseries -> insert(integer(field('numerosarja')));
          /records;
          #usecustom = 1; // will be saved to table tilaukset
          #debug_msg += 'This company (id = ' + #company_id 
            + ') uses custom series.\n';
        else;
          #debug_msg += 'This company (id = ' + #company_id 
            + ') does *not* use custom series.\n';
        /if;
    /inline; 


  // 2. define first free number

    local('inlinestuff' = array);

    if( #customseries );
      local('i' = integer);
                    #debug_msg += 'This company has ' + #customseries->size 
                      + ' custom number series.\n';
      iterate(#customseries,#i);
        #inlinestuff -> insert(-op = 'gte');
        #inlinestuff -> insert('ekanumero' = string(#i) + '000000000');
        #inlinestuff -> insert(-op = 'lt');
        #inlinestuff -> insert('ekanumero' = string(#i + 1) + '000000000');
        #firstnr = RA_orderSubSearch(#inlinestuff);
          if(#firstnr);
            if( RA_fit(-firstnr = #firstnr, -series_size = #series_size) );
                // the order will fit in the same 3-nr series
                #firstnr_ok = 'yes';
                #debug_msg += 'custom series nr '  
                + (string(#firstnr) -> substring(1,3)) + ' has free space.\n'; 
                loop_abort;
            else; // continue to the next custom series (hope it exists)
            /if;
          else; // could be their first order
            #debug_msg += 'This could be their first order with this cseries.';
            #firstnr = #i * 1000000000; // add 9 zeroes
              if( RA_fit(-firstnr = #firstnr, -series_size = #series_size) );
                #firstnr_ok = 'yes';
              /if;
          /if;
      /iterate;
      if( ! #firstnr_ok ); // Houston - we have a problem
        #error_msg += 'Custom series for this company have been used! '
          + 'Please contact the administrator at ' 
          + $opts -> find('admin_email') + '.\n';
        return(#error_msg);
      /if;

    else; // if company does not use custom series 

      #inlinestuff -> insert(-neq);
      #inlinestuff -> insert('varattusarja' = '1');
      #firstnr = RA_orderSubSearch(#inlinestuff);
            if( #firstnr < #lowestpublicnr );
                #firstnr = #lowestpublicnr;
            /if;
            #debug_msg += 'Testing ' + #firstnr + ' as first number.\n';
      if(RA_fit(-firstnr = #firstnr, -series_size = #series_size));
          // the order will fit in the same 3-nr series
          #firstnr_ok = 'yes';
          #debug_msg += 'The first-found series ' + string(#firstnr)->substring(1,3)
            + ' has enough free space.\n';
      else; // use the next series, if it is not custom  
          #firstnr = string(#firstnr) -> substring(1,3);
          loop(1000);
              #firstnr = integer(#firstnr) + 1;
              // is it custom?
              inline(-search,$dbconfig, -table=$table_prefix + 'varatutsarjat',
                  -op = 'eq',
                  'numerosarja' = #firstnr
                  );
                    if(found_count); //damn, this one is taken
                    else;
                      #firstnr_ok = 'yes';
                      #debug_msg += 'A new non-custom series ' 
                        + #firstnr + ' will be used.\n';
                      loop_abort;
                    /if;
              /inline;
          /loop;
          if(#firstnr_ok);
            #firstnr = #firstnr * 100000000; // add 8 zeros
          else;
            #error_msg += 'No free series was found!\n';
            return(#error_msg);
          /if;
      /if;
    /if; // end if custom series 

  // update order in db

      local('ytunnus' = MO_getNamebyID( -table='yritykset',
                                        -idvalue=#company_id,
                                        -namefield='ytunnus'));

      inline(-update,$dbconfig, -table=$table_prefix + 'tilaukset',
          -keyfield      = 'id',
          -keyvalue      = #order_nr,
          'debug'        = #debug_msg + 'sarja=' + #series_size + ', hinta=' + RA_getPrice(#series_size),
          'virhe'        = #error_msg,
          'ekanumero'    = #firstnr,
          'vikanumero'   = #firstnr + #series_size - 1,
          'varattusarja' = #usecustom,
          'ytunnus'      = #ytunnus,
          'yhteensa'     = RA_getPrice(#series_size),
          'yksikkohinta' = RA_getPrice(#series_size,-unitprice='yes')
          );
      /inline;

  return(array('firstnr' = #firstnr,'debug'='<b>Debug messages:</b>\n' + #debug_msg));

/define_tag;

define_tag('orderSubSearch', -namespace='RA',
                        -required='inlinestuff',-type='array',
                        -encodenone);
    local('error_msg'   = string);
    local('firstfreenr' = integer);

    inline(-search,$dbconfig, -table=$table_prefix + 'tilaukset',
      #inlinestuff,
      -maxRecords    = '1',
      -sortfield     = 'ekanumero',
      -sortorder     = 'descending'
    );
        MO_errorhandler(-level='normal');

      if(shown_count == 1);
        #firstfreenr = integer(field('ekanumero')) 
                     + integer(field('sarjan_koko'));
      else;
      // could be their first order with a reserved series
      //  #error_msg += 'Search for first free nr did not find a number?<br>';
      //  return(#error_msg);
      /if;
    /inline; 
    return(#firstfreenr);
/define_tag;

define_tag('fit', -namespace='RA',
                  -required='firstnr',-type='integer',
                  -required='series_size',-type='integer',
                  -encodenone);
    // returns true or false
    // example 
    // if( RA_fit(-firstnr = #firstnr, -series_size = #series_size) );

    return(string(#firstnr) -> substring(1,3) 
       == string(#firstnr + #series_size - 1) -> substring(1,3));
/define_tag;

// define_tag('filewriteAsync', -namespace='RA',
//                               -required='order_nr',-type='integer',
//                               -required='firstnr',-type='integer',
//                               -required='series_size',-type='integer');
//   local('error_msg' = string);
// 
// // Template For Copying Vars To Async Thread 
// // from: http://www.lassotech.com/async 
// // stuff on your page not async here... 
// // -------------- BEGIN OF ASYNC CODE -------------- 
// { 
// // -------------- BEGIN OF ASYNC CODE -------------- 
//     // -------------------- 
//     // process async! 
//     // -------------------- 
//     // 
//     // NOTE: Code below this point is processed in its own thread! 
//     // 
//     lasso_ExecutionTimeLimit(60 * 60); // in seconds, set this to something reasonable 
//     thread_SetPriority(Thread_GetCurrentID, Thread_Priority_Low); // set priority 
// 
//     // recreate vars - this makes all the vars on the page available to the async thread 
//     iterate( params, local('i')); 
//             local(#i->first) = #i->second; 
//     /iterate; 
// 
//    inline(-nothing,-username='myBlogUser',-password='aBu0etY!');
//       local('testfile' = file('/files/excel/rkn_tilausnro_test.xls', File_OpenWrite));
//       #testFile -> setEncoding('ISO-8859-1');
//       #testFile -> write('testing');
//       #testFile -> close;
// //      local('myFile' = file('/files/excel/rkn_tilausnro_' + #order_nr + '.xls', File_OpenWrite));
// //     loop(-from = #firstnr, -to = (#firstnr + #series_size - 1));
// ////      loop_count > (#firstnr + 1) ? loop_abort;    // test setting
// //        #myFile -> setEncoding('ISO-8859-1');
// //        #myFile -> write(SC_bankReference(loop_count) + '\r');
// //             If( file_currentError != error_NoError );
// //                #error_msg += 'file error - ' + file_currentError;
// //             /If;
// //      /loop;
// //      #myFile -> close;
//    /inline; 
// 
// 
// // -------------- END OF ASYNC CODE -------------- 
// }-> asasync(-params = array( 'order_nr' = #order_nr, 'firstnr' = #firstnr, 'series_size' = #series_size)); 
// // -------------- END OF ASYNC CODE -------------- 
// 
// 
// //   if(#error_msg);
// //     return(#error_msg);
// //   /if;
// /define_tag;

  define_tag('composeList',
    -namespace='RA',
    -required='order_nr',-type='integer',
    -required='firstnr',-type='integer',
    -required='series_size',-type='integer',
    -required='path',-type='string',
    -encodeNone);

    if(#series_size > 2000); // no excel file
      return(null);
    /if;
          local('out' = string);
  //     local('out' = 'Order was ' + #order_nr 
  //         + ', firstnr was ' + #firstnr 
  //         + ', series was ' + #series_size); 
      inline(-nothing,-username='myBlogUser',-password='aBu0etY!');
         local('myFile' = file(#path, File_OpenWrite));
         #myFile -> setEncoding('ISO-8859-1');
            loop(-from = #firstnr, -to = (#firstnr + #series_size - 1));
      //      loop_count > (#firstnr + 1) ? loop_abort;    // test setting
              #myFile -> write(SC_bankReference(loop_count) + '\r');
                   If( file_currentError != error_NoError );
                      return('File error - ' + file_currentError);
                   /If;
                   If( error_msg != 'No error');
                      return('Error - ' + error_msg);
                   /If;
            /loop;
         #myFile -> close;
      /inline;

    return(#out);
  /define_tag;

  define_tag('mailList',
    -namespace='RA',
    -required='order_nr',-type='integer',
    -required='series_size',-type='integer',
    -required='path',-type='string',
    -required='orderinfo',-type='string',
    -encodeNone);
          local('out' = string);

  // find the email address
    local('body' = string);

    local('to' = MO_getNameByID(-table='yritykset',-idvalue=$userid_ses,-namefield='email'));
    local('site') = $opts -> find('sitename');
    #site -> uppercase;

    local('page') = 'http://rahtikirjanumerot.logexpo.fi/order-form.html';

    local('body') = str('You have ordered ');
    #body += #series_size + ' ';
    #body += str('way bill numbers ') + str(' on the page ');
    #body += '\n\n' + #page;
    if(#series_size <= 2000);
      #body += '\n\nOhessa lista numeroista .xls-tiedostona.';
      #body += '\n\nVoit avata tiedoston Excelissä tai muussa taulukkolaskentaohjelmassa.';
      #body += '\n\nSen voi avata myös missä tahansa tekstieditorissa (ohjelmasta riippuen voi olla tarpeen vaihtaa tiedostopääte esimerkiksi muotoon .txt).';
    /if; 
    #body += '\n\nTilauksen perustiedot:\n\n';
    #body += #orderinfo;

    local('attach' = pair);
    if(#series_size <= 2000);
      #attach = pair(-attachments=#path);
    /if;

        email_send(
          -host=$smtp_server,
          -from=$opts -> find('webmaster_email'),
          -to=#to,
          -subject= #site +' - '+ str('Order') +' '+ str('Nr.') +' '+ #order_nr,
          -body=#body,
          #attach
        );

    return(#out);
  /define_tag;

// CUSTOM FORM PROCESSOR TAG 

Define_Tag('MO_customforms',-Required='myp');
    // custom form contents
	/* FIELD LIST (in table "forms")
	v1... v26 VARCHAR(255) NOT NULL
	t1...t4 TEXT NOT NULL
	d1...d2 DATE NOT NULL
	*/
    local('elements') = array;
    if(#myp == 'lahjoitajasenyys');
      local('elements') = array(
	Array('','','heading','','','','','','Lahjan saaja','',''),
	    Array('v1','Nimi','text','req','','','','','','',''),
	    Array('v2','Katuosoite','text','req','','','','','','',''),
	    Array('v3','Postinumero ja <br>-toimipaikka','text','req','','','','','','',''),
	    Array('v4','Puhelinnumero','text','req','','','','','','',''),
	    Array('v5','Email','text','req,email','','','','','','',''),
	Array('','','heading','','','','','','Lahjan antaja','',''),
	    Array('v6','Nimi','text','req','','','','','','',''),
	    Array('v7','Katuosoite','text','req','','','','','','',''),
	    Array('v8','Postinumero ja <br>-toimipaikka','text','req','','','','','','',''),
	    Array('v9','Puhelinnumero','text','req','','','','','','',''),
	    Array('v10','Email','text','req,email','','','','','','',''),
	Array('t1','Muuta','textarea','','','','','','','',''),
    );
    else(#myp == 'liity');
      local('elements') = array(
	    Array('v1','Nimi','text','req','','','','','','',''),
	    Array('v2','Katuosoite','text','req','','','','','','',''),
	    Array('v3','Postinumero ja <br>-toimipaikka','text','req','','','','','','',''),
	    Array('v4','Puhelinnumero','text','req','','','','','','',''),
	    Array('v5','Email','text','req,email','','','','','','',''),
	Array('t1','Muuta','textarea','','','','','','','',''),
    );
    /if;
    return(#elements);
/Define_Tag;

define_tag('getPrice',-nameSpace='RA',
      -required='amount',-type='integer',
      -optional='unitprice',
      -optional='use_comma'
      );
    /* RA_getPrice. get price for waybill nr order; pricelist in siteconfig 
      if #unitprice == 'yes', unitprice will be returned;/if; */ 
    local('out' = decimal);
    local('i' = pair);
    iterate($prices,#i);
      if(#amount <= #i -> first);
        #out = #i -> second;
        loop_abort;
      /if;
    /iterate;
    if(! local_defined('unitprice'));
      #out *= #amount;
    /if;
    #out = decimal(#out);
    if(local_defined('use_comma'));
      #out -> setFormat(-decimalChar=',',-precision=2);
    else;
      #out -> setFormat(-decimalChar='.',-precision=2);
    /if;
    // don't use comma here, otherwise db entry to decimal field 
    return(#out);
/define_tag;
?>

