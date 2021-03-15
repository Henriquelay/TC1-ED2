#include "../lib/unionFind.h"

union_t* UF_init(const size_t size, distanceSample_t* samples) {
    union_t* newStruct = (union_t*)malloc(sizeof(union_t));
    if (newStruct == NULL) {
        perror("Error allocating unionFind struct. Exiting");
        exit(1);
    }

    newStruct->array = (size_t*)malloc(sizeof(size_t) * size);
    if (newStruct->array == NULL) {
        perror("Error allocating unionFind array. Exiting");
        exit(1);
    }
    newStruct->size = (size_t*)malloc(sizeof(size_t) * size);
    if (newStruct->size == NULL) {
        perror("Error allocating unionFind ranks array. Exiting");
        exit(1);
    }

    for (size_t i = 0; i < size; i++) {
        newStruct->array[i] = i;
        newStruct->size[i] = 1;
        newStruct->sample = &samples[i];
    }

    return newStruct;
}

void UF_destroy(union_t* unionStruct) {
    free(unionStruct->array);
    free(unionStruct->size);
    free(unionStruct);
    unionStruct = NULL;
}

// Return the ancestor
size_t UF_find(union_t* unionStruct, size_t index) {
    while (unionStruct->array[index] != index) {
        // unionStruct->array[index] = unionStruct->array[unionStruct->array[index]];
        index = unionStruct->array[index];
    }
    return index;
}

// Joins 2 elements
char UF_union(union_t* unionStruct, size_t p, size_t q) {
    p = UF_find(unionStruct, p);
    q = UF_find(unionStruct, q);
    if (p == q) return 0;
    if (p < q) {
        unionStruct->array[p] = q;
        unionStruct->size[q] += unionStruct->size[p];
    } else {
        unionStruct->array[q] = p;
        unionStruct->size[p] += unionStruct->size[q];
    }
    return 1;
}
