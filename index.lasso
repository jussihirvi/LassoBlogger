<?LassoScript //>
include('inc/defaults.inc');

    var('page_title'='');

// stop editing here

// some values here override those in config.lasso

var('contents'='cont/'+ $myfilebody + '.inc');

// for naviglinks
var('rectype'        = 'post');
var('rectype_plural' = 'posts');

Include('gen/frame_default.inc');
?>

