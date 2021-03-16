/**
* This library is for handling dataSets as in they were a graph. Any graph algorithm should be here
**/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#define _GNU_SOURCE // Needed to use qsort_r when not using c11 or gnu89/gnu99

#include "./data.h"
#include "./unionFind.h"

unionCell_t* MST_kruskal(distanceDataSet_t* dataSet, size_t* groupsNumber);

#endif
