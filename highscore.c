// // /*
// // Authors:
// // Elias Hollstrand,
// // Mattias Kvist

// // Date: TODO
// // TTYDEV=/dev/cu.usbserial-A503WFGV

// // For copyright and licensing, see file COPYING
// // */

// // #include <stdio.h>
// // #include <stdint.h>
// // #include <stdlib.h>
// // #include <pic32mx.h>

// // uint8_t highscore;
// // uint8_t score;
// // int ledValue = 0;

// // volatile int *porte = (volatile int *)0xbf886110;
// /* Initialize LEDs*/
// volatile int *porte = (volatile int *)0xbf886110;
// volatile int *trise = (volatile int *)0xbf886100;

// // #define EEPROM_WRITE 0xA0 // 1010 0000
// // #define EEPROM_READ 0xA1  // 1010 0001
// // #define SCORE_ADDRESS 0xFFFF

// #define EEPROM_WRITE 0xA0 // 1010 0000
// #define EEPROM_READ 0xA1  // 1010 0001
// #define SCORE_ADDRESS 0x0230
// #define I2C_DELAY 500000 // Delay to allow the EEPROM to complete the write operation, experiment more

// // // uint8_t current_address_read() {
// // //     uint8_t data = 0;
// // //     i2c_start();
// // //     i2c_send(EEPROM_READ);

// // //     data = i2c_recv();

// // //     // Send NACK to EEPROM
// // //     i2c_nack();

// // //     i2c_stop();
// // //     return data;
// // // }

// // uint8_t eeprom_read_byte(uint16_t address) {
// //     // Step 1: Initialize I2C
// //     // (Assuming you have already configured and initialized the I2C module)

// //     // Step 2: Send start condition
// //     i2c_start();

// //     // Step 3: Send EEPROM device address with RW-bit = 0
// //     i2c_send(EEPROM_WRITE);

// //     // Step 4: Send the memory address you want to read from
// //     // i2c_send((uint8_t)(address >> 8)); // MSB of address
// //     i2c_send((uint8_t)(address >> 8)); // MSB of address
// //     i2c_send((uint8_t)(address & 0xFF)); // LSB of address

// //     // Step 5: Send restart condition
// //     i2c_restart();

// //     // Step 6: Send EEPROM device address with read bit set again
// //     i2c_send(EEPROM_READ);

// //     // Step 7: Receive the data from the EEPROM
// //     uint8_t data = i2c_recv();

// //     // Step 8: Send NACK to indicate the end of read
// //     i2c_nack();

// //     // Step 9: Send stop condition
// //     i2c_stop();

// //     return data;
// // }

// // void eeprom_write_byte(uint16_t address, uint8_t data) {
// //     // Step 1: Initialize I2C
// //     // (Assuming you have already configured and initialized the I2C module)

// //     // Step 2: Send start condition
// //     i2c_start();
// uint8_t eeprom_read_byte(uint16_t address) {
//     // Step 1: Send start condition
//     i2c_start();

//     // Step 2: Send EEPROM device address with RW-bit = 0
//     i2c_send(EEPROM_WRITE);

//     // Step 3: Send the memory address you want to read from
//     i2c_send((uint8_t)(address >> 8)); // MSB of address
//     i2c_send((uint8_t)(address & 0xFF)); // LSB of address

//     // Step 4: Send restart condition
//     i2c_restart();

//     // Step 5: Send EEPROM device address with read bit set again
//     i2c_send(EEPROM_READ);

//     // Step 6: Receive the data from the EEPROM
//     uint8_t data = i2c_recv();

//     // Step 7: Send NACK to indicate the end of read
//     i2c_nack();

//     // Step 8: Send stop condition
//     i2c_stop();

//     return data;
// }


// void eeprom_write_byte(uint16_t address, uint8_t data) {
//     // Step 1: Send start condition
//     i2c_start();

// //     // Step 3: Send EEPROM device address with RW-bit 
// //     i2c_send(EEPROM_WRITE);
//     // Step 2: Send EEPROM device address with RW-bit 
//     i2c_send(EEPROM_WRITE);

// //     // Step 4: Send the memory address you want to write to
// //     i2c_send((uint8_t)(address >> 8)); // MSB of address
// //     i2c_send((uint8_t)(address & 0xFF)); // LSB of address
//     // Step 3: Send the memory address you want to write to
//     i2c_send((uint8_t)(address >> 8)); // MSB of address
//     i2c_send((uint8_t)(address & 0xFF)); // LSB of address

// //     // Step 5: Send the data to be written
// //     i2c_send(data);
//     // Step 4: Send the data to be written
//     i2c_send(data);

// //     // Step 6: Send stop condition
// //     i2c_stop();
//     // Step 5: Send stop condition
//     i2c_stop();

// //     // Add a delay to allow the EEPROM to complete the write operation
// //     delay(1000000);
// // }
//     // Add a delay to allow the EEPROM to complete the write operation
//     delay(I2C_DELAY); // Delay to allow the EEPROM to complete the write operation, can display 255 without delay if read too quickly
// }

// // void delay(int cyc)
// // {
// //     int i;
// //     for (i = cyc; i > 0; i--)
// //         ;
// // }
// /* Function used to delay operations */
// void delay(int cyc)
// {
//     int i;
//     for (i = cyc; i > 0; i--)
//         ;
// }

