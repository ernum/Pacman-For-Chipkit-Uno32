/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
char *itoaconv(int num);
void labwork();
int nextprime(int inval);
void quicksleep(int cyc);
void tick(unsigned int *timep);

void show_score_and_lives(int dir);

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug(volatile int *const addr);

/* Declare bitmap array containing font */
extern const uint8_t const font[128 * 8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declar background for score "area" */
extern const uint8_t const score_background[384];
/* Declare bitmap containing a game board */
extern const uint8_t const board[512];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

extern uint8_t final_matrix[128][32];

extern uint8_t dot_coord_original[104][2];
extern uint8_t dot_coord_variable[104][2];
extern uint8_t intersection_coord[38][2];

extern uint8_t temp[512];

extern uint8_t board_matrix[128][32];

/* Start screen related */
extern uint8_t all_menu_screens[3][512];

/* First direction to test*/
extern int test_dir;
/* If test_dir doesn't work this is the previous direction.*/
extern int backup_dir;

extern int pacman_open_right[5][5];
extern int pacman_open_left[5][5];
extern int pacman_close[5][5];

extern int ghost_blinky[5][5];
extern int ghost_inky[5][5];
extern int ghost_clyde[5][5];
extern int ghost_pinky[5][5];

extern int hearts;

extern const uint8_t const opaque_heart[5][5];

extern const uint8_t const transparent_heart[5][5];

typedef struct {
    int x_pos;
    int y_pos;
}Character;

/* Numbers */
extern const uint8_t const all_nums[10][5][3];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
void delay(int);
void time2string(char *, int);
/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
void enable_interrupt(void);
