/*
 * file        : common.c
 * description : <insert here>
 * copyright   : (c) 2019 Kickmaker
 * author      : Luis CRUZ
 */

// ==================================================
//     INCLUDE
// ==================================================

#include "common.h"



// ==================================================
//     GLOBAL CONSTANTS
// ==================================================

const char * DEV_STATUS_STR [ DEV_STATUS_MAX ] =
{
    [ DEV_STATUS_UNKNOWN  ] = "UNKNOWN"  ,
    [ DEV_STATUS_DISABLED ] = "DISABLED" ,
    [ DEV_STATUS_ENABLED  ] = "ENABLED"  ,
};



// ==================================================
//     GLOBAL VARIABLES
// ==================================================

char B_YEAR [ 5 ] , B_MONTH [ 3 ] , B_DAY [ 3 ] , B_HOUR [ 3 ] , B_MIN [ 3 ] , B_SEC [ 3 ];



// ==================================================
//     PUBLIC FUNCTIONS
// ==================================================

void
COMMON_InitBuildDefs ( void )
{
    INIT_BUILD_YEAR ( B_YEAR  );
    INIT_BUILD_MONTH( B_MONTH );
    INIT_BUILD_DAY  ( B_DAY   );
    INIT_BUILD_HOUR ( B_HOUR  );
    INIT_BUILD_MIN  ( B_MIN   );
    INIT_BUILD_SEC  ( B_SEC   );
}

void
COMMON_ErrorHandler ( void )
{
    while ( 1 ) {}
}



// End of file
