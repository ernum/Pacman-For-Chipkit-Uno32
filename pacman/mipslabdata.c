/* mipslabdata.c
   This file compiled 2015 by F Lundevall
   from original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

char textbuffer[4][16];
uint8_t final_matrix[128][32];
uint8_t board_matrix[128][32];
uint8_t temp[512];
Score score_board[4];

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
	0, 0, 240, 128, 128, 128, 128, 0,
	3, 3, 3, 0, 128, 128, 128, 128,
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
	0, 0, 15, 1, 1, 1, 1, 0,
	192, 192, 192, 0, 1, 1, 1, 1,
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

/* CHARACTERS */

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

const uint8_t const opaque_heart[5][5] = {
	0, 1, 0, 1, 0,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	0, 1, 1, 1, 0,
	0, 0, 1, 0, 0,
};

const uint8_t const transparent_heart[5][5] = {
	0, 1, 0, 1, 0,
	1, 0, 1, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
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
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 0, 1,
		0, 0, 1,
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

// Each row is a column with x, y, x, y ... coordinates
uint8_t dot_coord_original [104][2] = {
	3, 3, 3, 28,
	7, 3, 7, 7, 7, 11, 7, 15, 7, 20, 7, 24, 7, 28,
	13, 3, 13, 7, 13, 12, 13, 19, 13, 24, 13, 28,
	18, 3, 18, 12, 18, 19, 18, 28,
	23, 3, 23, 7, 23, 12, 23, 15, 23, 19, 23, 24, 23, 28,
	26, 9, 26, 22,
	29, 3, 29, 9, 29, 12, 29, 19, 29, 22, 29, 28,
	33, 3, 33, 12, 33, 15, 33, 19, 33, 28,
	37, 3, 37, 7, 37, 12, 37, 19, 37, 24, 37, 28,
	43, 3, 43, 7, 43, 12, 43, 19, 43, 24, 43, 28,
	48, 3, 48, 12, 48, 15, 48, 19, 48, 28,
	53, 3, 53, 7, 53, 12, 53, 19, 53, 24, 53, 28,
	57, 12, 57, 19,
	61, 3, 61, 7, 61, 12, 61, 19, 61, 24, 61, 28,
	66, 3, 66, 12, 66, 15, 66, 19, 66, 28,
	71, 3, 71, 7, 71, 12, 71, 19, 71, 24, 71, 28,
	75, 3, 75, 28,
	79, 3, 79, 28,
	83, 3, 83, 28,
	87, 3,
	92, 3,
	96, 3, 96, 28,
	100, 3, 100, 28,
	104, 3, 104, 28,
	108, 3, 108, 7, 108, 11, 108, 15, 108, 20, 108, 24, 108, 28,
	113, 3, 113, 28,
};

uint8_t  dot_coord_variable [104][2] = {
	3, 3, 3, 28,
	7, 3, 7, 7, 7, 11, 7, 15, 7, 20, 7, 24, 7, 28,
	13, 3, 13, 7, 13, 12, 13, 19, 13, 24, 13, 28,
	18, 3, 18, 12, 18, 19, 18, 28,
	23, 3, 23, 7, 23, 12, 23, 15, 23, 19, 23, 24, 23, 28,
	26, 9, 26, 22,
	29, 3, 29, 9, 29, 12, 29, 19, 29, 22, 29, 28,
	33, 3, 33, 12, 33, 15, 33, 19, 33, 28,
	37, 3, 37, 7, 37, 12, 37, 19, 37, 24, 37, 28,
	43, 3, 43, 7, 43, 12, 43, 19, 43, 24, 43, 28,
	48, 3, 48, 12, 48, 15, 48, 19, 48, 28,
	53, 3, 53, 7, 53, 12, 53, 19, 53, 24, 53, 28,
	57, 12, 57, 19,
	61, 3, 61, 7, 61, 12, 61, 19, 61, 24, 61, 28,
	66, 3, 66, 12, 66, 15, 66, 19, 66, 28,
	71, 3, 71, 7, 71, 12, 71, 19, 71, 24, 71, 28,
	75, 3, 75, 28,
	79, 3, 79, 28,
	83, 3, 83, 28,
	87, 3,
	92, 3,
	96, 3, 96, 28,
	100, 3, 100, 28,
	104, 3, 104, 28,
	108, 3, 108, 7, 108, 11, 108, 15, 108, 20, 108, 24, 108, 28,
	113, 3, 113, 28,
};

uint8_t all_menu_screens[3][512] = {
    {
		255, 255, 255, 7, 7, 7, 7, 7,
		7, 135, 71, 71, 39, 39, 23,
		23, 23, 23, 23, 23, 23, 39,
		39, 71, 71, 135, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 135, 199, 71,
		103, 55, 23, 23, 23, 23, 23,
		23, 23, 23, 55, 39, 103, 199,
		135, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 247, 247,
		247, 119, 247, 247, 231, 7, 7,
		7, 199, 247, 247, 247, 247, 247,
		199, 7, 7, 7, 7, 231, 247,
		247, 119, 119, 247, 231, 7, 7,
		7, 135, 135, 135, 135, 135, 135,
		135, 135, 7, 7, 7, 255, 255,
		255, 255, 255, 255, 0, 0, 224,
		24, 6, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 128,
		128, 64, 64, 32, 32, 16, 17,
		10, 12, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 240, 28, 6, 3,
		0, 240, 220, 196, 4, 156, 240,
		0, 0, 240, 220, 196, 4, 156,
		240, 0, 1, 7, 60, 224, 0,
		0, 0, 0, 0, 0, 0, 0,
		127, 127, 127, 7, 7, 7, 3,
		0, 0, 0, 127, 127, 127, 28,
		127, 127, 127, 0, 0, 0, 0,
		63, 127, 127, 112, 112, 120, 56,
		0, 0, 0, 7, 7, 7, 7,
		7, 7, 7, 7, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 0,
		0, 15, 48, 192, 0, 0, 0,
		0, 0, 0, 0, 0, 2, 5,
		5, 8, 8, 16, 32, 32, 64,
		64, 128, 128, 0, 0, 0, 0,
		14, 17, 17, 17, 14, 0, 0,
		0, 0, 14, 17, 17, 17, 14,
		0, 0, 0, 0, 0, 255, 0,
		0, 0, 0, 1, 7, 5, 4,
		7, 0, 0, 0, 1, 7, 5,
		4, 7, 0, 0, 0, 0, 0,
		255, 0, 0, 0, 0, 0, 0,
		0, 0, 254, 254, 252, 240, 252,
		254, 254, 0, 0, 0, 248, 254,
		254, 158, 254, 254, 248, 0, 0,
		0, 0, 254, 254, 124, 240, 192,
		254, 254, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 255, 255, 255, 255, 255,
		255, 192, 192, 192, 192, 192, 193,
		194, 196, 196, 200, 200, 208, 208,
		208, 208, 208, 208, 208, 200, 200,
		196, 196, 194, 193, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192,
		199, 204, 200, 216, 208, 216, 200,
		206, 199, 204, 216, 208, 208, 216,
		204, 198, 195, 204, 216, 208, 208,
		220, 198, 195, 192, 192, 192, 192,
		192, 192, 192, 192, 207, 207, 192,
		193, 192, 207, 207, 192, 192, 192,
		207, 207, 207, 195, 207, 207, 207,
		192, 192, 192, 192, 207, 207, 192,
		193, 199, 207, 207, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 255, 255, 255,
	},
	{
		0, 254, 2, 2, 2, 2, 2, 2,
		130, 130, 130, 130, 130, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 130, 130,
		130, 130, 130, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 130, 130, 130, 130,
		130, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		130, 130, 130, 130, 130, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		250, 10, 234, 234, 234, 234, 234,
		234, 234, 234, 234, 106, 106, 106,
		234, 106, 106, 106, 106, 106, 234,
		234, 106, 106, 234, 234, 106, 106,
		106, 234, 234, 106, 106, 106, 106,
		106, 234, 234, 234, 234, 234, 234,
		234, 234, 10, 250, 242, 2, 254,
		0, 0, 255, 0, 0, 0, 0,
		62, 127, 255, 255, 255, 255, 255,
		127, 62, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 62, 127,
		255, 255, 255, 255, 255, 127, 62,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 62, 127, 255, 255,
		255, 255, 255, 127, 62, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		62, 127, 255, 255, 255, 255, 255,
		127, 62, 0, 0, 0, 0, 0,
		0, 0, 255, 0, 127, 127, 127,
		127, 127, 127, 127, 127, 118, 117,
		117, 123, 127, 127, 127, 112, 127,
		127, 127, 112, 125, 125, 112, 127,
		112, 125, 121, 118, 127, 127, 127,
		112, 127, 127, 127, 127, 127, 127,
		127, 127, 127, 127, 0, 255, 255,
		0, 255, 0, 0, 255, 0, 124,
		124, 96, 0, 124, 124, 84, 0,
		124, 124, 20, 0, 12, 124, 12,
		0, 0, 0, 124, 52, 92, 0,
		124, 124, 0, 124, 68, 116, 0,
		124, 16, 124, 0, 12, 124, 12,
		0, 0, 0, 0, 0, 0, 124,
		64, 124, 0, 124, 116, 28, 0,
		0, 0, 0, 0, 0, 0, 124,
		108, 56, 0, 124, 68, 124, 0,
		124, 32, 124, 0, 124, 56, 124,
		0, 0, 0, 0, 255, 1, 253,
		5, 5, 5, 5, 245, 69, 69,
		245, 5, 69, 69, 69, 5, 37,
		85, 85, 149, 5, 229, 21, 21,
		165, 5, 229, 21, 21, 229, 5,
		245, 85, 213, 37, 5, 245, 85,
		85, 21, 5, 5, 5, 253, 1,
		255, 255, 0, 255, 0, 0, 127,
		64, 95, 95, 78, 68, 64, 64,
		87, 85, 89, 64, 95, 95, 85,
		64, 95, 95, 80, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 95,
		112, 119, 116, 116, 116, 116, 117,
		116, 116, 117, 116, 116, 116, 116,
		116, 117, 117, 117, 116, 116, 116,
		117, 117, 116, 116, 116, 117, 117,
		116, 116, 117, 116, 116, 117, 116,
		117, 117, 117, 117, 116, 116, 116,
		119, 112, 127, 127, 64, 127, 0,
	},
	{
		0, 254, 2, 2, 2, 2, 2, 2,
		130, 130, 130, 130, 130, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 130, 130,
		130, 130, 130, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 130, 130, 130, 130,
		130, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		130, 130, 130, 130, 130, 2, 2,
		2, 2, 2, 2, 2, 2, 2,
		250, 10, 234, 42, 42, 42, 42,
		42, 42, 42, 42, 170, 170, 170,
		42, 170, 170, 170, 170, 170, 42,
		42, 170, 170, 42, 42, 170, 170,
		170, 42, 42, 170, 170, 170, 170,
		170, 42, 42, 42, 42, 42, 42,
		42, 234, 10, 250, 242, 2, 254,
		0, 0, 255, 0, 0, 0, 0,
		62, 127, 255, 255, 255, 255, 255,
		127, 62, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 62, 127,
		255, 255, 255, 255, 255, 127, 62,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 62, 127, 255, 255,
		255, 255, 255, 127, 62, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		62, 127, 255, 255, 255, 255, 255,
		127, 62, 0, 0, 0, 0, 0,
		0, 0, 255, 0, 127, 64, 64,
		64, 64, 64, 64, 64, 73, 74,
		74, 68, 64, 64, 64, 79, 64,
		64, 64, 79, 66, 66, 79, 64,
		79, 66, 70, 73, 64, 64, 64,
		79, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 127, 0, 255, 255,
		0, 255, 0, 0, 255, 0, 124,
		124, 96, 0, 124, 124, 84, 0,
		124, 124, 20, 0, 12, 124, 12,
		0, 0, 0, 124, 52, 92, 0,
		124, 124, 0, 124, 68, 116, 0,
		124, 16, 124, 0, 12, 124, 12,
		0, 0, 0, 0, 0, 0, 124,
		64, 124, 0, 124, 116, 28, 0,
		0, 0, 0, 0, 0, 0, 124,
		108, 56, 0, 124, 68, 124, 0,
		124, 32, 124, 0, 124, 56, 124,
		0, 0, 0, 0, 255, 1, 253,
		253, 253, 253, 253, 13, 189, 189,
		13, 253, 189, 189, 189, 253, 221,
		173, 173, 109, 253, 29, 237, 237,
		93, 253, 29, 237, 237, 29, 253,
		13, 173, 45, 221, 253, 13, 173,
		173, 237, 253, 253, 253, 253, 1,
		255, 255, 0, 255, 0, 0, 127,
		64, 95, 95, 78, 68, 64, 64,
		87, 85, 89, 64, 95, 95, 85,
		64, 95, 95, 80, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 95,
		112, 119, 119, 119, 119, 119, 118,
		119, 119, 118, 119, 119, 119, 119,
		119, 118, 118, 118, 119, 119, 119,
		118, 118, 119, 119, 119, 118, 118,
		119, 119, 118, 119, 119, 118, 119,
		118, 118, 118, 118, 119, 119, 119,
		119, 112, 127, 127, 64, 127, 0,
	},
};

const uint8_t const no_high_scores_recorded[512] = {
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 31, 223, 31, 255, 255,
	31, 223, 31, 255, 255, 255, 255,
	255, 255, 255, 255, 31, 127, 31,
	255, 31, 255, 63, 223, 223, 255,
	31, 127, 31, 255, 255, 255, 255,
	255, 255, 255, 255, 191, 95, 223,
	255, 63, 223, 223, 255, 31, 223,
	31, 255, 31, 95, 191, 255, 31,
	95, 95, 255, 191, 95, 223, 255,
	255, 255, 255, 255, 255, 255, 255,
	31, 95, 191, 255, 31, 95, 95,
	255, 63, 223, 223, 255, 31, 223,
	31, 255, 31, 95, 191, 255, 31,
	223, 63, 255, 31, 95, 95, 255,
	31, 223, 63, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 252, 255, 252,
	255, 255, 252, 253, 252, 255, 255,
	255, 255, 255, 255, 255, 255, 252,
	255, 252, 255, 252, 255, 254, 253,
	252, 255, 252, 255, 252, 255, 255,
	255, 255, 255, 255, 255, 255, 253,
	253, 254, 255, 254, 253, 253, 255,
	252, 253, 252, 255, 252, 255, 252,
	255, 252, 253, 253, 255, 253, 253,
	254, 255, 255, 255, 255, 255, 255,
	255, 255, 252, 255, 252, 255, 252,
	253, 253, 255, 254, 253, 253, 255,
	252, 253, 252, 255, 252, 255, 252,
	255, 252, 253, 254, 255, 252, 253,
	253, 255, 252, 253, 254, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
};

const uint8_t const game_over_screen[512] = {
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 7,
	251, 251, 251, 123, 255, 255, 3,
	219, 219, 219, 3, 255, 255, 3,
	247, 247, 247, 3, 255, 255, 3,
	187, 187, 187, 187, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 3, 251, 251, 251, 3, 255,
	255, 3, 255, 255, 255, 3, 255,
	255, 3, 187, 187, 187, 187, 255,
	255, 3, 219, 219, 219, 39, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 248, 247, 247, 247, 240, 255,
	255, 240, 255, 255, 255, 240, 255,
	255, 240, 255, 255, 255, 240, 255,
	255, 240, 247, 247, 247, 247, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 240, 247, 247, 247,
	240, 255, 255, 240, 247, 247, 247,
	248, 255, 255, 240, 247, 247, 247,
	247, 255, 255, 240, 255, 255, 255,
	240, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,
};

uint8_t high_score_input[512] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 136, 248, 136,
	0, 248, 8, 248, 0, 248, 40,
	56, 0, 248, 128, 248, 0, 8,
	248, 8, 0, 0, 0, 0, 112,
	136, 136, 0, 248, 40, 208, 0,
	248, 168, 168, 0, 248, 136, 112,
	0, 248, 168, 168, 0, 248, 8,
	248, 0, 8, 248, 8, 0, 136,
	248, 136, 0, 248, 40, 248, 0,
	248, 248, 128, 0, 144, 168, 72,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 224, 240,
	248, 248, 248, 248, 248, 240, 224,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 224, 240, 248,
	248, 248, 248, 248, 240, 224, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 224, 240, 248, 248,
	248, 248, 248, 240, 224, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 224, 240, 248, 248, 248,
	248, 248, 240, 224, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	3, 7, 15, 15, 15, 15, 15,
	7, 3, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3,
	7, 15, 15, 15, 15, 15, 7,
	3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 3, 7,
	15, 15, 15, 15, 15, 7, 3,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 3, 7, 15,
	15, 15, 15, 15, 7, 3, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 31, 31, 14, 4,
	0, 0, 23, 21, 25, 0, 31,
	31, 21, 0, 31, 31, 16, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 16, 16, 16, 16, 16,
	16, 16, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 16, 16, 16, 16, 16, 16,
	16, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	16, 16, 16, 16, 16, 16, 16,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
};