// // void score_display()
// // {
// //     display_string(1, itoaconv(score));
// void score_display()
// {
//     display_string(0, "Score: ");
//     display_string(1, itoaconv(score));

// //     highscore = eeprom_read_byte(SCORE_ADDRESS);
// //     display_string(3, itoaconv(highscore));
// //     display_update();
// // }
//     display_string(3, itoaconv(highscore));
//     display_update();
// }

// // void score_update()
// // {
// //     score++;
// //     if ((int)score > highscore)
// //     {
// //         highscore = score;
// //         eeprom_write_byte(SCORE_ADDRESS, highscore);
// //     }

// //     score_display();
// // }
// void score_update()
// {
//     score++;
//     if (score > highscore)
//     {
//         // highscore = score;
//         eeprom_write_byte(SCORE_ADDRESS, score);

//     }
//     score_display();
// }

// // /* Interrupt Service Routine */
// // void user_isr(void)
// // {
// //     if (IFS(1) & 0x1)
// //     { // button 4
// //         // read_highscore();
// //         score_update();
// //         delay(1000000);
// //         IFSCLR(1) = 0x1;
// //     }
// /* Interrupt Service Routine */
// void user_isr(void)
// {
//     if (IFS(1) & 0x1) // button 4, update score
//     {
//         score_update();
//         IFSCLR(1) = 0x1;
//     }

// //     if (IFS(0) & 0x80000) // switch 4, update led+=1
// //     {
// //         IFSCLR(0) = 0x80000;
// //         // Increase the LED value by 1 (roll over after 255)
// //         ledValue = (ledValue + 1) & 0xFF;
//     if (IFS(0) & 0x80000) // switch 4, update led+=1 and write ledvalue to EEPROM
//     {
//         IFSCLR(0) = 0x80000;
//         increment_LEDs();

// //         // Update the LEDs
// //         *porte = ledValue;

// //         eeprom_write_byte(SCORE_ADDRESS, 123);
// //         display_string(0, "Wrote to EEPROM");
// //         display_string(1, itoaconv(123));
// //         display_update();
// //     }
//         eeprom_write_byte(SCORE_ADDRESS, ledValue);
//         display_string(0, "Wrote to EEPROM");
//         display_string(1, itoaconv(ledValue));
//         display_update();
//     }

// //     if (IFS(0) & 0x8000) // switch 3, update led and write to EEPROM
// //     {
// //         IFSCLR(0) = 0x8000;
// //         // Increase the LED value by 1 (roll over after 255)
// //         ledValue = (ledValue + 1) & 0xFF;

// //         // Update the LEDs
// //         *porte = ledValue;
//     if (IFS(0) & 0x8000) // switch 3, read from EEPROM
//     {
//         IFSCLR(0) = 0x8000;

// //         highscore = eeprom_read_byte(SCORE_ADDRESS);
// //         display_string(0, "Read from EEPROM");
// //         display_string(1, itoaconv(highscore));
// //         display_update();
// //     }
//         highscore = eeprom_read_byte(SCORE_ADDRESS);
//         display_string(0, "Read from EEPROM");
//         display_string(3, itoaconv(highscore));
//         display_update();
//     }

// //     if (IFS(0) & 0x800) // switch 2, update led and read from EEPROM
// //     {
// //         IFSCLR(0) = 0x800;
// //         // Increase the LED value by 1 (roll over after 255)
// //         ledValue = (ledValue + 1) & 0xFF;

// //         // Update the LEDs
// //         *porte = ledValue;

// //         // highscore = readFromEEPROM(SCORE_ADDRESS);
// //         // read_highscore();
// //         // display_string(0, "Read from EEPROM");
// //         display_string(2, display_update());
// //         display_update();
// //     }
//     if (IFS(0) & 0x800) // switch 2
//     {
//         IFSCLR(0) = 0x800;
//         increment_LEDs();
//     }


// //     if (IFS(0) & 0x80) // switch 1, update led and update display
// //     {
// //         IFSCLR(0) = 0x80;
// //         // Increase the LED value by 1 (roll over after 255)
// //         ledValue = (ledValue + 1) & 0xFF;

// //         *porte = ledValue;

// //         display_update();
// //     }
// // }
//     if (IFS(0) & 0x80) // switch 1, update led and update display
//     {
//         IFSCLR(0) = 0x80;
//         increment_LEDs();
//     }
// }

// // void score_init()
// // {

// //     // Enable global interrupts
// //     enable_interrupt();
//     // Display string to indicate that the highscore is stored in EEPROM
// 	display_string(2, "Score in EEPROM: ");

// //     // Get the high score from EEPROM and store in highscore variable
// //     // read_highscore();
// //     score = I2C1RCV; // clear buffer

// //     volatile int *trise = (volatile int *)0xbf886100;
//     // Get the high score from EEPROM and store in highscore variable
//     highscore = eeprom_read_byte(SCORE_ADDRESS);

// //     // Display score and current highscore
// //     score_display();
// // }
//     // Display score and current highscore
//     score_display();
// }

// void increment_LEDs()
// {
//     // Increase the LED value by 1 (roll over after 255)
//     ledValue = (ledValue + 1) & 0xFF;

//     // Update the LEDs
//     *porte = ledValue;
// }
