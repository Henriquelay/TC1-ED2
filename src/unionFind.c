#include "../lib/unionFind.h"

union_t* UF_init(const size_t size, void** elemArray) {
    union_t* newStruct = (union_t*)malloc(sizeof(union_t));
    if (newStruct == NULL) {
        perror("Error allocating unionFind struct. Exiting");
        exit(1);
    }

    newStruct->arraySize = size;
    newStruct->array = (size_t*)malloc(sizeof(size_t) * newStruct->arraySize);
    if (newStruct->array == NULL) {
        perror("Error allocating unionFind array. Exiting");
        exit(1);
    }
    newStruct->size = (size_t*)malloc(sizeof(size_t) * newStruct->arraySize);
    if (newStruct->size == NULL) {
        perror("Error allocating unionFind ranks array. Exiting");
        exit(1);
    }

    for (size_t i = 0;i < size;i++) {
        newStruct->array[i] = i;
        newStruct->size[i] = 1;
        newStruct->elem = elemArray[i];
    }

    return newStruct;
}

union_t* UF_destroy(union_t* unionStruct) {
    free(unionStruct->array);
    free(unionStruct);
    return NULL;
}

// Return who is the ancestor
size_t UF_find(union_t* unionStruct, size_t index) {
    while (unionStruct->array[index] != index) {
        unionStruct->array[index] = unionStruct->array[unionStruct->array[index]];
        index = unionStruct->array[index];
    }
    return index;
}

// Joins 2 elements
void UF_union(union_t* unionStruct, const size_t p, const size_t q) {
    size_t i = UF_find(unionStruct, p);
    size_t j = UF_find(unionStruct, q);
    if (i == j) return;
    if (i < j) {
        unionStruct->array[i] = j;
        unionStruct->size[j] += unionStruct->size[i];
    } else {
        unionStruct->array[j] = i;
        unionStruct->size[i] += unionStruct->size[j];
    }
}
