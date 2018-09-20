/* ======== Header ============
 * File:  nokia5110.c
 * Author: Gavin Lyons
 * IDE: MPLAB X : v5.05
 * Complier: xc8 v2.00
 * URL: https://github.com/gavinlyonsrepo/pic_16F716_projects/tree/master/projects
 * Created on 18 September 2018, 00:03
 */

/* ======= Includes ========*/
#include <xc.h>
#include <stdint.h> /* For uint8_t definition */
#include <stdbool.h> /* For true/false definition */
#include <stdio.h> /* for sprintf */


/* ======== Define and Pragma Directives   ======== */

#define _XTAL_FREQ 16000000

// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG

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

/* ======= Globals========*/

// Define the ASCII table as Data array
// cols left to right 0x00 is  off 0xFF is all on
//  each character is five bytes wide one byte height. 
// so ! is 0 0 5F 0 0 
const unsigned char ASCII[][5] = {
{0x00, 0x00, 0x00, 0x00, 0x00} // 20
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 ! 
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 ?
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 ?
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d ?
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c forward slash
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

uint8_t potvalue=150; // to hold pot value from RA0
uint8_t counter=0;   //counter incremented by Timer0 overflow



/* ==== Function prototypes ===== */
void setup(void);
unsigned int GetADCValue(void);
void LCDInit(void);
void LCDDisplay(void);
void gotoXY(int , int );
void LCDClear(void);
void LCDWrite(unsigned char , unsigned char);
void LCDCharacter(char); 
void LCDString(char *characters); 
void __interrupt() my_ISR(void);


/* ====== Main loop ======= */

void main(void) {
    
    setup();
    LCDClear();
    while(1)
    {
       
    }    
    return;
}
/*===== End of Main=========*/


/* ======= Function Space ==========*/

/* Function LCDdisplay , Displays the 
 data to screen called every 2 seconds */
void LCDDisplay(void)
{
      LCDClear();
      char str[4];
      char strtwo[7];
      int voltvalue=1;
      LCDString("Battery Test");
      LCDString("ADC        :");     
      sprintf(str, "%d", potvalue); // convert int to string
      LCDString(str);
      gotoXY(0 , 3);
      LCDString("ADC * 129  :");     
      voltvalue = potvalue * 129;
      sprintf(strtwo, "%d", voltvalue);
      LCDString(strtwo);
}


/*Function: Interupt service routine for Timer0
 executed every 0.016 Second */
void __interrupt() my_ISR(void)
{
     if(T0IF) 
     {
        counter++;
        if(counter==120)  // 120 * 16mS ~= 2 seconds
        { 
          counter=0; 
          potvalue=GetADCValue();
          LCDDisplay();
        }
     }
     T0IF=0;
}


/*Function : LCDinit
This sends the  commands to the PCD8544 to  init LCD
*/
void LCDInit(void) {
    //Configure control pins
    LCD_DIN=0;
    LCD_CLK=0;
    LCD_DC=0;
    //Reset the LCD to a known state
    LCD_RST=0;
    LCD_RST=1;
    LCDWrite(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
    LCDWrite(LCD_COMMAND, 0xBF); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
    LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48: Try 0x13 or 0x14
    LCDWrite(LCD_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
    LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode. 0x0D for inverse
}

/* Function: LCDClear 
Clears the LCD by writing zeros to the entire screen
 */
void LCDClear(void) {
    for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
    LCDWrite(LCD_DATA, 0x00);
    gotoXY(0, 0); //After we clear the display, return to the home position
    }

/* Function: gotoXY gotoXY routine to position cursor 
 x - range: 0 to 84
 y - range: 0 to 5 ( 6 blocks one byte each 6*8 = 48*/
void gotoXY(int x, int y) {
    LCDWrite(0, 0x80 | x); // Column.
    LCDWrite(0, 0x40 | y); // Row.
}

/* Function: setup
 setup the PIC and LCD*/
void setup()
{
    //setup ADC for RA0 only 
    ADCON1=0x00;
    ADCON0=0xC1;
    //setup I/O
    TRISA=0x01;     //All outputs but RA0          
    TRISB=0x00;     // All outputs
    PORTA=0x00;
    PORTB=0x00;
    // Setup timer option_reg register
    T0CS=0;   // SELECT TIMER0
    PSA=0;    // PRESCALER ASSIGNED TO TIMER0
    //OPTION_REG &= 0x07;    // SELECT 1:256
    PS2=1;   
    PS1=1;
    PS0=1;
    
    //interupts intcon register      
    GIE=1;        // ENABLE GLOBAL INTERRUPT
    T0IE=1;       // TIMER0 INERRUPT ENABLE
    T0IF=0;    // CLEAR FLAG
    
    LCDInit(); //Init the LCD
}

/* Function: GetADCValue
 * gets ADC vale from RA0 returns unsigned int 8-bit value. 0->256
 */
unsigned int GetADCValue(void)
{
    __delay_ms(10);      // Time for Acqusition capacitor to charge up and show correct value
	GO_nDONE  = 1;		 // Enable Go/Done
	while(GO_nDONE)
    ;  //wait for conversion completion           
    return (ADRES);   // Return 10 bit ADC value
}


/* Function: LCDWrite 
There are two  banks in the LCD, data and commands. This
function sets the DC pin high or low depending, and then sends
the data byte
 */
void LCDWrite(unsigned char data_or_command, unsigned char data) {
    unsigned char i,d;
    d=data;
    if(data_or_command==0)LCD_DC=0;
    else LCD_DC=1;
    //data_or_command; 
    //Tell the LCD that we are writing either to data or a command
    //Send the data
    LCD_CE=0;
    for(i=0;i<8;i++)
    {
        LCD_DIN=0;
        if(d&0x80)LCD_DIN=1;
        LCD_CLK=1;
        d<<=1;
        LCD_CLK=0;
    }
   LCD_CE=1;
}

/* Function: LCDCharacter.
 This function takes in a character, looks it up in the font table/array
And writes it to the screen
Each character is 8 bits tall and 5 bits wide. We pad one blank column of
pixels on each side of the character for readability.
 */
void LCDCharacter(char character) {
    LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding
    for (int index = 0 ; index < 5 ; index++)
    LCDWrite(LCD_DATA, ASCII[character - 0x20][index]);
    //0x20 is the ASCII character for Space The font table starts with this character
    LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding
}

/* Function: LCDString.
 Given a string of characters, one by one is passed to the LCD
 */
void LCDString(char *characters) {
    while (*characters)
    LCDCharacter(*characters++);
}

/* =========== EOF ===========*/