#include "../lib/graph.h"


int sortMSTElements(const void* a, const void* b) {
    unionCell_t* rootA = UF_find((unionCell_t*)a);
    unionCell_t* rootB = UF_find((unionCell_t*)b);

    // Checking if same cluster (same root id)
    int cmp = rootA->id - rootB->id;
    // printf("On %ld - %ld = %d\n", rootA->id, rootB->id, cmp);
    if (cmp != 0) return cmp;
    sample_t* sampleA = ((unionCell_t*)a)->sample;
    sample_t* sampleB = ((unionCell_t*)b)->sample;

    cmp = strcmp(sampleA->id, sampleB->id);
    // printf("\tDraw. Comparing ids %s and %s: %d\n", sampleA->id, sampleB->id, cmp);
    return cmp;
}

// Also makes a pass on path compression
unionCell_t* sortSliceAndCompressMST(unionCell_t* MST, size_t* inputSize) {
    // Creating new array with different sizing (qsort won't stop on nEdges and will sort whole array)
    unionCell_t* slicedMST = malloc(sizeof(unionCell_t) * *inputSize);
    if (slicedMST == NULL) {
        perror("Error allocating new MST array. Exiting");
        exit(EXIT_FAILURE);
    }
    // I really don't like slicing allocated arrays. Oh well.
    // <! Getting slice item and doing path compression !>
    // TEST, Not compressing again
    // Don't to UF_finds after this point, this is a dirty hack <<
    for (size_t i = 0; i < *inputSize; i++) {
        slicedMST[i] = MST[i];
        // printf("[%ld] %p\n", i, (void*)root);
        // printf("Root antes = %ld\t", slicedMST[i].root != NULL ? slicedMST[i].root->id : 0);
        // slicedMST[i].root = slicedMST[i].root == NULL ? &slicedMST[i] : UF_find(&slicedMST[i]);
        // printf("Root depois = %ld\n", slicedMST[i].root->id);
    }

    // Uniting clusters together, not putting them ir order because I can't know where each starts and ends to compare with the first one
    qsort(slicedMST, *inputSize, sizeof(unionCell_t), &sortMSTElements);

    return slicedMST;
}

unionCell_t* MST_kruskal(distanceDataSet_t* distanceDataSet, size_t* K, dataSet_t* dataSet) {
    // 1 group per vertex
    size_t currentGroups = distanceDataSet->depth;
    unionCell_t* MST = UF_init(distanceDataSet->nElements, distanceDataSet->samples, dataSet->samples);

    // puts("Before:");
    // for (size_t i = 0; i < distanceDataSet->nElements; i++) {
    //     printf("[%ld] %s--%s dist: %Lf cluster: %ld\n", i, MST[i].distanceSample->from->id, MST[i].distanceSample->to->id, MST[i].distanceSample->distance, UF_find(&MST[i])->id);
    // }

    //puts("MST:");
    // Executing at most nElements - K times, don't even need to remove later
    // Sice it's sorted, it's on the 3 largest distances
    for (size_t i = 0; currentGroups != (*K); i++) {
        unionCell_t* p = UF_find(&MST[MST[i].distanceSample->from->index]), * q = UF_find(&MST[MST[i].distanceSample->to->index]);
        // printf("[%ld] %ld and %ld  \t| addr %p and %p. Grps: %ld\n", i, p->id, q->id, (void*)p, (void*)q, currentGroups);
        if (UF_union(p, q) == 1) {
            // printf("Unionizing roots %ld and %ld.\n", p->id, q->id);
            currentGroups--;
        }
    }

    return MST;
}

// Just to qsort change clusterSize at the same operation
typedef struct matrixRow_t {
    char** row;
    size_t clusterSize;
} matrixRow_t;

// Sort lines of the matrix
int sortOutputMatrix(const void* a, const void* b) {
    printf("%p %p\n", a, b);
    char** strA = ((matrixRow_t*)a)->row;
    char** strB = ((matrixRow_t*)b)->row;
    int cmp = strcmp(strA[0], strB[0]);
    printf("Comparing %s and %s = %d", strA[0], strB[0], cmp);
    return cmp;
}

void printOutput(char* filename, unionCell_t* MST, dataSet_t* dataSet, size_t* nVertex, size_t* K) {

    // puts("After:");
    // for (size_t i = 0; i < dataSet->nElements; i++) {
    //     printf("[%ld] dist: %Lf\tcluster: %ld\tID: %s\n", i, MST[i].distanceSample->distance, UF_find(&MST[i])->id, MST[i].sample->id);
    // }

    size_t clusterSizes[dataSet->nElements];

    // Loading cluster sizes
    clusterSizes[0] = UF_find(&MST[0])->size;
    for (size_t i = 1, clusterCounter = 1; i < dataSet->nElements; i++) {
        // printf("%ld ", i);
        // New cluster is found
        if (UF_find(&MST[i])->id != UF_find(&MST[i - 1])->id) {
            // puts("New cluster found!");
            clusterSizes[clusterCounter++] = UF_find(&MST[i])->size;
        }
    }

    printf("K = %ld\n", *K);
    matrixRow_t outMatrix[*K];
    for (size_t i = 0, acc = 0; i < *K; acc += clusterSizes[i++]) {
        outMatrix[i].clusterSize = clusterSizes[i];
        outMatrix[i].row = (char**)malloc(sizeof(char*) * outMatrix[i].clusterSize);
        if (outMatrix[i].row == NULL) {
            perror("Error allocating space for output matrix. Exiting");
            exit(EXIT_FAILURE);
        }
        printf("Accumulator = %ld\n", acc);
        for (size_t j = 0; j < outMatrix[i].clusterSize; j++) {
            outMatrix[i].row[j] = MST[acc + j].sample->id;
        }
    }

    puts("Output matrix presort:");
    for (size_t i = 0; i < *K; i++) {
        printf("Size: %ld\t", outMatrix[i].clusterSize);
        for (size_t j = 0; j < outMatrix[i].clusterSize; j++) {
            printf("%s,", outMatrix[i].row[j]);
        }
        puts("");
    }

    qsort(outMatrix, *K, sizeof(matrixRow_t*), &sortOutputMatrix);

    puts("Sorted output matrix:");
    for (size_t i = 0; i < *K; i++) {
        printf("Size: %ld\t", outMatrix[i].clusterSize);
        for (size_t j = 0; j < outMatrix[i].clusterSize; j++) {
            printf("%s,", outMatrix[i].row[j]);
        }
        puts("");
    }

    // Writing to file
    FILE* file = openFile(filename, "w");





    // puts("File output:");
    // Print first one
    // printf("%s", dataSet->samples[MST[0].id].id);
    // fprintf(file, "%s", dataSet->samples[MST[0].id].id);
    // // Print the rest
    // for (size_t i = 1; i < *nVertex; i++) {
    //     // Same cluster as previous
    //     // printf("[%ld] Current cluster: %ld cluster from prev: %ld\n", i, UF_find(&MST[i])->id, UF_find(&MST[i - 1])->id);
    //     if (UF_find(&MST[i])->id == UF_find(&MST[i - 1])->id) {
    //         // printf(",%s", dataSet->samples[MST[i].id].id);
    //         fprintf(file, ",%s", dataSet->samples[MST[i].id].id);
    //         // Different cluster
    //     } else {
    //         // printf("\n%s", dataSet->samples[MST[i].id].id);
    //         fprintf(file, "\n%s", dataSet->samples[MST[i].id].id);
    //     }
    // }

    closeFile(file);
}
