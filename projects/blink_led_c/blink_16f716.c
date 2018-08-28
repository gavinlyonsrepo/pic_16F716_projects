/*
 * File:   blink_16f716.c
 * Author: gavin lyons
 * URL: https://github.com/gavinlyonsrepo/pic_16F716_projects
 * IDE: MPLAB X v4.2 
 * Compiler: xc8 v1.45
 * Created on 27 August 2018, 18:50
 */
#include <xc.h>

#ifndef _XTAL_FREQ
  #define _XTAL_FREQ 1976000 //RC circuit. 
#endif

// BEGIN CONFIG
#pragma config FOSC = RC // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG


void main()
{
    TRISA=0x00;                // TRISA direction ALL pins output
    PORTA=0x00;                // make all GP port on port A low
    while(1)
    {
        RA2=1;                     // Make bit 2 PORTA port high
        __delay_ms(1000); 
        RA2=0;                     // Make bit 2 PORTA port low
        __delay_ms(1000);
    }

}

