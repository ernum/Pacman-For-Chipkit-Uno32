/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount = 0;
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Initialising PORTE */
volatile int *porte = (volatile int *)0xBF886110;

/* Interrupt Service Routine */
void user_isr(void)
{
  timeoutcount++; // Increment the variable for every time-out event

  int interrupt_request = (IFS(0) & 0xF1FF) >> 8;

  if (interrupt_request)
  {
    if (timeoutcount == 10)
    {
      /* Other functions */
      time2string(textstring, mytime);
      display_string(3, textstring);
      display_update();
      tick(&mytime);
      timeoutcount = 0;
    }
    IFS(0) = 0; // Reset event flag
  }
}

void enable_interrupt(void)
{
  asm volatile("ei");
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
  volatile int *trise = (volatile int *)0xBF886100;
  *trise &= 0xFF00;                  // Setting the bits 7 through 0 to output (without changing the function of other bits)
  TRISD = (TRISD & 0xF00F) | 0x0EF0; // XFEX XXXX 0111 1111 XXXX

  /* Initialisation of Timer 2 and Interrupt */
  T2CON = T2CON & 0x7FFF; // Turn timer off
  TMR2 = 0;               // Resetting time to 0 x
  PR2 = 31250;            // 80 Mhz / 256 / 10

  IPC(7) = (IPC(7) & 0xFFE3) | 0x001C; // Priority set to 7
  IPC(7) = (IPC(7) & 0xFFFC) | 0x0003; // Sub-priority set to 3
  IFS(0) = 0;                          // Clear IRQ flag again
  IEC(0) = (IEC(0) & 0xFEFF) | 0x0100; // Enable timer interrupts
  enable_interrupt();                  // Enable interrupts on the micro-controller

  T2CON = (T2CON & 0x7F87) | 0x8078; // Setting the ON bit, the prescaler to 256 and using a 32-bit timer.
}

/* This function is called repetitively from the main program */
void labwork(void)
{
}
