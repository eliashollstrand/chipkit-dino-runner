/**
 * @file enter_name.c
 * @brief This file contains the implementation of the enter_name module.
 *
 * The enter_name module is responsible for displaying the enter name screen
 * and variables related to the enter name screen.
 * 
 * @author Elias Hollstrand
 * @author Mattias Kvist
 * 
 * @date 2023-12-07
 *
 * For copyright and licensing, see file COPYING.
 */

#include <stdint.h>     /* Declarations of uint_32 and the like */
#include <pic32mx.h>    /* Declarations of system-specific addresses etc */
#include "declare.h"    /* Declarations for these labs */

char leaderboard_initials[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH];

char initials[3] = {'a', 'a', 'a'};

int letter_index = 0;

/**
 * @brief Draws the enter name screen.
 *
 * This function clears the display, draws the enter name screen with the
 * current initials, and displays the screen.
 */
void draw_enter_name() {
    clear_all_pixels();
    draw_string(0, 0, "congratulations");
    draw_string(0, 8, "you got a highscore");
    draw_string(0, 16, "enter name:");
    draw_string(75, 16, initials);
    draw_image(75 + letter_index * 6, 24, 5, 5, arrow_up);
    draw_string(0, 24, "btn3:save");
    display_objects();
}