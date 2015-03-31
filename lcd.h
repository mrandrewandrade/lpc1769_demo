/*
 * lcd.h
 *
 *  Created on: Mar 31, 2015
 *      Author: mrandrewandrade
 */

#ifndef LCD_H_
#define LCD_H_

#define NOTE_PIN_HIGH() GPIO_SetValue(0, 1<<26);
#define NOTE_PIN_LOW()  GPIO_ClearValue(0, 1<<26);
#define LCD_RS_PORT 0
#define LCD_RS_PIN 9
#define LCD_E_PORT 0
#define LCD_E_PIN 7
#define LCD_DB0_PORT 2
#define LCD_DB0_PIN 13
#define LCD_DB1_PORT 0
#define LCD_DB1_PIN 28
#define LCD_DB2_PORT 0
#define LCD_DB2_PIN 27
#define LCD_DB3_PORT 0
#define LCD_DB3_PIN 22
#define LCD_DB4_PORT 0
#define LCD_DB4_PIN 21
#define LCD_DB5_PORT 0
#define LCD_DB5_PIN 3
#define LCD_DB6_PORT 0
#define LCD_DB6_PIN 2
#define LCD_DB7_PORT 1
#define LCD_DB7_PIN 31


void init_lcd(void);  // Assigns data, RS, E pins and ports
void LCD_set_E(int value);
void LCD_set_RS(int value);
void lcd_data(char c);  // Write the data into the LCD
void lcd_string(const char *s);  // Display the string in the LCD
void lcd_line1();  // Takes the cursor to the 1st line
void lcd_line2();  // Takes the cursor to the 2nd line
void lcd_line3();  // Takes the cursor to the 3rd line
void lcd_line4();  // Takes the cursor to the 4th line
void lcd_clr();  //Clears the screen
void lcd_init();  //Initialize the LCD parameters


#endif /* LCD_H_ */
