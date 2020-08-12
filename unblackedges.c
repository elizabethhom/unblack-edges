/*
 * unblackedges.c
 * 
 * COMP40 - SPRING2020
 * Elizabeth Hom (ehom01) & Theo Nunez (trnunez01)
 * Last Modified: February 10, 2020
 * 
 * Purpose: Unblacks the edges of a supplied PBM image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <table.h>
#include "bit2.h"
#include "pnmrdr.h"
#include "stack.h"

#define BLACK 1
#define WHITE 0

/*
 * Coordinate
 *
 * Describes a coordinate of an image bit by its column and row.
 */
typedef struct Coordinate {
    int col;
    int row;
} Coordinate;

Bit2_T pbmRead(FILE *inputfp);
void   pbmWrite(FILE *outputfp, Bit2_T bit2);
void   printValues(int col, int row, Bit2_T bit2, int b, void *p1);
void   unblackLeftEdge(Bit2_T bit2);
void   unblackTopEdge(Bit2_T bit2);
void   unblackRightEdge(Bit2_T bit2);
void   unblackBottomEdge(Bit2_T bit2);
void   unblackAllEdges(Bit2_T bit2);
void   findNeighbors(Bit2_T bit2, Stack_T stack, int col, int row);

int main(int argc, char *argv[])
{
    assert(argc <= 2);
    
    if (argc == 1) {
        Bit2_T mainBit2 = pbmRead(stdin);
        assert(mainBit2 != NULL);

        unblackAllEdges(mainBit2);
        pbmWrite(stdout, mainBit2);
        Bit2_free(&mainBit2);
    } else if (argc == 2) {
        FILE *fp = fopen(argv[1], "rb");
        assert(fp != NULL);
        Bit2_T mainBit2 = pbmRead(fp);
        assert(mainBit2 != NULL);

        unblackAllEdges(mainBit2);
        pbmWrite(stdout, mainBit2);

        Bit2_free(&mainBit2);
        fclose(fp);
    }
}

/*
 *  pbmRead()
 *
 *  Parameters: file pointer to PBM
 *  Does:       Copies bits from PBM image into Bit2_T bit2
 *  Returns:    Created bit2 populated with PBM file values
 */
Bit2_T pbmRead(FILE *inputfp)
{
    Pnmrdr_T reader = Pnmrdr_new(inputfp);
    Pnmrdr_mapdata data = Pnmrdr_data(reader);

    assert(data.type == Pnmrdr_bit);
    assert(data.width != 0);
    assert(data.height != 0);

    Bit2_T bit2 = Bit2_new(data.width, data.height);

    // Copies PBM into bit2, bit by bit
    for (int i = 0; i < Bit2_height(bit2); i++) {
        for (int j = 0; j < Bit2_width(bit2); j++) {
            Bit2_put(bit2, j, i, Pnmrdr_get(reader));
        }
    }
    Pnmrdr_free(&reader);

    return bit2;
}

/*
 *  findNeighbors()
 *
 *  Parameters: Bit2_T, Stack, current bit's col and row
 *              *** It is a checked runtime error if an instance of the
 *              Coordinate created is NULL.
 *  Does:       Pushes a bit's neighboring black bits to the Stack
 *  Returns:    NA
 */
void findNeighbors(Bit2_T bit2, Stack_T stack, int col, int row)
{
    /* Checks Northern bit. If black, populates Coordinate and pushes
     * onto Stack */
    if (col - 1 >= 0 && Bit2_get(bit2, col - 1, row) == BLACK) {
        Coordinate *left = (Coordinate *)malloc(sizeof(Coordinate));
        assert(left != NULL);

        left->col = col - 1;
        left->row = row;

        Stack_push(stack, left);
    }

    // Checks Southern bit
    if (col + 1 < Bit2_width(bit2) && Bit2_get(bit2, col + 1, row) == BLACK) {
        Coordinate *right = (Coordinate *)malloc(sizeof(Coordinate));
        assert(right != NULL);
        right->col = col + 1;
        right->row = row;

        Stack_push(stack, right);
    }

    // Checks Western bit
    if (row - 1 >= 0 && Bit2_get(bit2, col, row - 1) == BLACK) {
        Coordinate *up = (Coordinate *)malloc(sizeof(Coordinate));
        assert(up != NULL);
        up->col = col;
        up->row = row - 1;

        Stack_push(stack, up);
    }

    // Checks Eastern bit
    if (row + 1 < Bit2_height(bit2) && Bit2_get(bit2, col, row + 1) == BLACK) {
        Coordinate *down = (Coordinate *)malloc(sizeof(Coordinate));
        assert(down != NULL);
        down->col = col;
        down->row = row + 1;

        Stack_push(stack, down);
    }
}

/*
 *  unblackAllEdges()
 *
 *  Parameters: Bit2_T
 *  Does:       Drives unblacking of PBM image's edges, starting from the
 *              top, bottom, left, to right edge
 *  Returns:    NA
 */
void unblackAllEdges(Bit2_T bit2)
{
    unblackTopEdge(bit2);
    unblackBottomEdge(bit2);
    unblackLeftEdge(bit2);
    unblackRightEdge(bit2);
}

/*
 *  unblackTopEdge()
 *
 *  Parameters: Bit2_T
 *  Does:       Unblacks black bits along top edge and neighbors by performing
 *              a DFS algorithm. Frees associated stack memory.
 *  Returns:    NA
 */
