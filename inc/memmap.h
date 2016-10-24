#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

#include <stdint.h>

#define MEMMAP_SIZE 32
#define MEMMAP_BIT 6
#define LENGTH_OFFSET 44
#define RESERVED 0
#define ADDR_OFFSET 48
#define AVAILABLE 1

struct memory_map_descriptor {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} __attribute__((packed));

extern struct memory_map_descriptor memory_map_descriptors[MEMMAP_SIZE];

void get_memory_map(void);
uint64_t memory_map_allocate(uint64_t size);
uint32_t get_memory_map_size(void);
uint64_t get_memory_size(void);
void print_memory_map(void);

#endif
