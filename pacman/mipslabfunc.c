/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

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


Character pacman;
Character blinky;
Character inky;
Character clyde;
Character pinky;
Score score_board[4] = {0};

int score[4] = {0};
int score_board_val[4] = {0};

int hearts;
int amount_of_plays = -1;

void init(); // forward declaration
void decrement_hearts(); // forward declaration
void character_start_pos(); // forward declaration

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

int abs(int x, int y) {
    if (x < y) {
        return y - x;
    }
    else {
        return x - y;
    }
}

void update_score(int score[4]) {
  convert_array_to_matrix((uint8_t*)temp, final_matrix);

  int ypos[4] = {8, 14, 20, 26};
  int i,j, digit;

  for (digit = 0; digit < 4; digit++) {
    for(i = 0; i < 3; i++) {
      for(j = 0; j < 5; j++) {
          final_matrix[124 + i][ypos[3 - digit] + j] = all_nums[score[digit]][j][i]; // matrix is flipped on its axis
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

  for (i = 0; i < 4; i++) {
    score[i] = 0;
  }

  convert_matrix_to_array(final_matrix, temp);
  display_board(0, temp);
}

int check_dir(Character character, int dir) {
    if (dir == 2 && final_matrix[character.x_pos][character.y_pos + 5] != 1 && character.y_pos < 31
            && final_matrix[character.x_pos + 1][character.y_pos + 5] != 1
            && final_matrix[character.x_pos + 2][character.y_pos + 5] != 1
            && final_matrix[character.x_pos + 3][character.y_pos + 5] != 1
            && final_matrix[character.x_pos + 4][character.y_pos + 5] != 1) {
        return 2;
    }
    else if (dir == 8 && final_matrix[character.x_pos][character.y_pos - 1] != 1 && character.y_pos > 0
            && final_matrix[character.x_pos + 1][character.y_pos - 1] != 1
            && final_matrix[character.x_pos + 2][character.y_pos - 1] != 1
            && final_matrix[character.x_pos + 3][character.y_pos - 1] != 1
            && final_matrix[character.x_pos + 4][character.y_pos - 1] != 1) {
        return 8;
    }
    else if (dir == 6 && final_matrix[character.x_pos + 5][character.y_pos] != 1
            && final_matrix[character.x_pos + 5][character.y_pos + 1] != 1
            && final_matrix[character.x_pos + 5][character.y_pos + 2] != 1
            && final_matrix[character.x_pos + 5][character.y_pos + 3] != 1
            && final_matrix[character.x_pos + 5][character.y_pos + 4] != 1) {
        return 6;
    }
    else if (dir == 4 && final_matrix[character.x_pos - 1][character.y_pos] != 1
            && final_matrix[character.x_pos - 1][character.y_pos + 1] != 1
            && final_matrix[character.x_pos - 1][character.y_pos + 2] != 1
            && final_matrix[character.x_pos - 1][character.y_pos + 3] != 1
            && final_matrix[character.x_pos - 1][character.y_pos + 4] != 1) {
        return 4;
    }
    else {
        return 0;
    }
}
/*
Checks if ghost is in an intersection and calculates it's direction.
New direction is based on next step closest to pacman.
*/


void intersection_direction(Character *character) {
    // Direction priority is Up, Left, Down, Right.
    // A ghost can't go back the same direction it came from.
    int i;
    for (i = 0; i < 38; i++) {
        if ((*character).x_pos == intersection_coord[i][0] &&
            (*character).y_pos == intersection_coord[i][1]) {
                // Up and down are both valid?
                if (check_dir(*character, 8) == 8 && check_dir(*character, 2) == 2 &&
                    (*character).dir != 2 && (*character).dir != 8) {
                    if (abs(pacman.y_pos, (*character).y_pos - 1) <=
                        abs(pacman.y_pos, (*character).y_pos + 1)) {
                        (*character).dir = 8;
                    }
                    else {
                        (*character).dir = 2;
                    }
                }
                // Up and left are both valid?
                else if (check_dir(*character, 8) == 8 && check_dir(*character, 4) == 4 &&
                    (*character).dir != 2 && (*character).dir != 6) {
                    if (abs(pacman.y_pos, (*character).y_pos - 1) <=
                        abs(pacman.y_pos, (*character).y_pos)) {
                        (*character).dir = 8;
                    }
                    else {
                        (*character).dir = 4;
                    }
                }
                // Up and right are both valid?
                else if (check_dir(*character, 8) == 8 && check_dir(*character, 6) == 6 &&
                    (*character).dir != 2 && (*character).dir != 4) {
                    if (abs(pacman.y_pos, (*character).y_pos - 1) <=
                        abs(pacman.y_pos, (*character).y_pos)) {
                        (*character).dir = 8;
                    }
                    else {
                        (*character).dir = 6;
                    }
                }
                // Left and down are both valid?
                else if (check_dir(*character, 4) == 4 && check_dir(*character, 2) == 2 &&
                    (*character).dir != 6 && (*character).dir != 8) {
                    if (abs(pacman.x_pos, (*character).x_pos - 1) <=
                        abs(pacman.x_pos, (*character).x_pos)) {
                        (*character).dir = 4;
                    }
                    else {
                        (*character).dir = 2;
                    }
                }
                // Left and Right are both valid?
                else if (check_dir(*character, 4) == 4 && check_dir(*character, 6) == 6 &&
                    (*character).dir != 6 && (*character).dir != 4) {
                    if (abs(pacman.x_pos, (*character).x_pos - 1) <=
                        abs(pacman.x_pos, (*character).x_pos + 1)) {
                        (*character).dir = 4;
                    }
                    else {
                        (*character).dir = 6;
                    }
                }
                // Down and Right are both valid?
                else if (check_dir(*character, 2) == 2 && check_dir(*character, 6) == 6 &&
                    (*character).dir != 8 && (*character).dir != 4) {
                    if (abs(pacman.y_pos, (*character).y_pos + 1) <=
                        abs(pacman.y_pos, (*character).y_pos)) {
                        (*character).dir = 2;
                    }
                    else {
                        (*character).dir = 6;
                    }
                }
            }
    }
}
/*
Moves the ghosts. (As of now only implemented for Blinky)
*/

void ghosts_move(Character *character) {
    if ((*character).dir != 0) {
        // check if in intersection
        intersection_direction(character);
        if (check_dir(*character, (*character).dir) == 2) {
            (*character).y_pos ++;
        }
        else if (check_dir(*character, (*character).dir) == 8) {
            (*character).y_pos --;
        }
        else if (check_dir(*character, (*character).dir) == 6) {
            (*character).x_pos ++;
        }
        else if (check_dir(*character, (*character).dir) == 4) {
            (*character).x_pos --;
        }
        //try new directions when stuck on wall.
        else if (check_dir(*character, 4) == 4 && (*character).dir != 6) {
            (*character).dir = 4;
        }
        else if (check_dir(*character, 2) == 2 && (*character).dir != 8) {
            (*character).dir = 2;
        }
        else if (check_dir(*character, 8) == 8 && (*character).dir != 2) {
            (*character).dir = 8;
        }
        else if (check_dir(*character, 6) == 6 && (*character).dir != 4) {
            (*character).dir = 6;
        }
        else if ((*character).x_pos >= 111) {
            (*character).dir = 4;
        }
        else if ((*character).x_pos <= 1) {
            (*character).dir = 6;
        }
    }
}

int move_counter;
int move_speed = 12;
int dots_eaten = 0;
int ghost_release_counter = 0;
void character_add(uint8_t matrix[128][32], int dir) {
    // Ghost collision detection
    if (abs(pacman.x_pos, blinky.x_pos) <= 2 && abs(pacman.y_pos, blinky.y_pos) <= 2 ||
        abs(pacman.x_pos, inky.x_pos) <= 2 && abs(pacman.y_pos, inky.y_pos) <= 2 ||
        abs(pacman.x_pos, clyde.x_pos) <= 2 && abs(pacman.y_pos, clyde.y_pos) <= 2 ||
        abs(pacman.x_pos, pinky.x_pos) <= 2 && abs(pacman.y_pos, pinky.y_pos) <= 2) {
        move_counter = 0;
        character_start_pos(final_matrix);
        hearts -= 1;
        test_dir = 0;
        backup_dir = 0;
        dir = -1;
    }
    // Add characters to board matrix
    int i,j;

    for (i = 0; i < 5; i++) {
      if ((IFS(0) & 0xF1FF) >> 8) {
        if (move_counter == move_speed) {
            ghosts_move(&blinky);
            if (ghost_release_counter == 20) {
                pinky.x_pos = 88;
                pinky.y_pos = 7;
                pinky.dir = 4;
            }
            ghosts_move(&pinky);
            if (ghost_release_counter == 30) {
                inky.x_pos = 88;
                inky.y_pos = 7;
                inky.dir = 4;
            }
            ghosts_move(&inky);
            if (ghost_release_counter == 40) {
                clyde.x_pos = 88;
                clyde.y_pos = 7;
                clyde.dir = 4;
            }
            ghosts_move(&clyde);
            move_counter = 0;
            ghost_release_counter ++;
        }
      }
      move_counter ++;

    }

    for(i = 0; i < 5; i++) {
      for(j = 0; j < 5; j++) {
          if (pacman.x_pos < 0) {
              pacman.x_pos = 111;
          }
          else if (pacman.x_pos > 111) {
              pacman.x_pos = 0;
          }
          final_matrix[pacman.x_pos + i][pacman.y_pos + j] = pacman_open_right[j][i];
          final_matrix[blinky.x_pos + i][blinky.y_pos + j] = ghost_blinky[j][i];
          final_matrix[inky.x_pos + i][inky.y_pos + j] = ghost_inky[j][i];
          final_matrix[clyde.x_pos + i][clyde.y_pos + j] = ghost_clyde[j][i];
          final_matrix[pinky.x_pos + i][pinky.y_pos + j] = ghost_pinky[j][i];

      }
    }

    // eaten dot detection
    int k;
    for(k = 0; k < 104; k++) {
        if (abs(pacman.x_pos + 2, dot_coord_variable[k][0]) <= 2 &&
            abs(pacman.y_pos + 2, dot_coord_variable[k][1]) <= 2) {
            dot_coord_variable[k][0] = 0;
            dot_coord_variable[k][1] = 0;
            increment_score(score);
            dots_eaten ++;
        }
    }
}

// Move calculation fo backup direction
int backup_dir;
void pacman_move_backup(int dir, uint8_t matrix[128][32]){
    if (check_dir(pacman, dir) == 2) {
        pacman.y_pos ++;
        character_add(final_matrix, dir);
    }
    else if (check_dir(pacman, dir) == 8) {
        pacman.y_pos --;
        character_add(final_matrix, dir);
    }
    else if (check_dir(pacman, dir) == 6) {
        pacman.x_pos ++;
        character_add(final_matrix, dir);
    }
    else if (check_dir(pacman, dir) == 4) {
        pacman.x_pos --;
        character_add(final_matrix, dir);
    }
    else {
        character_add(final_matrix, dir);
    }
}
/*
Checks direction of pac-man and if he collides with any walls.
*/
void pacman_move(int dir, uint8_t matrix[128][32]){
    if (check_dir(pacman, dir) == 2) {
        backup_dir = dir;
        pacman.y_pos ++;
        character_add(final_matrix, dir);
    }
    else if (check_dir(pacman, dir) == 8) {
        backup_dir = dir;
        pacman.y_pos --;
        character_add(final_matrix, dir);
    }
    else if (check_dir(pacman, dir) == 6) {
        backup_dir = dir;
        pacman.x_pos ++;
        character_add(final_matrix, dir);
    }
    else if (check_dir(pacman, dir) == 4) {
        backup_dir = dir;
        pacman.x_pos --;
        character_add(final_matrix, dir);
    }
    else {
        pacman_move_backup(backup_dir, final_matrix);
    }
}

void paint_out(int dir, uint8_t matrix[128][32]) {
    int i,j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
          final_matrix[blinky.x_pos + i][blinky.y_pos + j] = 0;
          final_matrix[inky.x_pos + i][inky.y_pos + j] = 0;
          final_matrix[clyde.x_pos + i][clyde.y_pos + j] = 0;
          final_matrix[pinky.x_pos + i][pinky.y_pos + j] = 0;
        }
    }
    if (dir != -1 && dir != 0) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
              final_matrix[pacman.x_pos + i][pacman.y_pos + j] = 0;
            }
        }
    }
    int k;
    for(k = 0; k < 105; k++) {
      final_matrix[dot_coord_variable[k][0]][dot_coord_variable[k][1]] = 0;
    }
}

