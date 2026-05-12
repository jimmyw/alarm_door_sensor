#include "cc1101.h"
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include <stddef.h>

/* CS pin: P2.3 */
#define CS_LOW()  (P2_bit.no3 = 0)
#define CS_HIGH() (P2_bit.no3 = 1)
#define MISO_PIN  (P1_bit.no1)

extern MD_STATUS R_CSI00_Send_Receive_Sync(uint8_t *const tx_buf,
                                           uint16_t tx_num,
                                           uint8_t *const rx_buf);

/*
 * 868 MHz, 2-FSK, 1.2 kbps, 5.2 kHz deviation
 * RX BW 58 kHz, 4-byte preamble, 32-bit sync word
 * Variable packet length, CRC, append status
 * 26 MHz crystal assumed
 */
static const uint8_t cc1101_config[][2] = {
    {0x02, 0x06}, /* IOCFG0  — GDO0: asserts on sync, deasserts end-of-packet */
    {0x03, 0x47}, /* FIFOTHR — RX FIFO threshold 33 bytes */
    {0x08, 0x05}, /* PKTCTRL0 — variable length, CRC on, whitening on */
    {0x09, 0x04}, /* PKTCTRL1 — append status (RSSI+LQI), no addr check */
    {0x0B, 0x06}, /* FSCTRL1 — IF frequency 152.3 kHz */
    {0x0D, 0x21}, /* FREQ2   — 868.0 MHz */
    {0x0E, 0x62}, /* FREQ1   */
    {0x0F, 0x76}, /* FREQ0   */
    {0x10, 0xF5}, /* MDMCFG4 — RX BW 58 kHz, DRATE_E=5 */
    {0x11, 0x83}, /* MDMCFG3 — DRATE_M=131 → 1.2 kbps */
    {0x12, 0x13}, /* MDMCFG2 — 2-FSK, 30/32 sync detect */
    {0x13, 0x22}, /* MDMCFG1 — 4-byte preamble, FEC off */
    {0x15, 0x15}, /* DEVIATN — 5.2 kHz deviation */
    {0x18, 0x18}, /* MCSM0   — auto-cal from IDLE→RX/TX */
    {0x19, 0x16}, /* FOCCFG  — freq offset compensation */
    {0x1A, 0x6C}, /* BSCFG   — bit sync config */
    {0x1B, 0x03}, /* AGCCTRL2 — AGC */
    {0x1C, 0x40}, /* AGCCTRL1 */
    {0x1D, 0x91}, /* AGCCTRL0 */
    {0x21, 0xB6}, /* FREND1  — front-end RX */
    {0x22, 0x10}, /* FREND0  — front-end TX, PA index 0 */
    {0x23, 0xE9}, /* FSCAL3  — freq synth cal */
    {0x24, 0x2A}, /* FSCAL2  */
    {0x25, 0x00}, /* FSCAL1  */
    {0x26, 0x1F}, /* FSCAL0  */
    {0x29, 0x59}, /* FSTEST  — for production test only */
    {0x2C, 0x81}, /* TEST2   */
    {0x2D, 0x35}, /* TEST1   */
    {0x2E, 0x09}, /* TEST0   */
};

#define CC1101_CONFIG_COUNT (sizeof(cc1101_config) / sizeof(cc1101_config[0]))

/* PA table: +10 dBm for 868 MHz */
static const uint8_t pa_table[8] = {0xC0, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00};

void cc1101_reset(void) {
    CS_HIGH();
    NOP();
    CS_LOW();
    while (MISO_PIN)
        ;
    uint8_t sres = CC1101_SRES;
    R_CSI00_Send_Receive_Sync(&sres, 1, NULL);
    while (MISO_PIN)
        ;
    CS_HIGH();
}

uint8_t cc1101_strobe(uint8_t strobe) {
    uint8_t status;
    CS_LOW();
    R_CSI00_Send_Receive_Sync(&strobe, 1, &status);
    CS_HIGH();
    return status;
}

void cc1101_write_reg(uint8_t addr, uint8_t value) {
    uint8_t tx[2];
    tx[0] = addr & 0x3F; /* write, single */
    tx[1] = value;
    CS_LOW();
    R_CSI00_Send_Receive_Sync(tx, 2, NULL);
    CS_HIGH();
}

uint8_t cc1101_read_reg(uint8_t addr) {
    uint8_t tx[2], rx[2];
    tx[0] = (addr & 0x3F) | 0x80; /* read, single */
    tx[1] = 0x00;
    CS_LOW();
    R_CSI00_Send_Receive_Sync(tx, 2, rx);
    CS_HIGH();
    return rx[1];
}

uint8_t cc1101_read_status(uint8_t addr) {
    uint8_t tx[2], rx[2];
    tx[0] = (addr & 0x3F) | 0xC0; /* read, burst (status register) */
    tx[1] = 0x00;
    CS_LOW();
    R_CSI00_Send_Receive_Sync(tx, 2, rx);
    CS_HIGH();
    return rx[1];
}

void cc1101_write_burst(uint8_t addr, const uint8_t *data, uint8_t len) {
    uint8_t hdr = (addr & 0x3F) | 0x40; /* write, burst */
    CS_LOW();
    R_CSI00_Send_Receive_Sync(&hdr, 1, NULL);
    R_CSI00_Send_Receive_Sync((uint8_t *)data, len, NULL);
    CS_HIGH();
}

void cc1101_read_burst(uint8_t addr, uint8_t *data, uint8_t len) {
    uint8_t hdr = (addr & 0x3F) | 0xC0; /* read, burst */
    CS_LOW();
    R_CSI00_Send_Receive_Sync(&hdr, 1, NULL);
    R_CSI00_Send_Receive_Sync(NULL, len, data);
    CS_HIGH();
}

void cc1101_idle(void) {
    cc1101_strobe(CC1101_SIDLE);
    while ((cc1101_read_status(CC1101_MARCSTATE) & 0x1F) != MARCSTATE_IDLE)
        ;
}

void cc1101_init(void) {
    cc1101_reset();

    /* Write configuration registers */
    for (uint8_t i = 0; i < CC1101_CONFIG_COUNT; i++) {
        cc1101_write_reg(cc1101_config[i][0], cc1101_config[i][1]);
    }

    /* Write PA table */
    cc1101_write_burst(0x3E, pa_table, 8);
}

void cc1101_tx_packet(const uint8_t *data, uint8_t len) {
    cc1101_idle();
    cc1101_strobe(CC1101_SFTX);                  /* flush TX FIFO */
    cc1101_write_reg(CC1101_TXFIFO, len);         /* length byte */
    cc1101_write_burst(CC1101_TXFIFO, data, len); /* payload */
    cc1101_strobe(CC1101_STX);                    /* start TX */

    /* Wait for TX to finish (GDO0 goes high then low, or poll MARCSTATE) */
    while ((cc1101_read_status(CC1101_MARCSTATE) & 0x1F) == MARCSTATE_TX)
        ;
}
