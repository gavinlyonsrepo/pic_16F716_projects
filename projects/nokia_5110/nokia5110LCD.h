/* ======== Header ============
 * File:  nokia5110LCD.h
 * Author: Gavin Lyons
 * IDE: MPLAB X : v5.15
 * Complier: xc8 v2.00
 * URL: https://github.com/gavinlyonsrepo/pic_16F716_projects
 * Created on 18 September 2018, 00:03
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef nokia5110LCD_H
#define	nokia5110LCD_H

#include <xc.h>
#include <stdint.h> /* For uint8_t definition */

// Pins for the Nokia 5110 LCD
#define LCD_CLK RB0
#define LCD_DIN RB1
#define LCD_DC RB2
#define LCD_CE RB5
#define LCD_RST RB4

//The DC pin tells the LCD if sending a command or data
#define LCD_COMMAND 0
#define LCD_DATA 1
// 84 by 48 pixels screen
#define LCD_X 84
#define LCD_Y 48

void LCDInit(void);
void LCDgotoXY(uint8_t , uint8_t );
void LCDClear(void);
void LCDWrite(unsigned char , unsigned char);
void LCDCharacter(char); 
void LCDString(char *characters); 

#endif	/* XC_HEADER_TEMPLATE_H */

