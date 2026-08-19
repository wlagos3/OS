#include "pl011.h"

#define PL011_BASE  0x09000000UL

#define PL011_DR    (*(volatile unsigned int *)(PL011_BASE + 0x00))  /* data */
#define PL011_FR    (*(volatile unsigned int *)(PL011_BASE + 0x18))  /* flags */
#define PL011_IBRD  (*(volatile unsigned int *)(PL011_BASE + 0x24))  /* baud, integer part */
#define PL011_FBRD  (*(volatile unsigned int *)(PL011_BASE + 0x28))  /* baud, fractional part */
#define PL011_LCR_H (*(volatile unsigned int *)(PL011_BASE + 0x2C))  /* line control */
#define PL011_CR    (*(volatile unsigned int *)(PL011_BASE + 0x30))  /* control */
#define PL011_ICR   (*(volatile unsigned int *)(PL011_BASE + 0x44))  /* interrupt clear */

#define PL011_FR_BUSY      (1 << 3)   /* still transmitting */
#define PL011_FR_TXFF      (1 << 5)   /* transmit FIFO full */
#define PL011_LCR_H_FEN    (1 << 4)   /* enable FIFOs */
#define PL011_LCR_H_WLEN8  (3 << 5)   /* 8 data bits */
#define PL011_CR_UARTEN    (1 << 0)
#define PL011_CR_TXE       (1 << 8)
#define PL011_CR_RXE       (1 << 9)
#define PL011_ICR_ALL      0x7FF


void pl011_init(void) {
    PL011_CR = 0;
    while (PL011_FR & PL011_FR_BUSY) {}
    PL011_IBRD = 13;
    PL011_FBRD = 1;
    PL011_LCR_H = PL011_LCR_H_WLEN8 | PL011_LCR_H_FEN;
    PL011_ICR = PL011_ICR_ALL;
    PL011_CR = PL011_CR_UARTEN | PL011_CR_TXE | PL011_CR_RXE;
}

void pl011_putc(char c) {
    while (PL011_FR & PL011_FR_TXFF) {}
    PL011_DR = c;
}

void pl011_puts(const char *s) {
    while (*s) {
        pl011_putc(*s++);
    }
}
