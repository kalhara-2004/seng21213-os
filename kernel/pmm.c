#include "pmm.h"
#include "string.h"

#define BLOCKS_PER_BYTE 8

static uint8_t *pmm_bitmap;
static uint32_t pmm_max_blocks;
static uint32_t pmm_used_blocks;

static uint8_t bitmap_array[1024];

static inline void mmap_set(uint32_t bit) {
    bitmap_array[bit / 8] |= (1 << (bit % 8));
}

static inline void mmap_unset(uint32_t bit) {
    bitmap_array[bit / 8] &= ~(1 << (bit % 8));
}

static inline int mmap_test(uint32_t bit) {
    return bitmap_array[bit / 8] & (1 << (bit % 8));
}

void pmm_init(uint32_t mem_size) {
    pmm_max_blocks = mem_size / PMM_BLOCK_SIZE;
    pmm_used_blocks = pmm_max_blocks;
    memset(bitmap_array, 0xFF, sizeof(bitmap_array));
    
    for (uint32_t i = 256; i < pmm_max_blocks && i < 8192; i++) {
        mmap_unset(i);
        pmm_used_blocks--;
    }
}

void *pmm_alloc_block(void) {
    for (uint32_t i = 0; i < pmm_max_blocks; i++) {
        if (!mmap_test(i)) {
            mmap_set(i);
            pmm_used_blocks++;
            return (void *)(i * PMM_BLOCK_SIZE);
        }
    }
    return 0;
}

void pmm_free_block(void *p) {
    uint32_t addr = (uint32_t)p;
    uint32_t block = addr / PMM_BLOCK_SIZE;
    if (mmap_test(block)) {
        mmap_unset(block);
        pmm_used_blocks--;
    }
}

uint32_t pmm_get_free_block_count(void) {
    return pmm_max_blocks - pmm_used_blocks;
}
