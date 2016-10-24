#include "test.h"
#include "buddy.h"
#include "slab.h"
#include "utilmemory.h"
#include "print.h"

#define TESTS 5

void test_buddy(void) {
	for (int i = 0; i < TESTS; i++) {
		void *p0 = allocate_empty_page(0);
		void *p1 = allocate_empty_page(1);
		void *p2 = allocate_empty_page(2);
		void *p3 = allocate_empty_page(3);
		void *p4 = allocate_empty_page(4);
		printf("%p %p %p %p %p\n", p0, p1, p2, p3, p4);
		free_page(p3, 3);
		free_page(p2, 2);
		free_page(p4, 4);
		free_page(p1, 1);
		free_page(p0, 0);
	}
}

void *pointers_small[TESTS];
void *pointers_big[TESTS];

void test_slab(void) {
	struct slab_allocator *allocator_small = create_slab_allocator(8, 1);
	struct slab_allocator *allocator_big = create_slab_allocator(PAGE_SIZE / 8, PAGE_SIZE / 2);
	for (uint32_t i = 0; i < TESTS; i++) {
		pointers_small[i] = slab_allocator_allocate(allocator_small);
		pointers_big[i] = slab_allocator_allocate(allocator_big);
		printf("%p %p\n", pointers_small[i], pointers_big[i]);
	}
	for (uint32_t i = 0; i < TESTS; i++) {
		slab_allocator_free(pointers_small[i]);
		slab_allocator_free(pointers_big[i]);
	}	
}
