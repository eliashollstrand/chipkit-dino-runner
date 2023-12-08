/**
 * @file display.c
 * @brief Functions for controlling and manipulating the display.
 *
 * This file contains functions for initializing the display, drawing pixels, rectangles, characters, numbers, and images on the display, and updating the display with the pixel data.
 * The display is divided into 4 sections, each represented by a 128x32 pixel array in the `pixel_data` variable.
 * The functions in this file use SPI communication to send data to the display module.
 * The display can be cleared, and individual pixels can be set or cleared using the `set_pixel()` and `clear_pixel()` functions.
 * Characters and numbers can be drawn on the display using the `draw_char()`, `draw_string()`, `draw_digit()`, and `draw_number()` functions.
 * Images can be drawn on the display using the `draw_image()` function.
 * The `display_objects()` function updates the display with the pixel data stored in the `pixel_data` array.
 *
 * @author Axel Isaksson
 * @author F Lundevall  
 * @author Elias Hollstrand 
 * @author Mattias Kvist
 *
 * @date 2023-11-01
 * 
 * For copyright and licensing, see file COPYING
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "declare.h"  /* Declatations for these labs */
#include <stdio.h>
#include <string.h>

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)
#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)
#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)
#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

/*
 * itoa
 * 
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 * 
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 * 
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 * 
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 * 
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 * 
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 * 
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 * 
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";
  
  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}

// ---------------------------------------------------------------------------------------------
// Code by Elias Hollstrand and Mattias Kvist

// array for pixel data
uint8_t pixel_data[4][128];

/**
 * @brief Sets a pixel at the specified coordinates.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void set_pixel(int x, int y) {
	int row = y / 8; // 8 pixels per row
	int col = x; // 128 pixels per column
	pixel_data[row][col] |= 1 << (y % 8); // set bit at position y%8
}

/**
 * Clears the pixel at the specified coordinates.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void clear_pixel(int x, int y) {
	int row = y / 8; // 8 pixels per row
	int col = x; // 128 pixels per column
	pixel_data[row][col] &= ~(1 << (y % 8)); // clear bit at position y%8
}

/**
 * @brief Clears all pixels in the pixel_data array.
 * 
 * This function iterates through the pixel_data array and sets all elements to 0.
 * The pixel_data array represents the display pixels.
 * 
 * @param None
 * @return None
 */
void clear_all_pixels() {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 128; j++) { 
			pixel_data[i][j] = 0;
		}
	}
}

/**
 * Fills a rectangle with pixels.
 *
 * @param x0 The x-coordinate of the top-left corner of the rectangle.
 * @param y0 The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 */
void fill_rectangle(int x0, int y0, int w, int h) {
	int x1 = x0 + w;
	int y1 = y0 + h;
	
	int i, j;
	for(i = x0; i < x1; i++) {
		for(j = y0; j < y1; j++) {
			set_pixel(i, j);
		}
	}
}

/**
 * @brief Updates the display with the pixel data stored in the pixel_data array.
 * 
 * This function sends the pixel data stored in the pixel_data array to the display.
 * It uses SPI communication to send the data to the display module.
 * The display is divided into 4 sections, and this function updates each section with the corresponding pixel data.
 * 
 * @note This function is based on display_update from labs.
 */
// Based on display_update from labs
void display_objects(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 128; j++) {
			spi_send_recv(pixel_data[i][j]);
		}
	}
}

/**
 * @brief Draws a character on the display.
 *
 * This function takes the coordinates (x, y) and a character 'c' as input and draws the corresponding character on the display.
 * The character is represented by an array of bits stored in the 'letters' array.
 * If the character is a number (ASCII value between 48 and 57), the corresponding number array is used instead.
 * The function iterates over the bits of the character array and draws a pixel at the corresponding position on the display if the bit is 1.
 *
 * @param x The x-coordinate of the character's top-left corner.
 * @param y The y-coordinate of the character's top-left corner.
 * @param c The character to be drawn.
 */
