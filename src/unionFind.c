#include "../lib/unionFind.h"

unionCell_t* UF_init(const size_t size, distanceSample_t* samples) {
    unionCell_t* newStruct = (unionCell_t*)malloc(sizeof(unionCell_t) * size);
    if (newStruct == NULL) {
        perror("Error allocating unionFind struct. Exiting");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < size; i++) {
        newStruct[i].root = NULL;
        newStruct[i].size = 1;
        newStruct[i].id = i;
        newStruct[i].sample = &samples[i];
    }

    return newStruct;
}

void UF_destroy(unionCell_t* unionStruct) {
    free(unionStruct);
    unionStruct = NULL;
}

// Return the ancestor
unionCell_t* UF_find(unionCell_t* unionCell) {
    while (unionCell->root != NULL) {
        // unionStruct[index].root = unionStruct[unionStruct[index].root].root;
        unionCell = unionCell->root;
    }
    return unionCell;
}

// Joins 2 elements
char UF_union(unionCell_t*  p, unionCell_t* q) {
    p = UF_find(p);
    q = UF_find(q);
    if (p == q) return 0;
    if (p->size < q->size) {
        p->root = q;
        q->size += p->size;
    } else {
        q->root = p;
        p->size += q->size;
    }
    return 1;
}
