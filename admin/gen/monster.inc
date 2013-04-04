<div class="center">
<?LassoScript //>
loop(1); // hack for imitating abort on L9


// LOCAL CHANGES
// upload: for Sokkeli, added  var
//   upload_usergroupid
//   upload_projectid
// END LOCAL
/*
update 2012-04-08 set emptyform=yes before empty form is displayed in formview
update 2011-09-21 vars rectype_en, rectype_en_part etc. deprecated; use rectype and rectype_pl (for plural) instead
featur 2011-09-20 new ctag MO_update for quick updates
update 2010-12-09 searchform break behavior changed. Now linebreak must be 
   explicitely set (in searchform slot 10). Default is no linebreak
update 2010-12-09 ctag stl deprecated, all translation now through str
update 2010-12-03 var formstuff deleted - was not in use 
update 2010-12-03 date parameters in searchfields and editfields  moved from 
  slot 8 to slot 4; slot 8 is now free for other use; deprecated code handled in MO_inputVars
update 2010-12-03 personid validation integrated in the monster system
update 2010-11-24 added companyid validation
update 2010-04-30
plenty of code  tidied up: monster (this file), and editform
addcode option removed

update 2009-12-23
bugfix: basicoptions->'time_showformat' was not respected
feature: added field "customdisplay" to sitepages table
  and associated functions in MO_adminnavig function, and stub file
feature: on file upload form, added js to automatically copy the selected 
  filename from the fileupload object to the filename field

update 2009-08-20
muutettu nosave muotoon  replacecode (no input tag)
*/

/*  SHORT HELP 

### $searchFields and $editFields ###
   (they are on the stub pages)

1=fieldnames, 2=legend, 3=input type (creationdate,moddate,hidden,noentry,text,password,textarea,textarea_fckeditor,select,selectmultiple,radio,checkbox,checkmultiple,replacecode,heading,heading2)
** "creationdate", "moddate": set #4 to "date" or "datetime"
** on search form: a "hidden" field can be searched with get action 
** - in that case add to the link "search=get"
** on edit form, the default value for a field can be entered in slot 12
** this works for "hidden" fields too
1=fieldname: on searchform, a proprietary way to send operator values 
  for example from a select list: set 1 to "--op"
4=validations (req, unique, email, alphanum, int, float, password, allowhtml, companyid, personid, date, datetime, time); separated by ; 
   (note: date & time fields will be automatically validated, when #8 is set to date or time)
   (note: password validation is good for username too)
5=size1 (=text:size, textarea:cols; if empty, use default)
        (for select/radio: list of values, separated by ;        (for checkbox, db value for checked checkbox)
6=size2 (textarea:rows; if empty, use default) 
        (for select/radio: list of visible texts, sep. by ;
7=help text
8= free slot (used to be date/datetime etc.) 
9 = replace field presentation on editform or searchform with this data
   (remember to set #3 to replacecode)
   available variables are  $fvalues->find(<fieldname>), 
                            $inputvars->find(<fieldname>)
10 (on searchform) =linebreak (break)
10 (on editform) = allowed value: saveIfNotEmpty (self-explanatory?)
11 (on searchform) = search operator (eq, neq, lt, ft, etc.)
11 (on editform) = encryption (md5,blowfish)
12 (on editform) = default value (used only on empty form)

### custom forms ###

1. trigger the form by checking the checkbox  "form" for a page
2. add form-element definitions to the ctag MO_customforms in inc/liblocal.inc
   - form is detected in /cont/cont.inc
   - form contents ($editfields) are generated in /custom/form/form.inc
3. modify response texts: MO_formresponsetext in inc/liblocal.inc
4. modify custom/preform/mypage.inc
   - main content is drawn here
   - postprocess (say thanks, send emails) 

### displaying images on a page ###

In admin stub file, enable fileupload. Also enable customPostform, and create a postform file, for example custom/postform/events-form.inc. Its purpose is to display the relevant pics on an admin page. See example in hyotykasvit/admin/events-form.html. 
The table name (via $mytable) and id will be written automatically to the table uploads. 
To display the files, use MO_collectUploadData, which returns an array of two arrays. Use both $myfiles->get(1), for "shown" files, and $myfiles->get(2), for "linked" files. 
*/

var('action' = '');
    If( ! Variable_Defined('emptyForm') );
	var('emptyForm' = Action_Param('emptyForm')); // see stub file
    /If;
    If( ! Variable_Defined('formTargetpage') ); // see stub file
      var('formTargetpage' = $myfilename);
    /If;
    If( ! Variable_Defined('fileUpload') ); // see stub file
      var('fileUpload' = string);    
    /If;
    if( ! variable_defined('customform') ); // see stub file
      var('customform' = 'no'); 
    /if;
    if( ! variable_defined('searchVars_ses') );
      var('searchVars_ses' = array); 
    /if;
    if( ! variable_defined('searchstuff_ses') );
      var('searchstuff_ses' = array); 
    /if;
    if( ! variable_defined('silentAdd') );
      var('silentAdd' = string); 
    /if;
    // handle deprecated vars $rectype_en, $rectype_en_part
    if( ! variable_defined('rectype') );
      var('rectype' = $rectype_en); 
    /if;
    if( ! variable_defined('rectype_pl') );
      var('rectype_pl' = $rectype_en_part); 
    /if;
    
