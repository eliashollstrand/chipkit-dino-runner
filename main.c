/*
Authors:
Axel Isaksson,
F Lundevall,
Elias Hollstrand,
Mattias Kvist

Date: 2023-11-01

For copyright and licensing, see file COPYING
*/

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

int main(void)
{
	/*
  This will set the peripheral bus clock to the same frequency
  as the sysclock. That means 80 MHz, when the microcontroller
  is running at 80 MHz. Changed 2017, as recommended by Axel.
*/
	SYSKEY = 0xAA996655; /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA; /* Unlock OSCCON, step 2 */
	while (OSCCON & (1 << 21))
		;				  /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while (OSCCON & (1 << 21))
		;		  /* Wait until PBDIV ready */
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

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

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

	TRISD = TRISD | 0x80;
	CNCON = 0x8000;

	IECSET(0) = 0x80000;
	IECSET(0) = 0x80000;
	IECSET(1) = 0x1;
	IPCSET(4) = 0x1c000000;
	IPCSET(6) = 0x1C0000;
	CNEN = 0x10000;

	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	IECSET(0) = 0x100;		// Enable T2 interrupts (bit 8)
	IECSET(0) = 0x80000;	// Enable switch 4 interrupts (bit 19)
	IECSET(0) = 0x8000;		// Enable switch 3 interrupts (bit 15)
	IECSET(0) = 0x800;		// Enable switch 2 interrupts (bit 11)
	IECSET(0) = 0x80;	// Enable switch 1 interrupts (bit 7)
	IPCSET(2) = 0b11100;	// Set priority level 7
	IPCSET(4) = 0x1c000000; // Set priority level 7 // Bits 28-26
	 // Initialize port D so that bits 11 through 5 of Port D are set as inputs
  	TRISD = TRISD | 0x0fe0;

	

	// Enable global interrupts
	enable_interrupt();

	// Set up i2c
	I2C1CON = 0x0; 
	/* I2C Baud rate should be less than 400 kHz, is generated by dividing
	the 40 MHz peripheral bus clock down */
	I2C1BRG = 0x0C2;
	I2C1STAT = 0x0; 
	I2C1CONSET = 1 << 13; // SIDL = 1 (disables I2C when CPU is idle)
	I2C1CON = 1 << 15; // I2C ON
	I2C1ADD = 1010000; // add EEPROM address to I2C1ADD register
	int data = I2C1RCV; // clear receive buffer

	display_init();
	display_string(0, "Score: ");
	display_string(2, "Highscore: ");

	display_update();

	score_init();

	// display_image(96, icon);

	while (1)
	{
	}
	return 0;
}
