#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic32mx.h>
#include <stdbool.h>

/* Wait for I2C bus to become idle */
void i2c_idle() {
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); // TRSTAT (transmit status bit) = 1 or ACKSTAT (acknowledge status bit) = 1
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool i2c_send(uint8_t data) {
	i2c_idle();
	I2C1TRN = data; // load data into transmit register
	i2c_idle();
	return !(I2C1STAT & (1 << 15)); // ACKSTAT bit, 1 = ack not received, 0 = ack received
}

/* Receive one byte from I2C bus */
uint8_t i2c_recv() {
	i2c_idle();
	I2C1CONSET = 1 << 3; // RCEN = 1 (receive enable bit)
	i2c_idle();
	I2C1STATCLR = 1 << 6; // I2COV = 0 (clear overflow bit)
	return I2C1RCV;
}

/* Send acknowledge conditon on the bus */
void i2c_ack() {
	i2c_idle();
	I2C1CONCLR = 1 << 5; // ACKDT = 0
	I2C1CONSET = 1 << 4; // ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
	i2c_idle();
	I2C1CONSET = 1 << 5; // ACKDT = 1  (nack sent)
	I2C1CONSET = 1 << 4; // ACKEN = 1 (ack sequence enable bit)
}

/* Send start conditon on the bus */
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; // SEN (initiate start condition, cleared by hardware when complete)
	i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; // RSEN (initiate restart condition, cleared by hardware when complete)
	i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
	i2c_idle();
	I2C1CONSET = 1 << 2; // PEN (initiate stop condition, cleared by hardware when complete)
	i2c_idle();
}