var('ref'          = action_param('ref'));
// naviglinks, searchform, editform, uploadform
// empty value for get search
var('add_confirmed'    = action_param('add_confirmed'));
var('update_confirmed' = action_param('update_confirmed'));
var('confirmation_sent'= action_param('confirmation_sent'));
var('keyvalue'         = action_param('keyvalue'));
var('skip'             = integer(action_param('skip')));
var('searchstuff_temp' = array); // will be built on searchstuff_ses
var('linkstuff'        = string);
var('searchToggle'     = string);
var('dbChange'         = integer);
var('dbChange_error'   = string);
var('reqFieldsFound'   = string);
var('fieldCounter'     = integer); 
var('fvalues'          = array); 

/* 
fieldCounter is used to label searchform fields
because sometimes there are 2 searchfields on the searchform  
both targeting the same db field; thus dbfieldname is not adequate
as a label. Also, searchform output must never use formparam names that could be identical to the editform formparam names
*/

// define action
If( Action_Param('add') );
  $action = 'add';
  $dbChange = 1;
else( Action_Param('update') );
  $action = 'update';
  $dbChange = 1;
else( Action_Param('delete') );
  $action = 'delete';
  $dbChange = 1;
else( Action_Param('delet2') );
  $action = 'delet2';
  $dbChange = 1;
else( Action_Param('upload') );
  $action = 'upload';
  $dbChange = 1;
/If;


// file upload vars
    var('upl_name' = Action_Param('upl_name')); //for uploadfile delete
    var('uploadErr' = integer);

    If( $fileupload == 'yes');
	var('upload_filename' 	= Action_Param('upload_filename'));
	var('upload_description'= Action_Param('upload_description'));
	var('upload_catid' 	= Action_Param('upload_catid'));
	var('upload_posttable' 	= Action_Param('upload_posttable'));
	var('upload_postid' 	= Action_Param('upload_postid'));
          var('i') = array;
          iterate($upload_imagesizes, $i); // for var def, see siteconfig.inc
              var('upload_' + ($i -> get(1))) = action_param('upload_' + ($i -> get(1)));
          /iterate;
	var('upload_rotateangle'= Action_Param('upload_rotateangle'));
	// next one for file remove (in LassoBlogger, page upl-list.lasso)
        var('namebody' = string);
	var('suffix'   = string);

	// check upload_path 
	// (in LB, defined in Options, in Monster CMS, in /gen/siteconfig.inc)
	If( ! $upload_path );
	    $speakBubble = 'To facilitate file uploads, please set the option <code>fileupload_path</code> on admin page Options > Uploads';
	Else( ! $upload_path -> EndsWith('/') );
	    $upload_path += '/';
	/If;
	If( ! $upload_path -> BeginsWith('/') );
	    $speakBubble = 'The var <code>upload_path</code> should begin with a slash! (This message is from  monster.inc). Please correct';
	/If;
	If( $speakBubble);
            MO_speakBubble($speakBubble);
	/If;

    /If; // end file upload vars

// compose searchstuff_ses and linkstuff

if($ref == 'searchform' || $ref == string ); // could be get action
  // reset and rebuild ses vars
  // if there is no session, it's ok? 
  $searchVars_ses = array; 
  $searchstuff_ses = array; 
  Iterate( $searchFields,var('this'));
      $fieldCounter += 1;
      // capture deprecated code, check formatting
        if( $this -> get(3) == 'nosave' || $this -> get(3) == 'override');
            fail(-1,('Field handlers "nosave" and "override" have been '
            + 'deprecated. Please correct your searchFields variable.'
            + 'Use "replacecode" (where you can use slot 9 for field '
            + 'presentation, if necessary), or "noentry" (if you want to '
            + 'save but not let the user modify the value).'));
        /if;
        if( $this -> size < 10 );
            fail(-1,('At least one line in searchfields array is too '
            + 'short. The line (array) must have at least 10 slots. The first '
            + 'offending line was ' + loop_count + '.'));
        /if;
      // end checks

      // helper vars
                  var('searchOp' = String);
                  If( $this -> Size > 10);
                      $searchOp = $this -> get(11); 
                  /If;
                  var('fname'  = $this -> get(1));  
                  var('fvalue' = action_param($fname));  
                  var('fvals'  = $this -> get(4) -> split(','));
      $searchVars_ses -> insert($fname = $fvalue); 

      if($fvalue);  // if searchfield not empty
          $searchToggle = 'doSearch'; // jos ei yhtään hakutermiä, 
                                      // ja riippuen searchbehavior-
                                      // muuttujasta, hakua ei ehkä
                                      // tehdä
          $linkstuff += ($fname + '=' + encode_url($fvalue) +'&');

          If( $searchOp );
            $searchstuff_ses -> insert('-Op' = $searchOp);
          /If;

          If( ($fvals -> find('date')) != array || ($fvals -> find('timestamp') != array) );

          // case( date & timestamp (pvm-kentät))
              $searchstuff_ses -> insert('we are' = ($fvals->find('timestamp') == array));
              If( (date($fvalue, -Format=($basicOptions -> find('date_showformat'))) -> type) == date);		// if wellformed
                  $searchstuff_ses -> insert($fname = 
                    Date_Format(Date($fvalue,
                      -Format=($basicOptions -> Find('date_showformat' ))),
                      -Format=($basicOptions -> find('date_dbformat')))
                  );
              /If;
          else( $fname == '--op' ); // proprietary way to pass operator vals
            // deprecated, use slot 11 for operator
            $searchstuff_ses -> insert('-Op' = $fvalue);
          else( ($this -> get(3) == 'checkbox') && ($this -> get(5) != array) ); 
            //default val
              $searchstuff_ses   -> Insert($fname = $this -> get(5));
          Else; // other cases than date
              $searchstuff_ses   -> Insert($fname = $fvalue);
          /If;
      /If; // if searchfield not empty
  /Iterate;
