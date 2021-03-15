#include "../lib/graph.h"

int compareDistanceSamples(const void* a, const void* b) {
    if (((distanceSample_t*)a)->distance < ((distanceSample_t*)b)->distance) return -1;
    if (((distanceSample_t*)a)->distance > ((distanceSample_t*)b)->distance) return 1;
    return 0;
}

union_t* kruskal(distanceDataSet_t* dataSet, size_t groupsNumber) {
    // Qsorting dataSet, Kruskal needs a sorted set
    qsort(dataSet->samples, dataSet->nElements, sizeof(distanceSample_t), &compareDistanceSamples);


    union_t* un = UF_init(dataSet->depth, dataSet->samples);
    size_t currentGroups = dataSet->depth;

    // Not executing K times, don't even need to remove later
    // Sice it's sorted, it's on the 3 largest distances
    for (size_t i = 0; currentGroups != groupsNumber; i++) {
        size_t p = dataSet->samples[i].from->index, q = dataSet->samples[i].to->index;
        if (UF_union(un, p, q) == 1) {
            currentGroups--;
        }
    }

    // puts("Final");
    // for (size_t i = 0; i < dataSet->depth; i++) {
    //     printf("%ld ", i);
    // }
    // puts("");
    // for (size_t i = 0; i < dataSet->depth; i++) {
    //     printf("%ld ", UF_find(un, i));
    // }
    // puts("");
    // for (size_t i = 0; i < dataSet->depth; i++) {
    //     printf("%s ", un->sample[i].from->id);
    // }
    // puts("");

    return un;
}

sample_t** sortMST(union_t* MST) {
    return NULL
}
