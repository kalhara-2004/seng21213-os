#include "vmm.h"
#include "string.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void vmm_init(void) {
    // Clear directory
    memset(page_directory, 0, sizeof(page_directory));

    // Identity map the first 4MB (1024 entries * 4KB pages)
    for (uint32_t i = 0; i < 1024; i++) {
        // supervisor level, read/write, present
        first_page_table[i] = (i * 0x1000) | 3;
    }

    // Set first page directory entry
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    // Load page directory into CR3
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(page_directory));

    // Enable paging bit (PG flag, bit 31) in CR0
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));
}
