/*
 * pwm.c
 *
 *  Created on: Mar 31, 2015
 *      Author: mrandrewandrade
 */

#include "lpc17xx_pinsel.h"
#include <math.h>

#include "pwm.h"


void init_pwm()
{
	// ensure PWM peripheral is powered on (it is powered on by default)
    LPC_SC->PCONP |= 1 << 6;

    LPC_PWM1->TCR = 2;                      // bring PWM module into reset
    LPC_PWM1->IR = 0xff;                    // clear any pending interrupts

    // configure P2.0 for PWM1.1 - O - Pulse Width Modulator 1, channel 1 output.
    LPC_PINCON->PINSEL4 = (LPC_PINCON->PINSEL4 & ~(0x3 << 0)) | (0x1 << 0);

    // Disable pullups for P2.0
    LPC_PINCON->PINMODE4 = (LPC_PINCON->PINMODE4 & ~0x3) | 0x2;

    // Set prescale so we have a resolution of 1us
    LPC_PWM1->PR = SystemCoreClock / (4 * 1000000) - 1;
    LPC_PWM1->MR0 = 20000;                  // set the period in us. 50Hz rate
    LPC_PWM1->MR1 = 1500;                   // set duty of 1.5ms
    LPC_PWM1->MR2 = 19000;                  // set a match that occurs 1ms
                                            // before the TC resets.
    LPC_PWM1->LER = 0x7;                    // set latch-enable register
    LPC_PWM1->MCR = 0x2 | (1 << 6);         // reset on MR0, interrupt on MR2
    LPC_PWM1->PCR = 1 << 9;                 // enable PWM1 with single-edge operation

    LPC_PWM1->TCR = (1 << 3) | 1;           // enable PWM mode and counting

}

void set_pwm_value (int value)
{
	LPC_PWM1->IR = 0xff;              // clear interrupt flags
	// write duty cycle value and set Latch-enable register
	//LPC_PWM1->MR1 = duty[n];
	LPC_PWM1->MR1 = value;
	LPC_PWM1->LER = 1 << 1;
}
