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

static int FPS = 10;

int character_x = 10;
int character_y = 10;
int i = 0;

void user_isr(void)
{
	if (IFS(0) & 0x100)
	{

		// Clear the interrupt flag
		IFSCLR(0) = 0x100;
		update_display();

		// Update the display
		// update_display();
	}

}

void update_display(void)
{
	// Clear the display
	clear_all_pixels();

	move_character();

	// Update the display
	display_objects();

	// Update the display
	// display_update();
}

void move_character()
{
	character_x++;
	fill_rectangle(character_x, character_y, 10, 10);
}

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
