#ifndef __START_ALLOCATOR_H__
#define __START_ALLOCATOR_H__

#include <stdint.h>

void  start_allocator_init(uint64_t size);
void* start_allocate(uint64_t size);
void* start_allocate_with_alignment(uint64_t size, uint64_t alignment);

#endif