void add_dots(uint8_t final_matrix[128][32], uint8_t dot_matrix[105][2]) {
    int i;
    for(i = 0; i < 105; i++) {
      final_matrix[dot_matrix[i][0]][dot_matrix[i][1]] = 1;
    }
}

void second_start_menu() {
  int i = 1;

  delay(1);

  int x_start = 46, y_start = 10, scores_vals;
  uint8_t temp_highscore_matrix[128][32];
  convert_array_to_matrix(high_score_page, temp_highscore_matrix);

  int i_loop, j_loop;
  int y_coords[4] = {0, 6, 12, 18};

  for (scores_vals = 0; scores_vals < amount_of_plays + 1; scores_vals++) {
    for (i_loop = 0; i_loop < 3; i_loop++) {
      for (j_loop = 0; j_loop < 5; j_loop++) {
        temp_highscore_matrix[x_start + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].name[0][j_loop][i_loop];
        temp_highscore_matrix[x_start + 4 + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].name[1][j_loop][i_loop];
        temp_highscore_matrix[x_start + 8 + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].name[2][j_loop][i_loop];

        temp_highscore_matrix[x_start + 16 + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].score_val[0][j_loop][i_loop];
        temp_highscore_matrix[x_start + 20 + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].score_val[1][j_loop][i_loop];
        temp_highscore_matrix[x_start + 24 + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].score_val[2][j_loop][i_loop];
        temp_highscore_matrix[x_start + 28 + i_loop][y_start + y_coords[amount_of_plays] + j_loop] = score_board[amount_of_plays].score_val[3][j_loop][i_loop];
      }
    }
  }

  convert_matrix_to_array(temp_highscore_matrix, high_score_page);
  while(1) {
    if (i == 1) {
      display_board(0, all_menu_screens[i]);
      if ((PORTD >> 7) & 0x1 != 0) {
        /* Select */
        break;
      } else if ((PORTF >> 1) & 0x1 != 0) {
        /* Down */
        i = 2;
      }
    } else if (i == 2) {
      display_board(0, all_menu_screens[i]);
      if ((PORTD >> 7) & 0x1 != 0) {
        /* Select */
        delay(1);
        while(1) {
          display_board(0, high_score_page);
          if ((PORTD >> 7) & 0x1 != 0) {
            break;
          }
        }
        display_board(0, all_menu_screens[i]);
        delay(1);
      } else if ((PORTD >> 5) & 0x7 != 0) {
        /* Up */
        i = 1;
      }
    }
  }
  test_dir = 0;
  delay(1);
}

