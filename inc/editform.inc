<?LassoScript //>
// new 2010-11
var('fieldvalue' = string);

'<form name="editform" ';
    if( $viewtype == 'formview' );
      ' id="editform"';
    /if;
      ' class="editform"';
' action="' + $formTargetpage; 
    if( $myp);   
      '?p='; $myp;   
    /if;
    '" method="POST">\n';
'<input type="hidden" name="ref" value="editform">\n';
'<input type="hidden" name="keyvalue" value="' + KeyField_Value + '">\n';
'<input type="hidden" name="skip" value="' + $skip + '">\n';

if( $viewtype == 'listview');
  '<tr>\n';
/if;

Iterate( $editfields, var('this')); // iterate fields

// 		* * * get ready for showing field * * * 
var('fieldName' = $this -> get(1));
var('fieldType' = $this -> get(3));
var('fieldvalue') = string;
if($fvalues -> size);
  $fieldValue = $fvalues -> find($fieldname);
/if;


If( $viewtype == 'formview');

   If( $fieldType == 'hidden'
     || $fieldType == 'moddate'
     || $fieldType == 'creationdate'); 
     // hide entire row but include input element for hidden
  MO_formfieldHandler(-this=$this,-fvalue=$fieldvalue);
   else( $fieldType == 'heading');
      '<tr>';
        '<td class="formblockheading" colspan="2" style="font-weight:bold;">'; 
        $fieldvalue;
        '</td>';
      '</tr>\n';
   else( $fieldType == 'heading2');
      '<tr>';
        '<th>&nbsp;</th>';
        '<td class="formblockheading2" colspan="2" style="">'; 
        $fieldvalue;
        '</td>';
      '</tr>\n';
   else;
      '<tr id="tr-'+ $fieldname + '">';
        '<th class="'+$textClass+'">'; // label
          $this -> (Get(2)); // label
            If( ($this -> Get(4)) >> 'req'); 
              $reqFieldsFound = 'yes';
              '*'; 
            /If; // if pakollinen kenttä
        '</th>\n';
        '<td id="td-' + $fieldname + '" style="">\n';
        MO_formfieldHandler(-this=$this,-fvalue=$fieldvalue);
            '<div class="helptext" id="help-'+$fieldname+'"> ';
              If( $this -> (Get(7) )); // show help texts
                  Process($this -> Get(7)); 	// process (27.10.05) 
              /If;
            '</div>\n';
            '<div style="clear:both;"></div>\n';
	    // 'TEST: fv-base = ' + $fvBaseCase;
	'</td>';
      '</tr>\r';
    /If;
/if; // end if formview 

if( $viewtype == 'listview'); // listview, hidden field: no cell
  If(     $fieldType == 'hidden' 
       || $fieldType == 'moddate'
       || $fieldType == 'creationdate');
   // hide the field but include input element for hidden
  MO_formfieldHandler(-this=$this,-fvalue=$fieldvalue);
  else;
    '<td valign="top" class="' + $inputtextClass + '">'; 
  MO_formfieldHandler(-this=$this,-fvalue=$fieldvalue);
    '</td>';
  /If;  '\n';
/If; // end case listview


/Iterate;


If( $viewtype == 'formview');		// prepare for buttons
    If( $reqFieldsFound == 'yes');
	'<tr><td></td><td colspan="2" style="font-size:1.2em;padding:8px 0 0 0">';
	    str('Fields marked with an asterisk (*) are required.');
	'</td></tr>';
    /If;
    '<tr><td>&nbsp;</td><td colspan="2" style="padding:8px 0 20px 0;">';
Else; // listview
    '<td class="' + $inputtextClass + '" style="text-align:center;">';
/If;

// which buttons?
var( 'showAbutton' = integer, 
	'showUbutton'   = integer, 
	'showDbutton'   = integer,
	'showQDbutton'  = integer);

If( $buttonlist -> find('add') == 'yes' && $viewtype != 'listview');
  $showAbutton = 1;
/If;
If( $foundCount > 0 && $emptyform != 'yes');
    // note: even when emptyform == yes, foundCount can be > 0
    // this depends on $searchbehavior
    If( $buttonlist -> find('update') == 'yes');
      $showUbutton = 1;
    /If;
    If( $buttonlist -> find('delete') == 'yes');
      $showDbutton = 1;
    /If;
    If( $buttonlist -> find('quickdelete') != Array);
        If( $buttonlist -> Find('quickdelete') == 'yes');
            $showQDbutton = 1;
        /If;
    /If;

/If;

If( $showAbutton == 1);
  '<input type="Submit" class="submit" name="add" value="';
    if( variable_defined('customAddbuttontext'));
      $customAddbuttontext;
    else;
      MO_buttonStr('add');
    /if;
  '">\n';
/If;
If( $showUbutton == 1);
  '<input type="Submit" class="submit" name="update" value="';
      MO_buttonStr('update');
  '">\n';
/If;
If( $showDbutton == 1);
  '<input type="Submit" class="submit" name="delete" value="';
      MO_buttonStr('delete');
  '">\n';
/If;
If( $showQDbutton == 1);
  '<input type="Submit" class="submit" name="delet2" value="';
      MO_buttonStr('quickdelete');
  '">\n';
/If;
// both listview and formview
'</td></tr>\n';

// If( $viewtype == 'formview');
// '<tr><td>&nbsp;</td><td colspan="2" style="padding-top:8px;">';
// 'See the public page: <a href="/';
// $publicPage; '" target="_blank">'; $publicPage; '</a>\n';
// '</td></tr>';
// /If;

'</form>';
?>
