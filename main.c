/*****************************************************************************
  EE3032 Project Demo
 ******************************************************************************/

#include <stdio.h>
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"

#include "pwm.h"
#include "lcd.h"

// SONAR Trig Ports and Pins
#define SONAR_TRIG_PORTNUM 0
#define SONAR_TRIG_PINNUM_1 6
#define SONAR_TRIG_PINNUM_2 0
#define SONAR_TRIG_PINNUM_3 1

// Capture Interrupt Ports and Pins
#define CAP_PORT_1 1
#define CAP_PIN_1 18
#define CAP_PORT_2 0
#define CAP_PIN_2 4
#define CAP_PORT_3 0
#define CAP_PIN_3 23

// LED Ports and Pins
#define LED_PORTNUM_1 2
#define LED_PORTNUM_2 2
#define LED_PORTNUM_3 2
#define LED_PINNUM_1 10
#define LED_PINNUM_2 11
#define LED_PINNUM_3 12

// Set Distance Ranges
#define SONAR_MAX 150000
#define SONAR_MED 100000
#define SONAR_MIN 50000

#define POLLING_COUNT 5;

static void init_led_GPIOs(void) {
    // Initialize button
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 0;
    PinCfg.Portnum = LED_PORTNUM_1;
    PinCfg.Pinnum = LED_PINNUM_1;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Portnum = LED_PORTNUM_2;
    PinCfg.Pinnum = LED_PINNUM_2;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Portnum = LED_PORTNUM_3;
    PinCfg.Pinnum = LED_PINNUM_3;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LED_PORTNUM_1, 1<<LED_PINNUM_1, 1);
    GPIO_SetDir(LED_PORTNUM_2, 1<<LED_PINNUM_2, 1);
    GPIO_SetDir(LED_PORTNUM_3, 1<<LED_PINNUM_3, 1);
}

uint32_t min_distance(uint32_t val1, uint32_t val2, uint32_t val3) {
    uint32_t min = SONAR_MAX;

    if (val1 < min) {
        min = val1;
    }
    if (val2 < min) {
        min = val2;
    }
    if (val3 < min) {
        min = val3;
    }

    return min;
}

void init_Timer(LPC_TIM_TypeDef *TIMx, uint32_t time) {
    TIM_TIMERCFG_Type TIM_ConfigStruct;
    TIM_MATCHCFG_Type TIM_MatchConfigStruct;

    // Initialize timer 0, prescale count time of 1ms
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
    TIM_ConfigStruct.PrescaleValue = 1;
    // use channel 0, MR0
    TIM_MatchConfigStruct.MatchChannel = 0;
    // Enable interrupt when MR0 matches the value in TC register
    TIM_MatchConfigStruct.IntOnMatch = TRUE;
    //Enable reset on MR0: TIMER will not reset if MR0 matches it
    TIM_MatchConfigStruct.ResetOnMatch = TRUE;
    //Stop on MR0 if MR0 matches it
    TIM_MatchConfigStruct.StopOnMatch = TRUE;
    //do no thing for external output
    TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    // Set Match value, count value is time (timer * 1000uS =timer mS )
    TIM_MatchConfigStruct.MatchValue = time;

    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(TIMx, TIM_TIMER_MODE, &TIM_ConfigStruct);
    TIM_ConfigMatch(TIMx, &TIM_MatchConfigStruct);
}

static void init_Counter(LPC_TIM_TypeDef *TIMx) {
    TIM_COUNTERCFG_Type TIM_ConfigStruct;
    TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;

    TIM_ConfigStruct.CounterOption = TIM_COUNTER_INCAP0;
    TIM_CaptureConfigStruct.CaptureChannel = 0;
    TIM_CaptureConfigStruct.RisingEdge = ENABLE;
    TIM_CaptureConfigStruct.FallingEdge = ENABLE;
    TIM_CaptureConfigStruct.IntOnCaption = ENABLE;

    TIM_Init(TIMx, TIM_COUNTER_ANY_MODE, &TIM_ConfigStruct);
    TIM_ConfigCapture(TIMx, &TIM_CaptureConfigStruct);
}

