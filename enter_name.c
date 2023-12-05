#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */


char leaderboard_initials[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH];

int letter_index = 0;

void draw_enter_name()
{
    clear_all_pixels();
    draw_string(0, 0, "enter name:");

    draw_string(0, 8, leaderboard_initials[0]);
    // draw_string(0, 16, leaderboard_initials[4]);
    // draw_string(0, 24, leaderboard_initials[5]);

    //draw string with initials from leaderboard_initials index 0 to 2
    char initials[3];
    // substring(leaderboard_initials[1], initials, 0, INITIALS_LENGTH);
    // draw_string(0, 8, initials);

    // draw_image(letter_index * 6, 16, 5, 5, arrow_up);

    display_objects();
}