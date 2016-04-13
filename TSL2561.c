/*
 * TSL2561.c
 *
 *  Created on: 01-04-2016
 *      Author: Mateusz
 */
#include "TSL2561.h"

extern int gain;
extern unsigned char time;
extern unsigned int ms;

double luminosity;

uint16_t DataReceive11 = 0;
uint8_t received1 = 0;


uint8_t _i2c_address;
uint8_t _error;

handle_t TimerIdTSL2561 = 0;
uint32_t StatusTimerTSL2561 = SYSTM001_ERROR;


void FIFO_Receive_Int_Handler1(void)
{
	/*uint16_t */DataReceive11 = 0;

	I2C001_ReadData(&I2C001_Handle2,&DataReceive11);
	//delay(10000);
	received1 = 1;
}

void startMeasurementsTSL2561(void)
{
	TimerIdTSL2561 = SYSTM001_CreateTimer(60,SYSTM001_PERIODIC,timerHandlerReceiveTSL2561,NULL/*&adrAndData*/);
	SYSTM001_StartTimer(TimerIdTSL2561);
}

void stopMeasurementsTSL2561(void)
{
	SYSTM001_StopTimer(TimerIdTSL2561);

	SYSTM001_DeleteTimer(TimerIdTSL2561);
}

void timerHandlerReceiveTSL2561(void *T)
{
	unsigned int data0, data1;

	/*int gain = 0;
	unsigned int ms;
	unsigned char time = 2;*/

	if(getData(&data0, &data1))
	{
	    double lux;    // Resulting lux value
	    int good;  // True if neither sensor is saturated

	    good = getLux(gain, ms, data0, data1, &lux);

	    if(good)
	    {
	    	double l = lux;

	    	luminosity = lux;

	    	//pomiary[Count] = l;
	    }
	}
}

void readTSL2561(void)
{
	unsigned int data0, data1;

	/*int gain = 0;
	unsigned int ms;
	unsigned char time = 2;*/

	if(getData(&data0, &data1))
	{
	    double lux;    // Resulting lux value
	    int good;  // True if neither sensor is saturated

	    good = getLux(gain, ms, data0, data1, &lux);

	    if(good)
	    {
	    	double l = lux;

	    	luminosity = lux;

	    	//pomiary[Count] = l;
	    }
	}
}

void SFE_TSL2561(void)
{
	;
}

int begin1(void)
{
	return(begin11(TSL2561_ADDR));
}

int begin11(char i2c_address)
{
	_i2c_address = i2c_address;

	return TRUE;
}

int setPowerUp(void)
	// Turn on TSL2561, begin integrations
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Write 0x03 to command byte (power on)
	return(writeByte(TSL2561_REG_CONTROL,0x03));
}

int setPowerDown(void)
	// Turn off TSL2561
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Clear command byte (power off)
	return(writeByte(TSL2561_REG_CONTROL,0x00));
}

int setTiming(int gain, unsigned char time)
	// If gain = false (0), device is set to low gain (1X)
	// If gain = high (1), device is set to high gain (16X)
	// If time = 0, integration will be 13.7ms
	// If time = 1, integration will be 101ms
	// If time = 2, integration will be 402ms
	// If time = 3, use manual start / stop
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	unsigned char timing;

	// Get timing byte
	if (readByte(TSL2561_REG_TIMING,&timing))
	{
		// Set gain (0 or 1)
		if (gain)
			timing |= 0x10;
		else
			timing &= ~0x10;

		// Set integration time (0 to 3)
		timing &= ~0x03;
		timing |= (time & 0x03);

		// Write modified timing byte back to device
		if (writeByte(TSL2561_REG_TIMING,timing))
			return(TRUE);
	}
	return(FALSE);
}


int setTiming1(int gain, unsigned char time, unsigned int *ms)
	// If gain = false (0), device is set to low gain (1X)
	// If gain = high (1), device is set to high gain (16X)
	// If time = 0, integration will be 13.7ms
	// If time = 1, integration will be 101ms
	// If time = 2, integration will be 402ms
	// If time = 3, use manual start / stop (ms = 0)
	// ms will be set to integration time
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Calculate ms for user
	switch (time)
	{
		case 0: *ms = 14; break;
		case 1: *ms = 101; break;
		case 2: *ms = 402; break;
		default: *ms = 0;
	}
	// Set integration using base function
	return(setTiming(gain,time));
}

int manualStart(void)
	// Starts a manual integration period
	// After running this command, you must manually stop integration with manualStop()
	// Internally sets integration time to 3 for manual integration (gain is unchanged)
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	unsigned char timing;

	// Get timing byte
	if (readByte(TSL2561_REG_TIMING,&timing))
	{
		// Set integration time to 3 (manual integration)
		timing |= 0x03;

		if (writeByte(TSL2561_REG_TIMING,timing))
		{
			// Begin manual integration
			timing |= 0x08;

			// Write modified timing byte back to device
			if (writeByte(TSL2561_REG_TIMING,timing))
				return(TRUE);
		}
	}
	return(FALSE);
}

