/**
 * @file i2c-func.c
 * @brief Functions for I2C communication
 *
 * This file contains the implementation of functions for I2C communication.
 * It provides the necessary functionality to communicate with devices using the I2C protocol.
 * This code is borrowed from the IS1200 example project hello_temperature: https://github.com/is1200-example-projects/hello-temperature/tree/master 
 *
 * These functions are used by highscore.c to communicate with the EEPROM memory.
 * 
 * @author Axel Isaksson
 * @author Mattias Kvist (Documentation)
 *
 * @date 2023-12-07
 *
 * For licensing information, see the file COPYING.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>
#include <stdbool.h>

/**
 * @brief Wait for I2C bus to become idle
 */
void i2c_idle() {
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); // TRSTAT (transmit status bit) = 1 or ACKSTAT (acknowledge status bit) = 1
}

/**
 * @brief Send one byte on I2C bus, return ack/nack status of transaction
 * @param data The byte to be sent
 * @return true if ack received, false if nack received
 */
bool i2c_send(uint8_t data) {
	i2c_idle();
	I2C1TRN = data; // load data into transmit register
	i2c_idle();
	return !(I2C1STAT & (1 << 15)); // ACKSTAT bit, 1 = ack not received, 0 = ack received
}

/**
 * @brief Receive one byte from I2C bus
 * @return The received byte
 */
uint8_t i2c_recv() {
	i2c_idle();
	I2C1CONSET = 1 << 3; // RCEN = 1 (receive enable bit)
	i2c_idle();
	I2C1STATCLR = 1 << 6; // I2COV = 0 (clear overflow bit)
	return I2C1RCV;
}

/**
 * @brief Send acknowledge condition on the bus
 */
void i2c_ack() {
	i2c_idle();
	I2C1CONCLR = 1 << 5; // ACKDT = 0
	I2C1CONSET = 1 << 4; // ACKEN = 1
}

/**
 * @brief Send not-acknowledge condition on the bus
 */
void i2c_nack() {
	i2c_idle();
	I2C1CONSET = 1 << 5; // ACKDT = 1  (nack sent)
	I2C1CONSET = 1 << 4; // ACKEN = 1 (ack sequence enable bit)
}

/**
 * @brief Send start condition on the bus
 */
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; // SEN (initiate start condition, cleared by hardware when complete)
	i2c_idle();
}

/**
 * @brief Send restart condition on the bus
 */
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; // RSEN (initiate restart condition, cleared by hardware when complete)
	i2c_idle();
}

/**
 * @brief Send stop condition on the bus
 */
void i2c_stop() {
	i2c_idle();
	I2C1CONSET = 1 << 2; // PEN (initiate stop condition, cleared by hardware when complete)
	i2c_idle();
}
