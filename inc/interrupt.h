#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stdint.h>
#define INT_NUM 256

struct idt_entry {
	uint16_t offset1;
	uint16_t seg_sel;
	uint16_t info;
	uint16_t offset2;
	uint32_t offset3;
	uint32_t reserved;
} __attribute__((packed));

void set_master_mask(unsigned char mask);
void set_slave_mask(unsigned char mask);
void set_master_bit(unsigned char bit, unsigned char value);
void set_slave_bit(unsigned char bit, unsigned char value);

void set_idt_entry(int ind, uint64_t entry);

void init_idt(void);
void init_pic(void);
void init_interrupts(void);

void master_eoi(void);
void slave_eoi(void);
void handler(uint64_t ind);

#endif
