<?LassoScript
include('inc/defaults.inc');

	var('hash') = LB_createCookiehash;
	var('path') = response_path;
	$path -> RemoveTrailing('admin/');
	    // delete the cookies
	    Cookie_Set( ('siteuser_' + $hash) 
	    = '',-Expires=(-1),-Domain=$myDomain,-Path=$path );
	    Cookie_Set( 'sitepass_' + $hash 
	    = '',-Expires=(-1),-Domain=$myDomain,-Path=$path );

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html><head>

<meta name="ROBOTS" content="NOINDEX">
<meta name="ROBOTS" content="NOFOLLOW">
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">

<title>Logout</title>

<link rel="stylesheet" href="themes/default/style.css" type="text/css">
<style type="text/css">
<!--
-->
</style>

</head>

<body>

<p>&nbsp;</p>
<p style="text-align:center;">
[str('You have logged out of the site.')]
</p>
<p style="text-align:center;">
<a href="[$opts -> find('siteurl')]">[str('See the site')]</a>&nbsp;|&nbsp;
<a href="index.lasso?showlogin=yes">[str('Login again')]</a></p>

</body>
</html>

