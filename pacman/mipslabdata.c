/* mipslabdata.c
   This file compiled 2015 by F Lundevall
   from original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

char textbuffer[4][16];
uint8_t final_matrix[128][32];
uint8_t temp[512];

const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

const uint8_t const icon[] = {
	255, 255, 255, 255, 255, 255, 127, 187,
	68, 95, 170, 93, 163, 215, 175, 95,
	175, 95, 175, 95, 223, 111, 175, 247,
	59, 237, 242, 254, 171, 254, 1, 255,
	255, 255, 15, 211, 109, 58, 253, 8,
	178, 77, 58, 199, 122, 197, 242, 173,
	242, 237, 186, 215, 40, 215, 41, 214,
	35, 175, 91, 212, 63, 234, 149, 111,
	171, 84, 253, 252, 254, 253, 126, 184,
	195, 52, 201, 22, 225, 27, 196, 19,
	165, 74, 36, 146, 72, 162, 85, 8,
	226, 25, 166, 80, 167, 216, 167, 88,
	106, 149, 161, 95, 135, 91, 175, 87,
	142, 123, 134, 127, 134, 121, 134, 121,
	132, 59, 192, 27, 164, 74, 177, 70,
	184, 69, 186, 69, 254, 80, 175, 217,
};

/* The score_board on the right of the screen.
   There are 12 columns. Each number in the array is
   a byte (8 bits). Each column is of length 32, so 4 numbers
   are used to describe each column (vertically represented).
   Update this accordingly for scores and lives. */
const uint8_t const score_background[] = {
	127, 64, 95, 80, 80, 64,
	255, 64, 83, 85, 89, 64, 0, 0, 0, 0,
	16, 134, 207, 158, 207, 134,
	255, 0, 95, 85, 221, 0, 0, 0, 0, 0,
	4, 97, 147, 39, 147, 97,
	255, 0, 213, 85, 119, 0, 0, 0, 0, 0,
	254, 24, 36, 73, 36, 24,
	255, 0, 1, 1, 253, 0, 0, 0, 0, 0,
};

