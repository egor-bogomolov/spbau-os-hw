#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#define TIMER_COMMAND_PORT 0x43
#define TIMER_DATA_PORT 0x40

void init_timer(uint16_t div);

#endif
