/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <string.h>

/* Declare a helper function which is local to this file */
static void num32asc(char *s, int);

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


Pacman pacman;

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc)
{
  int i;
  for (i = cyc; i > 0; i--)
    ;
}

/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
void tick(unsigned int *timep)
{
  /* Get current value, store locally */
  register unsigned int t = *timep;
  t += 1; /* Increment local copy */

  /* If result was not a valid BCD-coded time, adjust now */

  if ((t & 0x0000000f) >= 0x0000000a)
    t += 0x00000006;
  if ((t & 0x000000f0) >= 0x00000060)
    t += 0x000000a0;
  /* Seconds are now OK */

  if ((t & 0x00000f00) >= 0x00000a00)
    t += 0x00000600;
  if ((t & 0x0000f000) >= 0x00006000)
    t += 0x0000a000;
  /* Minutes are now OK */

  if ((t & 0x000f0000) >= 0x000a0000)
    t += 0x00060000;
  if ((t & 0x00ff0000) >= 0x00240000)
    t += 0x00dc0000;
  /* Hours are now OK */

  if ((t & 0x0f000000) >= 0x0a000000)
    t += 0x06000000;
  if ((t & 0xf0000000) >= 0xa0000000)
    t = 0;
  /* Days are now OK */

  *timep = t; /* Store new value */
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug(volatile int *const addr)
{
  display_string(1, "Addr");
  display_string(2, "Data");
  num32asc(&textbuffer[1][6], (int)addr);
  num32asc(&textbuffer[2][6], *addr);
  display_update();
}

void display_string(int line, char *s)
{
  int i;
  if (line < 0 || line >= 4)
    return;
  if (!s)
    return;

  for (i = 0; i < 16; i++)
    if (*s)
    {
      textbuffer[line][i] = *s;
      s++;
    }
    else
      textbuffer[line][i] = ' ';
}

uint8_t spi_send_recv(uint8_t data)
{
  while (!(SPI2STAT & 0x08));
  SPI2BUF = data;
  while (!(SPI2STAT & 1));
  return SPI2BUF;
}

void display_init(void)
{
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  quicksleep(10);
  DISPLAY_ACTIVATE_VDD;
  quicksleep(1000000);

  spi_send_recv(0xAE);
  DISPLAY_ACTIVATE_RESET;
  quicksleep(10);
  DISPLAY_DO_NOT_RESET;
  quicksleep(10);

  spi_send_recv(0x8D);
  spi_send_recv(0x14);

  spi_send_recv(0xD9);
  spi_send_recv(0xF1);

  DISPLAY_ACTIVATE_VBAT;
  quicksleep(10000000);

  spi_send_recv(0xA1);
  spi_send_recv(0xC8);

  spi_send_recv(0xDA);
  spi_send_recv(0x20);

  spi_send_recv(0xAF);
}

/* Function for matrix conversion */
void int_to_bin_digit(unsigned int in, int count, uint8_t *out)
{
    /* assert: count <= sizeof(int)*CHAR_BIT */
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}

int bin_to_int_decimal(uint8_t *in, int bits)
{
    int i, n;
    uint8_t sum = 0;
    for (i = 0; i < bits; i++) {
        n = *(in + i);
        sum += (n * (1 << (bits - (i + 1))));
    }
    return sum;
}

void revereseArray(uint8_t arr[], int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

int get_matrix_value(uint8_t matrix[128][32], int column, int start) {
    uint8_t num;
    uint8_t temp[8];
    int i;
    for (i = 0; i < 8; i++) {
        temp[7 - i] = matrix[column][start + i];
    }
    num = bin_to_int_decimal(temp, 8);
    return num;

}

void convert_array_to_matrix(uint8_t array[512], uint8_t matrix[128][32]) {
    int row, column;
    for (column=0; column<128; column++)
    {
        int board_pointer = column;
        uint8_t byte[8];
        int current_row = 0;
        int_to_bin_digit(array[board_pointer], 8, byte);
        revereseArray(byte, 0, 7);

        for(row=0; row<32; row++)
        {
            matrix[column][row] = byte[current_row];
            current_row++;

            if ((row % 8) == 7) {
                /* printf("%d ", array[board_pointer]); */
                board_pointer += 128;
                current_row = 0;
                int_to_bin_digit(array[board_pointer], 8, byte);
                revereseArray(byte, 0, 7);
            }
        }
    }
}

void convert_matrix_to_array(uint8_t matrix[128][32], uint8_t array[512]) {
    int position = -8;
    int i;
    for (i = 0; i<512; i++)
    {
        if (i % 128 == 0) {
            position += 8;
        }
        array[i] = get_matrix_value(matrix, i % 128, position);
    }
}


void display_score(int x, const uint8_t *data)
{
  int i, j;

  for (i = 0; i < 4; i++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    spi_send_recv(0x22);                        // Set page command
    spi_send_recv(i);                           // Page number

    spi_send_recv(x & 0xF);                     // Set low part of column.
    spi_send_recv(0x10 | ((x >> 4) & 0xF));     // Set high part of column.

    DISPLAY_CHANGE_TO_DATA_MODE;

    // 12 for the amount of columns used to represent the score and lives.
    for (j = 0; j < 12; j++)
      spi_send_recv(~data[i * 12 + j]);
  }
}

void display_board(int x, const uint8_t *data)
{
    int i, j;

  	for(i = 0; i < 4; i++) {
  		DISPLAY_CHANGE_TO_COMMAND_MODE;
  		spi_send_recv(0x22);
  		spi_send_recv(i);

  		spi_send_recv(x & 0xF);
  		spi_send_recv(0x10 | ((x >> 4) & 0xF));

  		DISPLAY_CHANGE_TO_DATA_MODE;

  		for(j = 0; j < 128; j++)
  			spi_send_recv(~data[i*128 + j]);
  	}
}

void clear_screen()
{
  int i, j;
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 16; j++)
    {
      textbuffer[i][j] = 0;
    }
  }

}


