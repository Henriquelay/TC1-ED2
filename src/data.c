#include "../lib/data.h"

#define BUFSIZE 1500

dataSet_t* loadData(FILE* file, const char* separator) {
    char buffer[BUFSIZE];
    size_t bufferSize = BUFSIZE;

    dataSet_t* dataSet = (dataSet_t*)malloc(sizeof(dataSet_t));
    if (dataSet == NULL) {
        perror("Error allocating new dataSet. Exiting");
        exit(1);
    };

    dataSet->nFeatures = getLineSize(file, *separator, buffer, &bufferSize);
    dataSet->nElements = countLines(file);


    // 0 will always be id, read as `char*`.
    dataSet->samples = (sample_t*)malloc(sizeof(sample_t) * dataSet->nElements);
    if (dataSet->samples == NULL) {
        perror("Error allocating new samples. Exiting");
        exit(1);
    }

    // void** line = NULL;
    for (size_t i = 0; i < dataSet->nElements; i++) {
        char** line = readLine(file, separator, buffer, &bufferSize, &dataSet->nFeatures);
        dataSet->samples[i].id = line[0];
        dataSet->samples[i].features = (long double*)malloc(sizeof(long double) * dataSet->nFeatures);
        if (dataSet->samples == NULL) {
            perror("Error allocating features for new sample. Exiting");
            exit(1);
        }
        for (size_t j = 0; j < dataSet->nFeatures; j++) {
            dataSet->samples[i].features[j] = strtold(line[j + 1], NULL);
            free(line[j+1]);
        }
        free(line);
    }

    return dataSet;
}

void printSample(const sample_t* sample, const size_t* nFeatures) {
    printf("%s", sample->id);
    for (size_t j = 0; j < *nFeatures; j++) {
        printf(",%Lf", sample->features[j]);
    }
    puts("");
}

void printDataSet(dataSet_t* dataSet) {
    for (size_t i = 0; i < dataSet->nElements; i++) {
        printSample(&dataSet->samples[i], &dataSet->nFeatures);
    }
}

void destroySample(sample_t* sample) {
    free(sample->features);
    free(sample->id);
}

void destroyDataSet(dataSet_t* dataSet) {
    for (size_t i = 0; i < dataSet->nElements; i++) {
        destroySample(&dataSet->samples[i]);
    }
    free(dataSet->samples);
    free(dataSet);
}
