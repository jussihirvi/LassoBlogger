<?LassoScript 
var('fromMetoRoot' = '../');
include( 'inc/defaults.inc');
include( 'inc/navig.inc');
var('mytable') = 'tilaukset';
var('br') = '<br>';
define_tag('br');return('<br>\n'); /define_tag;

    // set username & password
Inline( -nothing,
  -username=$filetagsUsername,
  -password=$filetagsPassword,
);
// file_create('testdir/');
//  dir('testdir/') -> create;
  var('myimg') = image('/lbj/content/uploads/post-related/GreenSpotStall.jpg');
  $myimg -> save ('test.jpg',-quality=40);
/inline;

?>

