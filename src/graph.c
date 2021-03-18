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

typedef struct clusterGroup_t {
    unionCell_t* root;
    // Not same as id anymore because it has been reordered
    size_t index;
} clusterGroup_t;

typedef struct matrixFirst_t {
    char** content;
    size_t index;
} matrixFirst_t;

// Sort lines of the matrix
int sortOutputMatrix(const void* a, const void* b) {
    matrixFirst_t aHead = *(matrixFirst_t*)a;
    matrixFirst_t bHead = *(matrixFirst_t*)b;
    char* strA = aHead.content[0];
    char* strB = bHead.content[0];
    // printf("Comparing %s X %s :\t", strA, strB);
    int cmp = strcmp(strA, strB);
    // printf("%d\n", cmp);
    return cmp;
}

void printOutput(char* filename, unionCell_t* MST, dataSet_t* dataSet, size_t* nVertex, size_t* K) {

    // puts("After:");
    // for (size_t i = 0; i < dataSet->nElements; i++) {
    //     printf("[%ld] dist: %Lf\tcluster: %ld\tID: %s\n", i, MST[i].distanceSample->distance, UF_find(&MST[i])->id, MST[i].sample->id);
    // }

    clusterGroup_t roots[*K];

    // Loading cluster sizes
    for (size_t i = 0, clusterCounter = 0; i < dataSet->nElements; i++) {
        // printf("%ld ", i);
        // New cluster root is found
        if (UF_find(&MST[i]) == &MST[i]) {
            // puts("New cluster found!");
            roots[clusterCounter].root = UF_find(&MST[i]);
            roots[clusterCounter++].index = i;
        }
    }

    // puts("ClusterRoots:");
    // for (size_t i = 0; i < *K; i++) {
    //     printf("[%ld] id: %ld\trootid: %ld\tindex: %ld\tsize:%ld\n", i, roots[i].root->id, roots[i].root->id, roots[i].index, roots[i].root->size);
    // }

    char** outMatrix[*K];
    for (size_t i = 0, acc = 0; i < *K; acc += roots[i++].root->size) {
        outMatrix[i] = (char**)malloc(sizeof(char*) * roots[i].root->size);
        if (outMatrix[i] == NULL) {
            perror("Error allocating space for output matrix row. Exiting");
            exit(EXIT_FAILURE);
        }
        for (size_t j = 0; j < roots[i].root->size; j++) {
            outMatrix[i][j] = MST[acc + j].sample->id;
        }
    }

    // puts("Output matrix presort:");
    // for (size_t i = 0; i < *K; i++) {
    //     printf("Size: %ld\t", roots[i].root->size);
    //     for (size_t j = 0; j < roots[i].root->size; j++) {
    //         printf("%s,", outMatrix[i][j]);
    //     }
    //     puts("");
    // }

    matrixFirst_t matrixHeads[*K];
    for (size_t i = 0; i < *K; i++) {
        matrixHeads[i].content = outMatrix[i];
        matrixHeads[i].index = i;
    }

    // puts("MatrixHeads:");
    // for(size_t i = 0; i < *K; i++) {
    //     printf("[%ld] content: %s\tindex: %ld\n", i, matrixHeads[i].content[0], matrixHeads[i].index);
    // }

    // puts("Inside qsort:");
    qsort(matrixHeads, *K, sizeof(matrixFirst_t), &sortOutputMatrix);

    // puts("MatrixHeads after sort:");
    // for(size_t i = 0; i < *K; i++) {
    //     printf("[%ld] content: %s\tindex: %ld\n", i, matrixHeads[i].content[0], matrixHeads[i].index);
    // }

    for (size_t i = 0; i < *K; i++) {
        outMatrix[i] = matrixHeads[i].content;
    }

    // puts("Output matrix posSort:");
    // for (size_t i = 0; i < *K; i++) {
    //     printf("Size: %ld\t", roots[matrixHeads[i].index].root->size);
    //     for (size_t j = 0; j < roots[matrixHeads[i].index].root->size; j++) {
    //         printf("%s,", outMatrix[i][j]);
    //     }
    //     puts("");
    // }

    // Writing to file
    FILE* file = openFile(filename, "w");

    // puts("Sorted output matrix on printf:");
    for (size_t i = 0; i < *K; i++) {
        // printf("%s", outMatrix[i][0]);
        fprintf(file, "%s", outMatrix[i][0]);
        for (size_t j = 1; j < roots[matrixHeads[i].index].root->size; j++) {
            // printf(",%s", outMatrix[i][j]);
            fprintf(file, ",%s", outMatrix[i][j]);
        }
        // printf("\n");
        fprintf(file, "\n");
        free(outMatrix[i]);
    }

    closeFile(file);
}
