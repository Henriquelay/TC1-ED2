#include "../lib/distances.h"
#include "../lib/graph.h"

#define SEPARATOR ","

int main(int argc, char** argv) {
    char* filename = argv[1];
    FILE* file = openFile(filename);
    // size_t tokenAmount = getLineSize(file, ",", buffer, &bufferSize);
    // char** tokens = readLine(file, ",", buffer, &bufferSize, &tokenAmount);

    // for (int i = 0; i < tokenAmount; i++) {
    //     printf("%s ", tokens[i]);
    // }
    // puts("");

    dataSet_t* dataPlot = loadData(file, SEPARATOR);
    closeFile(file);
    // puts("Loaded dataPlot:");
    // printDataSet(dataPlot);

    // FIXME printing B: in in on first line
    distanceDataSet_t* distanceSet = calculateDistances(dataPlot);
    // puts("Loaded distanceSet:");
    // printDistanceSet(distanceSet);

    size_t K = strtoul(argv[2], NULL, 10);
    unionCell_t * MST = MST_kruskal(distanceSet, &K);

    // Freeing ids here...
    for (size_t i = 0; i < dataPlot->nElements; i++) {
        free(dataPlot->samples[i].id);
    }
    // ...because these doesn't free ID's, they reuse the same address
    destroyDataSet(dataPlot);
    destroyDistanceDataSet(distanceSet);
    UF_destroy(MST);


    // char* outputFile = argv[3];
}
