#include "start_allocator.h"

#include "memmap.h"
#include "utilmemory.h"
#include "paging.h"
#include "utilities.h"

void *address;

void start_allocator_init(uint64_t size) {
	address = va(memory_map_allocate(size));
}

void* start_allocate(uint64_t size) {
	void *old_address = address;
	address = (void*) ((uint8_t*) address + size);
	return old_address;	                                        	
}

void* start_allocate_with_alignment(uint64_t size, uint64_t alignment) {
	address = align(address, alignment);
	return start_allocate(size);
}