const uint8_t const alphabet[26][5][3] = {
	{	// A
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
		1, 0, 1,
		1, 0, 1,
	},
	{ // B
		1, 1, 0,
		1, 0, 1,
		1, 1, 0,
		1, 0, 1,
		1, 1, 0
	},
	{ // C
		0, 1, 1,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		0, 1, 1,
	},
	{ // D
		1, 1, 0,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 1, 0,
	},
	{ // E
		1, 1, 1,
		1, 0, 0,
		1, 1, 1,
		1, 0, 0,
		1, 1, 1,
	},
	{ // F
		1, 1, 1,
		1, 0, 0,
		1, 1, 1,
		1, 0, 0,
		1, 0, 0,
	},
	{ // G
		0, 1, 1,
		1, 0, 0,
		1, 0, 1,
		1, 0, 1,
		0, 1, 1,
	},
	{ // H
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
		1, 0, 1,
		1, 0, 1,
	},
	{ // I
		1, 1, 1,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		1, 1, 1,
	},
	{ // J
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
	},
	{ // K
		1, 0, 1,
		1, 0, 1,
		1, 1, 0,
		1, 0, 1,
		1, 0, 1,
	},
	{ // L
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 1, 1,
	},
	{ // M
		1, 0, 1,
		1, 1, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
	},
	{ // N
		1, 1, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
	},
	{ // O
		1, 1, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
	},
	{ // P
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
		1, 0, 0,
		1, 0, 0,
	},
	{ // L
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 1,
	},
	{ // R
		1, 1, 0,
		1, 0, 1,
		1, 1, 0,
		1, 0, 1,
		1, 0, 1,
	},
	{ // S
		0, 1, 1,
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		1, 1, 0,
	},
	{ // T
		1, 1, 1,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
	},
	{ // U
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
	},
	{ // V
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 1, 0,
	},
	{ // W
		1, 0, 1,
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
		1, 0, 1,
	},
	{ // X
		1, 0, 1,
		1, 0, 1,
		0, 1, 0,
		1, 0, 1,
		1, 0, 1,
	},
	{ // Y
		1, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 0,
		0, 1, 0,
	}, 
	{ // Z
		1, 1, 1,
		0, 0, 1,
		0, 1, 0,
		1, 0, 0,
		1, 1, 1,
	},
};

uint8_t high_score_page[512] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 248, 32, 248, 0, 136,
	248, 136, 0, 112, 136, 200, 0,
	248, 32, 248, 0, 0, 0, 144,
	168, 72, 0, 112, 136, 136, 0,
	248, 136, 248, 0, 248, 40, 208,
	0, 248, 168, 168, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 224, 240, 248, 248,
	248, 248, 248, 240, 224, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 3, 7,
	15, 15, 15, 15, 15, 7, 3,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 31, 31, 14, 4, 0, 31,
	21, 10, 0, 14, 17, 17, 0,
	31, 4, 27, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
};
uint8_t intersection_coord[38][2] = {
	5, 1, 5, 26, // no teleport
	11, 1, 11, 26,
	21, 1, 21, 7, 21, 10, 21, 17, 21, 20, 21, 26,
	31, 11, 31, 17,
	35, 1, 35, 26,
	41, 1, 41, 26,
	46, 10, 46, 17,
	51, 10, 51, 17,
	59, 10, 59, 17,
	64, 10, 64, 10,
	69, 1, 69, 20, 69, 26,
	75, 20,
	79, 1, 79, 7,
	96, 1, 96, 7,
	100, 20,
	106, 1, 106, 20, 106 ,26, //no teleport
};
