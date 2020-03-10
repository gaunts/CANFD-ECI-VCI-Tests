/*
 * The MIT License
 *
 * Copyright (c) 2019 Luis CRUZ
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__



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



// ==================================================
//     TYPE DEFINITIONS
// ==================================================

typedef enum {

    LINKED_LIST_RESULT_SUCCESS = 0                ,
    LINKED_LIST_RESULT_ERROR_NULL_PARAM           ,
    LINKED_LIST_RESULT_ERROR_NODE_ALREADY_IN_LIST ,
    LINKED_LIST_RESULT_ERROR_NODE_NOT_IN_LIST     ,
    LINKED_LIST_RESULT_ERROR_MEMORY_ALLOCATION    ,
    LINKED_LIST_RESULT_ERROR_EMPTY_LIST           ,

} LINKED_LIST_RESULT_t;

typedef struct node {

    struct node * next;
    void        * data;

} NODE_t;

typedef struct list {

    NODE_t * head;

} LIST_t;



// ==================================================
//     PUBLIC PROTOTYPES
// ==================================================

LIST_t *
LINKED_LIST_CreateEmptyList ( void );

LINKED_LIST_RESULT_t
LINKED_LIST_AddNode ( LIST_t * list ,
                      void   * data );

LINKED_LIST_RESULT_t
LINKED_LIST_DeleteNode ( LIST_t * list ,
                         void   * data );

NODE_t *
LINKED_LIST_FindNode ( LIST_t * list ,
                       void   * data );

LINKED_LIST_RESULT_t
LINKED_LIST_DisplayList ( LIST_t * list );

LINKED_LIST_RESULT_t
LINKED_LIST_ReverseList ( LIST_t * list );

LINKED_LIST_RESULT_t
LINKED_LIST_DestroyList ( LIST_t * list );



// ==================================================
//     CPP GUARD [END]
// ==================================================

#ifdef __cplusplus
}
#endif



#endif /* __LINKED_LIST_H__ */
