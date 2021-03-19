#include "../lib/distances.h"

distanceDataSet_t* initDistanceDataSet(size_t nElements) {
    distanceDataSet_t* dataSet = (distanceDataSet_t*)malloc(sizeof(distanceDataSet_t));
    if (dataSet == NULL) {
        perror("Error allocating new dataSet. Exiting");
        exit(EXIT_FAILURE);
    };
    dataSet->depth = nElements;
    dataSet->nElements = (nElements / 2) * (nElements - 1);
    dataSet->samples = (distanceSample_t*)malloc(sizeof(distanceSample_t) * dataSet->nElements);
    if (dataSet->samples == NULL) {
        perror("Error allocating new samples. Exiting");
        exit(EXIT_FAILURE);
    }
    return dataSet;
}

long double euclidianDistance(long double* a, long double* b, size_t* nFeatures) {
    long double accumulator = 0;

    // SUM(abs(a - b)^2)
    for (size_t i = 0; i < *nFeatures; i++) {
        // printf("[dim = %ld Val a = %Lf b = %Lf]", *nFeatures, a[i], b[i]);

        accumulator += (a[i] - b[i]);
    }
    // printf(" Dist: %Lf\n", dist);
    return accumulator;
}

distanceDataSet_t* calculateDistances(dataSet_t* locationSet) {
    distanceDataSet_t* distanceSet = initDistanceDataSet(locationSet->nElements);

    for (size_t i = 0, count = 0; i < locationSet->nElements; i++) {
        for (size_t j = 0; j < i; j++, count++) {
            distanceSet->samples[count].from = &locationSet->samples[i];
            distanceSet->samples[count].to = &locationSet->samples[j];
            distanceSet->samples[count].distance = euclidianDistance(locationSet->samples[i].features, locationSet->samples[j].features, &locationSet->nFeatures);
        }
    }
    return distanceSet;
}

void printDistanceSet(distanceDataSet_t* dataSet) {
    for (size_t i = 0, k = 0; i < dataSet->depth; i++) {
        //printf("%s:", dataSet->samples[k].from->id);
        for (size_t j = 0; j < i; j++, k++) {
            //printf("\t%Lf", dataSet->samples[k].distance);
        }
        //puts("");
    }
}

void destroyDistanceDataSet(distanceDataSet_t* dataSet) {
    free(dataSet->samples);
    free(dataSet);
    dataSet = NULL;
}


int compareDistanceSamples(const void* a, const void* b) {
    if (((distanceSample_t*)a)->distance < ((distanceSample_t*)b)->distance) return -1;
    if (((distanceSample_t*)a)->distance > ((distanceSample_t*)b)->distance) return 1;
    return 0;
}