void game_over_screen_on_display() {
  int i, j, digit;
  for (i = 0; i < 3000; i++)  {
    display_board(0, game_over_screen);
  }

  display_board(0, high_score_input);
  uint8_t temp_matrix[128][32];
  int current_score[4] = {score[3], score[2], score[1], score[0]};
  int score_x_pos[3] = {55, 75, 95};
  int letter[3] = {0, 0, 0};

  /* Setup */
  convert_array_to_matrix(high_score_input, temp_matrix);

  for (digit = 0; digit < 3; digit++) {
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 5; j++) {
        temp_matrix[score_x_pos[digit] + i][22 + j] = alphabet[letter[digit]][j][i];
      }
    }
  }

  convert_matrix_to_array(temp_matrix, high_score_input);

  /* Remaining */
  while(1) {
    convert_array_to_matrix(high_score_input, temp_matrix);
      if ((PORTF >> 1) & 0x1 != 0) {
        if (letter[2] < 25) {
          letter[2]++;
        } else {
          letter[2] = 0;
        }
        for (i = 0; i < 3; i++) {
          for (j = 0; j < 5; j++) {
            temp_matrix[score_x_pos[2] + i][22 + j] = alphabet[letter[2]][j][i];
          }
        }
        delay(1);
      }
      if ((PORTD >> 5) & 0x7 != 0) {
        if (letter[1] < 25) {
          letter[1]++;
        } else {
          letter[1] = 0;
        }
        for (i = 0; i < 3; i++) {
          for (j = 0; j < 5; j++) {
            temp_matrix[score_x_pos[1] + i][22 + j] = alphabet[letter[1]][j][i];
          }
        }
        delay(1);
      }
      if ((PORTD >> 6) & 0x3 != 0) {
        if (letter[0] < 25) {
          letter[0]++;
        } else {
          letter[0] = 0;
        }
        for (i = 0; i < 3; i++) {
          for (j = 0; j < 5; j++) {
            temp_matrix[score_x_pos[0] + i][22 + j] = alphabet[letter[0]][j][i];
          }
        }
        delay(1);
      }
      if ((PORTD >> 7) & 0x1 != 0) {
        break;
      }
    convert_matrix_to_array(temp_matrix, high_score_input);
    display_board(0, high_score_input);
  }

  for (digit = 0; digit < 3; digit++) {
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 5; j++) {
        score_board[amount_of_plays].name[digit][j][i] = alphabet[letter[digit]][j][i];
      }
    }
  }

  for (digit = 0; digit < 4; digit++) {
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 5; j++) {
        score_board[amount_of_plays].score_val[digit][j][i] = all_nums[current_score[digit]][j][i];
      }
    }
  }
  second_start_menu();
}

