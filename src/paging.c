#include "paging.h"               
#include "utilmemory.h"
#include "start_allocator.h"
#include "memmap.h"
#include "utilities.h"

void init_entry(pte_t *entry, uint32_t flags) {
	if (pte_present(*entry))
		return;
	*entry = pa(start_allocate_with_alignment(PAGE_SIZE, PAGE_SIZE)) | flags;
}                          

void make_paging(uint64_t physical_address, uint64_t virtual_address, uint64_t size, pte_t *pml4) {
	for (; size > 0; physical_address += BIG_PAGE_SIZE, virtual_address += BIG_PAGE_SIZE, size -= BIG_PAGE_SIZE) {
		pte_t *pml4e = (pte_t*) pml4 + pml4_i(virtual_address);
		init_entry(pml4e, PTE_PRESENT | PTE_WRITE);
		pte_t *pdpte = (pte_t*) va(pte_phys(*pml4e)) + pml3_i(virtual_address);
		init_entry(pdpte, PTE_PRESENT | PTE_WRITE);
		pte_t *pde = (pte_t*) va(pte_phys(*pdpte)) + pml2_i(virtual_address);
		*pde = physical_address | PTE_PRESENT | PTE_WRITE | PTE_LARGE;
		flush_tlb_addr(virtual_address);
	}
}

void init_after_canonical_hole_mapping(pte_t *pml4) {
	make_paging(PHYSICAL_BASE, HIGH_BASE, get_memory_size(), pml4);
}
                                      	
void init_kernel_mapping(pte_t *pml4) {
	make_paging(PHYSICAL_BASE, VIRTUAL_BASE, KERNEL_SIZE, pml4);
}

void init_paging(void) {
	start_allocator_init((ceil(KERNEL_SIZE, BIG_PAGE_SIZE) + ceil(get_memory_size(), BIG_PAGE_SIZE)) * PAGE_SIZE * 2);
	pte_t *pml4 = (pte_t*) start_allocate_with_alignment(PAGE_SIZE, PAGE_SIZE);
	init_after_canonical_hole_mapping(pml4);
	init_kernel_mapping(pml4);
	store_pml4(pa(pml4));
}
