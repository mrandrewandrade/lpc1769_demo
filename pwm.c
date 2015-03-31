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

void init_multiple_pwm(void)
{
    //ensure PWM peripheral is powered on
    LPC_SC->PCONP |= 1 << 6 ;
    // bring PWM module into reset
    LPC_PWM1->TCR= 2;
    // clear any pending interrupts
    LPC_PWM1->IR = 0xff;
    //Clear all the pins for PWM operation before configuration
    LPC_PINCON->PINSEL4 &= ~0x3F;
    //Setup and provide the clock to PWM peripheral: Prescaler value is 1
    LPC_SC->PCLKSEL0 = 1 << 12;

    //P2.0 works as PWM1.0 output.pin42,MR1
    LPC_PINCON->PINSEL4 |= 1 << 0;
    //P2.1 works as PWM1.2 output.pin43,MR2
    LPC_PINCON->PINSEL4 |= 1 << 2;
    //P2.2 works as PWM1.3 output.pin44,MR3
    LPC_PINCON->PINSEL4 |= 1 << 4;
    //P2.3 works as PWM1.4 output.pin45,MR4
    LPC_PINCON->PINSEL4 |= 1 << 6;

    //enable P2.1,MR2 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 1;
    //enable P2.1,MR2 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 3;
    //enable P2.2,MR3 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 5;
    //enable P2.3,MR4 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 7;
    //Set the sytem clock
    LPC_PWM1->PR = SystemCoreClock / (4 * 1000000) - 1;

}

void haptic_actuator_one(int pulsewidth)
{
    //pin43,pwm1.2
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR1 = pulsewidth;
    LPC_PWM1 -> LER |= (1<<1);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 9;//Enable PWM channel 2 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer

}
void haptic_actuator_two(int pulsewidth)
{
    //pin43,pwm1.2
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR2 = pulsewidth;
    LPC_PWM1 -> LER |= (1<<2);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 10;//Enable PWM channel 2 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer

}

void speaker_output_one(int pulsewidth)
{
    //pin44,pwm1.3
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR3 = pulsewidth;
    LPC_PWM1 -> LER |= (1<<3);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 11;//Enable PWM channel 3 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer
}

void speaker_output_two(int pulsewidth)
{
    //pin45,pwm1.4
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR4 = pulsewidth;
    //LPC_PWM1->LER = 0x7F;
    LPC_PWM1 -> LER |= (1<<4);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 12;//Enable PWM channel 3 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer
}