void led_state(uint32_t led_portnum, uint32_t led_pinnum, uint32_t sonar_value, int step_num) {
    if (sonar_value < SONAR_MIN) {
        if (step_num % 2) {
            GPIO_SetValue(led_portnum, 1<<led_pinnum);
        } else {
            GPIO_ClearValue(led_portnum, 1<<led_pinnum);
        }
    } else if (sonar_value > SONAR_MIN && sonar_value < SONAR_MED) {
        if (step_num == 0 || step_num == 6) {
            GPIO_SetValue(led_portnum, 1<<led_pinnum);
        } else if (step_num == 3 || step_num == 9){
            GPIO_ClearValue(led_portnum, 1<<led_pinnum);
        }
    } else if (sonar_value > SONAR_MED && sonar_value < SONAR_MAX) {
        if (step_num == 0) {
            GPIO_SetValue(led_portnum, 1<<led_pinnum);
        } else if (step_num == 12){
            GPIO_ClearValue(led_portnum, 1<<led_pinnum);
        }
    }
}

void activate_feedback(uint32_t sonar_1, uint32_t sonar_2, uint32_t sonar_3, uint32_t is_approaching_1, uint32_t is_approaching_2, uint32_t is_approaching_3) {

    uint32_t step = 0;


    char buffer1 [20] = "";
    char buffer2 [20] = "";

    // Call functions for the LCD and speakers here

    //Format the LCD display strings with values
    sprintf(buffer1, "Dist: %d | %d | %d", (long)sonar_1, (long)sonar_2, (long)sonar_3);
    sprintf(buffer2, "Diff:%d | %d | %d", (long)is_approaching_1, (long)is_approaching_2, (long)is_approaching_3);

    //Display the LCD strings
    lcd_line1();
    lcd_string((const char*)buffer1);
    lcd_line2();
    lcd_string((const char*)buffer2);

    // Activate LEDs
    for (step = 0; step < 24; step++) {
        led_state(LED_PORTNUM_1, LED_PINNUM_1, sonar_1, step);
        led_state(LED_PORTNUM_2, LED_PINNUM_2, sonar_2, step);
        led_state(LED_PORTNUM_3, LED_PINNUM_3, sonar_3, step);
        Timer0_Wait(50);
    }

    /*
    // Activate haptic actuators
    uint32_t min = 0;
    min = min_distance(sonar_1, sonar_2, sonar_3);
    if (min < SONAR_MIN) {
    set_pwm_value(PWM_HI);
    } else if (min > SONAR_MIN && min < SONAR_MED) {
    set_pwm_value(PWM_MED);
    } else if (min > SONAR_MED && min < SONAR_MAX) {
    set_pwm_value(PWM_LO);
    }*/

    uint32_t sonar_1_output=0;
    if (sonar_1 < SONAR_MIN){
        sonar_1_output = PWM_HI;
    }
    else if (sonar_1 > SONAR_MIN && sonar_1 < SONAR_MED){
        sonar_1_output = PWM_MED;
    }
    else if (sonar_1 > SONAR_MED && sonar_1 < SONAR_MAX){
        sonar_1_output = PWM_LO;
    }
    uint32_t sonar_2_output=0;
    if (sonar_2 < SONAR_MIN){
        sonar_2_output = PWM_HI;
    }
    else if (sonar_2 > SONAR_MIN && sonar_2 < SONAR_MED){
        sonar_2_output = PWM_MED;
    }
    else if (sonar_2 > SONAR_MED && sonar_2 < SONAR_MAX){
        sonar_2_output = PWM_LO;
    }
    uint32_t sonar_3_output=0;
    if (sonar_3 < SONAR_MIN){
        sonar_3_output = PWM_HI;
    }
    else if (sonar_3 > SONAR_MIN && sonar_3 < SONAR_MED){
        sonar_3_output = PWM_MED;
    }
    else if (sonar_3 > SONAR_MED && sonar_3 < SONAR_MAX){
        sonar_3_output = PWM_LO;
    }

    uint32_t APPROACH_HIGH = 10000;
    uint32_t APPROACH_MED = 1000;
    uint32_t APPROACH_LOW = 100;

    //set output_x_level from 1-3 based on the speed 
    uint32_t output_1_level = (is_approaching_1 > APPROACH_HIGH) + (is_approaching_1 > APPROACH_MED) + (is_approaching_1 > APPROACH_LOW);
    uint32_t output_2_level = (is_approaching_2 > APPROACH_HIGH) + (is_approaching_2 > APPROACH_MED) + (is_approaching_2 > APPROACH_LOW);
    uint32_t output_3_level = (is_approaching_3 > APPROACH_HIGH) + (is_approaching_3 > APPROACH_MED) + (is_approaching_3 > APPROACH_LOW);

    pwm_one_on(sonar_1_output*(output_1_level== 3));
    pwm_two_on(sonar_1_output*(output_1_level== 3));

    pwm_three_on(sonar_2_output*(output_2_level== 3));
    pwm_four_on(sonar_2_output*(output_2_level== 3));

    pwm_five_on(sonar_3_output*(output_3_level== 3));
    pwm_six_on(sonar_3_output*(output_3_level== 3));

    Timer0_Wait(50);
    pwm_one_on(0);
    pwm_two_on(0);
    pwm_three_on(0);
    pwm_four_on(0);
    pwm_five_on(0);
    pwm_six_on(0);
    Timer0_Wait(50);

    pwm_one_on(sonar_1_output*(output_1_level> 1));
    pwm_two_on(sonar_1_output*(output_1_level> 1));

    pwm_three_on(sonar_2_output*(output_2_level> 1));
    pwm_four_on(sonar_2_output*(output_2_level> 1));

    pwm_five_on(sonar_3_output*(output_3_level> 1));
    pwm_six_on(sonar_3_output*(output_3_level> 1));

    Timer0_Wait(50);
    pwm_one_on(0);
    pwm_two_on(0);
    pwm_three_on(0);
    pwm_four_on(0);
    pwm_five_on(0);
    pwm_six_on(0);
    Timer0_Wait(50);

    pwm_one_on(sonar_1_output*(output_1_level> 0));
    pwm_two_on(sonar_1_output*(output_1_level> 0));

    pwm_three_on(sonar_2_output*(output_2_level> 0));
    pwm_four_on(sonar_2_output*(output_2_level> 0));

    pwm_five_on(sonar_3_output*(output_3_level> 0));
    pwm_six_on(sonar_3_output*(output_3_level> 0));

    Timer0_Wait(50);
    pwm_one_on(0);
    pwm_two_on(0);
    pwm_three_on(0);
    pwm_four_on(0);
    pwm_five_on(0);
    pwm_six_on(0);
    Timer0_Wait(50);


    // Turn off all LEDs
    GPIO_ClearValue(LED_PORTNUM_1, 1<<LED_PINNUM_1);
    GPIO_ClearValue(LED_PORTNUM_2, 1<<LED_PINNUM_2);
    GPIO_ClearValue(LED_PORTNUM_3, 1<<LED_PINNUM_3);


    // Set all PWM values back to 0, as well as reset speakers and LCD if required
}

