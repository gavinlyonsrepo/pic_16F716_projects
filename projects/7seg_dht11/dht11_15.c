
/* ======== Header ============
 * File:   dht11_15.c
 * Author: gavin lyons
 * URL: https://github.com/gavinlyonsrepo/pic_16F716_projects
 * IDE: MPLAB X v4.2 
 * Compiler: xc8 v1.45
  * Created on 10 September 2018, 21:17
 */

/* ======= Includes ========*/
#include <xc.h>
#include <stdint.h>

// #define _XTAL_FREQ
 #define _XTAL_FREQ 16000000


/* ======== Define and Pragma Directives   ======== */

// Setup CONFIG register
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config BODENV = 40      // Brown-out Reset Voltage bit (VBOR set to 4.0V)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)

// define the 4Digit select 7 segment to portA
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4
//  Interfacing DHT11 sensor signal in. 
#define DHT11_pin RA0


/* ======= Globals========*/

//   array to hold numbers 1-10 for seven segment
uint8_t data[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
uint8_t counter=0; //counter incremented by Timer0 overflow.
unsigned short Check, Temp, RH, Sum ;

/* ==== Function prototypes ===== */
void setup(void);
void display(void);

char ReadData(void);
void StartSignal(void);
void CheckResponse(void);
void DataParse(void);

/* ====== Main loop ======= */
void main(void) {
    setup();
    while(1)
    {
        display();
    }
}
/*===== End of Main=========*/


/* ======= Function Space ==========*/

void setup(void)
{
    __delay_ms(2000); //  start up Wait for sensor to stabilize
    //Turn off ADC
    ADCON0=0x00;
    //setup I/O
    TRISA=0x01;     //All outputs but RA0          
    TRISB=0x00;
    PORTA=0x00;
    PORTB=0x00;
    display(); 
    __delay_ms(50);
    // Setup timer option_reg register
     T0CS=0;   // SELECT TIMER0
     PSA=0;    // PRESCALER ASSIGNED TO TIMER0
     OPTION_REG &= 0x07;    // SELECT 1:256
     //interupts intcon register 
     GIE=1;        // ENABLE GLOBAL INTERRUPT
     T0IE=1;       // TIMER0 INTERRUPT ENABLE
     T0IF=0;    // CLEAR FLAG  
   
}

/* Function: display
 displays the DHt value on the seven segment display. */
void display(void)
{

    uint8_t  digit4, digit3, digit2 , digit1;
    
    if (Check == 0) // DHT did not respond if check 0 display zeros.
    {
        digit1 = 0;
        digit2 = 0;
        digit3 = 0;
        digit4 = 0;
    }else if (Check == 1) // Good respond, display data
    {
        if ((Sum == (RH + Temp)) )  // is the Checksum OK?
        {
            digit1 = (Temp / 10) % 10;
            digit2 = Temp % 10;
            digit3 = (RH / 10) % 10;
            digit4 = RH % 10;
        }else // bad checksum display all nines.
        {
            digit1 = 9;
            digit2 = 9;
            digit3 = 9;
            digit4 = 9;
        }
    }
    
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

/*Function: Interupt service routine for Timer0
 executed every 0.016 Second X 120 = 2 seconds approx */
void interrupt ISR(void)
{
     if(T0IF) 
     {
        counter++;
        if(counter==120)
        { 
          counter=0;
          StartSignal(); //send start signal to DHT11 sensor
          CheckResponse(); //get response from DHT11 sensor
          if (Check == 1) // Did DHT11 respond ? 
          {
             DataParse(); 
          }
          }
        }
     
     T0IF=0;
}


/* StartSignal Function: 
  sends request 18uS low pulse for data to the sensor*/
void StartSignal(){
    TRISA0 = 0;    //Set TRISIO0 as output
    DHT11_pin = 0;    //GP4 sends 0 to DHT11 (request)
    __delay_ms(18);
    DHT11_pin = 1;    //GP4 sends 1 DHT 
    __delay_us(30); 
    TRISA0 = 1;  //Set TRISIO0 as input
  }

/* Check Response function: 
  Check if DHT responded to Start signal 
  40uS low pulse 80uS high pulse*/
 void CheckResponse(){
    Check = 0;
    __delay_us(40); 
    if (DHT11_pin == 0){
    __delay_us(80);
    if (DHT11_pin == 1)   Check = 1;   __delay_us(40);}
 }
 
 /* ReadData function:
  Read in one byte of data from DHT11 sensor returns byte as char.
  */
 char ReadData(){
    char i, j;
    for(j = 0; j < 8; j++){
       while(!DHT11_pin); //Wait until GP4 goes HIGH (50uS)
       __delay_us(30);
       if(DHT11_pin == 0)
             i&= ~(1 << (7 - j));  //Its a zero Clear bit 
       else {i|= (1 << (7 - j));  //Its a 1 Set bit 
       while(DHT11_pin);}  //Wait until GP4 goes LOW
    }
 
 return i;
}
 
 /* dataParse function: read  the sensor data and put into data.*/
void DataParse()
{
            // Read in the four bytes
            RH = ReadData(); // intergal Humidity byte into RH
            ReadData();   // discard decimal byte
            Temp = ReadData(); //intergal temperature byte into temp
            ReadData();  // discard decimal byte
            Sum = ReadData(); // checksum byte into sum 
}

/* =========== EOF ===========*/
