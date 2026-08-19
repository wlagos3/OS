#include "power.h"


void shutdown(void) {
    register unsigned long x0 asm("x0") = 0x84000008UL; /* PSCI SYSTEM_OFF */
    asm volatile("hvc #0" : : "r"(x0) : "memory");
    for (;;);
}

void exit(void) {
    shutdown();
}

