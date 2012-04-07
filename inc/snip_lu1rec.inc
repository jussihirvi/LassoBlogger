<?LassoScript

// modifioi muuttujien $luokka ja $beginend eri arvoille: 

// $luokka: tai (taidemuseot), mus (muut museonäyttelyt), tap (tapahtumat) 

// $beginend: 
// 1 (alkavat näyttelyt), 2 (päättyvät näyttelyt), 3 (jatkuvat näyttelyt)

If: $luokka == 'tai' || $luokka == 'mus';

    // alkupvm
    If: (Field:'n_ap') != '32';
	(Field:'n_ap') + '.' + (Field:'n_ak') + '.' + (Field:'n_av') + '--';
	Else;
	($kuukaudet -> (Get:(Field:'n_ak'))) + '--';
    /If;

    // loppupvm
    If: (Field:'n_lp') != '32';
	(Field:'n_lp') + '.' + (Field:'n_lk') + '.' + (Field:'n_lv');
	Else;
	'--' + ($kuukaudet -> (Get:(Field:'n_lk')));
    /If;

Else;  // case $luokka = tap
    If: (Field:'n_ap') == '32' || (Field:'n_lp') == '32'; // if 32
	($kuukaudet -> (Get:(Field:'n_ak')));
    Else; 						// case not 32
	If: (Field:'n_apvm') == (Field:'n_lpvm');// if sama päivä
	    (Field:'n_ap') + '.' + (Field:'n_ak');
	Else: ((Field:'n_ak') == (Field:'n_lk')) &&
		((Field:'n_av') == (Field:'n_lv'));	// case sama kk ja v
	    (Field:'n_ap') + '--' + (Field:'n_lp') + '.' +  (Field:'n_ak');
	Else: (Field:'n_av') == (Field:'n_lv');		// case vain sama v
	    (Field:'n_ap') + '.' + (Field:'n_ak') + '--';
	    (Field:'n_lp') + '.' + (Field:'n_lk');
	Else;						// case eri v
	    (Field:'n_ap') + '.' + (Field:'n_ak') + '--';
	    (Field:'n_lp') + '.' + (Field:'n_lk') + '.' + (Field:'n_lv');
	/If; 						// end if sama päivä
    /If; 						// end if 32

/If;  // end case tap

'\t';

// näyttelyn nimi ym. tiedot
If: (Field:'n_klo') != '';
'<b>' + (Field:'n_klo')+'</b> ';
/If;
'<A HREF=\"yp_nayttely.lasso?hakutietueid=' + (Field:'tietueid') + '\">';
(Field:'n_nimi') + '</A>. ';
'<A HREF=\"yp_museo.lasso?hakutietueid=' + (Field:'museoid') + '\">';
(Field:'museot::museo') + '</A>, ' + (Field:'museot::paikkakunta'); 
'<br>';
?>
