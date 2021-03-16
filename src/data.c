#include "../lib/data.h"

#define BUFSIZE 1500

dataSet_t* initDataSet(size_t nFeatures, size_t nElements) {
    dataSet_t* dataSet = (dataSet_t*)malloc(sizeof(dataSet_t));
    if (dataSet == NULL) {
        perror("Error allocating new dataSet. Exiting");
        exit(1);
    };
    dataSet->nFeatures = nFeatures;
    dataSet->nElements = nElements;
    dataSet->samples = (sample_t*)malloc(sizeof(sample_t) * dataSet->nElements);
    if (dataSet->samples == NULL) {
        perror("Error allocating new samples. Exiting");
        exit(1);
    }
    return dataSet;
}

dataSet_t* loadData(char* filename, const char* separator) {
    FILE* file = openFile(filename, "r");
    char buffer[BUFSIZE];
    size_t bufferSize = BUFSIZE;

    dataSet_t* dataSet = initDataSet(getLineSize(file, *separator, buffer, &bufferSize), countLines(file));

    for (size_t i = 0; i < dataSet->nElements; i++) {
        char** line = readLine(file, separator, buffer, &bufferSize, &dataSet->nFeatures);
        dataSet->samples[i].id = line[0];
        dataSet->samples[i].index = i;
        dataSet->samples[i].features = (long double*)malloc(sizeof(long double) * dataSet->nFeatures);
        if (dataSet->samples == NULL) {
            perror("Error allocating features for new sample. Exiting");
            exit(1);
        }
        for (size_t j = 0; j < dataSet->nFeatures; j++) {
            dataSet->samples[i].features[j] = strtold(line[j + 1], NULL);
            free(line[j + 1]);
        }
        free(line);
    }
    closeFile(file);

    return dataSet;
}

void printSample(const sample_t* sample, const size_t* nFeatures) {
    //printf("%s:", sample->id);
    for (size_t j = 0; j < *nFeatures; j++) {
        // printf("\t[%ld]", j);
        //printf("\t%Lf", sample->features[j]);
    }
    //puts("");
}

void printDataSet(dataSet_t* dataSet) {
    for (size_t i = 0; i < dataSet->nElements; i++) {
        printSample(&dataSet->samples[i], &dataSet->nFeatures);
    }
}

// Does not free IDs
void destroySample(sample_t* sample) {
    free(sample->features);
    sample = NULL;
}

// Does not free IDs
void destroyDataSet(dataSet_t* dataSet) {
    for (size_t i = 0; i < dataSet->nElements; i++) {
        destroySample(&dataSet->samples[i]);
    }
    free(dataSet->samples);
    free(dataSet);
    dataSet = NULL;
}
