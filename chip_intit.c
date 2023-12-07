/**
 * @file chip_init.c
 * @brief Initialization function for the chip
 *
 * This file contains the implementation of the chip_init function,
 * which initializes various peripherals and settings of the chip.
 * 
 * @author Axel Isaksson
 * @author F Lundevall
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

const int FPS = 30;

/**
 * @brief Initialize the chip
 *
 * This function initializes the peripheral bus clock, sets up output pins,
 * configures SPI as master, initializes the display, sets up timers,
 * enables interrupts for switches, sets up change notice interrupts,
 * initializes input pins, and enables global interrupts.
 */
void chip_init(void) {
    /*
    This will set the peripheral bus clock to the same frequency
    as the sysclock. That means 80 MHz, when the microcontroller
    is running at 80 MHz. Changed 2017, as recommended by Axel.
    */
    SYSKEY = 0xAA996655; /* Unlock OSCCON, step 1 */
    SYSKEY = 0x556699AA; /* Unlock OSCCON, step 2 */
    while (OSCCON & (1 << 21))
        ;                 /* Wait until PBDIV ready */
    OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    while (OSCCON & (1 << 21))
        ;         /* Wait until PBDIV ready */
    SYSKEY = 0x0; /* Lock OSCCON */

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
    /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x20;
    /* SPI2CON bit ON = 1; */
    SPI2CONSET = 0x8000;

/*--------------------------------------------------------*/
/* Code by Elias Hollstrand and Mattias Kvist */
    /* Initialize display */
    display_init();

    /* Set up timers */

    /* Set up timer 2 for display refreshing */
    TMR2 = 0;                    // Reset timer counter
    PR2 = (80000000 / 256) / FPS; // Set period to 30 FPS
    T2CON = 0x8070;              // Enable timer 2 (bit 15) and set prescaler to 1:256 (bit 6 and 7)

    // /* Set up interrupts for timer 2*/
    IPCSET(2) = 0x1F;     // Set priority 7 and subpriority 3
    IFSCLR(0) = (1 << 8); // Clear interrupt flag for timer 2
    IECSET(0) = (1 << 8); // Enable interrupts for timer 2


    /* Set up timer 3 for obstacle movement */
    TMR3 = 0;                    // Reset timer counter
    PR3 = (80000000 / 256) / 10; // Set period to 100ms
    T3CON = 0x8070;              // Enable timer 3 (bit 15) and set prescaler to 1:256 (bit 6 and 7)

    /* Set up interrupts for switches */
    IECSET(0) = 0x80000; // Enable switch 4 interrupts (bit 19)
    IECSET(0) = 0x8000;  // Enable switch 3 interrupts (bit 15)
    IECSET(0) = 0x800;   // Enable switch 2 interrupts (bit 11)
    IECSET(0) = 0x80;    // Enable switch 1 interrupts (bit 7)

    /* Set up change notice interrupts for button 1 */
    TRISD = TRISD | 0x80;
    // CNCON = 0x8000;
    // IECSET(1) = 0x1;
    // IPCSET(4) = 0x1c000000; // Set priority level 7 // Bits 28-26
    // IPCSET(6) = 0x1C0000;
    // CNEN = 0x10000;

    /* Set up input pins*/
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    //Initialize port D so that bits 11 through 5 of Port D are set as inputs
    TRISD = TRISD | 0x0fe0;

    /* Initialize I2C*/
    I2C1CON = 0x0;        // Clear control register
    I2C1BRG = 0x0C2;      // Set Baud Generator Divisor to 100
    I2C1STAT = 0x0;       // Clear status register
    I2C1CONSET = 1 << 13; // SIDL = 1 (disables I2C when CPU is idle)
    I2C1CONSET = 1 << 15; // I2C ON
    // I2C1ADD = 0b1010000; // add EEPROM address to I2C1ADD register...THIS MESSED THINGS UP

    enable_interrupt(); // Enable global interrupts
}