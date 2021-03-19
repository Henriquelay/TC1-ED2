#include "../lib/graph.h"

int sortMSTElements(const void* a, const void* b) {
    unionCell_t* A = ((unionCell_t*)a);
    unionCell_t* B = ((unionCell_t*)b);
    unionCell_t* Aroot = UF_find(A);
    unionCell_t* Broot = UF_find(B);

    // Checking if same cluster name
    int cmp = strcmp(Aroot->sample->id, Broot->sample->id);
    // printf("On %s\tX\t %s = %d\n", Aroot->sample->id, Broot->sample->id, cmp);
    if (cmp != 0) return cmp;

    // Comparing sample id if same cluster
    cmp = strcmp(A->sample->id, B->sample->id);
    // printf("\tDraw. Comparing sample->ids %s and %s: %d\n", A->sample->id, B->sample->id, cmp);
    return cmp;
}

// Also makes a pass on path compression
unionCell_t* sortMST(unionCell_t* MST, size_t* nVertex) {
    // Creating new array with different sizing (qsort won't stop on nEdges and will sort whole array)
    unionCell_t* slicedMST = malloc(sizeof(unionCell_t) * *nVertex);
    if (slicedMST == NULL) {
        perror("Error allocating new MST array. Exiting");
        exit(EXIT_FAILURE);
    }
    // I really don't like slicing allocated arrays. Oh well.
    // <! Getting slice item and doing path compression !>
    // Don't to UF_finds after this point, this is a dirty hack <<
    for (size_t i = 0; i < *nVertex; i++) {
        slicedMST[i] = MST[i];
        // printf("[%ld] %p\n", i, (void*)root);
        // printf("[%ld] Root antes = %p\t", i, (void*)(slicedMST[i].root != NULL ? slicedMST[i].root->id : 0));
        // printf("[%ld] Root depois = %ld\n", i, slicedMST[i].root->id);
    }

    // Uniting clusters together, not putting them ir order because I can't know where each starts and ends to compare with the first one
    qsort(slicedMST, *nVertex, sizeof(unionCell_t), &sortMSTElements);

    // puts("After sorting for printing");
    // for(size_t i = 0; i < *nVertex; i++) {
    //     printf("[%ld] %s cluster: %s\n", i, MST[i].sample->id, UF_find(&MST[i])->sample->id);
    // }

    return slicedMST;
}

unionCell_t* MST_kruskal(distanceDataSet_t* distanceDataSet, size_t* K, dataSet_t* dataSet) {
    // 1 group per vertex
    size_t currentGroups = distanceDataSet->depth;
    unionCell_t* MST = UF_init(distanceDataSet->nElements, distanceDataSet->samples, dataSet->samples);

    // puts("After sorting for Kruskal");
    // for(size_t i = 0; i < dataSet->nElements; i++) {
    //     printf("[%ld] dist %Lf %s cluster: %s\n", i, MST[i].distanceSample->distance, MST[i].sample->id, UF_find(&MST[i])->sample->id);
    // }


    //puts("MST:");
    // Executing at most nElements - K times, don't even need to remove later
    // Sice it's sorted, it's on the 3 largest distances
    for (size_t i = 0; currentGroups != *K; i++) {
        unionCell_t* p = UF_find(&MST[MST[i].distanceSample->from->index]), * q = UF_find(&MST[MST[i].distanceSample->to->index]);
        // printf("[%ld] %ld and %ld  \t| addr %p and %p. Grps: %ld\n", i, p->id, q->id, (void*)p, (void*)q, currentGroups);
        if (UF_union(p, q) == 1) {
            // printf("Unionizing roots %ld and %ld.\n", p->id, q->id);
            currentGroups--;
        }
    }

    // puts("After Kruskal");
    // for(size_t i = 0; i < dataSet->nElements; i++) {
    //     printf("[%ld] %s cluster: %s\n", i, MST[i].sample->id, UF_find(&MST[i])->sample->id);
    // }

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
    clusterGroup_t roots[*K];

    // puts("Sorted:");
    // for(size_t i = 0; i < *nVertex; i++) {
    //     printf("[%ld] %s cluster: %s\n", i, MST[i].sample->id, UF_find(&MST[i])->sample->id);
    // }

    // Loading cluster sizes
    for (size_t i = 0, clusterCounter = 0; clusterCounter < *K; i++) {
        // printf("%ld ", i);
        // New cluster root is found
        if (UF_find(&MST[i]) == &MST[i]) {
            // puts("New cluster found!");
            roots[clusterCounter].root = &MST[i];
            // printf("Root %p\n", (void*)roots[clusterCounter].root);
            roots[clusterCounter++].index = i;
        }
    }

    // puts("ClusterRoots:");
    // for (size_t i = 0; i < *K; i++) {
    //     printf("[%ld] id: %s\t rootId: %ld\tindex: %ld\tsize:%ld\n", i, roots[i].root->sample->id, roots[i].root->id, roots[i].index, roots[i].root->size);
    // }

    // Building matrix to output
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

    // matrixFirst_t matrixHeads[*K];
    // for (size_t i = 0; i < *K; i++) {
    //     matrixHeads[i].content = outMatrix[i];
    //     matrixHeads[i].index = i;
    // }

    // puts("MatrixHeads:");
    // for(size_t i = 0; i < *K; i++) {
    //     printf("[%ld] content: %s\tindex: %ld\n", i, matrixHeads[i].content[0], matrixHeads[i].index);
    // }

    // puts("Inside qsort:");
    // qsort(matrixHeads, *K, sizeof(matrixFirst_t), &sortOutputMatrix);

    // puts("MatrixHeads after sort:");
    // for(size_t i = 0; i < *K; i++) {
    //     printf("[%ld] content: %s\tindex: %ld\n", i, matrixHeads[i].content[0], matrixHeads[i].index);
    // }

    // for (size_t i = 0; i < *K; i++) {
    //     outMatrix[i] = matrixHeads[i].content;
    // }

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
        for (size_t j = 1; j < roots[i].root->size; j++) {
            // printf(",%s", outMatrix[i][j]);
            fprintf(file, ",%s", outMatrix[i][j]);
        }
        // printf("\n");
        fprintf(file, "\n");
        free(outMatrix[i]);
    }

    closeFile(file);
}
