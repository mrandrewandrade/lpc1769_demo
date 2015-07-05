/*
 * pwm.c
 *
 *  Created on: Mar 31, 2015
 *      Author: mrandrewandrade
 */

#include "lpc17xx_pinsel.h"
#include <math.h>

#include "pwm.h"

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

    //P2.0 works as PWM1.1 output.pin42,MR1
    LPC_PINCON->PINSEL4 |= 1 << 0;
    //P2.1 works as PWM1.2 output.pin43,MR2
    LPC_PINCON->PINSEL4 |= 1 << 2;
    //P2.2 works as PWM1.3 output.pin44,MR3
    LPC_PINCON->PINSEL4 |= 1 << 4;
    //P2.3 works as PWM1.4 output.pin45,MR4
    LPC_PINCON->PINSEL4 |= 1 << 6;
    //P2.4 works as PWM1.5 output.pin46,MR5
    LPC_PINCON->PINSEL4 |= 1 << 8;
    //P2.4 works as PWM1.6 output.pin47,MR6
    LPC_PINCON->PINSEL4 |= 1 << 10;

    //enable P2.1,MR2 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 1;
    //enable P2.1,MR2 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 3;
    //enable P2.2,MR3 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 5;
    //enable P2.3,MR4 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 7;
    //enable P2.4,MR5 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 9;
    //enable P2.5,MR6 neither pull up nor pull down.
    LPC_PINCON->PINMODE4 |= 1 << 11;

    //Set the sytem clock
    LPC_PWM1->PR = SystemCoreClock / (4 * 1000000) - 1;

}

void pwm_one_on(int pulsewidth)
{
    //pin42,pwm1.1
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR1 = pulsewidth;
    LPC_PWM1 -> LER |= (1<<1);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 9;//Enable PWM channel 2 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer

}
void pwm_two_on(int pulsewidth)
{
    //pin43,pwm1.2
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR2 = pulsewidth;
    LPC_PWM1 -> LER |= (1<<2);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 10;//Enable PWM channel 2 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer

}

void pwm_three_on(int pulsewidth)
{
    //pin44,pwm1.3
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR3 = pulsewidth;
    LPC_PWM1 -> LER |= (1<<3);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 11;//Enable PWM channel 3 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer
}

void pwm_four_on(int pulsewidth)
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
void pwm_five_on(int pulsewidth)
{
    //pin45,pwm1.4
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR5 = pulsewidth;
    //LPC_PWM1->LER = 0x7F;
    LPC_PWM1 -> LER |= (1<<5);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 13;//Enable PWM channel 3 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer
}
void pwm_six_on(int pulsewidth)
{
    //pin45,pwm1.4
    LPC_PWM1->MR0 = 80000; //PWM freq = PWM clock/2000000=100M/2M=50Hz , so T=20ms
    LPC_PWM1->MR6 = pulsewidth;
    //LPC_PWM1->LER = 0x7F;
    LPC_PWM1 -> LER |= (1<<6);
    LPC_PWM1->MCR = 1 << 1; //Reset timer on Match0
    LPC_PWM1->PCR |= 1 << 14;//Enable PWM channel 3 output
    LPC_PWM1->TCR = (1 << 3) | 1;//Enable the timer
}
