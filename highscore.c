// // /*
// // Authors:
// // Elias Hollstrand,
// // Mattias Kvist

// // Date: TODO
// // TTYDEV=/dev/cu.usbserial-A503WFGV

// // For copyright and licensing, see file COPYING
// // */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>
#include "declare.h"
#include "stdbool.h"


int ledValue = 0;
uint8_t leaderboard_scores[NUM_LEADERBOARD_ENTRIES] = {1, 2, 3, 4, 5, 6};


// volatile int *porte = (volatile int *)0xbf886110;
/* Initialize LEDs*/
volatile int *porte = (volatile int *)0xbf886110;
volatile int *trise = (volatile int *)0xbf886100;

#define EEPROM_WRITE 0xA0 // 1010 0000
#define EEPROM_READ 0xA1  // 1010 0001
#define SCORE_ADDRESS 0x0230
#define INITIALS_ADDRESS 0x0000
#define I2C_DELAY 500000 // Delay to allow the EEPROM to complete the write operation, experiment more

uint8_t current_address_read() {
    uint8_t data = 0;
    i2c_start();
    i2c_send(EEPROM_READ);

    data = i2c_recv();

    // Send NACK to EEPROM
    i2c_nack();

    i2c_stop();
    return data;
}

/* Function used to delay operations */
void delay(int cyc)
{
    int i;
    for (i = cyc; i > 0; i--)
        ;
}

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


void eeprom_write_byte(uint16_t address, uint8_t data) {
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit 
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to write to, assuming 16 bit address (KOLLA PÅ DET HÄR)
    i2c_send((uint8_t)(address >> 8)); // MSB of address (ÖVRE HALVAN)
    i2c_send((uint8_t)(address & 0xFF)); // LSB of address (NEDRE HALVAN)

    // Step 4: Send the data to be written
    i2c_send(data);

    // Step 5: Send stop condition
    i2c_stop();

    // Add a delay to allow the EEPROM to complete the write operation
    delay(I2C_DELAY); // Delay to allow the EEPROM to complete the write operation, can display 255 without delay if read too quickly
}


void increment_LEDs()
{
    // Increase the LED value by 1 (roll over after 255)
    ledValue = (ledValue + 1) & 0xFF;

    // Update the LEDs
    *porte = ledValue;
}

uint8_t read_highscore()
{
    return eeprom_read_byte(SCORE_ADDRESS);
}

void write_highscore(uint8_t score)
{
    eeprom_write_byte(SCORE_ADDRESS, score);
}

void write_multiple_scores(uint8_t *scores)
{
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit 
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to write to, assuming 16 bit address (KOLLA PÅ DET HÄR)
    i2c_send((uint8_t)(SCORE_ADDRESS >> 8)); // MSB of address (ÖVRE HALVAN)
    i2c_send((uint8_t)(SCORE_ADDRESS & 0xFF)); // LSB of address (NEDRE HALVAN)

    // Step 4: Send the data to be written
    int i = 0;
    while (scores[i] != 0) {
        i2c_send(scores[i]);
        scores++;
    }

    // Step 5
    i2c_stop();

    // Add a delay to allow the EEPROM to complete the write operation
    delay(I2C_DELAY); // Delay to allow the EEPROM to complete the write operation, can display 255 without delay if read too quickly
}

void read_multiple_scores(uint8_t *scores, int size)
{
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
    while (i < size - 1) {
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

void write_initials(char *initials)
{
    // Step 1: Send start condition
    i2c_start();

    // Step 2: Send EEPROM device address with RW-bit 
    i2c_send(EEPROM_WRITE);

    // Step 3: Send the memory address you want to write to, assuming 16 bit address (KOLLA PÅ DET HÄR)
    i2c_send((uint8_t)(INITIALS_ADDRESS >> 8)); // MSB of address (ÖVRE HALVAN)
    i2c_send((uint8_t)(INITIALS_ADDRESS & 0xFF)); // LSB of address (NEDRE HALVAN)

    // Step 4: Send the data to be written
    int i = 0;
    while (initials[i] != 0) {
        i2c_send(initials[i]);
        initials++;
    }

    // Step 5
    i2c_stop();

    // Add a delay to allow the EEPROM to complete the write operation
    delay(I2C_DELAY); // Delay to allow the EEPROM to complete the write operation, can display 255 without delay if read too quickly
}

void read_initials(char *initials)
{
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

void read_leaderboard()
{
    read_multiple_scores(leaderboard_scores, NUM_LEADERBOARD_ENTRIES);
    read_initials(leaderboard_initials[0]);
}

void print_leaderboard()
{
    char initials[3];
    int i = 0;
    while (i < NUM_LEADERBOARD_ENTRIES) {
        draw_number(5 + i*20, 8, i + 1);
        draw_number(i*20, 15, leaderboard_scores[i]);
        substring(leaderboard_initials[i], initials, 0, INITIALS_LENGTH);
        draw_string(i*20, 22, initials);
        i++;
    }
}

void draw_leaderboard()
{
    clear_all_pixels();
    draw_string(30, 0, "leaderboard");
    print_leaderboard();
    display_objects();
}

void insert_initials(char *initials, int index)
{
    char to_shift[INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - INITIALS_LENGTH * index];
    substring(leaderboard_initials[index], to_shift, 0, INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - INITIALS_LENGTH * index); // Get the initials to shift
    int i;
    for (i = 0; i < INITIALS_LENGTH; i++) {
        leaderboard_initials[index][i] = initials[i];
    }

    for (i = 0; i < INITIALS_LENGTH * NUM_LEADERBOARD_ENTRIES - INITIALS_LENGTH * index; i++) {
        leaderboard_initials[index + 1][i] = to_shift[i];
    }
    write_initials(leaderboard_initials[index]);
}

void insert_score(uint8_t score)
{
    bool inserted = false;
    int i = 0;
    while (i < NUM_LEADERBOARD_ENTRIES) {
        if (score > leaderboard_scores[i]) {
            // Shift down all scores below this one
            int j;
            for (j = NUM_LEADERBOARD_ENTRIES - 1; j > i; j--) {
                leaderboard_scores[j] = leaderboard_scores[j - 1];
            }
            // Insert the new score
            leaderboard_scores[i] = score;
            insert_initials(leaderboard_initials[0], i);
            write_multiple_scores(leaderboard_scores);
            highscore = read_highscore();
            inserted = true;
            change_state(ENTER_NAME_STATE);
            break;
        }
        i++;
    }
    if (!inserted) {
        change_state(GAME_OVER_STATE);
    }
}
