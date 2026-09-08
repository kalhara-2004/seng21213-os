#ifndef RAMFS_H
#define RAMFS_H

#include <stdint.h>

#define MAX_FILES 16
#define MAX_FILENAME 32
#define FILE_SIZE 512

typedef struct {
    char name[MAX_FILENAME];
    uint8_t data[FILE_SIZE];
    uint32_t size;
    uint8_t used;
} ramfs_file_t;

void ramfs_init(void);
int ramfs_create(const char *filename, const char *content);
void ramfs_list(void);
int ramfs_read(const char *filename);

#endif
