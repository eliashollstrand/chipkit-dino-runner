/* 
Authors: 
Elias Hollstrand,
Mattias Kvist

Date: 2023-11-01

For copyright and licensing, see file COPYING 
*/

#include <stdint.h>
#include <pic32mx.h>
#include "declare.h"

int getsw(void) {
    // - Shift the bits of PORTD right by 8 positions
    // - Perform bitwise AND to extract only the four least significant bits of the shifted result, 
    // ensuring that the bits for switches SW4, SW3, SW2, and SW1 are preserved, and all other bits are set to zero.
    return (PORTD >> 8) & 0xf;
}

int getbtns(void) {
    // return an int with 1 in the position of the button(s) pressed

    // Perform bitwise AND to extract only the three least significant bits, 
    // ensuring that the bits for the buttons are preserved, and all other bits are set to zero.
    return (PORTD >> 5) & 0x7;
}