void reset_dot_matrix() {
  int i, j;
  for (i = 0; i < 104; i++) {
    for (j = 0; j < 2; j++) {
      dot_coord_variable[i][j] = dot_coord_original[i][j];
    }
  }
}

void character_start_pos(uint8_t final_matrix[128][32]) {
    pacman.x_pos = 88;
    pacman.y_pos = 26;
    blinky.x_pos = 88;
    blinky.y_pos = 7;
    blinky.dir = 4;
    inky.x_pos = 87;
    inky.y_pos = 14;
    inky.dir = 0;
    clyde.x_pos = 93;
    clyde.y_pos = 14;
    clyde.dir = 0;
    pinky.x_pos = 81;
    pinky.y_pos = 14;
    pinky.dir = 0;
    paint_out(0, final_matrix);
    ghost_release_counter = 0;
    move_counter = 0;
}

void update_hearts() {
    convert_array_to_matrix((uint8_t*)temp, final_matrix);
    int ypos[4] = {8, 14, 20, 26};
    int i,j, amount_of_hearts;

    int remaining = 4 - hearts;
    for (amount_of_hearts = 0; amount_of_hearts < hearts; amount_of_hearts++) {
      for(i = 0; i < 5; i++) {
        for(j = 0; j < 5; j++) {
            final_matrix[117 + i][ypos[amount_of_hearts] + j] = opaque_heart[j][i]; // matrix is flipped on its axis
        }
      }
    }

    if (remaining != 0) {
      for (amount_of_hearts = remaining; amount_of_hearts < 4; amount_of_hearts++) {
        for(i = 0; i < 5; i++) {
          for(j = 0; j < 5; j++) {
              final_matrix[117 + i][ypos[amount_of_hearts] + j] = transparent_heart[j][i]; // matrix is flipped on its axis
          }
        }
      }
    }

    convert_matrix_to_array(final_matrix, temp);
    display_board(0, temp);
}