uint32_t pulse_SONAR(uint32_t trig_port, uint32_t trig_pin, LPC_TIM_TypeDef *counter, LPC_TIM_TypeDef *timer, uint32_t cap_port, uint32_t cap_pin) {
    uint32_t timeout = 0;
    uint32_t signal_start_time = 0;
    uint32_t signal_end_time = 0;

    PINSEL_CFG_Type PinCfg;

    // Initialize Capture Interrupt
    PinCfg.Funcnum = 3;
    PinCfg.Portnum = cap_port;
    PinCfg.Pinnum = cap_pin;
    PINSEL_ConfigPin(&PinCfg);

    // Initialize TRIG
    PinCfg.Funcnum = 0;
    PinCfg.Portnum = trig_port;
    PinCfg.Pinnum = trig_pin;
    PINSEL_ConfigPin(&PinCfg);
    GPIO_SetDir(trig_port, 1<<trig_pin, 1);

    // Configure and enable timers and counters
    init_Counter(counter);
    init_Timer(timer, 60000);
    TIM_Cmd(counter, ENABLE);
    TIM_Cmd(timer, ENABLE);

    // Send signal HIGH to TRIG for 10 us
    GPIO_ClearValue(trig_port, 1<<trig_pin);
    Timer0_us_Wait(5);
    GPIO_SetValue(trig_port, 1<<trig_pin);
    Timer0_us_Wait(15);
    GPIO_ClearValue(trig_port, 1<<trig_pin);

    while (!signal_start_time && !timeout) {
        // Check if timeout
        if (TIM_GetIntStatus(timer, 0)) {
            TIM_ClearIntPending(timer, 0);
            timeout = 1;
        }
        // Check for Rising Edge
        if (TIM_GetIntCaptureStatus(counter, 0)) {
            TIM_ClearIntCapturePending(counter, 0);
            signal_start_time = TIM_GetCaptureValue(counter, 0);
            //printf("Start time: %d\n", signal_start_time);
        }
    }

    while (!signal_end_time && !timeout) {
        // Check for Falling Edge
        if (TIM_GetIntCaptureStatus(counter, 0)) {
            TIM_ClearIntCapturePending(counter, 0);
            signal_end_time = TIM_GetCaptureValue(counter, 0);
            //printf("End time: %d\n", signal_end_time);
        }
        // Check if timeout
        if (TIM_GetIntStatus(timer, 0)) {
            TIM_ClearIntPending(timer, 0);
            timeout = 1;
        }
    }

    if (TIM_GetIntStatus(timer, 0)) {
        TIM_ClearIntPending(timer, 0);
    }

    TIM_Cmd(timer, DISABLE);
    TIM_Cmd(counter, DISABLE);
    if (!timeout) {
        return signal_end_time - signal_start_time;
    } else {
        return -1;
    }
}

