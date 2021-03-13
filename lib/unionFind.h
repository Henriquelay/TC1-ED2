#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct union_t {
    size_t* array;
    // Array of sizes to calculate weight and balance the insertion
    size_t* size;
    // Number of elements on array
    size_t arraySize;
} union_t;

union_t* UF_init(const size_t size);
union_t* UF_destroy(union_t* unionStruct);
size_t UF_find(union_t* unionStruct, size_t index);
void UF_union(union_t* unionStruct, const size_t p, const size_t q);

#endif
