#include "memmap.h"
#include "print.h"
#include "serial.h"
#include "utilities.h"

extern const uint32_t multiboot_info;
extern char text_phys_begin[];
extern char bss_phys_end[];

struct size_descriptor {
  	uint32_t size;
  	struct memory_map_descriptor descriptor;
} __attribute__((packed));

struct memory_map_descriptor memory_map_descriptors[MEMMAP_SIZE];
uint32_t memory_map_size = 0;

void add_descriptor(uint32_t *index, uint64_t base_addr, uint64_t length, uint32_t type) {
	if (length == 0)
		return;
	memory_map_descriptors[*index].base_addr = base_addr;
	memory_map_descriptors[*index].length = length;
	memory_map_descriptors[*index].type = type;
	(*index)++;
}              

void get_memory_map(void) {
	uint32_t flags = *(uint32_t*) (uintptr_t) multiboot_info;
	if (get_bit(flags, MEMMAP_BIT) == 0) {
		puts("Unable to get memory map");
		return;
	}
	uint64_t kernel_begin = (uint64_t) text_phys_begin;
	uint64_t kernel_end = (uint64_t) bss_phys_end;
	uint32_t memory_map_length = *(uint32_t*) (uintptr_t) (multiboot_info + LENGTH_OFFSET);
	uint32_t memory_map_addr = *(uint32_t*) (uintptr_t) (multiboot_info + ADDR_OFFSET);	
	for (struct size_descriptor *desc = (struct size_descriptor*) (uintptr_t) memory_map_addr; 
		(uint64_t) desc < memory_map_addr + memory_map_length; 
		desc = (struct size_descriptor*) (uintptr_t) ((uintptr_t) desc + desc->size + sizeof(desc->size)))
	{
		struct memory_map_descriptor *descriptor = &desc->descriptor;
		uint64_t descriptor_begin = descriptor->base_addr;
		uint64_t descriptor_end = descriptor->base_addr + descriptor->length;
		if (descriptor_begin >= kernel_end || kernel_begin >= descriptor_end) {
		 	add_descriptor(&memory_map_size, descriptor_begin, descriptor->length, descriptor->type);
		 	continue;
		}
		if (descriptor_begin <= kernel_begin && kernel_begin < descriptor_end) {
			add_descriptor(&memory_map_size, descriptor_begin, kernel_begin - descriptor_begin, descriptor->type);
			if (kernel_end < descriptor_end)
				add_descriptor(&memory_map_size, kernel_end, descriptor_end - kernel_end, descriptor->type);
	   	}
	   	if (descriptor_begin >= kernel_begin && descriptor_begin < kernel_end && descriptor_end > kernel_end) {
			add_descriptor(&memory_map_size, kernel_end, descriptor_end - kernel_end, descriptor->type);
		}
	}
	add_descriptor(&memory_map_size, kernel_begin, kernel_end - kernel_begin, RESERVED);
}

uint32_t get_memory_map_size(void) {
	return memory_map_size;
}

uint64_t memory_map_allocate(uint64_t size) {
 	for (uint32_t i = 0; i < memory_map_size; i++) {
 		struct memory_map_descriptor *descriptor = &memory_map_descriptors[i];
 		if (descriptor->type == AVAILABLE && descriptor->length >= size) {
 			descriptor->length -= size;
 			add_descriptor(&memory_map_size, descriptor->base_addr + descriptor->length, size, RESERVED);
 			return descriptor->base_addr + descriptor->length;
 		}
 	}
 	return 0;
}

uint64_t get_memory_size(void) {
	uint64_t memory_size = 0;
	for (uint32_t i = 0; i < memory_map_size; i++) {
		if (memory_size < memory_map_descriptors[i].base_addr + memory_map_descriptors[i].length) {
			memory_size = memory_map_descriptors[i].base_addr + memory_map_descriptors[i].length;
		}
	}
	return memory_size;
}

void print_memory_map(void) {
 	for (uint32_t i = 0; i < memory_map_size; i++) {
 		struct memory_map_descriptor *descriptor = &memory_map_descriptors[i];
 		if (descriptor->type != 0)
 	 		printf("[%llx, %llx]: type = %u\n", descriptor->base_addr, descriptor->base_addr + descriptor->length - 1, descriptor->type);
 	 	else
 			printf("[%llx, %llx]: type = kernel\n", descriptor->base_addr, descriptor->base_addr + descriptor->length - 1); 
 	}
}