int main (void) {
    uint32_t sonar_1 = -1;
    uint32_t sonar_2 = -1;
    uint32_t sonar_3 = -1;

    uint32_t sonar_1_previous = 10000000;
    uint32_t sonar_2_previous = 10000000;
    uint32_t sonar_3_previous = 10000000;

    int32_t is_approching_1 = 0;
    int32_t is_approching_2 = 0;
    int32_t is_approching_3 = 0;

    init_led_GPIOs();
    //init_pwm();
    init_lcd();
    lcd_clr();
    lcd_init();

    init_multiple_pwm();

    while (1) {

        //average the reading values
        int iii =0;
        sonar_1= 0;
        sonar_2= 0;
        sonar_3= 0;
        for (iii=0, i<POLLING_COUNT, i++){

            sonar_3 += pulse_SONAR(SONAR_TRIG_PORTNUM, SONAR_TRIG_PINNUM_3, LPC_TIM3, LPC_TIM1, CAP_PORT_3, CAP_PIN_3);
            sonar_2 += pulse_SONAR(SONAR_TRIG_PORTNUM, SONAR_TRIG_PINNUM_2, LPC_TIM2, LPC_TIM1, CAP_PORT_2, CAP_PIN_2);    
        }
        sonar_1 = sonar_1/POLLING_COUNT;
        sonar_3 = sonar_3/POLLING_COUNT;
        sonar_2 = sonar_2/POLLING_COUNT;

        //Calculate distace moved in the poll
        is_approching_3 = sonar_3_previous - sonar_3;
        is_approching_2 =  sonar_2_previous - sonar_2;
        is_approching_1 =  sonar_1_previous - sonar_1;

        //Store previous reading
        sonar_1_previous = sonar_1;
        sonar_3_previous = sonar_3;
        sonar_2_previous = sonar_2;


        if (sonar_1 != -1 || sonar_2 != -1 || sonar_3 != -1) {
            activate_feedback(sonar_1, sonar_2, sonar_3);
        }

        printf("SONAR 1 distance: %d\n", sonar_1);
        printf("SONAR 2 prev: %d\n", sonar_1_previous);
        printf("SONAR 3 approaching: %d\n", is_approching_1);

        printf("SONAR 2 distance: %d\n", sonar_2);
        printf("SONAR 2 prev: %d\n", sonar_2_previous);
        printf("SONAR 2 approaching: %d\n", is_approching_2);

        printf("SONAR 3 distance: %d\n\n", sonar_3);
        printf("SONAR 3 prev: %d\n", sonar_3_previous);
        printf("SONAR 2 approaching: %d\n", is_approching_3);

        activate_feedback(sonar_1, sonar_2, sonar_3, is_approaching_1, is_approaching_2, is_approaching_3);
    }
}

void check_failed(uint8_t *file, uint32_t line) {
    /* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while(1);
}
