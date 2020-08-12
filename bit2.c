/*
 * bit2.c
 * 
 * COMP40 - SPRING2020
 * Elizabeth Hom (ehom01) & Theo Nunez (trnunez01)
 * Last Modified: February 10, 2020
 * 
 * Purpose: Implementation for Bit2 module, which is represented by a
 *          one-dimensional bit vector that has a length of width * height.
 */

#include "bit2.h"
#define T Bit2_T

/*
 * T
 *
 * Describes a Bit2_T instance, which contains the one-dimensional Bit
 * vector, the width and height of the Bit2_T, and lenght of the Bit vector
 */
struct T {
    Bit_T bitVector;
    int width;
    int height;
    int vectorLength;
};

/*
 *  Bit2_new()
 *
 *  Parameters: Bit2_T instance's height and width
 *  Does:       Creates and allocates memory for a Bit2_T instance with the
 *              given height and width dimensions
 *  Returns:    Created Bit2_T instance
 */
extern T Bit2_new(int width, int height)
{
    Bit2_T mainVector = (Bit2_T)malloc(sizeof(*mainVector));
    assert(mainVector != NULL);

    mainVector->width = width;
    mainVector->height = height;
    mainVector->vectorLength = width * height;

    mainVector->bitVector = Bit_new(mainVector->vectorLength);

    return mainVector;
}

/*
 *  Bit2_width()
 *
 *  Parameters: Bit2_T instance
 *  Does:       Obtains the width (number of columns) of Bit2_T instance
 *  Returns:    Bit2_T width
 */
extern int Bit2_width(T bit2)
{
    assert(bit2 != NULL);
    return bit2->width;
}

/*
 *  Bit2_height()
 *
 *  Parameters: Bit2_T instance
 *  Does:       Obtains the height (number of rows) of Bit2_T instance
 *  Returns:    Bit2_T height
 */
extern int Bit2_height(T bit2)
{
    assert(bit2 != NULL);
    return bit2->height;
}

/*
 *  Bit2_put()
 *
 *  Parameters: Bit2_T instance, column and row of placement, value being
 *              placed
 *  Does:       Puts value at specified col and row
 *  Returns:    Returns value previously at the col and row intersection before
 *              insertion
 */
extern int Bit2_put(T bit2, int col, int row, int value)
{
    assert(bit2 != NULL);

    int bitIndex = (bit2->width * row) + col;
    return Bit_put(bit2->bitVector, bitIndex, value);
}

/*
 *  Bit2_get()
 *
 *  Parameters: Bit2_T instance, desired row and col to retrieve from
 *  Does:       Retrieves the value stored at the specified col and row
 *  Returns:    Value at the specified col and row
 */
extern int Bit2_get(T bit2, int col, int row)
{
    assert(bit2 != NULL);

    int bitIndex = (bit2->width * row) + col;
    return Bit_get(bit2->bitVector, bitIndex);
}

/*
 *  Bit2_map_row_major()
 *
 *  Parameters: Bit2_T instance, an apply function, closure ptr
 *  Does:       Calls supplied apply function for each element in Bit vector,
 *              where col indices vary more rapidly than row indices
 *  Returns:    NA
 */
extern void Bit2_map_row_major(T bit2, void apply(int i, int j, T bit2, int b,
                               void *p1), void *cl)
{
    assert(bit2 != NULL);
    int bitIndex = 0;

    for (int j = 0; j < bit2->height; j++) {
        for (int i = 0; i < bit2->width; i++) {
            assert(bitIndex >= 0);
            assert(bitIndex < bit2->vectorLength);
            
            apply(i, j, bit2, Bit_get(bit2->bitVector, bitIndex), cl);
            bitIndex++;
        }
    }
}

/*
 *  Bit2_map_col_major()
 *
 *  Parameters: Bit2_T instance, an apply function, closure ptr
 *  Does:       Calls supplied apply function for each element in Bit vector,
 *              where row indices vary more rapidly than col indices
 *  Returns:    NA
 */
extern void Bit2_map_col_major(T bit2, void apply(int i, int j, T bit2, int b,
                               void *p1), void *cl)
{
    assert(bit2 != NULL);
    int bitIndex = 0;

    for (int i = 0; i < bit2->width; i++) {
        for (int j = 0; j < bit2->height; j++) {
            bitIndex = (bit2->width * j) + i;
            assert(bitIndex >= 0);
            assert(bitIndex < bit2->vectorLength);
            
            apply(i, j, bit2, Bit_get(bit2->bitVector, bitIndex), cl);
        }
    }
}

/*
 *  Bit2_freer()
 *
 *  Parameters: Pointer to Bit2_T
 *  Does:       Frees associated memory of the Bit2_T structure
 *  Returns:    NA
 */
extern void Bit2_free(T *bit2)
{
    assert(bit2 != NULL);
    assert(*bit2 != NULL);

    /* Must free Bit vector within Bit2_T struct before freeing the Bit2_T */
    Bit_free(&((*bit2)->bitVector));
    free(*bit2);
}

#undef T