/if; // end if ref
  // add -MaxRecords to searchstuff_ses
    If(   $viewtype == 'listview');
	$searchstuff_ses -> Insert(-MaxRecords = $maxrecords_listview); 
    Else( $viewtype == 'formview');
	$searchstuff_ses -> Insert(-MaxRecords = $maxrecords_formview); 
    /If;
    // nämä pitää restauroida uudestaan -updaten jälkeen

  // add default search params to searchstuff_ses
    If(   $defaultSearch != array);
	Iterate( $defaultSearch, var('i') );
          $searchstuff_ses -> Insert( $i->get(1) = $i->get(2) ); 
	/Iterate;
    /If;

// make vars
var('inputVars' = MO_inputVars($editfields));


//  *** validaatio - validation ***

  // required fields
  // unique fields
  // date fields
  // time fields
  // email validation
  // alphanumeric, integer and float validation
  // password validation
  // allowhtml validation
  // custom  validation

  If( $action == 'add' || $action == 'update');
        // monster validations
        // dependencies - $basicOptions, $dbConfig, $mytable
        // for unique-validation - $keyvalue
        $speakBubble = MO_val( -editfields=$editFields,
                                -inputvars=$inputVars,
                                -action=$action);
  /If;

  // Custom validation

  If( $customValidations == 'yes');
    // custom val must return $speakBubble, if val fails
    Include('custom/validation/' + $myfilebody + '.inc');
  /If;

  // last thing -  the speakBubble handler

  If( $speakBubble );
    MO_speakBubble($speakBubble,-style='color:#c00;font-size:120%;');
    var('dbChange_error' = 'validation failed');
  /If; 

// end validation


// transform vars where needed, and feed into inlinestuff

var('inlinestuff' = array);

If( ($ref == 'editform') && ! $dbChange_error ); // if no errors in validation

Iterate( $editFields,var('this'));

    // helper vars
    var('fname' 	= $this -> get(1));
    var('ftype' 	= $this -> get(3));
    var('fvals' 	= $this -> get(4) -> split(','));
    var('dbinput'       = string);
    var('saveIfNotEmpty'= string);
      if($this -> size > 9);
        $saveIfNotEmpty = $this -> get(10);
      /if;

    // in this iterate, don't touch any var($fname)
    // ...except for dates, because in case of update error, date 
    // has to be in dbformat to be displayed properly...

  If( $fvals -> find('date') != array ); // case date 

    If( $inputVars->find($fname) != array
     && $inputVars->find($fname) != '0000-00-00'); // if not empty
        // reformat for db
          $dbinput = 
            Date_Format(Date( $inputVars->find($fname),
              -Format=($basicOptions -> Find('date_showformat'))), 
              -Format=($basicOptions -> Find('date_dbformat'  )));
        // replace the existing value in map 
          $inputVars->insert($fname = $dbinput);
    /If; 

  else( $fvals -> find('time') != array ); // case time 

    If( $inputVars -> find($fname)); // if not empty
          $dbinput = 
	    Date_Format(Date( $inputVars->find($fname ),
	    -Format=($basicOptions -> (Find('time_showformat')))), 
	    -Format=($basicOptions -> (Find('time_dbformat'  ))));
    /If;

  else( $fvals -> find('datetime') != array ); // case datetime

    If( $inputVars->find($fname) != ''
     && $inputVars->find($fname) != '0000-00-00 00:00:00'); // if not empty
        // reformat for db
          $dbinput = 
            Date_Format(Date( $inputVars->find($fname),
              -Format=($basicOptions -> Find('datetime_showformat'))), 
              -Format=($basicOptions -> Find('datetime_dbformat'  )));
        // replace the existing value in map 
          $inputVars->insert($fname = $dbinput);
    /If; 

  else( $fvals -> find('timestamp') != array ); // case timestamp 

    If( $inputVars -> find($fname) != array ); // if not empty
        $dbinput = 
	    Date_Format(Date( $inputVars->find($fname),
	    -Format=($basicOptions -> (Find('time_showformat')))), 
	    -Format='%Y%m%d%H%M%S');
    /If; 
  else;
    $dbinput = $inputVars->find($fname);
  /If;
        // floating point - commas to dots
	if( $fvals -> find('float') != array );
          $dbinput = $inputVars -> find($fname);
          $dbinput -> replace($basicOptions->find('floatseparator_show'),'.');
        /if;

	if( $dbinput );
	    $dbinput -> Trim;
	/if;

	// check for design error

	If( $fname -> (BeginsWith('haku')));
          $speakBubble = 'Kentän nimi ei saa alkaa \"haku\"! Korjaa tietokanta ' +
          'ja editFields-muuttuja!';
          MO_speakBubble($speakBubble);
          Abort;
	/If;

    // record field value to inlinestuff?

    If( $ftype == 'creationDate' );
        if($action == 'add'); 
          If( $fvals -> find('date') != array );
            $inlinestuff -> (Insert($fname 
            = Date_Format(Date,
              -Format=($basicOptions -> Find('date_dbformat')))));
          Else( $fvals -> find('datetime') != array );
              $inlinestuff -> Insert($fname 
                = Date_Format(date,-Format='%Q %T'));
          /If;
        /if;
    Else( $ftype == 'modDate' ); 
        If( $fvals -> find('date') != array);
          $inlinestuff -> Insert($fname 
          = Date_Format(date,-Format=($basicOptions -> Find('date_dbformat'))));
        Else( $fvals -> find('datetime') != array);
            $inlinestuff -> Insert($fname 
              = (Date_Format((Date),-Format='%Q %T')));
        /If;
    Else( $ftype == 'replacecode' );
        // save only if there is input
        if( $inputVars->find($fname) != string );
            $inlinestuff -> insert($fname = $dbinput);
        /if;
    else( $ftype == 'hidden' && $this -> size > 11); // 
        If( $this -> get(12));
              $inlinestuff -> insert( $fname = $this->get(12) );
        else; // default case
              $inlinestuff -> insert( $fname = $dbinput );
        /if;
    Else( $ftype == 'heading'  ||
	  $ftype == 'heading2'  ||
          ($saveIfNotEmpty == 'saveIfNotEmpty'  && ! $dbinput ));
		// don't save
    Else;	// save
	    If( $this -> size > 10); // if encrypt (#11) is defined
		If( $this -> get(11) == 'md5');
		    $inlinestuff -> Insert($fname = 
		      encrypt_MD5($dbinput));
		Else( $this -> get(11) == 'blowfish');
		    $inlinestuff -> Insert($fname = 
		      Encrypt_Blowfish(($dbinput),-Seed=$seed));
		Else;
		    $inlinestuff -> (Insert($fname = $dbinput));
		/If;
	    Else;	// encrypt not defined
		    $inlinestuff -> Insert($fname = $dbinput);
	    /If;
    /If;
	
