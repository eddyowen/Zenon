## ZN helper functions to save some time

function( zn_log message )
    message( STATUS "[ZN] ${message}" )
endfunction()

function( zn_warning message )
    message( WARNING "[ZN][WARNING] ${message}" )
endfunction()

function( zn_error message )
    message( FATAL_ERROR "[ZN][FATAL] ${message}" )
endfunction()