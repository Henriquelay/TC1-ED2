#include "../lib/data.h"

int main(int argc, char** argv) {
    char* filename = argv[1];
    size_t K = strtoul(argv[2], NULL, 10);
    FILE* file = openFile(filename);
    // size_t tokenAmount = getLineSize(file, ",", buffer, &bufferSize);
    // char** tokens = readLine(file, ",", buffer, &bufferSize, &tokenAmount);

    // for (int i = 0; i < tokenAmount; i++) {
    //     printf("%s ", tokens[i]);
    // }
    // puts("");

    data_t* data = loadData(file, ",");
    closeFile(file);

    data_t* distances = getDistances(data);

    printData(data);


    kruskal(vectorizeData(distances), K);

    destroyData(data);

    // unsigned int k = atoi(argv[2]);
    // char* outputFile = argv[3];
    // free(tokens);
}
