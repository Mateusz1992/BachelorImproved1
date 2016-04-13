/*
 * TSL2561.h
 *
 *  Created on: 01-04-2016
 *      Author: Mateusz
 */

#ifndef TSL2561_H_
#define TSL2561_H_

#include <stdint.h>
#include <string.h>
#include <DAVE3.h>
#include <math.h>

extern double luminosity;

#define DELAY /*100*/1200

#define TRUE 1
#define FALSE 0

#define TSL2561_ADDR_0 0x29 // address with '0' shorted on board
#define TSL2561_ADDR   0x39 // default address
#define TSL2561_ADDR_1 0x49 // address with '1' shorted on board

// TSL2561 registers

#define TSL2561_CMD           0x80
#define TSL2561_CMD_CLEAR     0xC0
#define	TSL2561_REG_CONTROL   0x00
#define	TSL2561_REG_TIMING    0x01
#define	TSL2561_REG_THRESH_L  0x02
#define	TSL2561_REG_THRESH_H  0x04
#define	TSL2561_REG_INTCTL    0x06
#define	TSL2561_REG_ID        0x0A
#define	TSL2561_REG_DATA_0    0x0C
#define	TSL2561_REG_DATA_1    0x0E

void FIFO_Receive_Int_Handler1(void);

void startMeasurementsTSL2561(void);

void stopMeasurementsTSL2561(void);

void timerHandlerReceiveTSL2561(void *T);

void readTSL2561(void);

void SFE_TSL2561(void);

int begin1(void);

int begin11(char i2c_address);

int setPowerUp(void);

int setPowerDown(void);

int setTiming(int gain, unsigned char time);

int setTiming1(int gain, unsigned char time, unsigned int *ms);

int manualStart(void);

int manualStop(void);

int getData(unsigned int *data0, unsigned int *data1);

int getLux(unsigned char gain, unsigned int ms, unsigned int CH0, unsigned int CH1, double *lux);

int setInterruptControl(unsigned char control, unsigned char persist);

int setInterruptThreshold(unsigned int low, unsigned int high);

int clearInterrupt(void);

int getID(unsigned char *ID);

int getError(void);

int readByte(unsigned char address, unsigned char *value);

int writeByte(unsigned char address, unsigned char value);

int readUInt(unsigned char address, unsigned int *value);

int writeUInt(unsigned char address, unsigned int value);

//void FIFO_Receive_Int_Handler(void);

void delay2(int d);

double getLuminosity(void);

#endif /* TSL2561_H_ */