/Iterate;

/If; // (end if dbChange_error)


// create readable version of il-stuff

    var('inlinestuff_readable' = MO_readable($inlinestuff));

// copy searchvars_ses to another var for temporary processing
    $searchstuff_temp = $searchstuff_ses;


         /* if dbChange=1  - processing begins */

If( $dbChange ==1); 

// handle confirmations

if(variable_defined('confirm_add'));   // confirmation is in use  
   if($confirm_add == 'yes');
      if($confirmation_sent == 'yes');  // user has pressed a button          
        if($add_confirmed == 'ok');     // button was OK          
        else;                           // any other value
            $action = string;
        /if; 
      else;
        include('custom/addconfirmation/' + $myfilebody + '.inc');
      /if;
  /if;
/if;

if(variable_defined('confirm_update'));
  if($confirm_update == 'yes');
      if($confirmation_sent == 'yes');
          if($update_confirmed == 'ok');
          else; // any other value
            $action = string;
          /if;
      else;
        include('custom/updateconfirmation/' + $myfilebody + '.inc');
      /if;
  /if;
/if;

// 'test - inlinestuff = ' + $inlinestuff_readable + '<br>';
// Abort;

// 'TEST - keyvalue = ' + $keyvalue + '<br>';
If( $action=='update' && ! $dbChange_error );  // if action

inline( $dbConfig, -table=$table_prefix + $myTable,
  -keyfield='id',
  -keyvalue=$keyvalue,
  $inlinestuff,
  -update);


if( error_msg != Error_NoError);
	$dbChange_error = error_msg;
        MO_speakBubble(str('The update FAILED. ') + str('More info: '));
    MO_errorhandler(-level='debug');
Else;
    MO_speakBubble(str('The update was successful.'));
/If;

If( $updatebehavior == 1);
    // behavior #1 - search kept, updated rec shown only if search allows this
    // usually listview

    // keyvalue removed from searchstuff_temp
//    $searchstuff_temp -> 
//      (remove( $searchstuff_temp -> findPosition('-KeyValue') -> first));
    
Else( $updatebehavior == 2);
    // behavior #2 - search bypassed, only updated rec shown
    // usually formview

// 	// searchdata emptied
//         $skip = string;
// 	$searchstuff_temp = Array;
// 	$linkstuff = Array;
// 	// maxrecs-arvojen restaurointi
// 	If(   $viewtype == 'listview');
//           $searchstuff_temp -> Insert('-MaxRecords' = $maxrecords_listview); 
//         Else( $viewtype == 'formview');
//           $searchstuff_temp -> Insert('-KeyValue' = $keyvalue); 
//           $searchstuff_temp -> Insert('-MaxRecords' = $maxrecords_formview); 
// 		// tämä ei toimi listview-näkymässä
// 	/If;
// 	var('keyvalue' = KeyField_Value);

    // new version 201006 - experimental for now
    // wrap existing searchstuff in 'and'
    $searchstuff_temp -> insert(-opbegin  = 'and',1);
    $searchstuff_temp -> insert(-opend    = 'and');
    // wrap that in 'or'
    $searchstuff_temp -> insert(-opbegin  = 'or',1);
    $searchstuff_temp -> insert(-opend    = 'or');
    // add keyvalue wrapped in 'or' clause
    $searchstuff_temp -> insert(-opbegin  = 'or');
    $searchstuff_temp -> insert(-keyvalue = keyfield_value);
    $searchstuff_temp -> insert(-opend    = 'or');
    $skip = 0;
/If;

/Inline;


Else( $action == 'delete');
  '<div class="center" style="font-size:1.2em;padding:10px;"><b>';
    If( $lang == 'fi');
      'Haluatko varmasti poistaa?';
    Else;
      'Do you really want to delete?';
    /If;
  '</b></div>\n';

'<form class="center" action="' + $myfilename + '" method="POST">\n';

    // uploadfile delete
    If( Variable_Defined('upl_name') );
        '<input type="hidden" name="upl_name" value="'+$upl_name+'">\n';
    /If;
?>
<input type="hidden" name="keyvalue" value="[$keyvalue]">
<input type="hidden" name="skip" value="[$skip]">
<INPUT TYPE="Submit" NAME="delet2" VALUE="Ok">
<INPUT TYPE="Submit" NAME="-nothing" VALUE="[str('Cancel')]">
</FORM>