void init()
{
  dots_eaten = 0;
  convert_array_to_matrix((uint8_t*)board, final_matrix);
  reset_dot_matrix();
  character_start_pos(final_matrix);
  add_dots(final_matrix, dot_coord_original);
  convert_matrix_to_array(final_matrix, temp);
  display_board(0, temp);
}

void show_score_and_lives(int dir)
{
  /* Level difficulty increas */
  if (dots_eaten == 104) {
    if (move_speed != 4) {
      move_speed = move_speed - 4;
      dots_eaten = 0;
    }
    test_dir = 0;
    backup_dir = 0;
    init();
    update_score(score);
    update_hearts();
  } else {
    convert_array_to_matrix((uint8_t*)temp, final_matrix);
    decrement_hearts();
    update_score(score);
    paint_out(dir, final_matrix);
    pacman_move(dir, final_matrix);
    add_dots(final_matrix, dot_coord_variable);
    convert_matrix_to_array(final_matrix, temp);
    display_board(0, temp);
  }
}

void reset_hearts() {
  convert_array_to_matrix((uint8_t*)temp, final_matrix);

  int ypos[4] = {8, 14, 20, 26};
  int i,j,heart;

  if (amount_of_plays != -1) {
    game_over_screen_on_display();
    reset_score();
    init();
  }

  for (heart = 0; heart < 4; heart++) {
    for(i = 0; i < 5; i++) {
      for(j = 0; j < 5; j++) {
          final_matrix[117 + i][ypos[3 - heart] + j] = opaque_heart[j][i]; // matrix is flipped on its axis
      }
    }
  }

  convert_matrix_to_array(final_matrix, temp);
  display_board(0, temp);

  amount_of_plays++;
  hearts = 4;
}

