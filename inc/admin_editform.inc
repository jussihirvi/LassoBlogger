<?LassoScript //>
// special case (uh): if emptyform = yes and userlevel allows no add button
// then there's no use in showing empty form
// a cleaner way to handle this might be  before search inline, 
// using searchtoggle

    If: $emptyForm == 'yes' &&  
    $buttonlist -> (Find:'add') -> (get:1) -> (Get:2) == 'no';
    // no need to show an empty form! - skip this file
    Else; // show it


If: $viewtype == 'listview';
	'<tr>';
/If; 			// end case listview

// define what should be iterated
If: $emptyForm == 'yes';
    If: $dbChange_error != '' && $action == 'add_action';
	Var:'iterated' = $addFields;
	Else: $dbChange_error != '' && $action == 'update_action';
	Var:'iterated' = $editFields;
	Else;
	Var:'iterated' = $addFields;
    /If;
Else; 				// $emptyForm != yes
    Var:'iterated' = $editFields;
/If;


Iterate: $iterated, (Var:'this');

Variable:'fieldType' = $this -> (Get:3);


//      * * * define  basis for fieldvalue * * *

If: $dbChange_error != '';
    Variable:'fvBase' = (Var:($this -> (Get:1)));
Else:  $dbChange_error == '' && $emptyForm == 'yes'; 
    Variable:'fvBase' = $this -> (Get:9);
Else;
    Variable:'fvBase' = (Field:($this -> (Get:1)));
/If;


// 	* * * process fieldvalue * * * 


If: $emptyForm != 'yes';		// otherwise show empty form

// date - muunna pvm

    If: ($this -> (Get:8)) == 'date';

	If: $fvBase != ''
	&& $fvBase != '0000-00-00 00:00:00'; // if not empty
	    If: (Date:$fvBase) -> (Time) !='00:00:00'; 
		//datetime
	    Variable:'fieldvalue' =	(Date_Format:(Date:$fvBase),
		-Format=$date_showFormat);
	    Else;
	    Variable:'fieldvalue' = (Date_Format:(Date:$fvBase),
		-Format=$date_showFormat);
	    /If;
	Else; 
	    Var:'fieldvalue' = '';
	/If;
	
    Else: ($this -> (get:8)) == 'time';

		If: $fvBase != ''; // if not empty
		    Variable:'fieldvalue'= 
		    ((Date:$fvBase,-Format='%H:%M:%S') -> (Format:'%H:%M'));
		Else;
		    Variable:'fieldvalue'= '';
		/If; 
    // Else: $i == 'hetu';  // TANEMA SPECIAL
    //    Variable:'fieldvalue' = 
    // (Decrypt_Blowfish:$fvBase,-Seed=(Include:'../gen/seed.inc'));
    Else; 				// if not date or time (as of 050905)
	Variable:'fieldvalue' = $fvBase;
    /If; 

Else; 					// case emptyform=1 
	Variable:'fieldvalue' = $fvBase;
/If; 					// end if emptyform != 1

// add linktext (if any) to fieldvalue

    // special process defined in $editfields

    If: $this -> (Get:9) != '';
	$fieldvalue = (Process:$this -> (Get:9)); 
	    /* this will include the fielvalue from db, 
	        if #9 refers to [$fieldvalue] */
    /If;




// 		* * * get ready for showing field * * * 


    If: $viewtype == 'formview';

	If: $fieldType != 'hidden'
	 && $fieldType != 'moddate'
	 && $fieldType != 'creationdate'; // hide entire row
     // remember to duplicate these conditions for row close tags (below)
	    '<tr>';
	    '<th align="right" STYLE="padding-right:10px;">'; // nimilappu
		    $this -> (Get:2); // nimilappu
			If: ($this -> (Get:4)) >> 'req'; 
			$reqFieldsFound = 'yes';
			'*'; 
			/If; // if pakollinen kenttä
	    '</th>';
	    '<td STYLE="vertical-align:middle;">';
	    '<table border=\"0\" cellspacing=\"2\" cellpadding=\"0\">'; // emb table
	    '<tr>';
	    '<td class="' + $defaultClass + '" STYLE="vertical-align:middle;">'; 
	/If; // if hidden, moddate, creationdate

    Else: $viewtype == 'listview'; // listview, hidden field: no cell
	If: $fieldTYpe != 'hidden' 
	     && $fieldType != 'moddate'
	     && $fieldType != 'creationdate';
	    '<td valign="top" class="' + $defaultClass + '">'; 
	/If;
    /If; // end case listview
    



// now show fieldvalues

If: $fieldTYpe == 'noentry'; 	// case noentry
    Var:'fieldvalue',-EncodeBreak;
    '<input type=\"hidden\" name=\"' + ($this -> (Get:1));
    '\" value=\"' + $fieldvalue + '\">';

