/* 
Authors: 
Elias Hollstrand,
Mattias Kvist

Date: 2023-11-01

For copyright and licensing, see file COPYING 
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>

int highscore = 0;
static int JUMP_DELAY = 1;

/* Interrupt Service Routine */
void user_isr( void ) {
    if (IFS(1) & 0x1){
        IFSCLR(1) = 0x1;
        highscore_update();
    }
}

void highscore_init() {
    TRISD = TRISD | 0x80;
    CNCON = 0x8000;
    
    IECSET(0) = 0x80000;   
    IECSET(0) = 0x80000;   
    IECSET(1) = 0x1;
    IPCSET(4) = 0x1c000000;
    IPCSET(6) = 0x1C0000;
    CNEN = 0x10000;

    // Enable global interrupts
    enable_interrupt();

    highscore_update();
}

void highscore_update() {
    highscore++;
    display_string(1, itoaconv(highscore));
    display_update();
}




