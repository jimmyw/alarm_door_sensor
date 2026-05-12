#ifndef CC1101_H
#define CC1101_H

#include <stdint.h>

/* Command strobes */
#define CC1101_SRES 0x30
#define CC1101_SFSTXON 0x31
#define CC1101_SXOFF 0x32
#define CC1101_SCAL 0x33
#define CC1101_SRX 0x34
#define CC1101_STX 0x35
#define CC1101_SIDLE 0x36
#define CC1101_SPWD 0x39
#define CC1101_SFRX 0x3A
#define CC1101_SFTX 0x3B
#define CC1101_SNOP 0x3D

/* Status register read (OR with 0xC0) */
#define CC1101_PARTNUM 0x30
#define CC1101_VERSION 0x31
#define CC1101_MARCSTATE 0x35
#define CC1101_TXBYTES 0x3A
#define CC1101_RXBYTES 0x3B

/* MARCSTATE values */
#define MARCSTATE_IDLE 0x01
#define MARCSTATE_RX 0x0D
#define MARCSTATE_TX 0x13
#define MARCSTATE_RXFIFO_OVERFLOW 0x11
#define MARCSTATE_TXFIFO_UNDERFLOW 0x16

/* FIFO addresses */
#define CC1101_TXFIFO 0x3F
#define CC1101_RXFIFO 0x3F

void cc1101_init(void);
void cc1101_reset(void);
uint8_t cc1101_strobe(uint8_t strobe);
void cc1101_write_reg(uint8_t addr, uint8_t value);
uint8_t cc1101_read_reg(uint8_t addr);
uint8_t cc1101_read_status(uint8_t addr);
void cc1101_write_burst(uint8_t addr, const uint8_t *data, uint8_t len);
void cc1101_read_burst(uint8_t addr, uint8_t *data, uint8_t len);
void cc1101_tx_packet(const uint8_t *data, uint8_t len);
void cc1101_idle(void);
void cc1101_powerdown(void);

#endif
