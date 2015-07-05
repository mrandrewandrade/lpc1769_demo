
/*
 * lcd.c
 *
 *  Created on: Mar 31, 2015
 *      Author: mrandrewandrade
 */

#include "lcd.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_timer.h"
#include "stdio.h"

void init_lcd(void) //Assigns data, RS, E pins and ports
{
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB7_PORT;
    PinCfg.Pinnum = LCD_DB7_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB7_PORT, 1<<LCD_DB7_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB6_PORT;
    PinCfg.Pinnum = LCD_DB6_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB6_PORT, 1<<LCD_DB6_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB5_PORT;
    PinCfg.Pinnum = LCD_DB5_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB5_PORT, 1<<LCD_DB5_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB4_PORT;
    PinCfg.Pinnum = LCD_DB4_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB4_PORT, 1<<LCD_DB4_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB3_PORT;
    PinCfg.Pinnum = LCD_DB3_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB3_PORT, 1<<LCD_DB3_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB2_PORT;
    PinCfg.Pinnum = LCD_DB2_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB2_PORT, 1<<LCD_DB2_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB1_PORT;
    PinCfg.Pinnum = LCD_DB1_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB1_PORT, 1<<LCD_DB1_PIN, 1);

    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_DB0_PORT;
    PinCfg.Pinnum = LCD_DB0_PIN;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(LCD_DB0_PORT, 1<<LCD_DB0_PIN, 1);

    //Initialize E pin
    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_E_PORT;
    PinCfg.Pinnum = LCD_E_PIN;
    PINSEL_ConfigPin(&PinCfg);
    GPIO_SetDir(LCD_E_PORT, (1 << LCD_E_PIN), 1);

    //Initialize RS pin
    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = LCD_RS_PORT;
    PinCfg.Pinnum = LCD_RS_PIN;
    PINSEL_ConfigPin(&PinCfg);
    GPIO_SetDir(LCD_RS_PORT, (1 << LCD_RS_PIN), 1);

}

void LCD_set_E(int value)
{
    switch(value)
    {
        case 0:
            GPIO_ClearValue( LCD_E_PORT, (1<<LCD_E_PIN) );
            break;
        case 1:
            GPIO_SetValue( LCD_E_PORT, (1<<LCD_E_PIN) );
            break;
    }
}

void LCD_set_RS(int value)
{
    switch(value)
    {
        case 0:
            GPIO_ClearValue( LCD_RS_PORT, (1<<LCD_RS_PIN) );
            break;
        case 1:
            GPIO_SetValue( LCD_RS_PORT, (1<<LCD_RS_PIN) );
            break;
    }
}

void lcd_data(char c) //Write the data into the LCD
{
    GPIO_ClearValue(0, 0x1860000C);
    GPIO_ClearValue(1, 0x80000000);
    GPIO_ClearValue(2, 0x00002000);

    GPIO_SetValue(LCD_DB0_PORT, (c&1)<<LCD_DB0_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB1_PORT, (c&1)<<LCD_DB1_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB2_PORT, (c&1)<<LCD_DB2_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB3_PORT, (c&1)<<LCD_DB3_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB4_PORT, (c&1)<<LCD_DB4_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB5_PORT, (c&1)<<LCD_DB5_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB6_PORT, (c&1)<<LCD_DB6_PIN );
    c/=2;
    GPIO_SetValue(LCD_DB7_PORT, (c&1)<<LCD_DB7_PIN );

    Timer0_us_Wait(30);
    LCD_set_E(1);
    Timer0_us_Wait(3);
    LCD_set_E(0); //write data to LCD
    Timer0_us_Wait(160);
}

void lcd_string(const char *s)	//Display the string in the LCD
{
    LCD_set_RS(1);				//Enable write to LCD data reg
    while(*s)					//The loop runs till the end of the string
    {
        lcd_data(*s++);			//Write each character to the LCD data reg
        Timer0_us_Wait(45);
    }
    LCD_set_RS(0);				//Disable write to LCD data reg
}

void lcd_line1()			//Takes the cursor to the 1st line
{
    LCD_set_RS(0);
    lcd_data(0x80); // DDRAM address of 00
    Timer0_us_Wait(45);
}

void lcd_line2()			//Takes the cursor to the 2nd line
{
    LCD_set_RS(0);
    lcd_data(0xC0); // 0xC0 -> DDRAM address of 40
    Timer0_us_Wait(45);
}

void lcd_line3()			//Takes the cursor to the 3rd line
{
    LCD_set_RS(0);
    lcd_data(0x94); // DDRAM address of 14
    Timer0_us_Wait(45);
}

void lcd_line4()			//Takes the cursor to the 4th line
{
    LCD_set_RS(0);
    lcd_data(0xD4); // DDRAM address of 14
    Timer0_us_Wait(45);
}

void lcd_clr()				//Clears the screen
{
    LCD_set_RS(0);
    lcd_data(0x01);
    Timer0_Wait(2);
}

void lcd_init()				//Initialize the LCD parameters
{

    LCD_set_RS(0);
    LCD_set_E(0);
    GPIO_ClearValue(0, 0x1860000C);
    GPIO_ClearValue(1, 0x80000000);
    GPIO_ClearValue(2, 0x00002000);
    Timer0_Wait(15);

    lcd_data(0x01);             //Clears the screen
    Timer0_Wait(2);
    lcd_data(0x02);             //Return home
    Timer0_Wait(2);
    lcd_data(0x06);             //Enable entry mode
    Timer0_us_Wait(45);
    lcd_data(0x0C);             //Display on/off control
    Timer0_us_Wait(45);
    lcd_data(0x38);             //Function set 0x38 for 8-bit interface, 0x28 for 4-bit interface
    Timer0_us_Wait(45);
}
