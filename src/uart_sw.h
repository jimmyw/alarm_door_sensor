#pragma once
#include <stdint.h>

void uartsw_init(void);
int uartsw_puts(const char *s);
int uartsw_putchar(int c);
int uartsw_puthex(unsigned char c);
int uartsw_putd16(int16_t n);