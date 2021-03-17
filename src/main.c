#include "../lib/data.h"
#include "../lib/distances.h"
#include "../lib/unionFind.h"
#include "../lib/graph.h"

#include <time.h>

#define SEPARATOR ","

int main(int argc, char** argv) {
    char* filename = argv[1];
    clock_t timeStart = clock();
    clock_t timeRun = timeStart;
    dataSet_t* dataPlot = loadData(filename, SEPARATOR);
    printf("Parsed file in\t\t%f seconds\n", (double)(clock() - timeRun)/CLOCKS_PER_SEC);
    // printDataSet(dataPlot);

    timeRun = clock();
    distanceDataSet_t* distanceSet = calculateDistances(dataPlot);
    printf("Distances calculated in\t%f seconds\n", (double)(clock() - timeRun)/CLOCKS_PER_SEC);
    // printDistanceSet(distanceSet);

    size_t K = strtoul(argv[2], NULL, 10);
    unionCell_t *samples;
    timeRun = clock();
    unionCell_t * MST = MST_kruskal(distanceSet, &K, &samples, dataPlot);
    printf("MST calculated in\t%f seconds\n", (double)(clock() - timeRun)/CLOCKS_PER_SEC);
    // puts("Kruskelated:");

    char* outputFile = argv[3];
    timeRun = clock();
    printOutput(outputFile, MST, dataPlot, &dataPlot->nElements);
    printf("Outputted to file in\t%f seconds\n", (double)(clock() - timeRun)/CLOCKS_PER_SEC);
    // puts("Printered:");



    timeRun = clock();
    // Freeing string identificators here...
    for (size_t i = 0; i < dataPlot->nElements; i++) {
        free(dataPlot->samples[i].id);
    }
    // ...because these doesn't free ID's, the structures reuse the same address
    destroyDataSet(dataPlot);
    destroyDistanceDataSet(distanceSet);
    UF_destroy(samples);
    UF_destroy(MST);
    printf("Freed data in\t\t%f seconds\n", (double)(clock() - timeRun)/CLOCKS_PER_SEC);


    printf("---------------------------------\nTotal run time\t\t%f seconds\n", (double)(clock() - timeStart)/CLOCKS_PER_SEC);

    // puts("Freed. Bye.");
    return EXIT_SUCCESS;
}
