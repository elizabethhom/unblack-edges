/*
 *     COMP40 - SPRING 2020
 *     HW2 - iii
 *     ELIZABETH HOM & THEO NUNEZ
 *     EHOM01 & TNUNEZ01
 *     10 FEB 2020
 * 
 *     unblackedges.c - Unblacks the edges of a supplied pbm.
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

typedef struct Coordinate {
    int col;
    int row;
} Coordinate;

Bit2_T pbmRead(FILE *inputfp);
void pbmWrite(FILE *outputfp, Bit2_T bit2);
void printValues(int col, int row, Bit2_T bit2, int b, void *p1);
void unblackLeftEdge(Bit2_T bit2);
void unblackTopEdge(Bit2_T bit2);
void unblackRightEdge(Bit2_T bit2);
void unblackBottomEdge(Bit2_T bit2);
void unblackAllEdges(Bit2_T bit2);
void findNeighbors(Bit2_T bit2, Stack_T stack, int col, int row);

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
 * NAME:   pbmRead
 * DOES:   copies the bits from the pbm file into a Bit2_T named
 *         bit2
 * INPUT:  a file pointer from the pbm file
 * OUTPUT: returns the created bit2 containing the values from the pbm file
 */
Bit2_T pbmRead(FILE *inputfp)
{
    Pnmrdr_T reader = Pnmrdr_new(inputfp);

    Pnmrdr_mapdata data = Pnmrdr_data(reader);

    assert(data.type == Pnmrdr_bit);
    assert(data.width != 0);
    assert(data.height != 0);

    Bit2_T bit2 = Bit2_new(data.width, data.height);

    for (int i = 0; i < Bit2_height(bit2); i++) {
        for (int j = 0; j < Bit2_width(bit2); j++) {
            Bit2_put(bit2, j, i, Pnmrdr_get(reader));
        }
    }

    Pnmrdr_free(&reader);

    return bit2;
}

/* 
 * NAME:   findNeighbors
 * DOES:   pushes neighboring bits that are black to the stack
 * INPUT:  a bit2, a stack structure, the col (int) and row (int) of the bit
 *         whose neighbors are to be found. It is a checked runtime error if an
 *         instance of a Coordinate struct created is NULL.
 * OUTPUT: returns nothing
 */
void findNeighbors(Bit2_T bit2, Stack_T stack, int col, int row)
{
    if (col - 1 >= 0 && Bit2_get(bit2, col - 1, row) == BLACK) {
        Coordinate *left = (Coordinate *)malloc(sizeof(Coordinate));
        assert(left != NULL);

        left->col = col - 1;
        left->row = row;

        Stack_push(stack, left);
    }

    if (col + 1 < Bit2_width(bit2) && Bit2_get(bit2, col + 1, row) == BLACK) {
        Coordinate *right = (Coordinate *)malloc(sizeof(Coordinate));
        assert(right != NULL);
        right->col = col + 1;
        right->row = row;

        Stack_push(stack, right);
    }

    if (row - 1 >= 0 && Bit2_get(bit2, col, row - 1) == BLACK) {
        Coordinate *up = (Coordinate *)malloc(sizeof(Coordinate));
        assert(up != NULL);
        up->col = col;
        up->row = row - 1;

        Stack_push(stack, up);
    }

    if (row + 1 < Bit2_height(bit2) && Bit2_get(bit2, col, row + 1) == BLACK) {
        Coordinate *down = (Coordinate *)malloc(sizeof(Coordinate));
        assert(down != NULL);
        down->col = col;
        down->row = row + 1;

        Stack_push(stack, down);
    }
}

/* 
 * NAME:   unblackAllEdges
 * DOES:   drives the unblacking of the pbm file's edges starting from the
 *         top, bottom, left, to the right edge.
 * INPUT:  the bit2
 * OUTPUT: returns nothing
 */
void unblackAllEdges(Bit2_T bit2)
{
    unblackTopEdge(bit2);
    unblackBottomEdge(bit2);
    unblackLeftEdge(bit2);
    unblackRightEdge(bit2);
}

/* 
 * NAME:   unblackTopEdge
 * DOES:   unblacks the black bits on the top edge and all their neighbors by
 *         performing a depth first search mechanic, and frees stack memory
 * INPUT:  bit2
 * OUTPUT: returns nothing
 */
void unblackTopEdge(Bit2_T bit2)
{
    int topEdgeIndex = 0;
    Stack_T neighbors = Stack_new();
    for (int i = 0; i < Bit2_width(bit2); i++) {

        /* if the current bit is black, change it to white and add its
         * neighbors that are black to the stack
         */
        if (Bit2_get(bit2, i, topEdgeIndex) == BLACK) {
            Bit2_put(bit2, i, topEdgeIndex, WHITE);
            findNeighbors(bit2, neighbors, i, topEdgeIndex);
        }

        /* while the stack is not empty, change each bit to white and add its
         * neighbors that are black to the stack
         */
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
 * NAME:   unblackBottomEdge
 * DOES:   unblacks the black bits on the bottom edge and all their neighbors
 *         by performing a depth first search mechanic, and frees stack memory
 * INPUT:  bit2
 * OUTPUT: returns nothing
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
 * NAME:   iterateLeftEdge
 * DOES:   unblacks the black bits on the left edge and all their neighbors
 *         by performing a depth first search mechanic, and frees stack memory
 * INPUT:  bit2
 * OUTPUT: returns nothing
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
 * NAME:   iterateRightEdge
 * DOES:   unblacks the black bits on the right edge and all their neighbors
 *         by performing a depth first search mechanic, and frees stack memory
 * INPUT:  bit2
 * OUTPUT: returns nothing
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
 * NAME:   pbmWrite
 * DOES:   writes the bit2 out to the given file pointer
 * INPUT:  a file pointer to be written out to and the bit2
 * OUTPUT: returns nothing
 */
void pbmWrite(FILE *outputfp, Bit2_T bit2)
{
    assert(bit2 != NULL);
    fprintf(outputfp, "P1\n");

    fprintf(outputfp, "%d %d\n", Bit2_width(bit2), Bit2_height(bit2));

    Bit2_map_row_major(bit2, printValues, NULL);
}

/* 
 * NAME:   printValues
 * DOES:   an apply function supplied to the Bit2_map_row_major function that
 *         prints the values in the bit2
 * INPUT:  the bit2, and the col (int) and row (int) to be printed
 * OUTPUT: returns nothing
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
