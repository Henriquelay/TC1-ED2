/**
    * This library is for handling distances calculations and related manners.
**/

#ifndef _DISTS_H_
#define _DISTS_H_

#include <math.h>

#include "data.h"

// "Rows"
typedef struct distanceSample_t {
    long double distance;
    // On distanceSet
    sample_t  *from, *to;
} distanceSample_t;

// "Lines" of rows
typedef struct distanceDataSet_t {
    size_t nElements;
    size_t depth;
    distanceSample_t* samples;
} distanceDataSet_t;

distanceDataSet_t* calculateDistances(dataSet_t* points);
void printDistanceSet(distanceDataSet_t* dataSet);
void destroyDistanceDataSet(distanceDataSet_t* dataSet);

#endif
