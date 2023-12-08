/**
 * @file buttons.c
 * @brief Button handling functions
 * 
 * This file contains the implementation of functions for retrieving the values of switches and buttons.
 * The functions in this file provide an interface for reading the state of switches SW4, SW3, SW2, and SW1,
 * as well as the state of buttons BTN4, BTN3, and BTN2.
 * 
 * @author Elias Hollstrand
 * @author Mattias Kvist
 * 
 * @date 2023-12-07
 * 
 * For copyright and licensing, see file COPYING.
 */

#include <stdint.h>
#include <pic32mx.h>
#include "declare.h"

/**
 * @brief Retrieves the value of the switches SW4, SW3, SW2, and SW1.
 * 
 * This function shifts the bits of PORTD right by 8 positions and performs a bitwise AND operation
 * with 0xf to extract only the four least significant bits of the shifted result. This ensures that
 * the bits for switches SW4, SW3, SW2, and SW1 are preserved, while all other bits are set to zero.
 * 
 * @return The value of the switches SW4, SW3, SW2, and SW1.
 */
int getsw(void) {
    return (PORTD >> 8) & 0xf;
}


/**
 * @brief Returns an integer with 1 in the position of the button(s) pressed, when converted to binary.
 * 
 * This function performs a bitwise AND operation to extract only the three least significant bits
 * from the PORTD register, ensuring that the bits for the buttons are preserved, and all other bits
 * are set to zero.
 * 
 * @return An integer representing the button(s) pressed.
 */
int getbtns(void) {
    return (PORTD >> 5) & 0x7;
}
