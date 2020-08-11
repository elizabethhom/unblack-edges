/*
 *     COMP40 - SPRING 2020
 *     HW2 - iii
 *     ELIZABETH HOM & THEO NUNEZ
 *     EHOM01 & TNUNEZ01
 *     10 FEB 2020
 * 
 *     bit2.h - interface for Bit2, which supports a 2-dimensional Bit vector
 *     that has a width and height.
 */

#ifndef BIT2_H_
#define BIT2_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "uarray.h"
#include "bit.h"
#include "pnmrdr.h"

#define T Bit2_T
typedef struct T *T;

/* 
 * NAME:   Bit2_new
 * DOES:   Creates a Bit2 with the given height and width
 * INPUT:  the width (int) and height (int) of the Bit2
 *         It is a checked runtime error if the width or height is negative. It
 *         is also a checked runtime error if the memory for the Bit2 is
 *         allocated improperly.
 * OUTPUT: returns the created Bit2 of type Bit2_T
 */
extern T Bit2_new(int width, int height);

/* 
 * NAME:   Bit2_width
 * DOES:   returns the width of the Bit2
 * INPUT:  the bit2
 *         It is a checked runtime error if the Bit2 is NULL.
 */
extern int Bit2_width(T bit2);

/* 
 * NAME:   Bit2_height
 * DOES:   returns the height of the Bit2
 * INPUT:  the bit2
 */
extern int Bit2_height(T bit2);

/* 
 * NAME:   Bit2_put
 * DOES:   stores the given value at a specified col and row
 * INPUT:  the bit2, col (int), row (int), and value (int)
 *         It is a checked runtime error if the Bit2 is NULL.
 * OUTPUT: returns the previous value at the specified col and row
 */
extern int Bit2_put(T bit2, int col, int row, int value);

/* 
 * NAME:   Bit2_get
 * DOES:   retrieves and returns the value at a specified col and row in a bit2
 * INPUT:  the bit2, col (int), row (int)
 *         It is a checked runtime error if the Bit2 is NULL.
 */
extern int Bit2_get(T bit2, int col, int row);

/* 
 * NAME:   Bit2_map_row_major
 * DOES:   calls the supplied apply function for each element in the Bit2,
 *         where the col indices vary more rapidly than row indices
 * INPUT:  the bit2, a client-supplied apply function, a closure pointer
 *         It is a checked runtime error if the Bit2 is NULL.
 * OUTPUT: returns nothing
 */
extern void Bit2_map_row_major(T bit2, void apply(int i, int j, T bit2, int b,
                               void *p1), void *cl);

/* 
 * NAME:   Bit2_map_col_major
 * DOES:   calls the supplied apply function for each element in the Bit2,
 *         where the col indices vary more rapidly than row indices
 * INPUT:  the bit2, a client-supplied apply function, a closure pointer
 *         It is a checked runtime error if the Bit2 is NULL.
 * OUTPUT: returns nothing
 */
extern void Bit2_map_col_major(T bit2, void apply(int i, int j, T bit2, int b,
                               void *p1), void *cl);

/* 
 * NAME:   Bit2_free
 * DOES:   frees the associated memory of the Bit2 structure
 * INPUT:  a pointer to the bit2
 *         It is a checked runtime error if the Bit2 is NULL. It is also a
 *         checked runtime error if the pointer to the Bit2 that is passed
 *         in to the function is NULL.
 * OUTPUT: returns nothing
 */
extern void Bit2_free(T *bit2);

#undef T
#endif
