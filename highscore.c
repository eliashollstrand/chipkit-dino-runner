/*
Authors:
Elias Hollstrand,
Mattias Kvist

Date: TODO
TTYDEV=/dev/cu.usbserial-A503WFGV

For copyright and licensing, see file COPYING
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>

uint8_t highscore;
uint8_t score;
static int JUMP_DELAY = 1;
int ledValue = 0;

volatile int *porte = (volatile int *)0xbf886110;

#define EEPROM_WRITE 0xA0 // 1010 0000
#define EEPROM_READ 0xA1  // 1010 0001
#define EEPROM_MEM_ADD 0x100
#define SCORE_ADDRESS 0x0000
#define EEPROM_ADDR 0x50


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

uint8_t eeprom_read_byte(uint16_t address) {
    // Step 1: Initialize I2C
    // (Assuming you have already configured and initialized the I2C module)

    // Step 2: Send start condition
    i2c_start();

    // Step 3: Send EEPROM device address with read bit set
    i2c_send(EEPROM_ADDR | EEPROM_READ);

    // Step 4: Send the memory address you want to read from
    i2c_send((uint8_t)(address >> 8)); // MSB of address
    i2c_send((uint8_t)(address & 0xFF)); // LSB of address

    // Step 5: Send restart condition
    i2c_restart();

    // Step 6: Send EEPROM device address with read bit set again
    i2c_send(EEPROM_ADDR | EEPROM_READ);

    // Step 7: Receive the data from the EEPROM
    uint8_t data = i2c_recv();

    // Step 8: Send NACK to indicate the end of read
    i2c_nack();

    // Step 9: Send stop condition
    i2c_stop();

    return data;
}



void score_display()
{
    display_string(1, itoaconv(score));
    // read_highscore();
    // highscore = current_address_read();
    highscore = eeprom_read_byte(SCORE_ADDRESS);
    display_string(3, itoaconv(highscore));
    display_update();
}

void delay(int cyc)
{
    int i;
    for (i = cyc; i > 0; i--)
        ;
}

void read_highscore() {
    // Get the high score from EEPROM and store in highscore variable
    i2c_start();
    i2c_send(EEPROM_WRITE);
    i2c_send(EEPROM_MEM_ADD >> 8);
    i2c_send(EEPROM_MEM_ADD & 0xFF);
    i2c_restart();
    i2c_send(EEPROM_READ);
    highscore = i2c_recv();
    i2c_nack();
    i2c_stop();
}

// void write_highscore() {
//     // Write new highscore to EEPROM
//     i2c_start(); // Start I2C communication
//     i2c_send(EEPROM_WRITE); // Send the write command to the EEPROM
//     i2c_ack(); // Send an acknowledgment signal

//     // Send the high byte of the EEPROM address
//     i2c_send(EEPROM_MEM_ADD >> 8);
//     i2c_ack(); // Send an acknowledgment signal

//     // Send the low byte of the EEPROM address
//     i2c_send(EEPROM_MEM_ADD & 0xFF);
//     i2c_ack(); // Send an acknowledgment signal

//     i2c_send(score); // Send the score to the EEPROM
//     i2c_ack(); // Send an acknowledgment signal

//     i2c_stop(); // Stop I2C communication
// }

void writeToEEPROM(uint8_t data, uint16_t address)
{
    i2c_start(); // Start I2C communication

    // Send the control byte (device address + bit 0) 
    i2c_send(EEPROM_WRITE);

    // wait for ack from slave

    // Send the high byte of the EEPROM address
    i2c_send(address);

    // wait for ack from slave

    // Send the low byte of the EEPROM address
    i2c_send(address);

    // wait for ack from slave

    i2c_send(data); // Send the data word to the EEPROM

    // master generates a stop condition

    i2c_stop(); // Stop I2C communication

    // slave initiates internal write cycle and will not acknowledge.
    // if attempting to write to EEPROM before the write cycle is complete (WP pin high),
    // the device will acknowledge the command but no write cycle will occur, no data 
    // will be written, and the device will immediately accept a new command. After 
    // a byte Write command, the internal address counter will point to the address 
    // location following the one that was just written. WP = write protect pin
}

// uint8_t readFromEEPROM(uint16_t address)
// {
//     uint8_t data;
//     i2c_start(); // Start I2C communication

//     // Send the control byte (device address + write command)
//     i2c_send(EEPROM_WRITE);
    
//     //wait for ack

//     // Send the high byte of the EEPROM address
//     i2c_send(address >> 2);

//     // Send the low byte of the EEPROM address
//     i2c_send(address + 8);

//     i2c_restart(); // Restart I2C communication

//     // Send the control byte (device address + read command)
//     i2c_send(EEPROM_READ);

//     data = i2c_recv(); // Receive the data from the EEPROM
//     i2c_nack();        // Send an acknowledgment signal
//     i2c_stop(); // Stop I2C communication

//     // if(PIC32_IRQ_I2C1B || PIC32_IRQ_I2C1S || PIC32_IRQ_I2C1M) {
//     // display_string(0, "Error");
//     // }
//     return data;
// }

uint8_t readFromEEPROM(uint8_t address) {
    uint8_t data;

    do { // wait for ack
        i2c_start();
    } while (!i2c_send(EEPROM_WRITE)); // send control byte

    delay(100000);
    i2c_send(0x00); // high byte of address
    delay(100000);
    i2c_send(address); // low byte of address
    delay(100000);
    i2c_restart(); // restart
    delay(100000);
    i2c_send(EEPROM_READ); // read command
    delay(100000);
    data = i2c_recv(); // read data
    delay(100000);
    i2c_nack(); // send nack
    delay(100000);
    i2c_stop(); // stop
    delay(100000);
    
    return data;
    // return i2c_status;
}

void score_update()
{
    score++;
    // if (score > 10)
    // {
    //     highscore = score;
    //     writeToEEPROM(highscore, SCORE_ADDRESS);
    // }

    score_display();
}

/* Interrupt Service Routine */
void user_isr(void)
{
    if (IFS(1) & 0x1)
    { // button 4
        // read_highscore();
        score_update();
        delay(1000000);
        IFSCLR(1) = 0x1;
    }

    if (IFS(0) & 0x80000) // switch 4, update led+=1
    {
        IFSCLR(0) = 0x80000;
        // Increase the LED value by 1 (roll over after 255)
        ledValue = (ledValue + 1) & 0xFF;

        // Update the LEDs
        *porte = ledValue;
    }

    if (IFS(0) & 0x8000) // switch 3, update led and write to EEPROM
    {
        IFSCLR(0) = 0x8000;
        // Increase the LED value by 1 (roll over after 255)
        ledValue = (ledValue + 1) & 0xFF;

        // Update the LEDs
        *porte = ledValue;

        writeToEEPROM(ledValue, SCORE_ADDRESS);
        display_string(0, "Wrote to EEPROM");
        display_update();
    }

    if (IFS(0) & 0x800) // switch 2, update led and read from EEPROM
    {
        IFSCLR(0) = 0x800;
        // Increase the LED value by 1 (roll over after 255)
        ledValue = (ledValue + 1) & 0xFF;

        // Update the LEDs
        *porte = ledValue;

        // highscore = readFromEEPROM(SCORE_ADDRESS);
        read_highscore();
        display_string(0, "Read from EEPROM");
        display_update();
    }

    if (IFS(0) & 0x80) // switch 1, update led and update display
    {
        IFSCLR(0) = 0x80;
        // Increase the LED value by 1 (roll over after 255)
        ledValue = (ledValue + 1) & 0xFF;

        *porte = ledValue;

        display_update();
    }
}

void score_init()
{

    // Enable global interrupts
    enable_interrupt();

    // Get the high score from EEPROM and store in highscore variable
    // read_highscore();
    score = I2C1RCV; // clear buffer
    highscore = 10;

    volatile int *trise = (volatile int *)0xbf886100;

    // Display score and current highscore
    score_display();
}
