/**
 * @file gameover.c
 * @brief This file contains the implementation of the game over state.
 *
 * The game over state is responsible for displaying the game over screen
 * 
 * @author Mattias Kvist
 * 
 * @date 2023-12-07
 *
 * For copyright and licensing, see file COPYING.
 */
#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

int highscore;
int score;

/**
 * @brief Draws the game over screen with the score and highscore.
 * 
 * This function clears all pixels on the display and then draws the game over message,
 * the player's score, the highscore, and the button instructions.
 */
void draw_gameover() {
    clear_all_pixels();
    draw_string(40, 0, "game over"); 
    draw_string(0, 8, "your score: ");
    draw_number(65, 8, score);
    draw_string(0, 16, "highscore: ");
    draw_number(60, 16, highscore);
    draw_string(0, 24, "btn4:start btn2:menu");
    display_objects();
}