/*
Authors:
Axel Isaksson,
F Lundevall,
Elias Hollstrand,
Mattias Kvist

Date: 2023-11-01

For copyright and licensing, see file COPYING
*/

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

int main(void)
{
	/* Set up timers, interrupts, input and outputs, display, I2C etc. */
	chip_init();


	score_init();

	while (1)
	{
	}
	return 0;
}
