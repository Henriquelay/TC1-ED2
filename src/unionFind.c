#include "../lib/unionFind.h"

union_t* UF_init(const size_t size) {
    union_t* newStruct = (union_t*)malloc(sizeof(union_t));
    if (newStruct == NULL) {
        perror("Error allocating unionFind struct. Exiting");
        exit(1);
    }

    newStruct->size = size;
    newStruct->array = (size_t*)malloc(sizeof(size_t) * newStruct->size);

    for (size_t i = 0;i < size;i++) {
        newStruct->array[i] = i;
    }

    return newStruct;
}

union_t* UF_destroy(union_t* unionStruct) {
    free(unionStruct->array);
    free(unionStruct);
    return NULL;
}

int UF_find(union_t* unionStruct, size_t index) {
    while (unionStruct->array[index] != index) index = unionStruct->array[index];
    return index;
}

void UF_union(union_t* unionStruct, const size_t p, const size_t q) {
    size_t i = UF_find(unionStruct, p);
    size_t j = UF_find(unionStruct, q);
    unionStruct->array[i] = j;
}