int manualStop(void)
	// Stops a manual integration period
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	unsigned char timing;

	// Get timing byte
	if (readByte(TSL2561_REG_TIMING,&timing))
	{
		// Stop manual integration
		timing &= ~0x08;

		// Write modified timing byte back to device
		if (writeByte(TSL2561_REG_TIMING,timing))
			return(TRUE);
	}
	return(FALSE);
}

int getData(unsigned int *data0, unsigned int *data1)
	// Retrieve raw integration results
	// data0 and data1 will be set to integration results
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Get data0 and data1 out of result registers
	if (!readUInt(TSL2561_REG_DATA_0,data0))
	{
		return(FALSE);
	}


	if(!readUInt(TSL2561_REG_DATA_1,data1))
	{
		return(FALSE);
	}

	return(TRUE);
}

int getLux(unsigned char gain, unsigned int ms, unsigned int CH0, unsigned int CH1, double *lux)
	// Convert raw data to lux
	// gain: 0 (1X) or 1 (16X), see setTiming()
	// ms: integration time in ms, from setTiming() or from manual integration
	// CH0, CH1: results from getData()
	// lux will be set to resulting lux calculation
	// returns true (1) if calculation was successful
	// RETURNS false (0) AND lux = 0.0 IF EITHER SENSOR WAS SATURATED (0XFFFF)
{
	double ratio, d0, d1;

	// Determine if either sensor saturated (0xFFFF)
	// If so, abandon ship (calculation will not be accurate)
	if ((CH0 == 0xFFFF) || (CH1 == 0xFFFF))
	{
		*lux = 0.0;
		return(FALSE);
	}

	// Convert from unsigned integer to floating point
	d0 = CH0; d1 = CH1;

	// We will need the ratio for subsequent calculations
	ratio = d1 / d0;

	// Normalize for integration time
	d0 *= (402.0/ms);
	d1 *= (402.0/ms);

	// Normalize for gain
	if (!gain)
	{
		d0 *= 16;
		d1 *= 16;
	}

	// Determine lux per datasheet equations:

	if (ratio < 0.5)
	{
		*lux = 0.0304 * d0 - 0.062 * d0 * pow(ratio,1.4);
		return(TRUE);
	}

	if (ratio < 0.61)
	{
		*lux = 0.0224 * d0 - 0.031 * d1;
		return(TRUE);
	}

	if (ratio < 0.80)
	{
		*lux = 0.0128 * d0 - 0.0153 * d1;
		return(TRUE);
	}

	if (ratio < 1.30)
	{
		*lux = 0.00146 * d0 - 0.00112 * d1;
		return(TRUE);
	}

	// if (ratio > 1.30)
	*lux = 0.0;
	return(TRUE);
}

int setInterruptControl(unsigned char control, unsigned char persist)
	// Sets up interrupt operations
	// If control = 0, interrupt output disabled
	// If control = 1, use level interrupt, see setInterruptThreshold()
	// If persist = 0, every integration cycle generates an interrupt
	// If persist = 1, any value outside of threshold generates an interrupt
	// If persist = 2 to 15, value must be outside of threshold for 2 to 15 integration cycles
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Place control and persist bits into proper location in interrupt control register
	if (writeByte(TSL2561_REG_INTCTL,((control | 0B00000011) << 4) & (persist | 0B00001111)))
		return(TRUE);

	return(FALSE);
}

int setInterruptThreshold(unsigned int low, unsigned int high)
	// Set interrupt thresholds (channel 0 only)
	// low, high: 16-bit threshold values
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Write low and high threshold values
	if (writeUInt(TSL2561_REG_THRESH_L,low) && writeUInt(TSL2561_REG_THRESH_H,high))
		return(TRUE);

	return(FALSE);
}

int clearInterrupt(void)
	// Clears an active interrupt
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Set up command byte for interrupt clear
	/*Wire.beginTransmission(_i2c_address);
	Wire.write(TSL2561_CMD_CLEAR);
	_error = Wire.endTransmission();*/
	/*if (_error == 0)
		return(true);

	return(false);*/
	return TRUE;
}

int getID(unsigned char *ID)
	// Retrieves part and revision code from TSL2561
	// Sets ID to part ID (see datasheet)
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Get ID byte from ID register
	if (readByte(TSL2561_REG_ID,ID))
		return(TRUE);

	return(FALSE);
}

int getError(void)
	// If any library command fails, you can retrieve an extended
	// error code using this command. Errors are from the wire library:
	// 0 = Success
	// 1 = Data too long to fit in transmit buffer
	// 2 = Received NACK on transmit of address
	// 3 = Received NACK on transmit of data
	// 4 = Other error
{
	return(_error);
}

