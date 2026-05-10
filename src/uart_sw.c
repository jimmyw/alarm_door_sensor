#include "uart_sw.h"
#include <iodefine.h>
#include <iodefine_ext.h>

/* Software UART TX on P60 (physical pin 11), open-drain with external pull-up. */

static volatile unsigned short g_uart_sw_bit_delay_ticks = 128U;

static void tx_delay(void) {
  volatile unsigned short i;
  for (i = 0U; i < g_uart_sw_bit_delay_ticks; i++) {
    NOP();
  }
}

int uartsw_putchar(int c) {
  unsigned char i;
  unsigned char data = (unsigned char)c;

  DI();

  P6_bit.no0 = 0U; /* Start bit */
  tx_delay();

  for (i = 0U; i < 8U; i++) {
    P6_bit.no0 = (unsigned char)(data & 0x01U);
    tx_delay();
    data >>= 1;
  }

  P6_bit.no0 = 1U; /* Stop bit */
  tx_delay();
  tx_delay();

  EI();
  return (unsigned char)c;
}

int uartsw_puts(const char *s) {
  while ('\0' != *s) {
    (void)uartsw_putchar(*s++);
  }
  return 1;
}

void uartsw_init(void) {
  P6_bit.no0 = 1U;
  PM6_bit.no0 = 0U;
}
