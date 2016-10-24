#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stddef.h>
#include <stdint.h>

void init_serial(void);
void putc(char c);
void puts(const char *s);
void serial_write(const char *s, size_t size);

#endif
