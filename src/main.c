#include "desc.h"
#include "ints.h"
#include "print.h"
#include "backtrace.h"
#include "serial.h"
#include "interrupt.h"
#include "timer.h"
#include "memmap.h"
#include "buddy.h"
#include "slab.h"
#include "paging.h"
#include "test.h"


static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

void init(void) {
	init_serial();
	init_interrupts();
}

void main(void)
{

	qemu_gdb_hang();

	init();
	init_timer(0xFFFF);

	get_memory_map();
	print_memory_map();

	init_buddy();
	init_paging();
	init_slab_allocator();
	
	test_buddy();
	test_slab();
	
	while (1);
}
