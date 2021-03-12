#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t t_list;

t_list* UFinit(const size_t size);
void UFdestroy(t_list* list);
void UFmarkIndex(t_list* list, const size_t index);
char UFisMarked(t_list* list, const size_t index);
void UFprint(t_list* list, const size_t size);

#endif