void move(uint8_t matrix[128][32]) {
    int i,j;
    for(i = 0; i < 5; i++) {
      for(j = 0; j < 5; j++) {
          if (pacman.x_pos < 0) {
              pacman.x_pos = 111;
          }
          else if (pacman.x_pos > 111) {
              pacman.x_pos = 0;
          }
          final_matrix[pacman.x_pos + i][pacman.y_pos + j] = pacman_open[j][i];

      }
    }
}


void pacman_add(int dir, uint8_t matrix[128][32]){
    if (dir == 2 && final_matrix[pacman.x_pos][pacman.y_pos + 5] != 1 && pacman.y_pos < 31
            && final_matrix[pacman.x_pos + 1][pacman.y_pos + 5] != 1
            && final_matrix[pacman.x_pos + 2][pacman.y_pos + 5] != 1
            && final_matrix[pacman.x_pos + 3][pacman.y_pos + 5] != 1
            && final_matrix[pacman.x_pos + 4][pacman.y_pos + 5] != 1) {
        move(final_matrix);
        pacman.y_pos ++;
    }
    else if (dir == 8 && final_matrix[pacman.x_pos][pacman.y_pos - 1] != 1 && pacman.y_pos > 0
            && final_matrix[pacman.x_pos + 1][pacman.y_pos - 1] != 1
            && final_matrix[pacman.x_pos + 2][pacman.y_pos - 1] != 1
            && final_matrix[pacman.x_pos + 3][pacman.y_pos - 1] != 1
            && final_matrix[pacman.x_pos + 4][pacman.y_pos - 1] != 1) {
        move(final_matrix);
        pacman.y_pos --;
    }
    else if (dir == 6 && final_matrix[pacman.x_pos + 5][pacman.y_pos] != 1
            && final_matrix[pacman.x_pos + 5][pacman.y_pos + 1] != 1
            && final_matrix[pacman.x_pos + 5][pacman.y_pos + 2] != 1
            && final_matrix[pacman.x_pos + 5][pacman.y_pos + 3] != 1
            && final_matrix[pacman.x_pos + 5][pacman.y_pos + 4] != 1) {
        move(final_matrix);
        pacman.x_pos ++;
    }
    else if (dir == 4 && final_matrix[pacman.x_pos - 1][pacman.y_pos] != 1
            && final_matrix[pacman.x_pos - 1][pacman.y_pos + 1] != 1
            && final_matrix[pacman.x_pos - 1][pacman.y_pos + 2] != 1
            && final_matrix[pacman.x_pos - 1][pacman.y_pos + 3] != 1
            && final_matrix[pacman.x_pos - 1][pacman.y_pos + 4] != 1) {
        move(final_matrix);
        pacman.x_pos --;
    }
    else if (dir == 0) {
        pacman.x_pos = 5;
        pacman.y_pos = 13;
        move(final_matrix);
    }
    else {
        move(final_matrix);
    }
}

