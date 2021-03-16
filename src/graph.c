#include "../lib/graph.h"


int compareUFRoot(const void* a, const void* b) {
    // printf("On %s--%s X %s--%s", ((unionCell_t*)a)->sample->from->id, ((unionCell_t*)a)->sample->to->id, ((unionCell_t*)b)->sample->from->id, ((unionCell_t*)b)->sample->to->id);
    size_t idA = UF_find((unionCell_t*)a)->id;
    size_t idB = UF_find((unionCell_t*)b)->id;
    int cmp = idA - idB;
    // printf(". Comparing %ld and %ld: %d\n", idA, idB, cmp);
    if (cmp != 0) return cmp;

    cmp = strcmp(((unionCell_t*)a)->sample->from->id, ((unionCell_t*)b)->sample->from->id);
    // printf("Draw. Comparing ids %s and %s: %d\n", ((unionCell_t*)a)->sample->from->id, ((unionCell_t*)b)->sample->from->id, cmp);
    return cmp;
}

unionCell_t* sortMST(unionCell_t* MST, size_t* inputSize, size_t* K) {
    size_t nEdges = *inputSize - *K + 1;
    printf("nEdges: %ld\n", nEdges);

    // Creating new array with different sizing (qsort won't stop on nEdges and will sort whole array)
    unionCell_t* finalMST = malloc(sizeof(unionCell_t) * nEdges);
    if (finalMST == NULL) {
        perror("Error allocating new MST array. Exiting");
        exit(1);
    }
    for (size_t i = 0; i < nEdges; i++) {
        finalMST[i] = MST[i];
    }


    qsort(finalMST, nEdges, sizeof(unionCell_t), &compareUFRoot);
    // Printing
    puts("Sorted");
    for (size_t i = 0; i < nEdges; i++) {
        printf("[%ld] %s--%s dist: %Lf cluster: %ld\n", i, finalMST[i].sample->from->id, finalMST[i].sample->to->id, finalMST[i].sample->distance, UF_find(&finalMST[i])->id);
    }
    puts("");

    return finalMST;
}


int compareDistanceSamples(const void* a, const void* b) {
    if (((distanceSample_t*)a)->distance < ((distanceSample_t*)b)->distance) return -1;
    if (((distanceSample_t*)a)->distance > ((distanceSample_t*)b)->distance) return 1;
    return 0;
}

unionCell_t* MST_kruskal(distanceDataSet_t* distanceDataSet, size_t* K) {
    // Qsorting dataSet, Kruskal needs a sorted set
    qsort(distanceDataSet->samples, distanceDataSet->nElements, sizeof(distanceSample_t), &compareDistanceSamples);

    size_t nEdges = distanceDataSet->depth - *K + 1;

    // 1 group by vertex
    size_t currentGroups = distanceDataSet->depth;
    unionCell_t* un = UF_init(distanceDataSet->nElements, distanceDataSet->samples);

    puts("Before");
    for (size_t i = 0; currentGroups != *K; i++) {
        printf("[%ld] %s--%s dist: %Lf cluster: %ld\n", i, un[i].sample->from->id, un[i].sample->to->id, un[i].sample->distance, UF_find(&un[i])->id);
    }

    puts("MST:");
    // Not executing K times, don't even need to remove later
    // Sice it's sorted, it's on the 3 largest distances
    for (size_t i = 0; currentGroups != (*K); i++) {
        size_t p = un[i].sample->from->index, q = un[i].sample->to->index;
        printf("%ld and %ld  \t| ancestors: %ld and %ld\n", p, q, UF_find(&un[p])->id, UF_find(&un[q])->id);
        if (UF_union(&un[p], &un[q]) == 1) {
            printf("Uniting %ld and %ld. Cluster = %ld\n", p, q, UF_find(&un[p])->id);
            currentGroups--;
        }
    }

    puts("Final");
    for (size_t i = 0; i < nEdges; i++) {
        printf("[%ld] %s--%s dist: %Lf cluster: %ld\n", i, un[i].sample->from->id, un[i].sample->to->id, un[i].sample->distance, UF_find(&un[i])->id);
    }

    unionCell_t* MST = sortMST(un, &distanceDataSet->depth, K);
    UF_destroy(un);
    return MST;
}