const uint8_t const board[] = {
	127, 65, 65, 65, 193, 1, 1, 1,
	1, 1, 193, 1, 1, 1, 1, 1,
	193, 193, 193, 193, 193, 1, 1, 1,
	1, 1, 65, 65, 65, 65, 65, 65,
	193, 193, 193, 1, 1, 1, 1, 1,
	193, 1, 1, 1, 1, 1, 193, 193,
	193, 193, 193, 1, 1, 1, 1, 1,
	255, 0, 255, 1, 1, 1, 1, 1,
	193, 193, 193, 193, 193, 1, 1, 1,
	1, 1, 193, 65, 65, 65, 65, 1,
	1, 1, 1, 1, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65,
	1, 1, 1, 1, 1, 65, 65, 65,
	65, 193, 1, 1, 1, 1, 1, 193,
	65, 65, 65, 65, 127, 64, 95, 80,
	80, 64, 255, 64, 83, 85, 89, 64, // row 1
	16, 16, 16, 16, 31, 0, 0, 0,
	0, 0, 255, 128, 128, 128, 128, 128,
	131, 131, 131, 131, 131, 0, 0, 0,
	0, 0, 240, 128, 128, 128, 0, 0,
	3, 3, 3, 128, 128, 128, 128, 128,
	255, 128, 128, 128, 128, 128, 3, 3,
	3, 3, 3, 128, 128, 128, 128, 128,
	131, 130, 131, 128, 128, 128, 128, 128,
	3, 3, 3, 3, 3, 128, 128, 128,
	128, 128, 255, 0, 0, 0, 0, 0,
	240, 48, 48, 48, 48, 48, 48, 48,
	16, 16, 16, 16, 16, 48, 48, 48,
	48, 48, 48, 240, 0, 0, 0, 0,
	0, 255, 0, 0, 0, 0, 0, 31,
	16, 16, 16, 16, 16, 134, 207, 158,
	207, 134, 255, 0, 95, 85, 221, 0, // row 2
	4, 4, 4, 4, 252, 0, 0, 0,
	0, 0, 255, 1, 1, 1, 1, 1,
	193, 193, 193, 193, 193, 0, 0, 0,
	0, 0, 15, 1, 1, 1, 0, 0,
	192, 192, 192, 1, 1, 1, 1, 1,
	255, 1, 1, 1, 1, 1, 192, 192,
	192, 192, 192, 1, 1, 1, 1, 1,
	193, 65, 193, 1, 1, 1, 1, 1,
	192, 192, 192, 192, 192, 1, 1, 1,
	1, 1, 15, 0, 0, 0, 0, 0,
	15, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 15, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 252,
	4, 4, 4, 4, 4, 97, 147, 39,
	147, 97, 255, 0, 213, 85, 119, 0, // row 3
	254, 130, 130, 130, 131, 128, 128,
	128, 128, 128, 131, 128, 128, 128,
	128, 128, 131, 131, 131, 131, 131,
	128, 128, 128, 128, 128, 130, 130,
	130, 130, 130, 130, 131, 131, 131,
	128, 128, 128, 128, 128, 131, 128,
	128, 128, 128, 128, 131, 131, 131,
	131, 131, 128, 128, 128, 128, 128,
	255, 0, 255, 128, 128, 128, 128,
	128, 131, 131, 131, 131, 131, 128, 128,
	128, 128, 128, 130, 130, 130, 130, 130,
	130, 130, 130, 130, 130, 130, 130, 130,
	130, 130, 130, 130, 130, 130, 130, 130,
	130, 130, 130, 130, 130, 130, 130, 130,
	130, 130, 130, 128, 128, 128, 128, 128,
	131, 130, 130, 130, 130, 254, 24, 36,
	73, 36, 24, 255, 0, 1, 1, 125, 0,
};
// CHARACTERS

//PAC-MAN
int pacman_open_right[5][5] = {
	0,1,1,1,0,
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1,
	0,1,1,1,0,
};
int pacman_close[5][5] = {
	0,1,1,1,0,
	1,1,1,1,1,
	1,1,1,1,1,
	1,1,1,1,1,
	0,1,1,1,0,
};
int pacman_open_left[5][5] = {
	0,1,1,1,0,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1,
	0,1,1,1,0,
};
// BLINKY
int ghost_blinky[5][5] = {
	0,1,1,1,0,
	1,0,1,0,1,
	1,0,1,0,1,
	1,1,1,1,1,
	1,0,1,0,1,
};
// INKY
int ghost_inky[5][5] = {
	0,1,1,1,0,
	1,0,1,0,1,
	1,0,1,0,1,
	1,1,0,1,1,
	1,0,1,0,1,
};
// CLYDE
int ghost_clyde[5][5] = {
	0,1,1,1,0,
	1,0,1,0,1,
	1,1,0,1,1,
	1,1,0,1,1,
	1,0,1,0,1,
};
// PINKY
int ghost_pinky[5][5] = {
	0,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,1,1,1,1,
	1,0,1,0,1,
};
const uint8_t const all_nums[10][5][3] = {
	{
		1, 1, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
	},
	{
		1, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		1, 1, 1,
	},
	{
		1, 1, 1,
		0, 0, 1,
		1, 1, 1,
		1, 0, 0,
		1, 1, 1,
	},
	{
		1, 1, 1,
		0, 0, 1,
		1, 1, 1,
		0, 0, 1,
		1, 1, 1,
	},
	{
		1, 1, 1,
		1, 0, 0,
		1, 1, 1,
		0, 0, 1,
		1, 1, 1,
	},
	{
		1, 1, 1,
		1, 0, 0,
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
	},
	{
		1, 1, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
	},
	{
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
	},
	{
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
		0, 0, 1,
		1, 1, 1,
	},
};
