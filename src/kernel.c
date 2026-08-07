#define UART0_BASE  0x09000000UL
#define UARTDR (*(volatile unsigned int *)(UART0_BASE))
#define UARTFR (*(volatile unsigned int *)(UART0_BASE + 0x18))

// UART
void send_uart_char(char c) {
    while (UARTFR & (1 << 5)) {}
    UARTDR = c;
}
void send_uart_string(const char *s) {
    while (*s) {
        send_uart_char(*s++);
    }
}

void kernel_main(void) {
    send_uart_string("Hello Computer");
    for (;;);
}