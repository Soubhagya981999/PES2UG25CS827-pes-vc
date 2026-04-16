#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "index.h"

extern char *object_write(const char *type, const void *data, size_t size);

void index_load(Index *index) {
    FILE *f = fopen(".pes/index", "r");
    index->count = 0;

    if (!f) return;

    while (fscanf(f, "%s %s %ld %ld %s",
        index->entries[index->count].mode,
        index->entries[index->count].hash,
        &index->entries[index->count].mtime,
        &index->entries[index->count].size,
        index->entries[index->count].path) == 5) {

        index->count++;
    }

    fclose(f);
}

void index_save(Index *index) {
    FILE *f = fopen(".pes/index", "w");

    for (int i = 0; i < index->count; i++) {
        fprintf(f, "%s %s %ld %ld %s\n",
            index->entries[i].mode,
            index->entries[i].hash,
            index->entries[i].mtime,
            index->entries[i].size,
            index->entries[i].path);
    }

    fclose(f);
}

void index_add(Index *index, const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);

    char *hash = object_write("blob", data, size);

    strcpy(index->entries[index->count].mode, "100644");
    strcpy(index->entries[index->count].hash, hash);
    strcpy(index->entries[index->count].path, path);

    index->entries[index->count].size = size;
    index->entries[index->count].mtime = time(NULL);

    index->count++;
}
