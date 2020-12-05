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
   // Btn1. Down
   if ((PORTF >> 1) & 0x1 != 0) {
      show_score_and_lives(2);
   }
   // Btn2. Up
   if ((PORTD >> 5) & 0x7 != 0) {
      show_score_and_lives(8);
   }
   // Btn3. Right
   if ((PORTD >> 6) & 0x3 != 0) {
      show_score_and_lives(6);
   }
   // Btn4. Left
   if ((PORTD >> 7) & 0x1 != 0) {
      show_score_and_lives(4);
   }
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  TRISD |= 0xfe0; // trisd bits set to output
  TRISFSET = (1 << 1);
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  btnpress();
}
