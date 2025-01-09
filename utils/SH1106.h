/* 
 * File:   SH1106.h
 * Author: titien - maxime
 *
 * Created on November 22, 2024, 11:49 PM
 */

#ifndef SH1106_H
#define	SH1106_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#include "I2C.h"

// Paramètres de l'écran OLED SH1106
#define SH1106_WIDTH 128   // Largeur de l'écran
#define SH1106_PAGES 8

// Adresse de l'écran OLED SH1106
#define SH1106_ADDRESS 0b0111100

// Initialisation de l'écran OLED SH1106
void SH1106_Init(void);

// Permet de tester les pixels
void SH1106_TestOn(void);
// Permet d'éteindre les pixels
void SH1106_TestOff(void);

// Permet d'activer l'affichage
void SH1106_DisplayOn(void);
// Permet d'éteindre l'affichage
void SH1106_DisplayOff(void);

// Permet de déplacer le curseur sur l'écran
void SH1106_SetCursor(uint8_t page, uint8_t col);
// Permet d'écrire un caractère
void SH1106_WriteChar(char c);
// Permet de nettoyer l'écran
void SH1106_Clear(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SH1106_H */

