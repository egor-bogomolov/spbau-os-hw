#include <desc.h>
#include <ints.h>
#include "serial.h"
#include "interrupt.h"
#include "timer.h"


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

	puts("Initialization..");
	init();
	puts("Done");
	
	__asm__ volatile("int $255"); // nothing
	__asm__ volatile("int $39"); // master
	__asm__ volatile("int $40"); // slave + master
	init_timer(0xFF);
	
	
	while (1);
}
