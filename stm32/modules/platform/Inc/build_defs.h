/*
 * file        : build_defs.h
 * description : <insert here>
 * copyright   : (c) 2019 Kickmaker
 * author      : Luis CRUZ
 */

#ifndef __BUILD_DEFS_H__
#define __BUILD_DEFS_H__



// ==================================================
//     CPP GUARD [INIT]
// ==================================================

#ifdef __cplusplus
extern "C" {
#endif



// ==================================================
//     DATE
// ==================================================

#define B_YEAR_CH0          ( __DATE__[  7 ] )
#define B_YEAR_CH1          ( __DATE__[  8 ] )
#define B_YEAR_CH2          ( __DATE__[  9 ] )
#define B_YEAR_CH3          ( __DATE__[ 10 ] )

#define B_MONTH_IS_JAN      ( __DATE__[ 0 ] == 'J' && __DATE__[ 1 ] == 'a' && __DATE__[ 2 ] == 'n' )
#define B_MONTH_IS_FEB      ( __DATE__[ 0 ] == 'F' )
#define B_MONTH_IS_MAR      ( __DATE__[ 0 ] == 'M' && __DATE__[ 1 ] == 'a' && __DATE__[ 2 ] == 'r' )
#define B_MONTH_IS_APR      ( __DATE__[ 0 ] == 'A' && __DATE__[ 1 ] == 'p' )
#define B_MONTH_IS_MAY      ( __DATE__[ 0 ] == 'M' && __DATE__[ 1 ] == 'a' && __DATE__[ 2 ] == 'y' )
#define B_MONTH_IS_JUN      ( __DATE__[ 0 ] == 'J' && __DATE__[ 1 ] == 'u' && __DATE__[ 2 ] == 'n' )
#define B_MONTH_IS_JUL      ( __DATE__[ 0 ] == 'J' && __DATE__[ 1 ] == 'u' && __DATE__[ 2 ] == 'l' )
#define B_MONTH_IS_AUG      ( __DATE__[ 0 ] == 'A' && __DATE__[ 1 ] == 'u' )
#define B_MONTH_IS_SEP      ( __DATE__[ 0 ] == 'S' )
#define B_MONTH_IS_OCT      ( __DATE__[ 0 ] == 'O' )
#define B_MONTH_IS_NOV      ( __DATE__[ 0 ] == 'N' )
#define B_MONTH_IS_DEC      ( __DATE__[ 0 ] == 'D' )

#define B_MONTH_CH0 \
    ( ( B_MONTH_IS_OCT || B_MONTH_IS_NOV || B_MONTH_IS_DEC ) ? '1' : '0' )

#define B_MONTH_CH1 \
    ( \
        ( B_MONTH_IS_JAN ) ? '1' : \
        ( B_MONTH_IS_FEB ) ? '2' : \
        ( B_MONTH_IS_MAR ) ? '3' : \
        ( B_MONTH_IS_APR ) ? '4' : \
        ( B_MONTH_IS_MAY ) ? '5' : \
        ( B_MONTH_IS_JUN ) ? '6' : \
        ( B_MONTH_IS_JUL ) ? '7' : \
        ( B_MONTH_IS_AUG ) ? '8' : \
        ( B_MONTH_IS_SEP ) ? '9' : \
        ( B_MONTH_IS_OCT ) ? '0' : \
        ( B_MONTH_IS_NOV ) ? '1' : \
        ( B_MONTH_IS_DEC ) ? '2' : \
        /* error default */  '?'   \
    )

#define B_DAY_CH0           ( ( __DATE__[ 4 ] >= '0' ) ? ( __DATE__[4] ) : '0' )
#define B_DAY_CH1           ( __DATE__[ 5 ] )



// ==================================================
//     TIME
// ==================================================

#define B_HOUR_CH0          ( __TIME__[ 0 ] )
#define B_HOUR_CH1          ( __TIME__[ 1 ] )

#define B_MIN_CH0           ( __TIME__[ 3 ] )
#define B_MIN_CH1           ( __TIME__[ 4 ] )

#define B_SEC_CH0           ( __TIME__[ 6 ] )
#define B_SEC_CH1           ( __TIME__[ 7 ] )



// ==================================================
//     MACRO
// ==================================================

#define INIT_BUILD_YEAR( x ) \
    do {                     \
        x[ 0 ] = B_YEAR_CH0; \
        x[ 1 ] = B_YEAR_CH1; \
        x[ 2 ] = B_YEAR_CH2; \
        x[ 3 ] = B_YEAR_CH3; \
        x[ 4 ] = 0x00;       \
    } while ( false );

#define INIT_BUILD_MONTH( x ) \
    do {                      \
        x[ 0 ] = B_MONTH_CH0; \
        x[ 1 ] = B_MONTH_CH1; \
        x[ 2 ] = 0x00;        \
    } while ( false );

#define INIT_BUILD_DAY( x ) \
    do {                    \
        x[ 0 ] = B_DAY_CH0; \
        x[ 1 ] = B_DAY_CH1; \
        x[ 2 ] = 0x00;      \
    } while ( false );

#define INIT_BUILD_HOUR( x ) \
    do {                     \
        x[ 0 ] = B_HOUR_CH0; \
        x[ 1 ] = B_HOUR_CH1; \
        x[ 2 ] = 0x00;       \
    } while ( false );

#define INIT_BUILD_MIN( x ) \
    do {                    \
        x[ 0 ] = B_MIN_CH0; \
        x[ 1 ] = B_MIN_CH1; \
        x[ 2 ] = 0x00;      \
    } while ( false );

#define INIT_BUILD_SEC( x ) \
    do {                    \
        x[ 0 ] = B_SEC_CH0; \
        x[ 1 ] = B_SEC_CH1; \
        x[ 2 ] = 0x00;      \
    } while ( false );



// ==================================================
//     CPP GUARD [END]
// ==================================================

#ifdef __cplusplus
}
#endif



#endif /* __BUILD_DEFS_H__ */
