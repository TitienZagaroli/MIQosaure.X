/*
 * File:   I2C.c
 * Author: titien - maxime
 *
 * Created on November 22, 2024, 11:50 PM
 */

// Ce ficier contient le code généré par MCC

#include "I2C.h"

typedef struct
{
    size_t len;
    uint8_t *data;
}i2c2_buffer_t;

static i2c2_operations_t rd1RegCompleteHandler(void *ptr);
static i2c2_operations_t rd2RegCompleteHandler(void *ptr);
static i2c2_operations_t wr1RegCompleteHandler(void *ptr);
static i2c2_operations_t wr2RegCompleteHandler(void *ptr);

uint8_t I2C_Read1ByteRegister(i2c2_address_t address, uint8_t reg)
{
    uint8_t returnValue = 0x00;
    
    while(!I2C2_Open(address)); // sit here until we get the bus..
    I2C2_SetDataCompleteCallback(rd1RegCompleteHandler,&returnValue);
    I2C2_SetBuffer(&reg,1);
    I2C2_SetAddressNackCallback(NULL,NULL); //NACK polling?
    I2C2_MasterWrite();
    while(I2C2_BUSY == I2C2_Close()); // sit here until finished.
    
    return returnValue;
}   

uint16_t I2C_Read2ByteRegister(i2c2_address_t address, uint8_t reg)
{
    uint16_t returnValue =0x00; // returnValue is little endian

    while(!I2C2_Open(address)); // sit here until we get the bus..
    I2C2_SetDataCompleteCallback(rd2RegCompleteHandler,&returnValue);
    I2C2_SetBuffer(&reg,1);
    I2C2_SetAddressNackCallback(NULL,NULL); //NACK polling?
    I2C2_MasterWrite();
    while(I2C2_BUSY == I2C2_Close()); // sit here until finished.
  
    return (returnValue << 8 | returnValue >> 8);
}

void I2C_Write1ByteRegister(i2c2_address_t address, uint8_t reg, uint8_t data)
{
    while(!I2C2_Open(address)); // sit here until we get the bus..
    I2C2_SetDataCompleteCallback(wr1RegCompleteHandler,&data);
    I2C2_SetBuffer(&reg,1);
    I2C2_SetAddressNackCallback(NULL,NULL); //NACK polling?
    I2C2_MasterWrite();
    while(I2C2_BUSY == I2C2_Close()); // sit here until finished.
}

void I2C_Write2ByteRegister(i2c2_address_t address, uint8_t reg, uint16_t data)
{
    while(!I2C2_Open(address)); // sit here until we get the bus..
    I2C2_SetDataCompleteCallback(wr2RegCompleteHandler,&data);
    I2C2_SetBuffer(&reg,1);
    I2C2_SetAddressNackCallback(NULL,NULL); //NACK polling?
    I2C2_MasterWrite();
    while(I2C2_BUSY == I2C2_Close()); // sit here until finished.
}

static i2c2_operations_t rd1RegCompleteHandler(void *ptr)
{
    I2C2_SetBuffer(ptr,1);
    I2C2_SetDataCompleteCallback(NULL,NULL);
    return I2C2_RESTART_READ;
}

static i2c2_operations_t rd2RegCompleteHandler(void *ptr)
{
    I2C2_SetBuffer(ptr,2);
    I2C2_SetDataCompleteCallback(NULL,NULL);
    return I2C2_RESTART_READ;
}

static i2c2_operations_t wr1RegCompleteHandler(void *ptr)
{
    I2C2_SetBuffer(ptr,1);
    I2C2_SetDataCompleteCallback(NULL,NULL);
    return I2C2_CONTINUE;
}

static i2c2_operations_t wr2RegCompleteHandler(void *ptr)
{
    I2C2_SetBuffer(ptr,2);
    I2C2_SetDataCompleteCallback(NULL,NULL);
    return I2C2_CONTINUE;
}