void show_score_and_lives(int dir)
{
    convert_array_to_matrix((uint8_t*)board, final_matrix);
    pacman_add(dir, final_matrix);
    convert_matrix_to_array(final_matrix, temp);
    display_board(0, temp);
}

void update_score(int score[4]) {
  convert_array_to_matrix((uint8_t*)temp, final_matrix);

  int ypos[4] = {8, 14, 20, 26};
  int i,j, digit;

  for (digit = 0; digit < 4; digit++) {
    for(i = 0; i < 3; i++) {
      for(j = 0; j < 5; j++) {
          final_matrix[124 + i][ypos[digit] + j] = all_nums[score[digit]][j][i]; // matrix is flipped on its axis
      }
    }
  }

  convert_matrix_to_array(final_matrix, temp);
  display_board(0, temp);
}

/*  Increment integer in arrays
    to then display on chipkit.
*/
void increment_score(int score[4]) {
  score[0]++;
  if (score[0] > 9) {
      score[0] = 0;
      score[1]++;

      if (score[1] > 9) {
            score[1] = 0;
            score[2]++;

            if (score[2] > 9) {
                score[2] = 0;
                score[3]++;

                if (score[3] > 9) {
                    score[3] = 0;
                }
            }
      }
  }
  update_score(score);
}

void reset_score()
{
  convert_array_to_matrix((uint8_t*)temp, final_matrix);

  int ypos[4] = {8, 14, 20, 26};
  int i,j, digit;

  for (digit = 0; digit < 4; digit++) {
    for(i = 0; i < 3; i++) {
      for(j = 0; j < 5; j++) {
          final_matrix[124 + i][ypos[digit] + j] = all_nums[0][j][i]; // matrix is flipped on its axis
      }
    }
  }

  convert_matrix_to_array(final_matrix, temp);
  display_board(0, temp);
}


void display_update(void)
{
  int i, j, k;
  int c;
  for (i = 0; i < 4; i++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(0x0);
    spi_send_recv(0x10);

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < 16; j++)
    {
      c = textbuffer[i][j];
      if (c & 0x80)
        continue;

      for (k = 0; k < 8; k++)
        spi_send_recv(font[c * 8 + k]);
    }
  }
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc(char *s, int n)
{
  int i;
  for (i = 28; i >= 0; i -= 4)
    *s++ = "0123456789ABCDEF"[(n >> i) & 15];
}

/*
 * itoa
 *
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 *
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 *
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 *
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 *
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 *
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 *
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 *
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ (24)
char *itoaconv(int num)
{
  register int i, sign;
  static char itoa_buffer[ITOA_BUFSIZ];
  static const char maxneg[] = "-2147483648";

  itoa_buffer[ITOA_BUFSIZ - 1] = 0; /* Insert the end-of-string marker. */
  sign = num;                       /* Save sign. */
  if (num < 0 && num - 1 > 0)       /* Check for most negative integer */
  {
    for (i = 0; i < sizeof(maxneg); i += 1)
      itoa_buffer[i + 1] = maxneg[i];
    i = 0;
  }
  else
  {
    if (num < 0)
      num = -num;        /* Make number positive. */
    i = ITOA_BUFSIZ - 2; /* Location for first ASCII digit. */
    do
    {
      itoa_buffer[i] = num % 10 + '0'; /* Insert next digit. */
      num = num / 10;                  /* Remove digit from number. */
      i -= 1;                          /* Move index to next empty position. */
    } while (num > 0);
    if (sign < 0)
    {
      itoa_buffer[i] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return (&itoa_buffer[i + 1]);
}