<?LassoScript //>
$dbchange_error='delete dialog';


Else( $action == 'delet2'); // delet2 action

// get the path first for possible file removal
    var('movedpaths') = string;
    var('failedpaths')= string;
    var('thispath')   = string;
    var('path')       = $upload_path + 'uploads/';
      // LassoBlogger - we need to know if the file is post-related
      if( integer(MO_getNamebyID(
          -table = 'uploads', 
          -idvalue  = $keyvalue, 
          -namefield = 'upl_catid')) > 0 
      );
        $path += 'filevault/';
      else;
        $path += 'post-related/';
      /if;

    inline( $dbConfig, -table=$table_prefix + $myTable,
        -KeyField='id',
        -KeyValue=$keyvalue,
        -delete);

If( error_msg != Error_NoError );
  $dbChange_error = 'deleteError';
  $speakBubble = str('The delete operation FAILED. ') + str('More info: ');
  $speakBubble += error_code + ', ' + error_msg;
Else;
  $speakBubble = str('A record has been deleted.');
/If;
  MO_speakBubble($speakBubble);

/Inline;


// move uploaded file to trash, if any 

If( ! $dbChange_error && $upl_name );

  // set username & password
  inline( -username=$filetagsUsername,
          -password=$filetagsPassword,
	  -nothing);

    // check the path to trash folder (defined in siteconfig.inc)

    if( !($upload_trashpath -> EndsWith('/')));
	$upload_trashpath += '/';
    /if;
    if( ! File_Exists( $upload_trashpath) || ! File_IsDirectory($upload_trashpath) );
	$speakBubble += 'The trash folder cannot be found. This code is from';
	$speakBubble += ('<code>admin/cont/' + $myfilebody + '.inc</code>.');
        MO_speakBubble($speakBubble);
	Abort;
    /if;

    // ok - move the file to trash

    var('aName' = $upl_name -> split('.'));
    if($aName -> size != 2);
      $speakBubble += 'Something wrong with the filename. Suffix missing?';
        MO_speakBubble($speakBubble);
	abort;
    /if;
    var('namebody') = $aName -> get(1);
    var('suffix') = $aName -> get(2);
    var('thumbsuffix') = string($suffix);
    if($suffix == 'png'); // exception
      $thumbsuffix = 'jpg';
    /if;

    // thumbnail
    $thispath = $path + 'thumbnails/' + $namebody + '.' + $thumbsuffix;
    if(file_exists($thispath));
      file_move( $thispath,$upload_trashpath+$namebody+'.'+$thumbsuffix,-FileOverwrite);
      if( File_CurrentError == 'No Error');
          $movedpaths += ($thispath + '<br>');
      else;
          $failedpaths += $thispath + ', error= '+file_currenterror+'<br>';
      /if;
    else;
          $failedpaths += $thispath+'<br>';
    /if;

    // original file
    $thispath = $path + $upl_name;
    if(file_exists($thispath));
      file_move( $thispath,$upload_trashpath+$upl_name,-fileoverwrite);
      if( file_currentError == 'No Error');
          $movedpaths += ($thispath + '<br>');
      else;
          $failedpaths += $thispath + ', error= '+file_currenterror+'<br>';
      /if;
    else;
          $failedpaths += $thispath + '<br>';
    /if;

    // sized images
    var('i') = array;

    iterate($upload_imagesizes, $i);
      $thispath = $path + $i->get(1) + '/' + $upl_name;
      if(file_exists($thispath));
	file_move( $thispath,$upload_trashpath+$upl_name,-fileoverwrite);
	if( file_currenterror == 'No Error');
	    $movedpaths += ($thispath + '<br>');
        else;
          $failedpaths += $thispath + ', error= '+file_currenterror+'<br>';
	/if;
      else;
          $failedpaths += $thispath + '<br>';
      /if;
    /iterate;
 
    if($movedpaths);
        $speakBubble = str('Following files were moved to the trash folder:');
        $speakBubble += '<br>' + $movedpaths;
    else;
        $speakBubble = str('The file was NOT moved to the trash.');
        $speakBubble += 'Tried the following paths: ' + $failedpaths;
    /if;
    MO_speakBubble($speakBubble);
/Inline;

/if; // end file move

// end delet2 action


Else( $action == 'add' && ! $dbChange_error);   // add action 

Inline( $dbConfig, -table=$table_prefix + $myTable,
  -KeyField='id',
  $inlinestuff,
  -add);

If( error_msg != Error_NoError );
    $dbChange_error = error_msg;
    MO_errorhandler(-level='debug');
    $speakBubble += str('The add operation FAILED. ') + str('More info: ');
    $speakBubble += (error_code + ', ' + error_msg);
    MO_speakBubble($speakBubble);
//    Abort;
Else;
    if( $silentAdd != 'yes');
        $speakBubble += str('A record has been added.');
    /if;
/If;
MO_speakBubble($speakBubble);

    var('keyValue' = KeyField_Value);

    // searchdata reset
    If(   $viewtype == 'formview');
      $searchstuff_temp = $defaultSearch;
      $searchstuff_temp -> insert('-keyValue' = $keyValue);
    else ($searchstuff_temp -> size > 1 ); // not only -maxrecs
      // add the following to the existing search
      $searchstuff_temp -> Insert('-opBegin' = 'and',1); // insert first
      $searchstuff_temp -> Insert('-opBegin' = 'or',1);  // insert first
      $searchstuff_temp -> Insert('-opEnd' = 'and');     // insert last...
      $searchstuff_temp -> Insert('-opBegin' = 'and');
      $searchstuff_temp -> Insert('id' = $keyValue);
      $searchstuff_temp -> Insert('-opEnd' = 'and');
      $searchstuff_temp -> Insert('-opEnd' = 'or');
    /If;

