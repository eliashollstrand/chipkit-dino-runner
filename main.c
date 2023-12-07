/**
 * @file main.c
 * @brief Main file for the Chipkit Dino Runner game.
 * 
 * This file contains the main function and other supporting functions for the game.
 * It includes declarations for system-specific addresses, game states, and other necessary headers.
 * The game logic is implemented using a state machine, where the current state determines the behavior of the game.
 * The main function initializes the system, sets the initial game state to MENU_STATE, and reads the leaderboard.
 * It then enters an infinite loop, where the game logic and display updates are performed based on the current state.
 * 
 * @author Axel Isaksson
 * @author F Lundevall
 * @author Elias Hollstrand
 * @author Mattias Kvist
 * 
 * @date 2023-12-06
 * 
 * For copyright and licensing, see file COPYING.
 */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

GameState currentState;
int delay_counter;

/**
 * @brief Checks for input and performs corresponding actions based on the current state.
 * 
 * This function is responsible for checking the input buttons and performing the appropriate actions
 * based on the current state of the game. It checks for button presses and updates the game state
 * accordingly. The specific actions performed depend on the current state of the game.
 * 
 */
void check_for_input() {
	if (delay_counter >= 4) {
		switch (currentState) {
		case MENU_STATE:
			if(getbtns() & 0x4) { // BTN4
				change_state(GAME_STATE);
			}
			else if(getbtns() & 0x1) { // BTN2
				menu_page++;
			}

			delay_counter = 0;
			break;

		case GAME_OVER_STATE:
			if(getbtns() & 0x4) { // BTN4
				change_state(GAME_STATE);
			}
			else if(getbtns() & 0x1) { // BTN2
				change_state(MENU_STATE);
			}

			delay_counter = 0;
			break;

		case ENTER_NAME_STATE:
			if(getbtns() & 0x4) {
				initials[letter_index]++;
				if(initials[letter_index] > 'z') {
					initials[letter_index] = 'a';
				}
			}
			else if(getbtns() & 0x2) {
				insert_initials(initials, leaderboard_index);
				change_state(GAME_OVER_STATE);
			}
			else if(getbtns() & 0x1) {
				letter_index = (letter_index + 1) % 3;
			}

			delay_counter = 0;
			break;

		default:
			break;
		}
	}
}

/**
 * @brief Interrupt service routine for handling interrupts.
 * 
 * This function is called when an interrupt is triggered. For Timer 2 interrupts it 
 * increments the delay_counter, and based on the current state, it performs different actions:
 * - MENU_STATE: Checks for user input and draws the menu.
 * - GAME_STATE: Updates the game logic and updates the display.
 * - GAME_OVER_STATE: Checks for user input and draws the game over screen.
 * - ENTER_NAME_STATE: Checks for user input and draws the enter name screen. Reads the leaderboard.
 */
void user_isr(void) {
	if(IFS(0) & 0x100) { // Timer 2 interrupt
		IFSCLR(0) = 0x100;
		delay_counter++;

		switch(currentState) {
		case MENU_STATE:
			check_for_input();
			draw_menu();
			break;

		case GAME_STATE:
			update_game();
			update_display();
			break;

		case GAME_OVER_STATE:
			check_for_input();
			draw_gameover();
			break;

		case ENTER_NAME_STATE:
			check_for_input();
			draw_enter_name();
			read_multiple_scores(leaderboard_scores, NUM_LEADERBOARD_ENTRIES);
			break;

		default:
			break;
		}
	}
}

/**
 * Changes the state of the game to the specified new state.
 * Resets the game and initializes the delay counter.
 *
 * @param newState The new state to change to.
 */
void change_state(GameState newState) {
	currentState = newState;
	delay_counter = 0;

	switch(currentState) {
		case GAME_STATE:
			reset_game();
			break;

		default:
			break;
	}
}

/**
 * @brief The main function of the program.
 * 
 * This function initializes the chip, sets the initial state to MENU_STATE,
 * reads the leaderboard scores, and sets the highscore to the first score in the leaderboard.
 * It then enters an infinite loop to keep the program running.
 * 
 * @return 0 indicating successful program execution. (This is never reached.)
 */
int main(void) {
	chip_init(); // Set up timers, interrupts, input and outputs, display, I2C etc.
	currentState = MENU_STATE;
	read_leaderboard();
	highscore = leaderboard_scores[0];

	while (1) {} // Infinite loop to keep the program running
	return 0;
}