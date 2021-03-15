#include "../lib/data.h"

#define SEPARATOR ","

int main(int argc, char** argv) {
    char* filename = argv[1];
    // size_t K = strtoul(argv[2], NULL, 10);
    FILE* file = openFile(filename);
    // size_t tokenAmount = getLineSize(file, ",", buffer, &bufferSize);
    // char** tokens = readLine(file, ",", buffer, &bufferSize, &tokenAmount);

    // for (int i = 0; i < tokenAmount; i++) {
    //     printf("%s ", tokens[i]);
    // }
    // puts("");

    dataSet_t* data = loadData(file, SEPARATOR);
    closeFile(file);

    puts("Loaded data:");
    printDataSet(data);

    // data_t* distances = getDistances(data);
    // puts("Distances matrix:");
    // printDistanceMatrix(distances);

    // dataVector_t* dataVec = vectorizeData(distances, data);
    // puts("Vectorized data:");
    // printVectorizedData(dataVec);

    // kruskal(dataVec, K, data);

    destroyDataSet(data);

    // char* outputFile = argv[3];
}
