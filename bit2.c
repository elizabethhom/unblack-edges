/*
 *     COMP40 - SPRING 2020
 *     HW2 - iii
 *     ELIZABETH HOM & THEO NUNEZ
 *     EHOM01 & TNUNEZ01
 *     10 FEB 2020
 * 
 *     bit2.c - implementation for Bit2, which is represented as a
 *     1-dimensional bit vector that has a length of width * height.
 */


#include "bit2.h"

#define T Bit2_T

struct T {
    Bit_T bitVector;
    int width;
    int height;
    int vectorLength;
};

/* 
 * NAME:   Bit2_new
 * DOES:   Creates and allocates space for a Bit2 with the given height and
 *         width
 * INPUT:  the bit2's height (int) and width (int)
 * OUTPUT: returns the created Bit22 of type Bit2_T
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
 * NAME:   Bit2_width
 * DOES:   gets the width (number of columns) of the Bit2
 * INPUT:  the Bit2
 * OUTPUT: the width (int)
 */
extern int Bit2_width(T bit2)
{
    assert(bit2 != NULL);
    return bit2->width;
}

/* 
 * NAME:   Bit2_height
 * DOES:   gets the height (number of rows) of the Bit2
 * INPUT:  the Bit2
 * OUTPUT: the height (int)
 */
extern int Bit2_height(T bit2)
{
    assert(bit2 != NULL);
    return bit2->height;
}

/*
 * NAME:   Bit2_put
 * DOES:   puts the value at the specified col and row, returns the value that
 *         was previously there before insertion
 * INPUT:  the Bit2, and the desired row (int) and column (int) to insert at
 * OUTPUT: returns the previous value at the specified row and col
 */
extern int Bit2_put(T bit2, int col, int row, int value)
{
    assert(bit2 != NULL);

    int bitIndex = (bit2->width * row) + col;
    return Bit_put(bit2->bitVector, bitIndex, value);
}

/*
 * NAME:   Bit2_get
 * DOES:   retrieves the value at the specified col and row
 * INPUT:  the Bit2, and the desired row (int) and column (int) to retrieve
 *         from.
 * OUTPUT: returns the value at the specified row and col
 */
extern int Bit2_get(T bit2, int col, int row)
{
    assert(bit2 != NULL);

    int bitIndex = (bit2->width * row) + col;
    return Bit_get(bit2->bitVector, bitIndex);
}

/* 
 * NAME:   Bit22_map_row_major
 * DOES:   calls the supplied apply function for each element in the bit vector
 *         where column indices vary more rapidly than row indices
 * INPUT:  the Bit2, an apply function, a closure pointer
 * OUTPUT: returns nothing
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
 * NAME:   UArray2_map_col_major
 * DOES:   calls the supplied apply function for each element in the bit vector
 *         where row indices vary more rapidly than col indices
 * INPUT:  the Bit2, an apply function, a closure pointer
 * OUTPUT: returns nothing
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
 * NAME:   Bit2_free
 * DOES:   frees the associated memory of the Bit2 structure
 * INPUT:  a pointer to the Bit2
 * OUTPUT: returns nothing
 */
extern void Bit2_free(T *bit2)
{
    assert(bit2 != NULL);
    assert(*bit2 != NULL);

    /* must free Bit vector within Bit2 struct before freeing the Bit2 */
    Bit_free(&((*bit2)->bitVector));
    free(*bit2);
}

#undef T
