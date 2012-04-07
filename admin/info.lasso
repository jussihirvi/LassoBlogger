<?LassoScript
var('fromMetoRoot' = '../');
Include('inc/defaults.inc');
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Info</title>

<link rel="stylesheet" href="/includes/style.css" type="text/css">

<style type="text/css">
<!--
body {  padding:14px; 
  margin:0; 
  font-family: Verdana, Geneva, Arial, Sans-serif; 
  font-size:62.5%;
}


h4 {margin:0;padding:0;font-size:1.2em;float:left;padding-right:8px; }

p, ul, ol {font-size:1.2em; margin-top:10px; margin-bottom:10px; }


-->
</style>

</head>
<body>
[if($lang == 'fi')]
<h1>Ohjeita</h1>
<h2>Tiedoston lataaminen</h2>
<a name="upload">
<h4>Mit� tiedostoja voi ladata?</h4>
<p>
T�ll� hetkell� sallitut tiedostotyypit: <b>[$allowed_file_suffixes]</b>. Tiedoston sallittu enimm�iskoko on <b>[$upload_maxk]</b> kilotavua.
</p>
<a name="upload">
<h4>Tallenna nimell�...</h4>
<p>
Voit joko k�ytt�� oletusarvoa tai kirjoittaa tiedostolle uuden nimen. 
</p>
<a name="scaleParams">
<h4>Tallenna m��r�kokoisena (suositellaan)
</h4>
<p>
Jos ladattava tiedosto on kuva (jpg, gif) ja yksi tai useampi m��r�koko 
on valittu, kuvasta tehd��n lataamisen yhteydess� valitunlaiset kopiot. Alkuper�ist� kuvatiedostoa ei t�ll�in tallenneta palvelimelle lainkaan.  
</p>
<p>
Sivuston julkisella puolella on oma esityslogiikka jokaiselle m��r�koolle. Pienemm�st� kuvasta johtaa aina linkki suurempaan kuvaesitykseen.  
</p>
<p>
Jos ladattava kuva on pienempi kuin jokin valituista m��r�kooista, kuvaa ei muokata, vaan alkuper�ist� kuvaa k�ytet��n kyseiseen n�ytt�tarkoitukseen. 
</p>
[else]
<h1>Info</h1>
<h2>Uploading a file</h2>
<a name="upload">
<h4>What files can uploaded?</h4>
<p>
At the moment these file suffixes are allowed: <b>[$allowed_file_suffixes]</b>. The maximun file size is <b>[$upload_maxk]</b> kilobytes.
</p>
<a name="upload">
<h4>Save as...</h4>
<p>
You can either use the default value, or write a new name for the file.
</p>
<a name="scaleParams">
<h4>Scale (recommended)
</h4>
<p>
If the file to be uploaded is an image (jpg, gif), and one or more sizes have been selected with the scale selector, scaled copies will be made during upload. the original image file will in this case not be kept on the server at all.  
</p>
<p>
On the public side of this site there is a certain logic for displaying each image size. There will be a link from a smaller to a bigger version. 
</p>
<p>
If the image to be uploaded is smaller than some of the selected target sizes, the image will not be scaled, but the original image will be used for that size. 
</p>
[/if]
</body>
</html>
