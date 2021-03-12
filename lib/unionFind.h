#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <stdlib.h>

typedef struct union_t {
    size_t* array;
    size_t* size;
    size_t arraySize;
} union_t;

union_t* UF_init(const size_t size);
union_t* UF_destroy(union_t* unionStruct);
int UF_find(union_t* unionStruct, size_t index);
void UF_union(union_t* unionStruct, const size_t p, const size_t q);

#endif
