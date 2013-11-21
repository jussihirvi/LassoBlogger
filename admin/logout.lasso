<?LassoScript//>
var('fromMetoRoot' = '../');
Include( 'inc/defaults.inc');

var('myTable'='pagecontents');
var('contents'='cont/logout.inc');

var('rectype' = 'page');
var('rectype_pl' = 'pages');
var('rectype_fi' = 'sivu');
var('rectype_fi_part' = 'sivuja');

If( $lang == 'fi');
    var('page_title'='Uloskirjautuminen');
Else;
    var('page_title'='Logout');
/If;


	Include('gen/frame_admin.inc');

?>

