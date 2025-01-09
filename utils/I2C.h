/* 
 * File:   I2C.h
 * Author: titien
 *
 * Created on November 22, 2024, 11:47 PM
 */

// Ce ficier contient le code généré par MCC

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#include "../mcc_generated_files/i2c2_master.h"

#include <stdint.h>
#include <stdio.h>

uint8_t  I2C_Read1ByteRegister(i2c2_address_t address, uint8_t reg);
uint16_t I2C_Read2ByteRegister(i2c2_address_t address, uint8_t reg);

void I2C_Write1ByteRegister(i2c2_address_t address, uint8_t reg, uint8_t data);
void I2C_Write2ByteRegister(i2c2_address_t address, uint8_t reg, uint16_t data);

void I2C_WriteNBytes(i2c2_address_t address, uint8_t *data, size_t len);
void I2C_ReadNBytes(i2c2_address_t address, uint8_t *data, size_t len);

void I2C_ReadDataBlock(i2c2_address_t address, uint8_t reg, uint8_t *data, size_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

