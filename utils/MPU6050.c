/*
 * File:   MPU6050.c
 * Author: titien - maxime
 *
 * Created on November 22, 2024, 11:51 PM
 */

#include "MPU6050.h"

// Initialisation du MPU6050
// Réalisée en suivant le datasheet du MPU6050
uint8_t MPU6050_Init(void)
{
    uint8_t buffer_u8 = 0;
    
    // On lit le registre WHO_AM_I
    buffer_u8 = I2C_Read1ByteRegister(MPU6050_ADDRESS, MPU6050_REG_WHO_AM_I);
    
    // On sort de veille
    I2C_Write1ByteRegister(MPU6050_ADDRESS, 0x6B, 0x00);
    // Plage de mesure accélération ±2g
    I2C_Write1ByteRegister(MPU6050_ADDRESS, 0x1C, 0x00);
    // Plage de mesure gyroscope ±250°/s
    I2C_Write1ByteRegister(MPU6050_ADDRESS, 0x1B, 0x00);
    
    return buffer_u8;
}

// Lecture des valeurs des accélérations
void MPU6050_Read_AccelXY(int16_t *x, int16_t *y)
{
    // On lit les valeurs des accélérations
    // On lit l'axe X
    *x = I2C_Read2ByteRegister(MPU6050_ADDRESS, MPU6050_REG_ACCEL_XOUT_H);
    // On lit l'axe Y
    *y = I2C_Read2ByteRegister(MPU6050_ADDRESS, MPU6050_REG_ACCEL_YOUT_H);
}