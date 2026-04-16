#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "index.h"

extern char *object_write(const char *type, const void *data, size_t size);
extern char *tree_from_index(Index *index);

char *commit_create(Index *index, const char *message) {
    char *tree_hash = tree_from_index(index);

    char content[4096];
    sprintf(content,
        "tree %s\n"
        "author PES User\n"
        "time %ld\n\n"
        "%s\n",
        tree_hash, time(NULL), message
    );

    char *commit_hash = object_write("commit", content, strlen(content));

    FILE *f = fopen(".pes/refs/heads/main", "w");
    fprintf(f, "%s", commit_hash);
    fclose(f);

    return commit_hash;
}
