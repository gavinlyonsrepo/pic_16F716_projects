
Overview
--------------------------------------------
* Name: DHT11 sensor to Seven segment, PIC16F716
* Description: A program to display the data of a DHT11 sensor to a 4-digit seven segment display (SMA420564) makes use of timer0 interrupt feature to refresh the value. 

* Author: Gavin Lyons.

Table of contents
---------------------------

  * [Overview](#overview)
  * [Features](#features)


Features
----------------------

In this folder the source code in C can be found in file dht11_15.c

The Data is read on a interrupt generated by overflow of timer0.

The DHT returns temperature and humidity readings (ranges 0-50'C and 20-90% respectively.) .
The digits of the temperature are displayed on left hand side of LED module
and the digits of the humidity on right hand side. i.e. if the sensor is reported 25'C and 49% 
humidity the Display reads 2549.

PortB is connected to the anodes of the SMA420564,
With exception of RB7 which is not connected. 
The Decimal point pin on SMA420564 is not connected as its not needed for this application. 
PortA is connected to the common cathode of each digit thru a transistor switch.
with exception of RA0 which is connected to the Sensor signal line.
RA4 requires a 10K pullup resistor to function as digital output here
as it is Open drain when configured as output.

MCLR pin is tied high thru a pullup. as it is the active low reset. 

The clock is 16Mhz. The Sensor output is read on the timer overflow.

1. Clock Source in Mhz	16	Mhz			
2. Fosc   	16000000	Hz			
3. Fosc / 4	4000000	Hz			
4. Time Period	2.5E-07	sec			
5. Prescaler	256				
6. Timer0 Interrupt Period	0.016384	sec			
7. Frequency	61.03515625	Hz			
8. Period of Frequency Input To Timer0	6.4E-05	sec			
9. Period of Time for each Timer0 Count	0.016384	sec			
					
10. Preload (TMR0 value)	6				
11. Delay (Timer0 Period with Preload)	0.016	sec		0.016	sec
					
12. Required Timer	1	sec			
13. Number of Interrupts	62.5				

The timer interrupts overflows every .016 mS and a incremental counter refresh's display every 2 second approx.


**Error codes**

1. 0000 = Communications Error. No response to the PICs request signal from sensor.
2. 9999 = Checksum Error. Received all data from sensor but it failed checksum. 


Communication Format with DHT11 can be separated into three stages.

1. Request
2. Response
3. Data Reading 5 bytes.
4. Sum the 1st 4 bytes and check if the result is the same as CheckSum(5th byte)

| Pic 16F716 pin  | LED module SMA420564 |
| ------ | ------ |
| RA1 18 | D1 12  |
| RA2 1| D2 9 |
| RA3 2 | D3 8 |
| RA4 3 | D4 6 |
| RB0 6| a 11 |  
| RB1 7| b 7 |
| RB2 8| c 4 |
| RB3 9| d 2|
| RB4 10| e 1 |
| RB5 11| f 10 |
| RB6 12| g 5 |


![PIC](https://github.com/gavinlyonsrepo/pic_16F716_projects/blob/master/images/7segpinout.png)



Schematic
----------------------------------


![PIC](https://github.com/gavinlyonsrepo/pic_16F716_projects/blob/master/images/7segDHT11.png)

 