Else: $fieldType == 'nosave'; 	// case nosave
    Var:'fieldvalue',-EncodeNone; 
	// was: encodebreak; but that doesn't work, if there is link text 
	// (html code in that case changes to entities)
//    '<input type=\"hidden\" name=\"' + ($this -> (Get:1));
//    '\" value=\"' + $fieldvalue + '\">';

Else: $fieldType == 'creationDate'; 	// case creationDate
//    Var:'fieldvalue',-EncodeNone; 
// current date is saved instead of fieldvalue

Else: $fieldType == 'modDate'; 	// case modDate
//    Var:'fieldvalue',-EncodeNone; 
// current date is saved instead of fieldvalue

Else: $fieldType == 'saveIfNotEmpty'; 	// case saveIfNotEmpty
'<input type="' + 'text' + '" name="';
($this -> (Get:1));
    '" STYLE="width:';
	If: ($this -> (Get:5)) == ''; // use default
	$size1default; Else; ($this -> (Get:5)); 
	/If;
    ';" value="';
    If: $dbChange_error != ''; (Var:($this -> (Get:1))); /If;
    '">';

Else: $fieldType == 'hidden'; 	// case hidden
    '<input type=\"hidden\" name=\"' + ($this -> (Get:1));
    '\" value=\"' + $fieldvalue + '\">\r';

Else: $fieldType == 'text'; 	// case text
'<input type=\"' + ($this -> (Get:'3')) + '\" name=\"';
($this -> (Get:1));
    '\" STYLE=\"width:';
	If: ($this -> (Get:5)) == ''; // use default
	$size1default; Else; ($this -> (Get:5)); 
	/If;
    ';" value=\"' + (Var:'fieldvalue',-encodeHTML) + '\">'; // enc html 050921

Else: $fieldType == 'textarea';	// case textarea
    '<Textarea name=\"';
    $this -> (Get:1);
    '\" style=\"vertical-align:text-top; width:';
	If: ($this -> (Get:5)) == '';	// use default
	$size1default; Else; ($this -> (Get:5)); 
	/If;
    ';" rows=\"'; 
	If: ($this -> (Get:6)) == ''; // use default
	$size2default; Else; ($this -> (Get:6)); 
	/If;
    '\" wrap=\"soft\" cols="40">'; // if cols attribute is not here, 
    				   // the text won't wrap in OS9/NS7

    (Var:'fieldvalue',-encodeHTML);		// enc html 050921
    '</Textarea>';

Else: $fieldType == 'textarea_fckeditor';	// case textarea_fckeditor
// init type to get the local basepath var
var:'fck_xxx' = fck_editor: -instancename='test';
var:'fck_basepath' = $fck_xxx -> 'basepath';
// example value /RSC/admin/fckeditor/

    var('myeditor') = fck_editor(
         -instancename=$this -> (Get:1),
         -basepath=$fck_basepath,
	 -width='800',
         -initialvalue=(Var:'fieldvalue',-encodeHTML)
    );
$myeditor->create;

Else: $fieldType == 'select';	// case select
Variable:'valuelist' = ($this -> (Get:5)) -> (Split:';');
Variable:'showlist' = ($this -> (Get:6)) -> (Split:';');
'\r<select name=\"' + ($this -> (Get:1)) + '\" size=\"1\">';
Loop: ($valuelist -> Size);
    '<option value=\"' + ($valuelist -> (Get:(Loop_Count))) + '\"';
    If: $fieldvalue == ($valuelist -> (Get:(Loop_Count))); 
    ' SELECTED';
    /If;
    '>' + ($showlist -> (Get:(Loop_Count))) + '</option>\r';
/Loop;
'</select>\r';

Else: $fieldType >> 'selectMultiple';	// case selectMultiple
Variable:'valuelist' = ($this -> (Get:5)) -> (Split:';');
Variable:'showlist' = ($this -> (Get:6)) -> (Split:';');
Variable:'fieldvalueTemp' = $fieldvalue -> (Split:'\r');
'\r<select name=\"'; ($this -> (Get:1)); '\" size=\"';
$fieldType -> (Get:($fieldType -> Size));
'\" MULTIPLE>';
Variable:'myValue' = '';
Loop: ($valuelist -> Size);
$myValue = $valuelist -> (Get:(Loop_Count));
    '<option value=\"' + $myValue + '\"';
    Loop: ($fieldValueTemp -> Size);
	If: $fieldvalueTemp -> (Get:(Loop_Count)) == $myValue; 
	    ' SELECTED';
	/If;
    /Loop;
    '>' + ($showlist -> (Get:(Loop_Count))) + '</option>\r';
/Loop;
'</select>\r';