void draw_char(int x, int y, char c) {
	int char_number;
	if(c == 32) {
		char_number = 26; // Space
	} else if(c == 58) {
		char_number = 27; // Colon
	} else {
		char_number = c - 'a';
	}
	const uint8_t* data = letters[char_number];

	if(48 <= c && c <= 57) { // Numbers
		char_number = c - '0';
		data = numbers[char_number];

		int i = 0;
		for(; i < 20; i++) { // 20 bits per number (4x5)
			if(data[i] == 1) {
				set_pixel(x + i % 4, y + i / 4);
			}
		}
	} else {
		int i = 0;
		for(; i < 25; i++) { // 25 bits per letter (5x5)
			if(data[i] == 1) {
				set_pixel(x + i % 5, y + i / 5);
			}
		}
	}
}

/**
 * Draws a string at the specified coordinates.
 *
 * @param x The x-coordinate of the starting position.
 * @param y The y-coordinate of the starting position.
 * @param s The string to be drawn.
 */
void draw_string(int x, int y, char* s) {	
	int i = 0;
	while(s[i]) {
		draw_char(x + i * 6, y, s[i]);
		i++;
	}
}
 
/**
 * @brief Draws a digit on the display at the specified coordinates.
 *
 * This function takes in the x and y coordinates of the top-left corner of the digit
 * and the character representing the digit to be drawn. The character should be a digit
 * from '0' to '9'. The function assumes that there is a 2D array called 'numbers' which
 * contains the pixel data for each digit.
 *
 * @param x The x-coordinate of the top-left corner of the digit.
 * @param y The y-coordinate of the top-left corner of the digit.
 * @param n The character representing the digit to be drawn.
 */
void draw_digit(int x, int y, char n) {
	int number = n - '0';
	const uint8_t* data = numbers[number];

	int i = 0;
	for(; i < 20; i++) {
		// if the bit is 1, draw a pixel
		if(data[i] == 1) {
			set_pixel(x + i % 4, y + i / 4);
		}
	}
}

/**
 * @brief Draws a number on the display at the specified coordinates.
 *
 * This function takes an integer and converts it to a string using the itoaconv() function.
 * It then iterates through each character in the string and calls the draw_digit() function
 * to draw each digit on the display.
 *
 * @param x The x-coordinate of the starting position of the number.
 * @param y The y-coordinate of the starting position of the number.
 * @param n The number to be drawn on the display.
 */
void draw_number(int x, int y, int n) {
	char *s = itoaconv(n);

	int i = 0;
	while(s[i]) {
		draw_digit(x + i * 5, y, s[i]);
		i++;
	}
}

/**
 * Draws an image on the display at the specified position.
 *
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param data A pointer to the image data.
 */
void draw_image(int x, int y, int width, int height, const uint8_t *data) {
	int outOfBounds = -x;

	int i = 0;
	for(; i < width * height; i++) {
		int col = i % width;
		int row = i / width;

		if(outOfBounds > 0){
			if(col >= outOfBounds) {
				if(data[i] == 1) {
					set_pixel(x + col, y + row);
				}
			}
		} else {
			if(data[i] == 1) {
				set_pixel(x + col, y + row);
			}
		}
	}
}

/**
 * Copies a substring from the source string to the destination string.
 *
 * @param source The source string from which to copy the substring.
 * @param dest The destination string where the substring will be copied to.
 * @param start The starting index of the substring in the source string.
 * @param length The length of the substring to be copied.
 */
void substring(char source[], char dest[], int start, int length) {
	int i, j;
	// Copy 'length' characters from 'source' starting at index 'start'
	for(i = start, j = 0; j < length && source[i] != '\0'; ++i, ++j) {
		dest[j] = source[i];
	}

	// Null-terminate the destination string
	dest[j] = '\0';
}