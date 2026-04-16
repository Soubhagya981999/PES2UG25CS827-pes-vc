#include <stdio.h>
#include <string.h>
#include "index.h"

extern char *object_write(const char *type, const void *data, size_t size);

char *tree_from_index(Index *index) {
    char buffer[4096] = {0};

    for (int i = 0; i < index->count; i++) {
        strcat(buffer, "100644 ");
        strcat(buffer, index->entries[i].hash);
        strcat(buffer, " ");
        strcat(buffer, index->entries[i].path);
        strcat(buffer, "\n");
    }

    return object_write("tree", buffer, strlen(buffer));
}
