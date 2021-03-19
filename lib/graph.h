/**
* This library is for handling dataSets as in they were a graph. Any graph algorithm should be here
**/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#define _GNU_SOURCE

#include "data.h"
#include "unionFind.h"

unionCell_t* MST_kruskal(distanceDataSet_t* distanceDataSet, size_t* groupsNumber, dataSet_t* dataSet);
void printOutput(char* filename, unionCell_t* MST, dataSet_t* dataSet, size_t* nVertex, size_t* K);

unionCell_t* sortMST(unionCell_t* MST, size_t* nElements);

#endif
