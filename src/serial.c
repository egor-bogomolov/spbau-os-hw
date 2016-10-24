#include "serial.h"
#include "ioport.h"

#define BASE_PORT 0x3F8

#define INT_PORT (BASE_PORT + 1)
#define SPEED_PORT_L (BASE_PORT + 0)
#define SPEED_PORT_H (BASE_PORT + 1)
#define INFO_PORT (BASE_PORT + 3)
#define STATUS_PORT (BASE_PORT + 5)
#define IO_PORT (BASE_PORT + 0)

#define DIV_ON (1 << 7)
#define INT_OFF 0
#define SERIAL_INFO 3
#define SPEED 0x000C

#define CAN_WRITE (1 << 5)

#define LAST8 0xFF

void init_serial(void) {
	out8(INFO_PORT, DIV_ON);
	out8(SPEED_PORT_L, SPEED & LAST8);
	out8(SPEED_PORT_H, (SPEED >> 8) & LAST8);
	out8(INFO_PORT, SERIAL_INFO);
	out8(INT_PORT, INT_OFF);
}

void putc(char c) {
	out8(IO_PORT, c);
	while(!(in8(CAN_WRITE & STATUS_PORT)));
}

void puts(const char *s) {
	while(*s) {
		putc(*s);
		++s;
	}
	putc('\n');
}

void serial_write(const char *s, size_t size) {
	while(size--) {
		putc(*s);
		++s;
	}
}