/Inline;

Else( $action == 'upload'); // upload action
  include('inc/mon_upload.inc');
/If;   // end if action

/If;   // if dbChange=1

//  *** hakulomake / search form *** 

// test, if searchform should be shown
Var('shownfields' = array);
if($dbchange_error != 'delete dialog');
  // check searchFields
  If( $searchFields != array ); 
      Iterate($searchFields, var('this'));
          If( $this -> get(3) != 'hidden' && $this -> get(3) != 'noentry');
            $shownFields -> insert($this->get(3));
          /If;
      /Iterate;
  /If; 
/if;

If( $shownFields != array ); // there are showable searchfields, so show!


?>

<form action="[$myfilename]" method="POST">
<input type="hidden" name="ref" value="searchform">
<table id="searchformtable" cellpadding="2" cellspacing="2" style="margin:0 auto 0 auto;text-align:center;">

<tr>
<td id="searchformheader" class="[$textClass]" colspan="2" style="text-align:center;"><B>[If( $lang == 'en')]Search for [$rectype_pl][Else]Hae [$rectype_fi_part][/If]:</B></td></tr>
<tr><td class="[$textClass]">

<?LassoScript 

var('break_setting' = string); // used to pass info from prev searchfield 
				 // to the next one

    // have to check first, if there are break-settings
    var('break_exists' = string);
	Iterate( $searchFields, var('this'));
	    If( $this -> get(10) == 'break');
	    $break_exists = 'yes';
	    /If;
	/Iterate;

var('fieldCounter' = 0); 	// actionparam names will be "searchparam1" etc.

Iterate( $searchFields, var('this'));

  $fieldCounter += 1;

  If( $this -> get(3) != 'hidden'
  &&  $this -> get(3) != 'replacecode');
      if($fieldCounter == 1 );
        $break_setting = string;
        // otherwise use setting from prev rec
      /if;
      // break or nobreak?
      If( $break_setting == 'break'); 
          // open table row
          '<tr><td class="' + $textClass + '">';
      /If;

      $this -> get(2) + ' '; 	// kentän nimi

      // contents cell
      If( $break_exists == 'yes');
          '</td><td class="' + $textClass + '">';
      /If;
      $break_setting = $this -> get(10);

      // helper
      var('fname'  = $this -> get(1));	        
      var('fvalue' = string);
      if($searchvars_ses -> find($fname) -> size);
        $fvalue = $searchVars_ses -> find($fname) -> get(1) -> second;  
      /if;

  // date - muunna pvm
      If( $this -> get(4) == 'date' && $fValue); 
        If( date($fValue,-Format=($basicOptions -> Find('date_showformat'))) -> Type != date);// date not valid
          $speakBubble=str('The search date seems to be malformed - please check!');
        /If; 					// end if valid
      /If; 

    if($shownfields == array('select')); // only one shown field, and it is select
      MO_formfieldHandler(-this=$this,-fvalue=$fValue,-customcode='onchange="this.form.submit();"');
    else;
      MO_formfieldHandler(-this=$this,-fvalue=$fValue);
    /if;

    If( ($this -> get(10)) == 'break'); // break or nobreak?
      '</td></tr>';
    /If;
  /If; // end if hidden
/Iterate;

?>
</td>
<td>
<INPUT TYPE="Submit" NAME="-nothing" VALUE="[str('Search')]">
<INPUT TYPE="Hidden" NAME="-nothing" VALUE="Hae"></td>
</tr>
</table>
</FORM>

<?LassoScript //>

	// end hakulomake
/If; 	// end if shownFields



// custom searchvalidations 

    If( Variable_Defined('customSearchvalidations'));
	If( $customSearchvalidations == 'yes' );
	Include('custom/searchvalidation/' + $myfilebody + '.inc');
	/If;
    /If;

If( $searchBehavior == 2 && $searchToggle == string); // no search terms
        $speakBubble = str('Please do a search!');
/If;
// speakBubble for searchvalidations 
    If( $speakBubble);
        MO_speakBubble($speakBubble);
        Abort;
    /If;

// the main search

// Lasso 9 is not friendly with aborts? therefore have to use if statement

if(! $dbchange_error || $dbchange_error == 'validation failed'); // if dbchange_error

inline( -search, $dbConfig, -table=$table_prefix + $myTable,
    -KeyField='id',
    $searchstuff_temp,
    -skipRecords=$skip,
    $sortstuff);

// patch - this was needed for displaying id on file-upload form
If(Variable_Defined('id'));
    if( ! $id );
	$id = field('id');
    /if;
/if;

// 'test - '; $action; '<br>';
// // search_args - 
//     MO_errorhandler(-level='debug');


Var('foundCount' = found_count);
Var('errorcode'  = error_code);

if( $action == 'delet2');
Else( (($errorcode == '-1728' 	&& $dbType == 'fmp')
   || ($foundCount == 0 	&& $dbType == 'mysql')
   || $errorcode == '26279936' 	&& $dbType == 'fmp')
   && $dbChange == 1 );
   // no records found, though there really should have been...
   If(!($dbChange_error)); // if validation failed, don't tell
        MO_speakBubble(str('Error')+' - '+str('No records were found')+'!');
        MO_speakBubble($speakBubble);
    /If;
