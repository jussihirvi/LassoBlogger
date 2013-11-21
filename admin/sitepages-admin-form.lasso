<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/monsterdefaults.inc');

var('myTable'='sitepages_admin');

var('rectype' = 'page');
var('rectype_pl' = 'pages');
var('rectype_fi' = 'sivu');
var('rectype_fi_part' = 'sivuja');

If( $lang == 'fi');
    var('page_title'='Sivut');
    Else;
    var('page_title'='Pages');
/If;

// for how to use $searchFields and $editFields, 
// see the beginning of adminpage_monster.inc

	// collect categories select list
	inline( -database=$myDb, -table=$table_prefix + 'sitecats_admin',
	    -username=$dbUsername,
	    -password=$dbPassword,
	    -sortfield='sortcode',
	    -findall);
	    var('catlist_field' = '');
	    var('catlist_show' = '--VALITSE');
	    var( 'catlist_field_array' = array);
	    var( 'catlist_show_array'  = array);
	    records;
		$catlist_field += ';' + field('id');
		$catlist_show  += ';' + field('catname');
		$catlist_field_array -> insert(field('id'));
		$catlist_show_array  -> insert(field('catname'));
	    /records;
	/inline;

	var('i' = string);
	var('lc' = integer);
	var('pagelist_field' = '');
	var('pagelist_show' = '--VALITSE');
	iterate( $catlist_field_array, $i);
	    $lc += 1;
	    inline( -database=$myDb, -table=$table_prefix + $myTable,
		-username=$dbUsername,
		-password=$dbPassword,
		'catid'=$i,
		-sortfield='sortcode',
		-search);
		records;
		    $pagelist_field += ';' + field('id');
		    $pagelist_show  += ';' + $catlist_show_array -> get($lc) + '/' + field('menuname');
		/records;
	    /inline;
	/iterate;
// search form
var('searchFields'=Array(
//    array('ID','ID','hidden','','','','','','',''),
    array('id','Sivu','select','',$pagelist_field,$pagelist_show,'','','','nobreak'),
));

var('headerinfo_temp' = '<font size="-1"><b>[$fieldvalue]</b> (luotu <b>[Field(\'creationdate\')]</b>, muokattu <b>[Field(\'moddate\')]</b>)</font>');

// edit form
var('editFields'=Array(
array('ID','ID','replacecode','','','','','',$headerinfo_temp,''),
array('creationdate','Luontipvm','creationdate','','','','','datetime',''),
array('moddate','Muokkauspvm','moddate','','','','','datetime',''),
array('domain','Käytä salattua yhteyttä','replacecode','','','','','',
'[if($fieldvalue)]Kyllä[else]Ei[/if]',''),
array('catid','Kategoria','select','',$catlist_field,$catlist_show,'','','',''),
array('address','Osoite','text','req,unique','','','esim. <b>isannointi</b>. Ei välilyöntejä, ei erikoismerkkejä','','',''),
array('menuname','Valikkonimi','text','','','','','','',''),
array('heading','Pääotsikko','text','','','','','','',''),
//    array('menuname_lang2','Valikkonimi (en)','text','','','','','','',''),
array('content','Sivun sisältö','textarea_ckeditor', 'allowhtml','','','','','',''),
//    array('content_lang2','Sivun sisältö (en)','textarea','','','','','','',''),
array('draft','Luonnos','checkbox','','','', 'Luonnos-sivuja ei näytetä päävalikossa','','',''),
array('customdisplay','Räätälöity esitys','checkbox','','','','Sivun sisältöä ei määritellä tällä sivulla. Julkinen esitys ohjelmoidaan tiedostossa <code>/cont/&lt;osoite&gt;.inc</code>','','',''),
array('form','Sivulla on lomake','checkbox','','','','Lomakkeen sisältö ohjelmoidaan funktiossa MO_formelements, ks. <code>/inc/libmonster.inc</code>','','',''),
    ));

// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'catid',
'-sortfield' = 'sortcode',
'-sortfield' = 'menuname',
'-sortorder' = 'ascending'
));

var('publicPage'=''); // don't comment out (yet)

var('defaultSearch' = array);


// var('formTargetpage' = '');   // if target=self, comment this out
var('buttonlist' = (map('add'='no','update'='yes','delete'='yes')));
var('customSearchvalidations' 	= 'no');
var('customValidations' 		= 'no'); 
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');

	Include('gen/frame_admin.inc');

?>