void decrement_hearts() {
  if (hearts == 0) {
    reset_hearts();
  } else {
    convert_array_to_matrix((uint8_t*)temp, final_matrix);
    int ypos[4] = {8, 14, 20, 26};
    int i,j;

    for(i = 0; i < 5; i++) {
      for(j = 0; j < 5; j++) {
          final_matrix[117 + i][ypos[hearts] + j] = transparent_heart[j][i]; // matrix is flipped on its axis
      }
    }

    convert_matrix_to_array(final_matrix, temp);
    display_board(0, temp);
  }
}

void start_menu() {
  int i = 0;
  int counter = 0;
  while(1) {
    display_board(0, all_menu_screens[i]);
    counter++;
    if (counter == 3000) {
      break;
    }
  }
  i = 1;
  while(1) {
    if (i == 1) {
      display_board(0, all_menu_screens[i]);
      if ((PORTD >> 7) & 0x1 != 0) {
        /* Select */
        break;
      } else if ((PORTF >> 1) & 0x1 != 0) {
        /* Down */
        i = 2;
      }
    } else {
      display_board(0, all_menu_screens[i]);
      if ((PORTD >> 7) & 0x1 != 0) {
        /* Select */
        counter = 0;
        while(1) {
          display_board(0, no_high_scores_recorded);
          counter++;
          if (counter == 3000) {
            break;
          }
        }
      } else if ((PORTD >> 5) & 0x7 != 0) {
        /* Up */
        i = 1;
      }
    }
  }
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