Else( (($errorcode == '-1728'   && $dbType == 'fmp')
    || ($foundcount == 0        && $dbType == 'mysql')
    || $errorcode == '26279936' && $dbType == 'fmp') );
    // no records found, but it's no big deal
	If( $emptyForm == 'yes' || $searchBehavior == '3');
	    // don't tell nothing was found
	Else;
	    If( $lang == 'fi');
		$speakBubble= MO_titleCase($rectype_fi_part) + ' ei löytynyt';
		Else;
		$speakBubble='No ' + $rectype_pl+ ' were found';
                // $speakBubble += ('test value ' + $searchstuff_ses); 
	    /If;
            MO_speakBubble($speakBubble);
	/If;
Else( error_msg != Error_NoError );
    $speakBubble=str('An error has occurred.') + ' ' + str('More info: ');
    $speakBubble += $errorcode + ', ' + error_msg;
    MO_speakBubble($speakBubble);
/If;

// custom Preform

    If( Variable_Defined('customPreform') );
	If( $customPreform == 'yes');

          // capture and process values to be displayed
          var('fv_map') = MO_fvalues(
              -editfields=$editfields,
              -dbChange_error=$dbChange_error,
              -viewtype=$viewtype);
          $fvalues              = $fv_map -> find('fvalues');
          var('fvalues_errors') = $fv_map -> find('errors');
          var('fvalues_diagn')  = $fv_map -> find('diagn');
          // display errors
          if($fvalues_errors);
            iterate($fvalues_errors,local('i'));
              $speakBubble = '<b>' + #i->first + '</b> ' + #i->second;
            /iterate;
            MO_speakBubble($speakBubble,-style='color:#c00;font-size:1.4em;');
          /if;

          Include( 'custom/preform/' + $myfilebody + '.inc');
	/If;
    /If;


// naviglinks

// don't show, if action=add)

If( ! Variable_Defined('showNaviglinks') );
    var('showNaviglinks' = '');
    If(  $action == 'add'
      || found_count < 1
      || $emptyForm == 'yes'
      || $searchBehavior == 3);
          $showNaviglinks = 'no';
    /If;
/If;

If( $showNaviglinks != 'no');
        MO_naviglinks(
            -frommetoroot=$fromMetoRoot,
            -linkstuff=$linkstuff,
            -textclass=$textclass,
            -rectype=$rectype,
            -rectype_pl=$rectype_pl);
/If;
    '<div style="height:10px;"></div>\r';

// end naviglinks

//    ***  editointilomake  -  edit form *** 

'<TABLE class="editformtable" CELLSPACING="0" CELLPADDING="0" width="100%">\n';
if( $viewtype == 'listview'  			// case listview
    && !($action == 'delet2' && found_count == 0) 
    && $customform != 'yes'
    );
  // for listview, show empty form above the results
        if($buttonlist->find('add') == 'yes' );
          MO_listview_emptyform(-editfields=$editfields);
        /if;
            if(found_count);
              MO_listview_fieldlabels($editfields);
              MO_listview_helptexts($editfields);
            /if;
/if;   // end case listview

  var('formShown') = string;
  Records; 
          // capture and process values to be displayed
          var('fv_map') = MO_fvalues(
              -editfields=$editfields,
              -dbChange_error=$dbChange_error,
              -viewtype=$viewtype);
          $fvalues              = $fv_map -> find('fvalues');
          var('fvalues_errors') = $fv_map -> find('errors');
          var('fvalues_diagn') = $fv_map -> find('diagn');
          // display errors
          if($fvalues_errors);
            iterate($fvalues_errors,local('i'));
              $speakBubble = '<b>' + #i->first + '</b> ' + #i->second;
            /iterate;
            MO_speakBubble($speakBubble,-style='color:#c00;font-size:1.4em;');
          /if;
    if($emptyform != 'yes');
      If( variable_defined('customform') &&  var('customform') == 'yes');
        Include('custom/form/' + $myfilebody + '.inc');
      Else;
        Include( 'inc/editform.inc');
      /If;
      $formShown = 'yes';
    /if;
  /Records;

if( $viewtype == 'formview' && ! $formShown && $buttonlist->Find('add') == 'yes' );
    $emptyform = 'yes'; // a way to inform form elements about empty form
    include('inc/editform.inc');
/if;

/inline;
// don't close the table yet...

// file upload

// File upload form

    If( $fileUpload == 'yes' && $emptyform != 'yes');
    '<tr>';
	// compose texts
        var('txt')  = str('File upload');
        var('txt2') = str('Start upload');

?>


<form name="fileUpload" action="[$formTargetpage]" method="POST" enctype="multipart/form-data">
    <input type="hidden" name="ref" value="uploadform">
    <input type="hidden" name="skip" value="[$skip]">
    <th align="right" valign="top" style="font-size:1.2em;padding:20px 10px 0 0;border-top:1px solid #999;">[$txt]</th>
    <td STYLE="border-top:1px solid #999; padding-top:20px;" colspan="10">
    <table class="fileUpload">
	<tr><td>[str('The file to be uploaded')]*
        <br>
        <a href="info.lasso"
        onclick="return popitup('info.lasso#upload')">
        <img src="[$myimages]/info.jpg" title="[str('Info')]" alt="info"> 
        </a> 
        </td>
	<td colspan="2"><input type="file" name="upload_selector" value="" onChange="this.form.upload_filename.value=this.form.upload_selector.value;">
        </td>
	</tr>

	<tr>
	<td>[str('Save as...')]*
        <br>
        <a href="info.lasso"
        onclick="return popitup('info.lasso#uploadName')">
        <img src="[$myimages]/info.jpg" title="[str('Info')]" alt="info"> 
        </a> 
        </td>
	<td><input type="text" name="upload_filename" 
        value="[$upload_filename]" style="width:140px;">
	<div class="helptext">Ei skandeja eikä välilyöntejä. Muista myös tiedostopääte (esim. .pdf)</div>
        </td>
	</tr>
	<tr>
	<td>[str('File description')]</td>
	<td><input type="text" name="upload_description" 
        value="[$upload_description]" style="width:140px;">
	<div class="helptext">Julkinen tieto, näytetään tooltip-tekstinä</div>
        </td>
	</tr>
