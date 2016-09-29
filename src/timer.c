#include "timer.h"
#include "interrupt.h"
#include "ioport.h"

#define LAST8 0xFF

void init_timer(uint16_t div) {
	set_master_bit(0, 1);
	
	out8(TIMER_COMMAND_PORT, (2 << 1) | (3 << 4));
	out8(TIMER_DATA_PORT, div & LAST8);
	out8(TIMER_DATA_PORT, (div >> 8) & LAST8);
	
	set_master_bit(0, 0);
}
