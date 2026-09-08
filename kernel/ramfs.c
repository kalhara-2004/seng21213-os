#include "ramfs.h"
#include "string.h"
#include "vga.h"

static ramfs_file_t files[MAX_FILES];

void ramfs_init(void) {
    memset(files, 0, sizeof(files));
}

int ramfs_create(const char *filename, const char *content) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {
            int j = 0;
            while (filename[j] != '\0' && j < MAX_FILENAME - 1) {
                files[i].name[j] = filename[j];
                j++;
            }
            files[i].name[j] = '\0';
            uint32_t len = strlen(content);
            if (len >= FILE_SIZE) len = FILE_SIZE - 1;
            for (uint32_t k = 0; k < len; k++) {
                files[i].data[k] = content[k];
            }
            files[i].data[len] = '\0';
            files[i].size = len;
            files[i].used = 1;
            return 0;
        }
    }
    return -1;
}

void ramfs_list(void) {
    vga_puts("RAMFS Directory Listing:\n");
    int count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            vga_puts("  - ");
            vga_puts(files[i].name);
            vga_puts(" (");
            vga_printf("%d", files[i].size);
            vga_puts(" bytes)\n");
            count++;
        }
    }
    if (count == 0) {
        vga_puts("  (No files found)\n");
    }
}

int ramfs_read(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, filename) == 0) {
            vga_puts((char *)files[i].data);
            vga_puts("\n");
            return 0;
        }
    }
    vga_puts("File not found!\n");
    return -1;
}
