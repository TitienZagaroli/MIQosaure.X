/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F18446
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

#include "utils/SH1106.h"
#include "utils/MPU6050.h"

// ----------------------------------------------------------------------------
// ---------------------------------- DEFINES ---------------------------------
// ----------------------------------------------------------------------------

#define ACCEL_THRESHOLD 3000	// Seuil de basculement pour déplacer le personnage
#define POS_MAX 36				// Nombre de positions durant le jeu
#define TIME_BAR_MAX 48			// Nombre max de la barre de temps


// ----------------------------------------------------------------------------
// ---------------------------------- TYPES -----------------------------------
// ----------------------------------------------------------------------------

// Définition du type point_t qui permet de stocker les coordonnées d'un point
// Sur la grille de jeu
typedef struct {
	uint8_t x;
	uint8_t y;
} point_t;


// ----------------------------------------------------------------------------
// ---------------------------------- CONSTANTS -----------------------------
// ----------------------------------------------------------------------------

// Permet d'écrire "MIQosaure" sur l'écran OLED
// MIQosaure : Text -> Bitmap 6x8
const uint8_t MIQosaure_6x8[9][6] = {
	{0x00, 0x1F, 0x02, 0x04, 0x02, 0x1F},
	{0x00, 0x11, 0x1F, 0x11, 0x00, 0x00},
	{0x00, 0x0E, 0x11, 0x19, 0x0E, 0x10},
	{0x00, 0x0C, 0x12, 0x12, 0x0C, 0x00},
	{0x00, 0x14, 0x16, 0x1A, 0x0A, 0x00},
	{0x00, 0x10, 0x15, 0x15, 0x1E, 0x00},
	{0x00, 0x0E, 0x10, 0x10, 0x1E, 0x00},
	{0x00, 0x1E, 0x02, 0x02, 0x04, 0x00},
	{0x00, 0x0C, 0x1A, 0x1A, 0x0C, 0x00},
};

// Permet d'écrire "Game Over" sur l'écran OLED
// Game Over : Text -> Bitmap 6x8
const uint8_t Game_Over_6x8[11][6] = {
{0x00, 0x0E, 0x11, 0x15, 0x1D, 0x00},
{0x00, 0x10, 0x15, 0x15, 0x1E, 0x00},
{0x00, 0x1E, 0x02, 0x1C, 0x02, 0x1C},
{0x00, 0x0C, 0x1A, 0x1A, 0x0C, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x0E, 0x11, 0x11, 0x0E, 0x00},
{0x00, 0x06, 0x08, 0x10, 0x08, 0x06},
{0x00, 0x0C, 0x1A, 0x1A, 0x0C, 0x00},
{0x00, 0x1E, 0x02, 0x02, 0x04, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x17, 0x00, 0x00, 0x00},
};

// --- Grille de jeu ---
// Case vide par défaut
const uint8_t game_grid[8] = 		{0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
// Case occupée par le personnage
const uint8_t game_cursor[8] = 		{0xFF, 0x81, 0x99, 0xBD, 0xBD, 0x99, 0x81, 0xFF};
// Case occupée par la destination
const uint8_t game_dest[8] = 		{0xFF, 0xE7, 0xC3, 0x81, 0x81, 0xC3, 0xE7, 0xFF};
// Case qui masque le dinosaure
const uint8_t game_dino_grid[8] = 	{0xFF, 0xC3, 0xA5, 0x99, 0x99, 0xA5, 0xC3, 0xFF};

// --- Barre de progression ---
// Barre de progression basse
const uint8_t bar_low = 0x01;
// Barre de progression haute
const uint8_t bar_high = 0x3D;

// Image du dinosaure qui permet de l'afficher sur l'écran OLED
// Inspiré du dinosaure de Google Chrome
const uint8_t image[6][6][8] = {
	{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0xF8, 0xFC, 0xFC, 0xFF, 0xEF, 0xC7, 0xC7, 0xE7}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC}},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0x8F, 0x8F, 0x8F}, {0x8F, 0x8F, 0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F}},
    {{0xFE, 0xFE, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0}, {0xC0, 0xC0, 0xF0, 0xF8, 0xF8, 0xF8, 0xFE, 0xFE}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0x61, 0xE1, 0xE1, 0xE1, 0xE1, 0x01}, {0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC}, {0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F}, {0x7F, 0x00, 0x00, 0x00, 0x03, 0x03, 0x01, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0F}, {0x1F, 0x1F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF}, {0xFF, 0xFF, 0x1F, 0x1F, 0x07, 0x03, 0x03, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xCF, 0xCF}, {0x47, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01}, {0xFF, 0xFF, 0xC0, 0xC0, 0x40, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
};

