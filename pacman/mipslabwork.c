/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

volatile int* porte = (volatile int*)0xbf886110;// initialize porte
volatile int* portf = (volatile int*)0xbf886110;// initialize porte

int test_dir;

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}


int getbtns(void){
  return (PORTD >> 5) & 0x0007;
  return (PORTF >> 1) & 0x1;
}

void btnpress(){
   if ((PORTF >> 1) & 0x1 != 0) {
      test_dir = 2;
   }
   // Btn2. Up
   if ((PORTD >> 5) & 0x7 != 0) {
      test_dir = 8;
   }
   // Btn3. Right
   if ((PORTD >> 6) & 0x3 != 0) {
      test_dir = 6;
   }
   // Btn4. Left
   if ((PORTD >> 7) & 0x1 != 0) {
      test_dir = 4;
   }
   show_score_and_lives(test_dir);
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  TRISD |= 0xfe0; // trisd bits set to output
  TRISFSET = (1 << 1);

  /* Initialisation and functionality of Timer 2 */
  TMR2 = 0;                          // Resetting time to 0 x
  T2CON = (T2CON & 0x7F87) | 0x8078; // Setting the ON bit, the prescaler to 256 and using a 32-bit timer.
  PR2 = 312.5;                       // 80 Mhz / 256 / 10
  IFS(0) = 0;                        // Set event flag to  0.

}

/* This function is called repetitively from the main program */
//void labwork(int* hearts)
void labwork()
{
   /* Get the 8th bit which is the interrupt for timer */
   int event_flag = (IFS(0) & 0xF1FF) >> 8;
   if (event_flag)
   {
      IFS(0) = 0;     // Reset event flag
      btnpress();
   }
}
