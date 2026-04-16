#include <openssl/sha.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char *object_write(const char *type, const void *data, size_t size) {
    char header[64];
    sprintf(header, "%s %zu", type, size);

    size_t header_len = strlen(header) + 1;
    size_t total_len = header_len + size;

    char *buffer = malloc(total_len);
    memcpy(buffer, header, header_len);
    memcpy(buffer + header_len, data, size);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)buffer, total_len, hash);

    char *hex = malloc(65);
    for (int i = 0; i < 32; i++)
        sprintf(hex + i*2, "%02x", hash[i]);

    char dir[256], path[256];
    sprintf(dir, ".pes/objects/%.2s", hex);
    mkdir(".pes", 0755);
    mkdir(".pes/objects", 0755);
    mkdir(dir, 0755);

    sprintf(path, "%s/%s", dir, hex + 2);

    int fd = open(path, O_WRONLY | O_CREAT, 0644);
    write(fd, buffer, total_len);
    close(fd);

    free(buffer);
    return hex;
}
