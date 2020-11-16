#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void)
{
    /* Reading bits 11 through 8 of PORTD and shifting them to the end - 0000 0000 0000 XXXX; X can be 1 or 0 depending on the status */
    return (PORTD >> 8) & 0xF;
}

int getbtns(void)
{
    /* Reading bits 7 through 5 of PORTD and shifting them to the end - 0000 0000 0000 0XXX; X can be 1 or 0 depending on the status */
    return (PORTD >> 5) & 0x7; // 0 1 1 1
}