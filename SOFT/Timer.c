
#include <LPC21XX.H>                           
#include "Timer.h"
#include "main.h"

long volatile timeval;


/* Setup the Timer Counter 0 Interrupt */
void t0_init (void) 
{
SET_REG(PINSEL1,3,(29-16)*2,2);
SET_REG(PINSEL1,3,(27-16)*2,2);

T0PR=10;
T0MR0 = 1000;                              // 10mSec = 150.000-1 counts
T0MCR = 3;                                   // Interrupt and Reset on MR0
                                   // Timer0 Enable
VICVectAddr0 = (unsigned long)timer0_interrupt;           // set interrupt vector in 0
VICVectCntl0 = 0x20 | 4;                     // use it for Timer 0 Interrupt
VICIntEnable = 0x00000010;                   // Enable Timer0 Interrupt

SET_REG(T0EMR,1,4+(1*2),2);
SET_REG(T0EMR,1,4+(3*2),2);
T0MR1 = 500;
T0MR3 = 500; 

T0TCR = 1;
}





/* Setup the Timer Counter 0 Interrupt */
void t1_init (void) 
{
T1MR0 = 60000;                              // 10mSec = 150.000-1 counts
T1MCR = 3;                                   // Interrupt and Reset on MR0
T1TCR = 1;                                   // Timer0 Enable
VICVectAddr1 = (unsigned long)timer1_interrupt;           // set interrupt vector in 0
VICVectCntl1 = 0x20 | 5;                     // use it for Timer 0 Interrupt
VICIntEnable = 0x00000020;                   // Enable Timer0 Interrupt
}
