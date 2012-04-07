070718
/if; // end if dbchange_error (see just before main search)
Files in this file will be run at two places of monster.inc
1) add or update action (with array $this  and map $inputVars available)
2) delete action (array $this NOT available)

Therefore, you have to use the following basic structure in the validation file: 

    if( $action == 'add' || $action == 'update' );
	// validation code here...
	// here you may refer to $this
    else( $action == 'delete' );
	// validation code here...
    /if;

