#include "interrupt.h"
#include "ioport.h"
#include "ints.h"
#include "desc.h"
#include "memory.h"
#include "serial.h"

#define LAST8 0xFF
#define LAST16 0xFFFF
#define LAST32 0xFFFFFFFF

#define INT_GATE (14 << 8)
#define VALID_ENTRY (1 << 15)

#define MASTER_COMMAND_PORT 0x20
#define SLAVE_COMMAND_PORT 0xA0
#define MASTER_DATA_PORT 0x21
#define SLAVE_DATA_PORT 0xA1

#define INIT_COMMAND ((1 << 4) | 1)

extern uint64_t int_handler[];	
struct idt_entry idt[INT_NUM];

unsigned char master_mask;
unsigned char slave_mask;

void set_master_mask(unsigned char mask) {
	master_mask = mask;
	out8(MASTER_DATA_PORT, mask);
}

void set_slave_mask(unsigned char mask) {
	slave_mask = mask;
	out8(SLAVE_DATA_PORT, mask);
}

void set_master_bit(unsigned char bit, unsigned char value) {
	if (value)
		set_master_mask(master_mask | (1 << bit));
	else
		set_master_mask(master_mask & (LAST8 ^ (1 << bit)));
}

void set_slave_bit(unsigned char bit, unsigned char value) {
	if (value)
		set_slave_mask(slave_mask | (1 << bit));
	else
		set_slave_mask(slave_mask & (LAST8 ^ (1 << bit)));
}

void set_idt_entry(int ind, uint64_t entry) {
	idt[ind].offset1 = entry & LAST16;
	idt[ind].seg_sel = KERNEL_CS;
	idt[ind].info = INT_GATE | VALID_ENTRY;
	idt[ind].offset2 = (entry >> 16) & LAST16;
	idt[ind].offset3 = (entry >> 32) & LAST32;
	idt[ind].reserved = 0;
}

void init_idt(void) {
	for (int i = 0; i < INT_NUM; ++i) {
		set_idt_entry(i, int_handler[i]);	
	}
	struct desc_table_ptr idtr;
	idtr.addr = (uint64_t)idt;
	idtr.size = sizeof(idt) - 1;
	write_idtr(&idtr);
}

void init_pic(void) {
	disable_ints();
	
	out8(MASTER_COMMAND_PORT, INIT_COMMAND);
	out8(MASTER_DATA_PORT, 32); 
	out8(MASTER_DATA_PORT, (1 << 2));
	out8(MASTER_DATA_PORT, 1);
	out8(SLAVE_COMMAND_PORT, INIT_COMMAND);
	out8(SLAVE_DATA_PORT, 40); 
	out8(SLAVE_DATA_PORT, 2);
	out8(SLAVE_DATA_PORT, 1);	
	
	set_slave_mask(LAST8);
	set_master_mask(LAST8 ^ (1 << 2));
	
	enable_ints();
}

void init_interrupts(void) {
	init_idt();
	init_pic();
}

void master_eoi(void) {
	out8(MASTER_COMMAND_PORT, (1 << 5));
}

void slave_eoi(void) {
	out8(SLAVE_COMMAND_PORT, (1 << 5));
}

void handler(uint64_t ind) {
	puts("Trying to handle..");
	if (32 <= ind && ind < 40) {
		puts("Master");
		master_eoi();
	}
	if (40 <= ind && ind < 48) {
		puts("Slave");
		master_eoi();
		slave_eoi();
	}
	puts("Done.");
} 
