/**
    * This library is for correcly handling the data structure defined for this project.
**/

/**
    * The data structures decided on will be a lower triangular matrix without diagonal, where `matrix[i][j]` is the distance from `i` to `j`.
    * This decision was made taking these things in consideration:
    * The output needs to be sorted but the input isn't. So it would have to be sorted afterwards. Not using a inherently-sorted structure like a tree allows to have much faster (O(1))) access and insertion cost, while keeping effectively the same amount of data on memory. By using a quasi-linear (O(n log n)) sorting algorithm like quickSort, time could be gained since sorting is an operation that will be done once.
    * The distance calculation will be Euclidian, so it's symmetric: `matrix[i][j] == matrix[j][i]`, henceforth there's no need to store one of them, saving almost half memory.
    * REVIEW -
    * The diagonal will not be stored also, because `matrix[i][i] == 0`. This would take a lot of time from accesses, but by shorting the conditionals checking for `i < j`, we could lose not much time and still save `N * sizeof(long float)` bytes on the memory.
**/

#ifndef _DATA_H_
#define _DATA_H_

#include "./fileReader.h"

typedef struct data_t {
    // [0] is char* Identifier, [1...] is long double* feature;
    void*** dataMatrix;
    size_t i, j;
} data_t;


data_t* loadData(FILE* file, const char* separator);
void printData(const data_t* dataStruct);
void destroyData(data_t* data);


#endif
