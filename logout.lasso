<?LassoScriptinclude('inc/defaults.inc');	var('hash') = LB_createCookiehash;	var('path') = response_path;	$path -> RemoveTrailing('admin/');	    // delete the cookies	    Cookie_Set( ('siteuser_' + $hash) 	    = '',-Expires=(-1),-Domain=$myDomain,-Path=$path );	    Cookie_Set( 'sitepass_' + $hash 	    = '',-Expires=(-1),-Domain=$myDomain,-Path=$path );?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"><HTML><HEAD><META NAME="ROBOTS" CONTENT="NOINDEX"><META NAME="ROBOTS" CONTENT="NOFOLLOW"><META http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"><TITLE>Logout</TITLE><link rel="stylesheet" href="themes/default/style.css" type="text/css"><style type="text/css"><!----></style></HEAD><BODY><p>&nbsp;</p><p STYLE="text-align:center;">[str('You have logged out of the site.')]</p><p STYLE="text-align:center;"><A HREF="[$opts -> find('siteurl')]">[str('See the site')]</A>&nbsp;|&nbsp;<A HREF="index.lasso?showlogin=yes">[str('Login again')]</A></p></BODY></HTML>