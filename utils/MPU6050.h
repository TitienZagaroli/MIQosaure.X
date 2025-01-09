/* 
 * File:   MPU6050.h
 * Author: titien - maxime
 *
 * Created on November 22, 2024, 11:48 PM
 */

#ifndef MPU6050_H
#define	MPU6050_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#include "I2C.h"

// Définition des adresses et des registres
#define MPU6050_ADDRESS 0b1101000       // Adresse du MPU6050
#define MPU6050_REG_WHO_AM_I 0x75       // Registre de l'ID du MPU6050
#define MPU6050_REG_ACCEL_XOUT_H 0x3B   // Registre de l'accélération selon l'axe X
#define MPU6050_REG_ACCEL_YOUT_H 0x3D   // Registre de l'accélération selon l'axe Y
#define MPU6050_REG_ACCEL_ZOUT_H 0x3F   // Registre de l'accélération selon l'axe Z

// Initialisation du MPU6050
uint8_t MPU6050_Init(void);
// Lecture des valeurs des accélérations
void MPU6050_Read_AccelXY(int16_t *x, int16_t *y);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU6050_H */

