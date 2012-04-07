<hr>
<div id="footer">
    <div id="headerimg">

	<p>
	<?LassoScript
	'<i>';
	$basicOptions -> 'blogname';
	'</i> '; (str:'is powered by'); 
	?>
	<A HREF="http://www.greenspot.fi/LassoBlogger" TARGET="_blank" TITLE="LB Development Blog">LassoBlogger</A>[emptystrID:'1']
		<br><a href="[$fromMetoRoot]feeds/?feed=rss2">[str:'Entries'] (RSS)</a>
		| <a href="[$fromMetoRoot]feeds/?feed=comments-rss2">[str:'Comments'] (RSS)</a> 
		| [_date_msec - $start] ms
		<!-- 16 queries. 0.068 seconds. -->
	</p>

    </div> <!-- end footerimg -->
</div> <!-- end  footer -->