<?LassoScript
    If( $mytable == 'uploads');
    '<tr><td>'; str('Category'); '</td>';
    '<td colspan="2">';
	Inline(-FindAll,$dbConfig,
	    -table=$table_prefix+'uploadcats',
	    -KeyField='id',
	    -SortField='upc_name');
	    '<select name="upload_catid" size="1">\r';
		Records;
		    '<option value="'; Field('id'); '">'; Field('upc_name');
		    '</option>\r';
		/Records;
	    '</select>';
	/Inline;
    '</td>';
    '</tr>';
    else;
	// post-related file
	'<input type="hidden" name="upload_posttable" value="'; $myTable '">';
	'<input type="hidden" name="upload_postid" value="'; $fvalues->find('id'); '">';
    /If; // end if myfilename!=man-postedit.lasso
?>
	<tr>
	<td style="padding:4px 0 4px 0;">
        <a id="showRotateParams" href="[$myfilename]">[str('Rotate image')]</a></td>
	<td colspan="2" id="rotateParams" class="display_none" style="padding:4px;line-height:1.7;">
	<input type="radio" name="upload_rotateangle" value="0"> 0&deg;&nbsp;&nbsp;<br>
	<input type="radio" name="upload_rotateangle" value="270"> +90&deg;&nbsp;&nbsp;<br>
	<input type="radio" name="upload_rotateangle" value="90"> &ndash;90&deg;&nbsp;&nbsp;<br>
	<input type="radio" name="upload_rotateangle" value="180"> 180&deg;&nbsp;&nbsp;
	</td>
	<td></td>
	</tr>
	<tr>
	<td style="padding: 4px 0 4px 0;">
        <a id="showScaleParams" href="[$myfilename]">
        [str('Scale image while uploading')]</a><br>
        <a href="info.lasso"
        onclick="return popitup('info.lasso#scaleParams')">
        <img src="[$myimages]/info.jpg" title="[str('Info')]" alt="info"> 
        </a> 
        </td>
	<td colspan="2" id="scaleParams" class="display_none" 
        style="padding:4px;line-height:1.7;">
        <div style="float:left;">
	<?LassoScript
	var('i') = array;
	iterate($upload_imagesizes, $i);
	    '<input type="checkbox" name="upload_';
	    $i -> get(1);
	    '" value="';
//	    var('upload_' + ($i -> get(1)));
	    'yes';
	    '"> ';
	    $i -> get(3);
	    '<br>\n';
	/iterate;
	?>
	</td>
	<td></td>
	</tr>
	<input type="hidden" name="upload_subpath" value="">
	<tr><td> </td>
	<td>
	<INPUT TYPE="Submit" class="submit" NAME="upload" VALUE="[$txt2]">
	<INPUT TYPE="Hidden" NAME="upload" VALUE="[$txt2]">
	</td></tr>
    </table>
    </tr>
</FORM>

<?LassoScript // >
//     Inline( -Database=$myDb,
// 	    -Username='f1letags',
// 	    -Password='allowed10',
// 	    -Nothing);
//  	    If( File_Exists('/logos/' + $ytunnus + '.gif') );
// 		'</td><td style="border 1px solid black;">';
//  		'<IMG SRC="/logos/' + $ytunnus + '.gif" alt="logo">';
// 		'</td><td>';
// 
// 	    /If;  // end if file exists
//     /Inline;
// 

//     '</td></tr>\r';

/If; // end if fileupload = yes

// END file upload form 


// lisäyslomake  - add form

// If( (   (  ($errorcode == '-1728' && $dbType == 'fmp')
// 	|| ($Foundcount == 0 && $dbType == 'mysql')
// 	|| ($searchBehavior == 3)
//         )
// 	&& $dbChange != 1) 
//     ||  ($emptyForm == 'yes' && 
//           $buttonlist->(Find('add'))->get(1)->get(2) == 'yes')
//     ||  ($action == 'add' && $dbChange_error) );
// 
// // the table is still open
// 
// '<form name="MyForm2" action="';
//     $formTargetpage; 
//     if( $myp);
// 	'?p='; $myp;
//     /if;
//     '" method="POST">';
// $formstuff; 		// * hakumuuttujat *
// 
// $emptyForm = 'yes'; // if we get this far, show empty form anyway
// 
// Include( $fromMetoRoot + 'inc/admin_editform.inc');
// 
// '</FORM>';
// 
// /If;  


'</table>';

// customPostform

'<div id="postform">';
    If( (Variable_Defined('customPostform')));
	If( $customPostform == 'yes');
	    include( 'custom/postform/' + $myfilebody + '.inc');
	/If;
    /If;
'</div>\n';

/if; // end if dbchange_error (see just before main search)
?>


[Output_None]
<P class="[$textClass]" STYLE="text-align:center;margin-left:auto;margin-right:auto;"><FONT SIZE="-1"><A HREF="[var('publicPage')]" TARGET="_blank">[If($lang=='en')]Check the public page (will open in a new window)[Else]Tarkista julkinen sivu (avautuu uuteen ikkunaan)[/If]</A>.</FONT></P>
[/Output_None]

</div> <!-- end center -->
[/loop]