// Ordre des positions à découvrir sur l'image du dinosaure
const uint8_t positions[36][2] = {
	{5, 1}, {1, 0}, {5, 5}, {3, 4}, {3, 2}, {2, 3},
	{4, 4}, {1, 2}, {1, 1}, {3, 1}, {5, 0}, {5, 2},
	{2, 2}, {4, 2}, {0, 2}, {4, 0}, {3, 0}, {1, 4},
	{0, 5}, {2, 4}, {1, 3}, {2, 0}, {2, 1}, {5, 4},
	{0, 3}, {4, 1}, {0, 1}, {5, 3}, {0, 4}, {3, 3},
	{4, 3}, {3, 5}, {4, 5}, {2, 5}, {1, 5}, {0, 0}
};


// ----------------------------------------------------------------------------
// ---------------------------------- MAIN ------------------------------------
// ----------------------------------------------------------------------------

/*
                         Main application
 */
void main(void)
{    
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // User code


	// ---------------------
	// ----- Variables -----
	// ---------------------
	int8_t time_bar = 0;				// Barre de temps
	uint8_t pos_index = 0;				// Index de la position à découvrir

    int16_t accel_x, accel_y = 0;		// Valeurs x et y de l'accéléromètre

	uint8_t random = 0;					// Valeur aléatoire (générée à partir des valeurs de l'accéléromètre)

	point_t game_cursor_pos = {3, 4};	// Position du curseur
	point_t game_cursor_pos_old;		// Position précédente du curseur
	point_t game_dest_pos = {6, 7};		// Position de la destination


	// --------------------------
    // ----- Initialisation -----
	// --------------------------
	
    SH1106_Init();			// Initialisation de l'écran OLED
	SH1106_Clear();			// Effacer l'écran OLED
    SH1106_DisplayOn();		// Allumer l'écran OLED


	// -----------------------------------------
	// ----- Affichage de l'écran de début -----
	// -----------------------------------------

	// On seplace sur la page 0, à la colonne 9*8=72	
	SH1106_SetCursor(0, 8*9);

	// On affiche les 9 lettres de "MIQosaure"
	// Pour chaque lettre
	for(uint8_t letter = 0; letter < 9; letter++) {
		// Pour 
		for(uint8_t row = 0; row < 6; row++) {
			// On affiche la lettre
			SH1106_WriteChar(MIQosaure_6x8[letter][row]);
		}
	}
    
    // Afficher le tableau de jeu
	// Pour chaque page
    for (uint8_t page = 0; page < 8; page++) {
		// On se place sur la page, à la colonne 0
		SH1106_SetCursor(page, 0);
		// Pour chaque case
		for(uint8_t case_num = 0; case_num < 8; case_num++) {
			// Pour chaque colonne
			for(uint8_t col = 0; col < 8; col++) {
				// On affiche la case
				SH1106_WriteChar(game_grid[col]);
			}
		}
    }

	// Afficher le tableau du dinosaure
	// Pour chaque colonne
	for(uint8_t col = 0; col < 6; col++) {
		// On se place sur la colonne, à la page 9*8=72
		SH1106_SetCursor(col + 2, 8*9);
		// Pour chaque case
		for(uint8_t case_num = 0; case_num < 6; case_num++) {
			// Pour chaque ligne
			for(uint8_t row = 0; row < 8; row++) {
				// On affiche la case
				SH1106_WriteChar(game_dino_grid[row]);
			}
		}
	}

	// Afficher le curseur et la destination
	// On se place sur la position du curseur
	SH1106_SetCursor(game_cursor_pos.x, game_cursor_pos.y * 8);
	// Pour chaque colonne
	for(uint8_t col = 0; col < 8; col++) {
		// On affiche la case
		SH1106_WriteChar(game_cursor[col] | game_grid[col]);
	}
	// On se place sur la position de la destination
	SH1106_SetCursor(game_dest_pos.x, game_dest_pos.y * 8);
	for(uint8_t col = 0; col < 8; col++) {
		SH1106_WriteChar(game_dest[col] | game_grid[col]);
	}

    __delay_ms(100);


	// --------------------------
    // ----- Initialisation -----
	// --------------------------

    // Initialisation de l'accéléromètre
    MPU6050_Init();

	
	// ------------------------
    // ----- Début du jeu -----
	// ------------------------

	// Initialisation de la barre de temps
	time_bar = TIME_BAR_MAX;

	// Afficher la barre de progression
	SH1106_SetCursor(1, 8*9);
	for(uint8_t col = 0; col < TIME_BAR_MAX; col++) {
		SH1106_WriteChar(bar_high);
	}

	// Boucle principale
    while (1)
    {	
		// Lecture des valeurs de l'accéléromètre
        MPU6050_Read_AccelXY(&accel_x, &accel_y);

		// Sauvegarder la position précédente du curseur
		game_cursor_pos_old = game_cursor_pos;

		// Déplacement du curseur en fonction de l'accélération
		// Si l'accélération est supérieure à ACCEL_THRESHOLD, on déplace le curseur vers la droite
		if (accel_x > ACCEL_THRESHOLD) {
			// Le masque permet de passer d'un coté à l'autre de la grille
			game_cursor_pos.x = (game_cursor_pos.x + 1) & 0x07;
		}
		// Si l'accélération est inférieure à -ACCEL_THRESHOLD, on déplace le curseur vers la gauche
		if (accel_x < -ACCEL_THRESHOLD) {
			// Le masque permet de passer d'un coté à l'autre de la grille
			game_cursor_pos.x = (game_cursor_pos.x - 1) & 0x07;
		}
		// Si l'accélération est supérieure à ACCEL_THRESHOLD, on déplace le curseur vers le bas
		if (accel_y > ACCEL_THRESHOLD) {
			// Le masque permet de passer d'un coté à l'autre de la grille
			game_cursor_pos.y = (game_cursor_pos.y + 1) & 0x07;
		}
		// Si l'accélération est inférieure à -ACCEL_THRESHOLD, on déplace le curseur vers le haut
		if (accel_y < -ACCEL_THRESHOLD) {
			// Le masque permet de passer d'un coté à l'autre de la grille
			game_cursor_pos.y = (game_cursor_pos.y - 1) & 0x07;
		}

		// Si le curseur a bougé, on le déplace et on affiche le nouveau curseur
		if (game_cursor_pos.x != game_cursor_pos_old.x || game_cursor_pos.y != game_cursor_pos_old.y) {
			// On efface le curseur précédent
			SH1106_SetCursor(game_cursor_pos_old.x, game_cursor_pos_old.y * 8);
			// On affiche la case vide
			for(uint8_t col = 0; col < 8; col++) {
				SH1106_WriteChar(game_grid[col]);
			}

			// Si le curseur est sur la destination, on change la destination
			if (game_cursor_pos.x == game_dest_pos.x && game_cursor_pos.y == game_dest_pos.y) {
				do {
					// On lit les valeurs de l'accéléromètre
					MPU6050_Read_AccelXY(&accel_x, &accel_y);
					// On génère une valeur pseudo-aléatoire
					random = ((uint8_t)((accel_x + accel_y) & 0xFF));
					// On déplace la destination
					game_dest_pos.x = ((random >> 2) & 0x07) ^ (random & 0x03);
					game_dest_pos.y = ((random >> 5) & 0x07) ^ (random & 0x03);
				} while (game_dest_pos.x == game_cursor_pos.x && game_dest_pos.y == game_cursor_pos.y);

				// On se place sur la nouvelle position de la destination
				SH1106_SetCursor(game_dest_pos.x, game_dest_pos.y * 8);
				// On affiche la case
				for(uint8_t col = 0; col < 8; col++) {
					SH1106_WriteChar(game_dest[col]);
				}

				// On affiche la case du dinosaure
				if(pos_index < 36) {
					// On se place sur la position du dinosaure
					SH1106_SetCursor(positions[pos_index][0]+2, 8 * (positions[pos_index][1] + 9));
					// Pour chaque ligne
					for(uint8_t row = 0; row < 8; row++) {
						// On affiche la case
						SH1106_WriteChar(image[positions[pos_index][0]][positions[pos_index][1]][row]);
					}
					// On incrémente l'index de la position
					pos_index++;
				}

				time_bar = time_bar + 1;
			}

			// On affiche le nouveau curseur
			SH1106_SetCursor(game_cursor_pos.x, game_cursor_pos.y * 8);
			// Pour chaque colonne
			for(uint8_t col = 0; col < 8; col++) {
				// On affiche la case
				SH1106_WriteChar(game_cursor[col]);
			}
		}

		// Si on a pas encore découvert toutes les positions	
		if(pos_index < POS_MAX) {
			// On décrémente la barre de temps
			time_bar--;
			// Si la barre de temps est inférieure à 0, on affiche "Game Over"
			if(time_bar < 0) {
				// On éteint l'écran OLED
				SH1106_DisplayOff();
				// On efface l'écran OLED
				SH1106_Clear();

				// Afficher "Game Over"
				// On se place sur la colonne 3, à la page 8*4=32
				SH1106_SetCursor(3, 8*4);
				// Pour chaque lettre
				for(uint8_t letter = 0; letter < 11; letter++) {
					// Pour chaque ligne
					for(uint8_t row = 0; row < 6; row++) {
						// On affiche la lettre
						SH1106_WriteChar(Game_Over_6x8[letter][row]);
					}
				}
				// On allume l'écran OLED
				SH1106_DisplayOn();
				// On reste sur cette page
				while(1);
			}
		}

		// Afficher la barre de progression
		SH1106_SetCursor(1, 8*9);
		// Pour chaque colonne
		for(uint8_t col = 0; col < time_bar; col++) {
			// On affiche la case
			SH1106_WriteChar(bar_high);
		}
		for(uint8_t col = time_bar; col < TIME_BAR_MAX; col++) {
			// On affiche la case
			SH1106_WriteChar(bar_low);
		}

		// Attendre 150ms
		__delay_ms(150);	// Le delai permet d'avoir une meilleure expérience de jeu
    }
}
/**
 End of File
*/