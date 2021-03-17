#include "../lib/graph.h"


int compareUFRoot(const void* a, const void* b, void* dataSet) {
    // printf("On %s--%s X %s--%s", ((unionCell_t*)a)->sample->from->id, ((unionCell_t*)a)->sample->to->id, ((unionCell_t*)b)->sample->from->id, ((unionCell_t*)b)->sample->to->id);
    size_t idA = UF_find((unionCell_t*)a)->id;
    size_t idB = UF_find((unionCell_t*)b)->id;
    int cmp = idA - idB;
    // printf(". Comparing %ld and %ld: %d\n", idA, idB, cmp);
    if (cmp != 0) return cmp;

    cmp = strcmp(((dataSet_t*)dataSet)->samples[((unionCell_t*)a)->id].id, ((dataSet_t*)dataSet)->samples[((unionCell_t*)b)->id].id);
    // printf("Draw. Comparing ids %s and %s: %d\n", ((unionCell_t*)a)->sample->from->id, ((unionCell_t*)b)->sample->from->id, cmp);
    return cmp;
}

unionCell_t* sortMST(unionCell_t* MST, size_t* inputSize, dataSet_t* dataSet) {
    // Creating new array with different sizing (qsort won't stop on nEdges and will sort whole array)
    unionCell_t* finalMST = malloc(sizeof(unionCell_t) * *inputSize);
    if (finalMST == NULL) {
        perror("Error allocating new MST array. Exiting");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < *inputSize; i++) {
        finalMST[i] = MST[i];
    }


    qsort_r(finalMST, *inputSize, sizeof(unionCell_t), &compareUFRoot, dataSet);
    // puts("Sorted:");
    // for (size_t i = 0; i < *inputSize; i++) {
    //     printf("[%ld] %s--%s\tdist: %Lf\tcluster: %ld\n", i, finalMST[i].sample->from->id, finalMST[i].sample->to->id, finalMST[i].sample->distance, UF_find(&finalMST[i])->id);
    // }

    return finalMST;
}


int compareDistanceSamples(const void* a, const void* b) {
    if (((distanceSample_t*)a)->distance < ((distanceSample_t*)b)->distance) return -1;
    if (((distanceSample_t*)a)->distance > ((distanceSample_t*)b)->distance) return 1;
    return 0;
}

unionCell_t* MST_kruskal(distanceDataSet_t* distanceDataSet, size_t* K, unionCell_t** samples, dataSet_t* dataSet) {
    // Qsorting dataSet, Kruskal needs a sorted set
    qsort(distanceDataSet->samples, distanceDataSet->nElements, sizeof(distanceSample_t), &compareDistanceSamples);

    // 1 group per vertex
    size_t currentGroups = distanceDataSet->depth;
    unionCell_t* un = UF_init(distanceDataSet->nElements, distanceDataSet->samples);

    //puts("Before:");
    // for (size_t i = 0; i < distanceDataSet->nElements; i++) {
        //printf("[%ld] %s--%s dist: %Lf cluster: %ld\n", i, un[i].sample->from->id, un[i].sample->to->id, un[i].sample->distance, UF_find(&un[i])->id);
    // }

    //puts("MST:");
    // Executing at most nElements - K times, don't even need to remove later
    // Sice it's sorted, it's on the 3 largest distances
    for (size_t i = 0; currentGroups != (*K); i++) {
        unionCell_t* p = UF_find(&un[un[i].sample->from->index]), * q = UF_find(&un[un[i].sample->to->index]);
        // printf("[%ld] %ld and %ld  \t| addr %p and %p. Grps: %ld\n", i, p->id, q->id, (void*)p, (void*)q, currentGroups);
        if (UF_union(p, q) == 1) {
            // printf("Unionizing roots %ld and %ld.\n", p->id, q->id);
            currentGroups--;
        }
    }

    // puts("Final:");
    // for (size_t i = 0; i < nEdges; i++) {
    //     printf("[%ld] %s--%s\tdist: %Lf\tcluster: %ld\n", i, un[i].sample->from->id, un[i].sample->to->id, un[i].sample->distance, UF_find(&un[i])->id);
    // }


    unionCell_t* MST = sortMST(un, &distanceDataSet->depth, dataSet);
    *samples = un;
    return MST;
}

void printOutput(char* filename, unionCell_t* MST, dataSet_t* dataSet, size_t* nVertex) {
    FILE* file = openFile(filename, "w");

    // puts("File output:");
    // Print first one
    // printf("%s", dataSet->samples[MST[0].id].id);
    fprintf(file, "%s", dataSet->samples[MST[0].id].id);
    // Print the rest
    for (size_t i = 1; i < *nVertex; i++) {
        // Same cluster as previous
        // //printf("[%ld] Current cluster: %ld cluster from prev: %ld\n", i, UF_find(&MST[i])->id, UF_find(&MST[i - 1])->id);
        if (UF_find(&MST[i])->id == UF_find(&MST[i - 1])->id) {
            // printf(",%s", dataSet->samples[MST[i].id].id);
            fprintf(file, ",%s", dataSet->samples[MST[i].id].id);
            // Different cluster
        } else {
            // printf("\n%s", dataSet->samples[MST[i].id].id);
            fprintf(file, "\n%s", dataSet->samples[MST[i].id].id);
        }
    }

    closeFile(file);
}
