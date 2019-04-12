/* ======== Header ============
 * File:  nokia5110.c
 * Author: Gavin Lyons
 * IDE: MPLAB X : v5.15
 * Complier: xc8 v2.00
 * URL: https://github.com/gavinlyonsrepo/pic_16F716_projects
 * Created on 18 September 2018, 00:03
 */

/* ======= Includes ========*/
#include <xc.h>
#include "nokia5110LCD.h"
#include <stdint.h> /* For uint8_t definition */
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

/* ======= Globals========*/
uint8_t potvalue=150; // to hold pot value from RA0
uint8_t counter=0;   //counter incremented by Timer0 overflow

/* ==== Function prototypes ===== */
void setup(void);
unsigned int GetADCValue(void);
void Data_Display(void);
void __interrupt() my_ISR(void);

/* ====== Main loop ======= */

void main(void) {
    
    setup();
    LCDClear();
    while(1)
    {
       //Everything is done on timer0 interrupt.
    }    
    return;
}
/*===== End of Main=========*/


/* ======= Function Space ==========*/

/* Function LCDdisplay , Displays the 
 data to screen called every 2 seconds */
void Data_Display(void)
{
      LCDClear();
      
      char strADC[4];
      char strVolt1[2];
      char strVolt2[5];
      uint16_t voltvalue=1;
 
      //calculations 
       sprintf(strADC, "%d", potvalue); // convert int to string
       voltvalue = potvalue * 129;
      sprintf(strVolt1, "%u", (( voltvalue/10000)%10));
      sprintf(strVolt2, "%u%u%u%u", (( voltvalue/1000)%10), (( voltvalue/100)%10), (( voltvalue/10)%10), ( voltvalue%10 ));
      
      
      LCDString("3.3V Battery");
      LCDgotoXY(0 , 1);
      LCDString("Tester. GL");
      LCDgotoXY(0 , 2);
      LCDString("ADC Value:");
      LCDgotoXY(0 , 3);
      LCDString(strADC);
      LCDgotoXY(0 , 4);
      LCDString("Voltage:");     
      LCDgotoXY(0 , 5);
      LCDString(strVolt1);
      LCDString(".");
      LCDString(strVolt2);
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
          Data_Display();
        }
     }
     T0IF=0;
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

/* =========== EOF ===========*/
