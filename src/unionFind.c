#include "../lib/unionFind.h"

#define VARSIZE sizeof(uint8_t)

t_list* UFinit(const size_t size) {
    t_list* list = (uint8_t*)calloc(size / VARSIZE, sizeof(uint8_t));
    if (list == NULL) {
        perror("Error allocating list");
        exit(1);
    }

    return list;
}

void UFdestroy(t_list* list) {
    free(list);
    list = NULL;
}

void UFmarkIndex(t_list* list, const size_t index) {
    list[index / VARSIZE] = list[index / VARSIZE] & (0x01 << index % VARSIZE);
}

char UFisMarked(t_list* list, const size_t index) {
    return ((list[(index / VARSIZE)] >> index % VARSIZE) & 0x01);
}

void UFprint(t_list* list, const size_t size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", list[i / VARSIZE] & (0x01 << i % VARSIZE));
    }
    puts("");
}

