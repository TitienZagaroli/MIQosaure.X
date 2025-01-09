/*
 * File:   SH1106.c
 * Author: titien - maxime
 *
 * Created on November 22, 2024, 11:51 PM
 */

#include "SH1106.h"

// Initialisation du SH1106
// Réalisée en suivant le datasheet du SH1106
void SH1106_Init(void)
{
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xAE); // Désactiver l'écran (Display OFF)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xD5); // Réglage de la fréquence d'oscillation
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x80); // Fréquence par défaut
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xA8); // Réglage de la hauteur (Multiplex ratio)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x3F); // 64 lignes
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xD3); // Décalage de l'affichage (Display offset)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x00); // Pas de décalage
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x40); // Définir la ligne de départ de RAM (Start line 0)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xAD); // Activer la pompe de charge interne
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x8B); // Pompage interne ON
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xA1); // Configurer le mappage des colonnes (Seg0 -> Seg127)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xC8); // Configurer la direction de balayage des lignes (COM remap)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xDA); // Réglage de la configuration du COM
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x12); // Configuration alternative, pas de parité
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x81); // Réglage de la luminosité (Contrast control)
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xBF); // Luminosité maximale
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xD9); // Réglage de la période de précharge
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x22); // Paramètre de précharge par défaut
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xDB); // Réglage du niveau de seuil de VCOMH
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x40); // Niveau de VCOMH par défaut
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xA6); // Mode normal (Non inversé)
}


void SH1106_TestOn(void)
{
    // Permet de tester les pixels
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xA5);
}

void SH1106_TestOff(void)
{
    // Permet d'éteindre les pixels
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xA4);
}


void SH1106_DisplayOn(void)
{
    // Permet d'activer l'affichage
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xAF);
}

void SH1106_DisplayOff(void)
{
    // Permet de désactiver l'affichage
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xAE);
}

// Fonction permettant déplacer le curseur sur l'écran
void SH1106_SetCursor(uint8_t page, uint8_t col)
{
    // On ajoute 2 à la colonne car on a un offset de 2 colonnes
    col += 2;
    // On sélectionne la page   
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0xB0 + page);
    // On sélectionne la colonne basse
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x00 + (col & 0x0F));
    // On sélectionne la colonne haute
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x00, 0x10 + (col >> 4));
}

// Fonction permettant d'afficher un caractère (une colonne)
void SH1106_WriteChar(char c)
{
    I2C_Write1ByteRegister(SH1106_ADDRESS, 0x40, c);
}

// Fonction permettant de nettoyer l'écran
void SH1106_Clear(void)
{
    // On parcourt toutes les pages
    for(uint8_t page = 0; page < 8; page++) {
        // On se place sur la première colonne de la page
        SH1106_SetCursor(page, 0);
        // On parcourt toutes les colonnes de la page
        for(uint8_t col = 0; col < SH1106_WIDTH; col++) {
            // On écrit un caractère nul
            SH1106_WriteChar(0x00);
        }
    }
}
