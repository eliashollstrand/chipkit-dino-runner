/**
 * @file highscore.c
 * @brief Implementation of highscore functionality.
 *
 * This file contains the implementation of functions related to highscore management.
 * It includes functions for reading and writing scores and initials to an EEPROM memory,
 * updating LEDs with the current score value, printing the leaderboard on the screen,
 * and inserting scores and initials into the leaderboard.
 *
 * @author Elias Hollstrand
 * @author Mattias Kvist
 *
 * @date 2023-12-06
 *
 * For copyright and licensing, see file COPYING.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>
#include "declare.h"
#include "stdbool.h"

int ledValue = 0;
uint8_t leaderboard_scores[NUM_LEADERBOARD_ENTRIES];

// Initialize LEDs
volatile int *porte = (volatile int *)0xbf886110;
volatile int *trise = (volatile int *)0xbf886100;

#define EEPROM_WRITE 0xA0 // 1010 000 (0)
#define EEPROM_READ 0xA1  // 1010 000 (1)
#define SCORE_ADDRESS 0x0230 
#define INITIALS_ADDRESS 0x0000
#define I2C_DELAY 500000 // Delay to allow the EEPROM to complete the write operation (minimum 5 ms)


/**
 * @brief Delays the program execution for a specified number of cycles.
 * 
 * @param cyc The number of cycles to delay the program execution.
 */
void delay(int cyc) {
    int i;
    for (i = cyc; i > 0; i--)
        ;
}

/**
 * @brief Reads a byte from the EEPROM at the specified address.
 *
 * This function performs the necessary steps to read a byte from the EEPROM.
 * It starts by sending the start condition, followed by sending the EEPROM device address
 * with the write bit set to 0. Then, it sends the memory address to read from.
 * After that, it sends the restart condition and sends the EEPROM device address
 * with the read bit set to 1. Finally, it receives the data from the EEPROM,
 * sends a NACK to indicate the end of read, and sends the stop condition.
 *
 * @param address The memory address to read from in the EEPROM.
 * @return The byte read from the EEPROM.
 */
uint8_t eeprom_read_byte(uint16_t address) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit = 0
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to read from
    i2c_send((uint8_t)(address >> 8)); // MSB of address
    i2c_send((uint8_t)(address & 0xFF)); // LSB of address

    // Step 4: Send restart condition
    i2c_restart();

    // Step 5: Send EEPROM device address with read bit set again
    i2c_send(EEPROM_READ);

    // Step 6: Receive the data from the EEPROM
    uint8_t data = i2c_recv();

    // Step 7: Send NACK to indicate the end of read
    i2c_nack();

    // Step 8: Send stop condition
    i2c_stop();

    return data;
}

/**
 * @brief Writes a byte of data to the EEPROM at the specified address.
 * 
 * This function performs the necessary steps to write a byte of data to the EEPROM.
 * It sends the start condition, EEPROM device address with the write bit, memory address,
 * data to be written, and finally the stop condition. It also includes a delay to allow
 * the EEPROM to complete the write operation.
 * 
 * @param address The memory address to write the data to.
 * @param data The byte of data to be written.
 */
void eeprom_write_byte(uint16_t address, uint8_t data) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit 
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to write to, assuming 16 bit address
    i2c_send((uint8_t)(address >> 8)); // MSB of address
    i2c_send((uint8_t)(address & 0xFF)); // LSB of address

    // Step 4: Send the data to be written
    i2c_send(data);

    // Step 5: Send stop condition
    i2c_stop();

    // Add a delay to allow the EEPROM to complete the write operation
    delay(I2C_DELAY);
}

/**
 * @brief Updates the LEDs with the current score value.
 * 
 * This function sets the value of the LEDs to the current score value.
 * 
 * @return None
 */
void update_LEDs() {
    ledValue = score;

    // Update the LEDs
    *porte = ledValue;
}

/**
 * @brief Reads multiple scores from an EEPROM memory.
 *
 * @param scores The array to store the read scores.
 * @param size The number of scores to read.
 */
void read_multiple_scores(uint8_t *scores, int size) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit = 0
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to read from
    i2c_send((uint8_t)(SCORE_ADDRESS >> 8)); // MSB of address
    i2c_send((uint8_t)(SCORE_ADDRESS & 0xFF)); // LSB of address

    // Step 4: Send restart condition
    i2c_restart();

    // Step 5: Send EEPROM device address with read bit set again
    i2c_send(EEPROM_READ);

    // Step 6: Receive the data from the EEPROM
    int i = 0;
    while(i < size - 1) {
        scores[i] = i2c_recv();
        i2c_ack();
        i++;
    }

    // Receive the last data byte from the EEPROM
    scores[i] = i2c_recv();
    i2c_nack();

    // Step 8: Send stop condition
    i2c_stop();
}

/**
 * @brief Writes multiple scores to an EEPROM memory.
 *
 * @param scores An array of scores to be written.
 */
void write_multiple_scores(uint8_t *scores) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit 
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to write to, assuming 16 bit address (KOLLA PÅ DET HÄR)
    i2c_send((uint8_t)(SCORE_ADDRESS >> 8)); // MSB of address (ÖVRE HALVAN)
    i2c_send((uint8_t)(SCORE_ADDRESS & 0xFF)); // LSB of address (NEDRE HALVAN)

    // Step 4: Send the data to be written
    int i = 0;
    while(scores[i] != 0) {
        i2c_send(scores[i]);
        scores++;
    }

    // Step 5
    i2c_stop();

    // Add a delay to allow the EEPROM to complete the write operation
    delay(I2C_DELAY); // Delay to allow the EEPROM to complete the write operation, can display 255 without delay if read too quickly
}

