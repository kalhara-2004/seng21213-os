#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

#define PMM_BLOCK_SIZE 4096

void pmm_init(uint32_t mem_size);
void *pmm_alloc_block(void);
void pmm_free_block(void *p);
uint32_t pmm_get_free_block_count(void);

#endif

