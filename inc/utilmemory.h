#ifndef __UTILMEMORY_H__
#define __UTILMEMORY_H__

#include "memory.h"
#include <stdint.h>

#define BIG_PAGE_SIZE (PAGE_SIZE << 9)
#define HIGH_BASE     0xffff800000000000
#define PHYSICAL_BASE 0x0000000000000000
#define KERNEL_SIZE   (1ull << 31)

#define KERNEL_PHYS(x) ((x) - VIRTUAL_BASE)
#define KERNEL_VIRT(x) ((x) + VIRTUAL_BASE)
#define PA(x)          ((x) - HIGH_BASE)
#define VA(x)          ((x) + HIGH_BASE)

#define BOOTMEM_SIZE   (4ull * 1024ull * 1024ull * 1024ull)

typedef uintptr_t phys_t;
typedef uintptr_t virt_t;

static inline uintptr_t kernel_phys(void *addr) {
	return KERNEL_PHYS((uintptr_t)addr);
}

static inline void *kernel_virt(uintptr_t addr) { 
	return (void *)KERNEL_VIRT(addr);
}

static inline phys_t pa(const void *addr) {
	return PA((virt_t)addr);
}

static inline void *va(phys_t addr) {
	return (void *)VA(addr);
}


#endif
