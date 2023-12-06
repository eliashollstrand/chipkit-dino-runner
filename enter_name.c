#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */


char leaderboard_initials[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH];

char initials[3] = {'a', 'a', 'a'};

int letter_index = 0;

void draw_enter_name()
{
    clear_all_pixels();
    draw_string(0, 0, "congrtulations");
    draw_string(0, 8, "you got a highscore");
    draw_string(0, 16, "enter name:");
    draw_string(75, 16, initials);
    draw_image(75 + letter_index * 6, 24, 5, 5, arrow_up);
    draw_string(0, 24, "b3:confirm");

    display_objects();
}