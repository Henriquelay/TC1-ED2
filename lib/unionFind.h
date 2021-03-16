#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <stdlib.h>
#include <stdio.h>

#include "distances.h"

typedef struct unionCell_t {
    struct unionCell_t* root;
    size_t size, id;
    distanceSample_t* sample;
} unionCell_t;

unionCell_t* UF_init(const size_t size, distanceSample_t* samples);
void UF_destroy(unionCell_t* unionStruct);
unionCell_t* UF_find(unionCell_t* unionStruct);
char UF_union(unionCell_t* p, unionCell_t* q);

#endif
