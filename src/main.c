#include "../lib/data.h"
#include "../lib/distances.h"
#include "../lib/unionFind.h"
#include "../lib/graph.h"

#define SEPARATOR ","

int main(int argc, char** argv) {
    char* filename = argv[1];
    dataSet_t* dataPlot = loadData(filename, SEPARATOR);
    // puts("Loaded dataPlot:");
    // printDataSet(dataPlot);

    distanceDataSet_t* distanceSet = calculateDistances(dataPlot);
    // puts("Loaded distanceSet:");
    // printDistanceSet(distanceSet);

    size_t K = strtoul(argv[2], NULL, 10);
    unionCell_t *samples;
    unionCell_t * MST = MST_kruskal(distanceSet, &K, &samples, dataPlot);
    // puts("Kruskelated:");

    char* outputFile = argv[3];
    printOutput(outputFile, MST, dataPlot, &dataPlot->nElements);
    // puts("Printered:");


    // Freeing string identificators here...
    for (size_t i = 0; i < dataPlot->nElements; i++) {
        free(dataPlot->samples[i].id);
    }
    // ...because these doesn't free ID's, the structures reuse the same address
    destroyDataSet(dataPlot);
    destroyDistanceDataSet(distanceSet);
    UF_destroy(samples);
    UF_destroy(MST);

    // puts("Freed. Bye.");
}
