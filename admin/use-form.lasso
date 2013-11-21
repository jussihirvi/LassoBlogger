<?LassoScript //>
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
var('pageSecuritylevel') = 1;

var('myTable'='users');
var('rectype' = 'user');
var('rectype_pl' = 'users');
var('rectype_fi' = 'käyttäjä');
var('rectype_fi_part' = 'käyttäjiä');

If( $lang == 'fi');
    var('page_title'='Käyttäjät');
    Else;
    var('page_title'='Users');
/If;

var('searchFields'=Array(
array('ID','ID','hidden','','','','','','','','eq'),
));

if($userlevel_ses >= 8);
  var('levels1') = '10;9;8;7;6;5;4;3;2;1;0';
  var('levels2') = '10 can do anything;9;8 can edit users and options;7;6;5 can edit categories and links;4 can administer comments;3 can upload;2 can write;1 can write drafts;0';
else;
  var('levels1') = string($userlevel_ses);
  var('levels2') = string($userlevel_ses);
/if;

var('id_temp' = "<span style=\"font-size:0.9em;\">ID = <b>[field('id')]</b>, [str('Created')] <b>[field('use_registered')]</b></span>
");
// edit form
var('editFields')=Array(
array('id','','replacecode','','','','','',$id_temp),
Array('use_login','Login Name','text','req;unique','100px','','Must be unique','','',''),
Array('use_pass','Password','password','req;password','100px','','Not visible, but can be changed','','','','md5'),
Array('use_level','Level','select','req',$levels1,$levels2,'Every writer may edit his/her own messages and those of lower-level users','',''),
Array('use_firstname','Name','text','','','','','',''),
Array('use_lastname','','text','','','','','',''),
Array('use_nickname','Public Name','text','req,unique','','','Must be unique','',''),
Array('use_email','Email','text','email,unique','','','Must be unique','',''),
Array('use_url','Website','text','','','','without "http://"','',''),
Array('use_icq','ICQ','text','int','','','must be integers','',''),
Array('use_aim','AIM','text','','','','','',''),
Array('use_msn','MSN','text','','','','','',''),
Array('use_yim','Yahoo','text','','','','','',''),
Array('use_description','Description','textarea','','','','','',''),
);


// define sorting for the main search inline
var('sortstuff' = array(
'-sortfield' = 'id',
'-sortorder' = 'ascending'
));

If( $userlevel_ses < 8);
  var('defaultSearch') = array('id'=$userid_ses);	
  var('buttonlist')    = map('add'='no','update'='yes','delete'='no');
Else;
  var('defaultSearch') = array;	
  var('buttonlist')    = map('add'='yes','update'='yes','delete'='yes');
/If;

// var('formTargetpage' = '');   // if target=self, comment this out
var('customValidations' = 'yes'); // delete validation 
	// if yes, the associated include file must be in place
var('customPreform' = 'no'); 
var('customPostform' = 'no'); 

var('fileupload' = 'no');   
var('allowed_file_suffixes' = 'gif jpg doc pdf ppt');
var('fileupload_maxk' = '5000');

    Include( 'gen/frame_admin.inc');
?>

