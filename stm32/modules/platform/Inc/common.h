/*
 * file        : common.h
 * description : <insert here>
 * copyright   : (c) 2019 Kickmaker
 * author      : Luis CRUZ
 */

#ifndef __COMMON_H__
#define __COMMON_H__



// ==================================================
//     CPP GUARD [INIT]
// ==================================================

#ifdef __cplusplus
extern "C" {
#endif



// ==================================================
//     INCLUDE
// ==================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>

#include "device.h"
#include "config.h"
#include "build_defs.h"
#include "console.h"

#include "main.h"



// ==================================================
//     MACRO
// ==================================================

#define STRINGIFY( x )          #x
#define STR( x )                STRINGIFY( x )
#define ARRAY_SIZE( x )         ( sizeof( x ) / sizeof( x[ 0 ] ) )
#define NOP()                   asm volatile( "nop\n\t" )
#define WFI()                   asm volatile( "wfi" )
#define BCD_TO_INT( x )         ( ( ( ( x & 0xF0 ) >> 4 ) * 10 ) + ( x & 0x0F ) )
#define SWAP_16BITS( x )        ( ( x << 8 ) | ( ( x >> 8 ) & 0xFF ) )
#define SWAP_32BITS( x )        ( ( ( x >> 24 ) & 0xff       ) | \
                                  ( ( x <<  8 ) & 0xff0000   ) | \
                                  ( ( x >>  8 ) & 0xff00     ) | \
                                  ( ( x << 24 ) & 0xff000000 ) )



// ==================================================
//     TYPE DEFINITIONS
// ==================================================

typedef enum {

    PERIPHERAL_CS_STATUS_DISABLED = 0 ,
    PERIPHERAL_CS_STATUS_ENABLED      ,

} PERIPHERAL_CS_STATUS_t;

typedef void ( * PERIPHERAL_CS_CALLBACK_t )( PERIPHERAL_CS_STATUS_t status , void * arg );

typedef struct {

    GPIO_TypeDef * port;
    uint16_t       pin;

} GPIO_t;

typedef enum {

    POWER_OFF = 0 ,
    POWER_ON      ,

} POWER_t;

typedef enum {

    DEV_STATUS_UNKNOWN = 0 ,
    DEV_STATUS_DISABLED    ,
    DEV_STATUS_ENABLED     ,
    DEV_STATUS_MAX         ,

} DEV_STATUS_t;



// ==================================================
//     EXTERN VARIABLES
// ==================================================

extern const char * DEV_STATUS_STR [ DEV_STATUS_MAX ];
extern char B_YEAR [ 5 ] , B_MONTH [ 3 ] , B_DAY [ 3 ] , B_HOUR [ 3 ] , B_MIN [ 3 ] , B_SEC [ 3 ];



// ==================================================
//     PUBLIC PROTOTYPES
// ==================================================

void
COMMON_InitBuildDefs ( void );

void
COMMON_ErrorHandler ( void );



// ==================================================
//     CPP GUARD [END]
// ==================================================

#ifdef __cplusplus
}
#endif



#endif /* __COMMON_H__ */
