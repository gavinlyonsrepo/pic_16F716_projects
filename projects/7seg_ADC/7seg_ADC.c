
/* ======== Header ============
 * File:   7seg_ADC.c
 * Author: gavin lyons
 * URL: https://github.com/gavinlyonsrepo/pic_16F716_projects
 * IDE: MPLAB X v4.2 
 * Compiler: xc8 v1.45
 * Created on 04 September 2018, 22:59
 */

/* ======= Includes ========*/
#include <xc.h>
#include <stdint.h>

// #define _XTAL_FREQ
 #define _XTAL_FREQ 16000000


/* ======== Define and Pragma Directives   ======== */

// Setup CONFIG register
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)

// define the 4Digit select 7 segment to portA
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4

/* ======= Globals========*/

//   array to hold numbers 1-10 for seven segment
uint8_t  data[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
uint8_t potvalue=0; //varible to hold pot value from RA0
uint8_t counter=0; //counter incremented by Timer0 overflow.

 /* ======== Function space ===========*/

/* Function: Setup
 * Setups the SPECIAL FUNCTION REGISTERS  registers of PIC
 */
void setup(void)
{
    //setup ADC for RA0 only 
    ADCON1=0x00;
    ADCON0=0xC1;
    //setup I/O
    TRISA=0x01;     //All outputs but RA0          
    TRISB=0x00;
    PORTA=0x00;
    PORTB=0x00;
    // Setup timer option_reg register
     T0CS=0;   // SELECT TIMER0
     PSA=0;    // PRESCALER ASSIGNED TO TIMER0
     OPTION_REG &= 0x07;    // SELECT 1:256
     //interupts intcon register 
     GIE=1;        // ENABLE GLOBAL INTERRUPT
     T0IE=1;       // TIMER0 INTERRUPT ENABLE
     T0IF=0;    // CLEAR FLAG
     
    __delay_ms(500);
}

/* Function: GetADCValue
 * gets ADC vale from RA0 returns unsigned int 8-bit value. 0->256
 */
unsigned int GetADCValue(void)
{
    __delay_ms(10);      // Time for Acqusition capacitor to charge up and show correct value
	GO_nDONE  = 1;		 // Enable Go/Done
	while(GO_nDONE);     //wait for conversion completion
    return (ADRES);   // Return 10 bit ADC value
}

/*Function: Interupt service routine for Timer0
 executed every 0.016 Second */
void interrupt ISR(void)
{
     if(T0IF) 
     {
        counter++;
        if(counter==60)
        { 
          counter=0;
          potvalue=GetADCValue();
        }
     }
     T0IF=0;
}

/* Function: display
 displays the pot value on the seven segment display. */
void display(void)
{

    uint8_t  digit4, digit3, digit2 , digit1;
    digit1 = 0;
    digit2 = (potvalue/100)%10;
    digit3 = (potvalue/10)%10;
    digit4 = potvalue%10;
    
    PORTB=data[digit1];
     D1=0x01;
     __delay_ms(5);
     D1=0x00;
     
     PORTB=data[digit2];
     D2=0x01; 
     __delay_ms(5);
     D2=0x00;
     
     PORTB=data[digit3];
     D3=0x01; 
     __delay_ms(5);
     D3=0x00;

     PORTB=data[digit4];
     D4=0x01; 
     __delay_ms(5);
     D4=0x00;
     
}


/* ====== Main loop ======= */
void main()
{
    setup();
    potvalue=GetADCValue();
    while(1)
    {      
               display();          
    }

}
/* ====== EOF  =======*/
