#include "../lib/data.h"

// TODO - Check if a Macro to derefence (*((long double*)b[i])) is viable

void printData(const data_t* dataStruct) {
    for (size_t i = 0; i < dataStruct->i; i++) {
        printf("%s", (char*)dataStruct->dataMatrix[i][0]);
        for (size_t j = 1; j < dataStruct->j; j++) {
            printf(",%.15Lf", *((long double*)dataStruct->dataMatrix[i][j]));
        }
        puts("");
    }
}

// TODO check before freeing -- don't check whole array
void destroyData(data_t* data) {
    for (size_t i = 0; i < data->i; i++) {
        for (size_t j = 0; j < data->j; j++) {
            free(data->dataMatrix[i][j]);
        }
        free(data->dataMatrix[i]);
    }
    free(data->dataMatrix);
    free(data);
}

data_t* loadData(FILE* file, const char* separator) {
    char buffer[5000];
    size_t bufferSize = 0;

    data_t* dataStruct = (data_t*)malloc(sizeof(data_t));
    if (dataStruct == NULL) {
        perror("Error allocating new dataStructure. Exiting");
        exit(1);
    };

    dataStruct->i = countLines(file);
    dataStruct->j = getLineSize(file, separator, buffer, &bufferSize);


    // 0 will always be id, read as `char*`.
    dataStruct->dataMatrix = (void***)malloc(sizeof(void**) * dataStruct->i);
    if (dataStruct->dataMatrix == NULL) {
        perror("Error allocating new dataString lines. Exiting");
        exit(1);
    }

    // void** line = NULL;
    for (size_t i = 0; i < dataStruct->i; i++) {
        dataStruct->dataMatrix[i] = readLine(file, separator, buffer, &bufferSize, &dataStruct->j);
    }

    return dataStruct;
}

long double distance(long double** a, long double** b, size_t dimensions) {
    long double accumulator = 0;
    for (size_t i = 1; i <= dimensions; i++) {
        // printf("dim = %ld Val a = %Lf b = %Lf\n", dimensions, *a[i], *b[i]);
        if ((*a[i]) < (*b[i])) {
            accumulator += ((*b[i]) - (*a[i])) * ((*b[i]) - (*a[i]));
        } else {
            accumulator += ((*a[i]) - (*b[i])) * ((*a[i]) - (*b[i]));
        }
    }
    // TODO Try using sqrtl. Defined in <tgmath.h>
    return sqrt(accumulator);
}

data_t* getDistances(data_t* data) {
    data_t* distances = (data_t*)malloc(sizeof(data_t));
    if (distances == NULL) {
        perror("Error allocating new distancesure. Exiting");
        exit(1);
    };

    distances->i = data->i;
    // Data has [0] as identifiers, won't be using for calculations;
    distances->j = data->j - 1;
    // 0 will always be id, read as `char*`.
    distances->dataMatrix = (void***)malloc(sizeof(long double**) * distances->i);
    if (distances->dataMatrix == NULL) {
        perror("Error allocating new dataString lines. Exiting");
        exit(1);
    }


    for (size_t i = 0; i < distances->i; i++) {
        distances->dataMatrix[i] = (void**)malloc(sizeof(void*) * i);
        if (distances->dataMatrix[i] == NULL) {
            perror("Error allocating new dataString lines on distances struct. Exiting");
            exit(1);
        }
        for (size_t j = 0; j < i; j++) {
            distances->dataMatrix[i][j] = (long double*)malloc(sizeof(long double));
            if (distances->dataMatrix[i] == NULL) {
                perror("Error allocating new dataString lines on distances struct. Exiting");
                exit(1);
            }
            *((long double*)distances->dataMatrix[i][j]) = distance((long double**)data->dataMatrix[i], (long double**)data->dataMatrix[j], distances->j);
        }
    }

    return distances;
}

int compareDataVecs(const void* a, const void* b) {
    if (*(((dataVector_t*)a)->distance) < *(((dataVector_t*)b)->distance)) return -1;
    if (*(((dataVector_t*)a)->distance) > *(((dataVector_t*)b)->distance)) return 1;
    return 0;
}

dataVector_t* vectorizeData(data_t* data) {
    // It's a triangle
    size_t cells = data->i * (data->i - 1) / 2;
    dataVector_t* vector = (dataVector_t*)malloc(sizeof(dataVector_t) * cells);
    if (vector == NULL) {
        perror("Erro allocating dataVector. Exiting");
        exit(1);
    }

    for (size_t i = 0, k = 0; i < data->i; i++) {
        for (size_t j = 0; j < i; j++, k++) {
            vector[k].distance = data->dataMatrix[i][j];
            vector[k].i = i;
            vector[k].j = j;
        }
    }

    // Sorting on distances
    qsort(vector, cells, sizeof(dataVector_t), &compareDataVecs);

    // for (size_t i = 0; i < cells; i++) {
    //     printf("%Lf ", *vector[i].distance);
    // }
    // puts("");

    return vector;
}

// data_t* kruskal(data_t* data) {
//     union_t* un = UF_init(data->i);
// 
// }
