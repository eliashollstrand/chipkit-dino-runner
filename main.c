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

GameState currentState;

check_for_input()
{
	switch (currentState)
	{
	case MENU_STATE:
		if (getbtns() & 0x4)
		{
			change_state(GAME_STATE);
		}
		else if (getbtns() & 0x1)
		{
			menu_page++;
		}
		break;
	
	case GAME_OVER_STATE:
		if (getbtns() & 0x4)
		{
			change_state(GAME_STATE);
		}
		else if (getbtns() & 0x1)
		{
			change_state(MENU_STATE);
		}
		break;

	default:
		break;
	}
}

void user_isr(void)
{
	if (IFS(0) & 0x100) // Timer 2 interrupt
	{
		// Clear the interrupt flag
		IFSCLR(0) = 0x100;

		switch (currentState)
		{
		case MENU_STATE:
			check_for_input();
			draw_menu();
			break;

		case GAME_STATE:
			// Update game logic
			update_game();

			// Update the display
			update_display();
			break;

		case GAME_OVER_STATE:
			check_for_input();
			draw_gameover();
			break;

		default:
			break;
		}
	}
}

void change_state(GameState newState)
{
	currentState = newState;
	switch (currentState)
	{
	case GAME_STATE:
		// Reset game variables
		reset_game();
		break;

	default:
		break;
	}
}

int main(void)
{
	/* Set up timers, interrupts, input and outputs, display, I2C etc. */
	chip_init();
	currentState = MENU_STATE;
	highscore = read_highscore();

	// highscore = read_highscore();

	while (1)
	{
	}
	return 0;
}
