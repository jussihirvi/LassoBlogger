<hr>
<div id="footer">
    <div id="footerimg">

	<p>
	<?LassoScript
	'<i>';
	$opts -> find('blogname');
	'</i> '; str('is powered by'); 
	?>
	<a href="http://www.greenspot.fi/LassoBlogger" target="_blank" title="LB Development Blog">LassoBlogger</a>[emptystrID('1')]
		<br /><a href="[$fromMetoRoot]feeds/?feed=rss2">[str('Entries')] (RSS)</a>
		| <a href="[$fromMetoRoot]feeds/?feed=comments-rss2">[str('Comments')] (RSS)</a> 
		| [_date_msec - $start] ms
		<!-- 16 queries. 0.068 seconds. -->
	</p>

    </div> <!-- end footerimg -->
</div> <!-- end  footer -->
