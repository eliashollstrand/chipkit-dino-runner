#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

int highscore;
int menu_page = 0;

draw_menu()
{
    clear_all_pixels();
    fill_rectangle(0, 0, 127, 1);
    fill_rectangle(0, 31, 127, 1);
    switch (menu_page%4)
    {
    case 0:
        draw_string(25, 3, "menu controls");
        draw_string(0, 10, "start: btn4");
        draw_string(0, 17, "next: btn2");
        break;
    case 1:
        draw_string(40, 3, "controls");
        draw_string(0, 10, "jump: btn4");
        draw_string(0, 17, "duck: btn3");
        break;
    case 2:
        draw_leaderboard();
        break;
    case 3:
        draw_string(40, 3, "credits");
        draw_string(0, 10, "mattias kvist");
        draw_string(0, 17, "elias hollstrand");
        break;

    default:
        break;
    }
    display_objects();
}

