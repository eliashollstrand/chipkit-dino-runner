/**
 * @file menu.c
 * @brief This file contains the implementation of the menu state.
 *
 * The menu state is responsible for displaying the menu screen
 * 
 * @author Elias Hollstrand
 * @author Mattias Kvist
 * 
 * @date 2023-12-07
 *
 * For copyright and licensing, see file COPYING.
 */
#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declarations for these labs */

int highscore;
int menu_page = 0;

/**
 * Function to draw the menu on the display.
 * This function clears all pixels, fills the top and bottom borders,
 * and displays different menu pages based on the value of menu_page.
 * Each menu page displays different strings on the display.
 */
void draw_menu() {
    // Clear all pixels on the display
    clear_all_pixels();

    // Fill the top and bottom borders
    fill_rectangle(0, 0, 127, 1);
    fill_rectangle(0, 31, 127, 1);

    // Display different menu pages based on the value of menu_page
    switch(menu_page % 4) {
    case 0:
        // Display menu controls
        draw_string(25, 3, "menu controls");
        draw_string(0, 10, "start: btn4");
        draw_string(0, 17, "next: btn2");
        break;
    case 1:
        // Display controls
        draw_string(40, 3, "controls");
        draw_string(0, 10, "jump: btn4");
        draw_string(0, 17, "duck: btn3");
        break;
    case 2:
        // Display leaderboard
        draw_leaderboard();
        break;
    case 3:
        // Display credits
        draw_string(40, 3, "credits");
        draw_string(0, 10, "mattias kvist");
        draw_string(0, 17, "elias hollstrand");
        break;

    default:
        break;
    }

    // Display the objects on the display
    display_objects();
}