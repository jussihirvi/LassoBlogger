[If: $action == 'add_action']

    [If: $dbChange_error == '']
    <div class="wrap">
    <p>[str:'The registration has succeeded.'] </p>
    <p>[str:'Your username is'] <strong>[$use_login]</strong> 
    [str:'and your password'] <strong>[$use_pass]</strong>. 
    [str:'Be sure to remember these values.'] </p>
    <p>[str:'You can now log in at'] <A HREF="admin/">
    [str:'the admin pages']</A> [str:'and start writing posts (if your initial permissions allow it)']!</p>
    </div>
    [Abort]

    <?LassoScript
    Else: $dbChange_error != 'validation failed';
    Else;
    /If;
Else;
Abort; // IF ACTION NOT ADD
/If;
?>

