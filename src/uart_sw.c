#include "uart_sw.h"
#include <iodefine.h>
#include <iodefine_ext.h>

/* Software UART TX on P60 (physical pin 11), open-drain with external pull-up.
 */

#define NOP_COUNT 162U
#define PORT P2_bit.no2
#define PORTM PM2_bit.no2

static void tx_delay(void) {
  volatile unsigned short i;
  for (i = 0U; i < NOP_COUNT; i++) {
    NOP();
  }
}

int uartsw_putchar(int c) {
  unsigned char i;
  unsigned char data = (unsigned char)c;

  DI();

  PORT = 0U; /* Start bit */
  tx_delay();

  for (i = 0U; i < 8U; i++) {
    PORT = (unsigned char)(data & 0x01U);
    tx_delay();
    data >>= 1;
  }

  PORT = 1U; /* Stop bit */
  tx_delay();
  tx_delay();

  EI();
  return (unsigned char)c;
}

int uartsw_puthex(unsigned char c) {
  const char hex[] = "0123456789ABCDEF";
  uartsw_putchar(hex[c >> 4]);
  uartsw_putchar(hex[c & 0x0F]);
  return 2;
}

int uartsw_puts(const char *s) {
  while ('\0' != *s) {
    (void)uartsw_putchar(*s++);
  }
  return 1;
}

void uartsw_init(void) {
  PORT = 1U;
  PORTM = 0U;
}
