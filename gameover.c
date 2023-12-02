#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

int highscore;
int score;

draw_gameover()
{
    clear_all_pixels();
    draw_string(40, 0, "game over"); 
    draw_string(0, 8, "your score: ");
    draw_number(65, 8, score);
    draw_string(0, 16, "highscore: ");
    draw_number(60, 16, highscore);
    draw_string(0, 24, "btn4:start btn2:menu");
    display_objects();
}