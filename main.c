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
int delay_counter;

check_for_input()
{
	if (delay_counter >= 4)
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

			delay_counter = 0;
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

			delay_counter = 0;
			break;

		case ENTER_NAME_STATE:
			if (getbtns() & 0x4) {
				initials[letter_index]++;
				if (initials[letter_index] > 'z') {
					initials[letter_index] = 'a';
				}
			}
			else if (getbtns() & 0x2) {
				insert_initials(initials, 0);
				change_state(GAME_OVER_STATE);
			}
			else if (getbtns() & 0x1) {
				letter_index = (letter_index + 1)%3;
			}

			delay_counter = 0;
			break;

		default:
			break;
		}
	}
}

void user_isr(void)
{
	if (IFS(0) & 0x100) // Timer 2 interrupt
	{
		// Clear the interrupt flag
		IFSCLR(0) = 0x100;
		delay_counter++;

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

		case ENTER_NAME_STATE:
			check_for_input();
			draw_enter_name();
			break;

		default:
			break;
		}
	}
}

void change_state(GameState newState)
{
	currentState = newState;
	delay_counter = 0;
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

// char test[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH] = {{'a', 'b', 'c'}, {'d', 'e', 'f'}, {'g', 'h', 'i'}, {'j', 'k', 'l'}, {'m', 'n', 'o'}};
char test[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH] = {"abc", "def", "ghi", "jkl", "mno", "pqr"};

int main(void)
{
	/* Set up timers, interrupts, input and outputs, display, I2C etc. */
	chip_init();
	currentState = MENU_STATE;
	read_leaderboard();
	highscore = leaderboard_scores[0];

	

	while (1)
	{
	}
	return 0;
}