int readByte(unsigned char address, unsigned char *value)
{
	I2C001_DataType data1;
	data1.Data1.TDF_Type = I2C_TDF_MStart;
	data1.Data1.Data = ((_i2c_address<<1) | I2C_WRITE);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data1);/*);*/

	delay2(DELAY*10);

	I2C001_DataType data2;
	data2.Data1.TDF_Type = I2C_TDF_MTxData;
	data2.Data1.Data = ((address & 0x0F) | TSL2561_CMD);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data2);/*);*/

	delay2(DELAY*10);

	//delay2(15000);
	I2C001_DataType data3;
	data3.Data1.TDF_Type = I2C_TDF_MRStart;
	data3.Data1.Data = ((_i2c_address<<1) | I2C_READ);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data3);/*);*/

	delay2(DELAY*10);


	I2C001_DataType data4;
	data4.Data1.TDF_Type = I2C_TDF_MRxAck1;
	data4.Data1.Data = ubyteFF;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data4);/*);*/

	delay2(DELAY);

	I2C001_DataType data5;
	data5.Data1.TDF_Type = I2C_TDF_MStop;
	data5.Data1.Data = ubyteFF;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data5);/*);*/

	int k = 0;
	while(0 == received1){k++; if(k > 4000) break; }
	received1 = 0;
	delay2(DELAY*10);
	int j = USIC_GetRxFIFOFillingLevel(I2C001_Handle2.I2CRegs);

	*value = (uint8_t)DataReceive11;

	luminosity = (uint8_t)DataReceive11;

	return TRUE;
}

int writeByte(unsigned char address, unsigned char value)
{
	uint8_t c = (_i2c_address<<1);
	I2C001_DataType data1;
	data1.Data1.TDF_Type = I2C_TDF_MStart;
	data1.Data1.Data = (c | I2C_WRITE);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data1);/*);*/

	delay2(DELAY);

	I2C001_DataType data2;
	data2.Data1.TDF_Type = I2C_TDF_MTxData;
	data2.Data1.Data = ((address & 0x0F) | TSL2561_CMD);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data2);/*);*/

	delay2(DELAY);

	I2C001_DataType data3;
	data3.Data1.TDF_Type = I2C_TDF_MTxData;
	data3.Data1.Data = value;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data3);/*);*/

	delay2(DELAY);

	I2C001_DataType data4;
	data4.Data1.TDF_Type = I2C_TDF_MStop;
	data4.Data1.Data = ubyteFF;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data4);/*);*/

	delay2(DELAY * 100);

	return TRUE;
}

int readUInt(unsigned char address, unsigned int *value)
{
	char high, low;

	int k = 0;

	uint16_t buffer = 0;
	uint16_t buffer1 = 0;


	I2C001_DataType data1;
	data1.Data1.TDF_Type = I2C_TDF_MStart;
	data1.Data1.Data = ((_i2c_address<<1) | I2C_WRITE);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data1);/*);*/

	delay2(DELAY);

	I2C001_DataType data2;
	data2.Data1.TDF_Type = I2C_TDF_MTxData;
	data2.Data1.Data = ((address & 0x0F) | TSL2561_CMD);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data2);/*);*/
	/*{
		USIC_FlushTxFIFO(I2CRegs);
	}*/
	delay2(DELAY);

	I2C001_DataType data3;
	data3.Data1.TDF_Type = I2C_TDF_MRStart;
	data3.Data1.Data = ((_i2c_address<<1) | I2C_READ);
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data3);/*);*/
	/*{
		USIC_FlushTxFIFO(I2CRegs);
	}*/
	delay2(DELAY);

	I2C001_DataType data4;
	data4.Data1.TDF_Type = I2C_TDF_MRxAck0;
	data4.Data1.Data = ubyteFF;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data4);/*);*/
	/*{
		USIC_FlushTxFIFO(I2CRegs);
	}*/
	delay2(DELAY);

	while(0 == received1){ k++; if(k > 4000) break; }
	received1 = 0;
	k = 0;

	buffer = DataReceive11;

	I2C001_DataType data5;
	data5.Data1.TDF_Type = I2C_TDF_MRxAck1;
	data5.Data1.Data = ubyteFF;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data5);/*);*/
	/*{
		USIC_FlushTxFIFO(I2CRegs);
	}*/
	delay2(DELAY);

	while(0 == received1){ k++; if(k > 4000) break; }
	received1 = 0;
	k = 0;

	buffer1 = DataReceive11;

	I2C001_DataType data6;
	data6.Data1.TDF_Type = I2C_TDF_MStop;
	data6.Data1.Data = ubyteFF;
	/*while(!*/I2C001_WriteData(&I2C001_Handle2,&data6);/*);*/

	delay2(DELAY * 100);

	buffer1 <<= 8;
	buffer1 |= buffer;

	*value = buffer1;
	luminosity = buffer1;

	return TRUE;


}

int writeUInt(unsigned char address, unsigned int value)
{
	writeByte(address, value); //low byte

	writeByte(address + 1, (value >> 8)); //high byte

	return TRUE;
}

/*void FIFO_Receive_Int_Handler1(void)
{
	DataReceive11 = 0;

	I2C001_ReadData(&I2C001_Handle2,&DataReceive11);

	received1 = 1;
}*/

void delay2(int d)
{
	for(int i = 0; i < d; i++) i++;
}

double getLuminosity(void)
{
	return luminosity;
}

