<?LassoScript //>
include('inc/defaults.inc');

    var('page_title')= str('Thanks for your comment!');

// some values here override those in config.lasso

var('contents')='cont/'+ $myfilebody + '.inc';

var('pageSecuritylevel') = 1; // available to the users of same level & up

var('error') = integer;
var('errorText') = string;

Include('gen/frame_default.inc');
?>