Else: $fieldType == 'radio';	// case radio
Variable:'valuelist' = ($this -> (Get:5)) -> (Split:';');
Variable:'showlist' = ($this -> (Get:6)) -> (Split:';');
Loop: ($valuelist -> Size);
'\r<INPUT TYPE=\"radio\" NAME=\"' + ($this -> (Get:1)) + '\" VALUE=\"';
($valuelist -> (Get:(Loop_Count))) + '\"';
    If: $fieldvalue == ($valuelist -> (Get:(Loop_Count))); 
    ' CHECKED';
    /If;
'> ' + ($showlist -> (Get:(Loop_Count))) + '&nbsp;';
/Loop;

Else: $fieldType == 'checkbox';	// case checkbox
    If: $this -> (Get:5) == '';
	Variable:'checkedValue' = 'on';
	Else;
	Variable:'checkedValue' = $this -> (get:5);
    /If;
'\r<INPUT TYPE="checkbox" CLASS="checkbox"';
    ' NAME="' + ($this -> (Get:1)) + '"';
    ' VALUE="' + $checkedValue + '"';
    If: $fieldvalue == $checkedValue; 
    ' CHECKED';
    /If;
'> ';

Else: $fieldType == 'override';	// case override 
					// (defined in $searchfields, slot 9)
$fieldvalue;

/If; 					// if type 

// end show fieldvalues


If: $viewtype == 'formview';		// case formview: helppitekstit

    If: $fieldType != 'hidden'
     && $fieldType != 'moddate'
     && $fieldType != 'creationdate'; 	// not possible for hidden fields etc.
	If: ($this -> (Get:7)) != '';
	    '</td><td class="formview_help" valign="top">';
	    '<span class="' + $defaultClass + '">';
		(Process:($this -> (Get:7))); 
	    '</span>';
	/If;
    /If;

    If: $fieldType != 'hidden'
     && $fieldType != 'moddate'
     && $fieldType != 'creationdate'; // entire row has been hidden
     // remember to duplicate these conditions for row open (above)
	'</td></tr></table>'; 		// case formview  close emb table & row
	'</td></tr>\r';
    /If; 

Else: $fieldType != 'hidden';	// case listview: close cell
					// (if not hidden field)
    '&nbsp;&nbsp;</td>\r';

Else; 					// case listview, hidden field
    '\r';
/If; 

If: $emptyForm == 'yes'; // we are processing addform
	(Var:'fieldvalue') = '';	// for emptyform; must be reset
	/If;
/Iterate;




If: $viewtype == 'formview';		// prepare for buttons

    If: $reqFieldsFound == 'yes';
	'<tr><td></td><td colspan="2" STYLE="padding-top:8px;">';
	     (str:'Fields marked with asterisk (*) are required');
	'</td></tr>';
    /If;

    '<tr><td>&nbsp;</td><td colspan="2" STYLE="padding-top:8px;">';

Else; 					// listview
    '<td valign="top" class="' + $defaultClass + '" STYLE="text-align:center;">';
/If;

// which buttons?
Variable: 'showAbutton' = (Integer), 
	'showUbutton' = (Integer), 
	'showDbutton' = (Integer),
	'showD2button' = (Integer);

If: $buttonlist -> (Find:'add') -> (get:1) -> (Get:2) == 'yes'
  && ($emptyForm == 'yes' || $userlevel_ses > 1);
$showAbutton = 1;
/If;
If: $emptyForm != 'yes';
    If: $buttonlist -> (Find:'update') -> (get:1) -> (Get:2) == 'yes';
    $showUbutton = 1;
    /If;
    If: $buttonlist -> (Find:'delete') -> (get:1) -> (Get:2) == 'yes';
    $showDbutton = 1;
    /If;
    If: $buttonlist -> (Find:'quickdelete') != Array;
	If: $buttonlist -> (Find:'quickdelete') -> (get:1) -> (Get:2) == 'yes';
	$showD2button = 1;
	/If;
    /If;
/If;

// addbutton text
If: $emptyForm == 'yes';
    Var:'enText' = 'Save';
    Var:'fiText' = 'Tallenna';
    Else;
    Var:'enText' = 'New ' + $rectype_en + ' with this data';
    Var:'fiText' = 'Uusi ' + $rectype_fi + ' lomakkeen tiedoilla';
/If;
?>
[If: $showAbutton == 1]
<input type="Submit" name="add_action" value="[If: $lang == 'en'][$enText][Else][$fiText][/If]" class="edit">
[/If]
[If: $showUbutton == 1]
<input type="Submit" name="update_action" value="[If: $lang == 'en']Save changes[Else]Tallenna muutokset[/If]" class="edit">
[/If]
[If: $showDbutton == 1]
<input type="Submit" name="delete_action" value="[If: $lang == 'en']Delete[Else]Poista[/If]" class="edit">
[/If]
[If: $showD2button == 1]
<input type="Submit" name="delet2_action" value="[If: $lang == 'en']Delete (NO WARNING)[Else]Poista (VAROITTAMATTA)[/If]" class="edit">
[/If]
</td></tr>


<?LassoScript
/If; // see top of the file
?>
