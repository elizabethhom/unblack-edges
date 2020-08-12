/*
 * bit2.h
 * 
 * COMP40 - SPRING2020
 * Elizabeth Hom (ehom01) & Theo Nunez (trnunez01)
 * Last Modified: February 10, 2020
 * 
 * Purpose: Interface for Bit2 module, which supports a 2-dimensional Bit
 *          vector of given width and height.
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
 *  Bit2_new()
 *
 *  Parameters: Width and height of Bit2_T instance
 *              *** It is a checked runtime error if the width or height are
 *                  negative. It is also a checked runtime error if the memory
 *                  for the Bit2 is allocated improperly.
 *  Does:       Creates a Bit2_T intance of the provided width and height
 *  Returns:    The created Bit2 of type Bit2_T
 */
extern T Bit2_new(int width, int height);

/*
 *  Bit2_width()
 *
 *  Parameters: Bit2_T instance
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Returns the width of the Bit2_T instance
 */
extern int Bit2_width(T bit2);

/*
 *  Bit2_height()
 *
 *  Parameters: Bit2_T instance
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Returns the height of the Bit2_T instance
 */
extern int Bit2_height(T bit2);

/*
 *  Bit2_put()
 *
 *  Parameters: Bit2_T instance, column and row of placement, value (int)
 *              to be placed
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Stores the value at the specified col and row
 *  Returns:    The previous value at the specified col and row
 */
extern int Bit2_put(T bit2, int col, int row, int value);

/*
 *  Bit2_get()
 *
 *  Parameters: Bit2_T instance, column and row
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Returns the value at a specified col and row in Bit2_T instance
 */
extern int Bit2_get(T bit2, int col, int row);

/*
 *  Bit2_map_row_major
 *
 *  Parameters: Bit2_T instance, client-supplied apply function, closure ptr
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Calls supplied apply function at each element in Bit2_T
 *              instance, where col indices vary more rapidly than row indices
 *  Returns:    NA
 */
extern void Bit2_map_row_major(T bit2, void apply(int i, int j, T bit2, int b,
                               void *p1), void *cl);

/*
 *  Bit2_map_col_major
 *
 *  Parameters: Bit2_T instance, client-supplied apply function, closure ptr
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Calls supplied apply function at each element in Bit2_T
 *              instance, where row indices vary more rapidly than col indices
 *  Returns:    NA
 */
extern void Bit2_map_col_major(T bit2, void apply(int i, int j, T bit2, int b,
                               void *p1), void *cl);

/*
 *  Bit2_free
 *
 *  Parameters: Pointer to Bit2_T instance
 *              *** It is a checked runtime error if the Bit2_T is NULL
 *  Does:       Frees associated memory of the Bit2_T structure
 *  Returns:    NA
 */
extern void Bit2_free(T *bit2);

#undef T
#endif