void unblackTopEdge(Bit2_T bit2)
{
    int topEdgeIndex = 0;
    Stack_T neighbors = Stack_new();
    
    for (int i = 0; i < Bit2_width(bit2); i++) {
        
        /* If the current bit is black, change to white and add black neighbors
         * to the stack */
        if (Bit2_get(bit2, i, topEdgeIndex) == BLACK) {
            Bit2_put(bit2, i, topEdgeIndex, WHITE);
            findNeighbors(bit2, neighbors, i, topEdgeIndex);
        }

        /* While the stack is not empty, change each bit to white and add its
         * black neighbors to the stack */
        while (Stack_empty(neighbors) != 1) {
            Coordinate *topElem = Stack_pop(neighbors);
            Bit2_put(bit2, topElem->col, topElem->row, WHITE);
            findNeighbors(bit2, neighbors, topElem->col, topElem->row);

            free(topElem);
        }
    }

    Stack_free(&neighbors);
}

/*
 *  unblackBottomEdge()
 *
 *  Parameters: Bit2_T
 *  Does:       Unblacks black bits along bottom edge and neighbors by performing
 *              a DFS algorithm. Frees associated stack memory.
 *  Returns:    NA
 */
void unblackBottomEdge(Bit2_T bit2)
{
    int bottomEdgeIndex = Bit2_height(bit2) - 1;
    Stack_T neighbors = Stack_new();
    
    for (int i = 0; i < Bit2_width(bit2); i++) {
        if (Bit2_get(bit2, i, bottomEdgeIndex) == BLACK) {
            Bit2_put(bit2, i, bottomEdgeIndex, WHITE);
            findNeighbors(bit2, neighbors, i, bottomEdgeIndex);
        }

        while (Stack_empty(neighbors) != 1) {
            Coordinate *topElem = Stack_pop(neighbors);
            Bit2_put(bit2, topElem->col, topElem->row, WHITE);
            findNeighbors(bit2, neighbors, topElem->col, topElem->row);

            free(topElem);
        }
    }

    Stack_free(&neighbors);
}

/*
 *  unblackLeftEdge()
 *
 *  Parameters: Bit2_T
 *  Does:       Unblacks black bits along left edge and neighbors by performing
 *              a DFS algorithm. Frees associated stack memory.
 *  Returns:    NA
 */
void unblackLeftEdge(Bit2_T bit2)
{
    int leftEdgeIndex = 0;
    Stack_T neighbors = Stack_new();
    
    for (int i = 0; i < Bit2_height(bit2); i++) {
        if (Bit2_get(bit2, leftEdgeIndex, i) == BLACK) {
            Bit2_put(bit2, leftEdgeIndex, i, WHITE);
            findNeighbors(bit2, neighbors, leftEdgeIndex, i);
        }

        while (Stack_empty(neighbors) != 1) {
            Coordinate *topElem = Stack_pop(neighbors);
            Bit2_put(bit2, topElem->col, topElem->row, WHITE);
            findNeighbors(bit2, neighbors, topElem->col, topElem->row);

            free(topElem);
        }
    }

    Stack_free(&neighbors);
}

/*
 *  unblackRightEdge()
 *
 *  Parameters: Bit2_T
 *  Does:       Unblacks black bits along right edge and neighbors by performing
 *              a DFS algorithm. Frees associated stack memory.
 *  Returns:    NA
 */
void unblackRightEdge(Bit2_T bit2)
{
    int rightEdgeIndex = Bit2_width(bit2) - 1;
    Stack_T neighbors = Stack_new();
    
    for (int i = 0; i < Bit2_height(bit2); i++) {
        if (Bit2_get(bit2, rightEdgeIndex, i) == BLACK) {
            Bit2_put(bit2, rightEdgeIndex, i, WHITE);
            findNeighbors(bit2, neighbors, rightEdgeIndex, i);
        }

        while (Stack_empty(neighbors) != 1) {
            Coordinate *topElem = Stack_pop(neighbors);
            Bit2_put(bit2, topElem->col, topElem->row, WHITE);
            findNeighbors(bit2, neighbors, topElem->col, topElem->row);

            free(topElem);
        }
    }

    Stack_free(&neighbors);
}

/*
 *  pbmWrite()
 *
 *  Parameters: output file pointer, Bit2_T
 *  Does:       Writes processed Bit2_T to given file pointer
 *  Returns:    NA
 */
void pbmWrite(FILE *outputfp, Bit2_T bit2)
{
    assert(bit2 != NULL);
    fprintf(outputfp, "P1\n");

    fprintf(outputfp, "%d %d\n", Bit2_width(bit2), Bit2_height(bit2));

    Bit2_map_row_major(bit2, printValues, NULL);
}

/*
 *  printValues()
 *
 *  Parameters: Bit2_T, column and row to be printed
 *  Does:       An apply function supplied to Bit2_map_row_major that prints
 *              the values in the Bit2_T instance
 *  Returns:    NA
 */
void printValues(int col, int row, Bit2_T bit2, int b, void *p1)
{
    (void) b;
    (void) p1;

    assert(bit2 != NULL);
    printf("%d", Bit2_get(bit2, col, row));

    if (col == Bit2_width(bit2) - 1 && row != Bit2_height(bit2) - 1) {
        printf("\n");
    }
}