/**
 * @brief Reads the initials from the EEPROM memory.
 *
 * @param initials The array to store the read initials.
 */
void read_initials(char *initials) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit = 0
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to read from
    i2c_send((uint8_t)(INITIALS_ADDRESS >> 8)); // MSB of address
    i2c_send((uint8_t)(INITIALS_ADDRESS & 0xFF)); // LSB of address

    // Step 4: Send restart condition
    i2c_restart();

    // Step 5: Send EEPROM device address with read bit set again
    i2c_send(EEPROM_READ);

    // Step 6: Receive the data from the EEPROM
    int i = 0;
    while (i < INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - 1) {
        initials[i] = i2c_recv();
        i2c_ack();
        i++;
    }

    // Receive the last data byte from the EEPROM
    initials[i] = i2c_recv();
    i2c_nack();

    // Step 8: Send stop condition
    i2c_stop();
}

/**
 * @brief Writes the initials to the EEPROM memory.
 * 
 * This function writes the provided initials to the specified memory address in the EEPROM.
 * It follows a series of steps to send the start condition, device address, memory address,
 * and the data to be written. Finally, it sends the stop condition and adds a delay to allow
 * the EEPROM to complete the write operation.
 * 
 * @param initials The initials to be written to the EEPROM.
 */
void write_initials(char *initials) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit 
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to write to, assuming 16 bit address (KOLLA PÅ DET HÄR)
    i2c_send((uint8_t)(INITIALS_ADDRESS >> 8)); // MSB of address (ÖVRE HALVAN)
    i2c_send((uint8_t)(INITIALS_ADDRESS & 0xFF)); // LSB of address (NEDRE HALVAN)

    // Step 4: Send the data to be written
    int i = 0;
    while(initials[i] != 0) {
        i2c_send(initials[i]);
        initials++;
    }

    // Step 5
    i2c_stop();

    // Add a delay to allow the EEPROM to complete the write operation
    delay(I2C_DELAY); // Delay to allow the EEPROM to complete the write operation, can display 255 without delay if read too quickly
}

/**
 * @brief Reads the scores and initials from the EEPROM memory.
 */
void read_leaderboard() {
    read_multiple_scores(leaderboard_scores, NUM_LEADERBOARD_ENTRIES);
    read_initials(leaderboard_initials[0]);
}

/**
 * @brief Prints the leaderboard entries on the screen.
 *
 * Each entry consists of a rank, score, and initials.
 * The leaderboard entries are displayed in a loop until all entries are printed.
 * The function uses the draw_number, substring, and draw_string functions to display the leaderboard entries.
 */
void print_leaderboard() {
    char initials[3];
    
    int i = 0;
    while(i < NUM_LEADERBOARD_ENTRIES) {
        draw_number(5 + i*20, 8, i + 1);
        draw_number(i*20, 15, leaderboard_scores[i]);
        substring(leaderboard_initials[i], initials, 0, INITIALS_LENGTH);
        draw_string(i*20, 22, initials);
        i++;
    }
}

/**
 * @brief Draws the leaderboard on the screen.
 *
 * This function clears all pixels, displays the title "leaderboard",
 * prints the leaderboard data.
 */
void draw_leaderboard() {
    clear_all_pixels();
    draw_string(30, 0, "leaderboard");
    print_leaderboard();
    display_objects();
}

/**
 * @brief Inserts the given initials at the specified index in the leaderboard.
 *
 * Shifts the existing initials to make room for the new initials.
 *
 * @param initials The initials to be inserted.
 * @param index The index at which the initials should be inserted.
 */

void insert_initials(char *initials, int index) {
    char to_shift[INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - INITIALS_LENGTH * index];
    // Get the initials to shift
    substring(leaderboard_initials[index], to_shift, 0, INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - INITIALS_LENGTH * (index-1)); 
    
    int i;
    // Insert the new initials
    for(i = 0; i < INITIALS_LENGTH; i++) {
        leaderboard_initials[index][i] = initials[i];
    }

    // Insert entries that were shifted
    for(i = 0; i < INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - INITIALS_LENGTH * index; i++) {
        leaderboard_initials[index + 1][i] = to_shift[i];
    }

    write_initials(leaderboard_initials[0]);
}

/**
 * @brief Inserts a score into the leaderboard.
 *
 * This function takes a score as input and inserts it into the leaderboard
 * in descending order. If the score is higher than any existing scores in
 * the leaderboard, it will be inserted at the appropriate position and the
 * leaderboard will be updated accordingly. If the score is not higher than
 * any existing scores, the function will change the game state to GAME_OVER_STATE.
 *
 * @param score The score to be inserted into the leaderboard.
 */
void insert_score(uint8_t score) {
    bool inserted = false;
    
    int i = 0;
    while(i < NUM_LEADERBOARD_ENTRIES) {
        if(score > leaderboard_scores[i]) {
            // Shift down all scores below this one
            int j;
            for (j = NUM_LEADERBOARD_ENTRIES - 1; j > i; j--) {
                leaderboard_scores[j] = leaderboard_scores[j - 1];
            }
            // Insert the new score
            leaderboard_scores[i] = score;
            leaderboard_index = i;
            write_multiple_scores(leaderboard_scores);
            inserted = true;
            highscore = leaderboard_scores[0];
            change_state(ENTER_NAME_STATE);
            break;
        }

        i++;
    }

    if(!inserted) {
        change_state(GAME_OVER_STATE);
    }
}
