
Overview
--------------------------------------------
* Name: ADC value to Nokia 5110 LCD display
* Description: An example program to display an 8-bit ADC value(0-255) in a
Nokia 5110 LCD display(PCD8544) makes use of timer0 interrupt feature to read the value. A pot is connected to an
Analog input of the pic16F716.

* Author: Gavin Lyons.

Table of contents
---------------------------

  * [Overview](#overview)
  * [Features](#features)


Features
----------------------

In this folder the source code in c can be found in Nokia_5110.c

The 8-bit ADC (0 to 255) value is read from a pot attached to pin RA0.

Most of PortB is connected to the Nokia 5110 LCD.
Five pins are required for the LCD.

MCLR pin is tied high thru a pullup. as it is the active low reset.

The clock is 16Mhz. The ADC is read on the timer0 interrupt.

1. Clock Source in Mhz  16  Mhz
2. Fosc     16000000    Hz
3. Fosc / 4 4000000 Hz
4. Time Period  2.5E-07 sec
5. Prescaler    256
6. Timer0 Interrupt Period  0.016384    sec
7. Frequency    61.03515625 Hz
8. Period of Frequency Input To Timer0  6.4E-05 sec
9. Period of Time for each Timer0 Count 0.016384    sec

10. Preload (TMR0 value)    0
11. Delay (Timer0 Period with Preload)  0.016   sec     0.016   sec

12. Required Timer  1   sec
13. Number of Interrupts    62.5

The timer interrupts overflows every .016 mS
and a incrementalist counter refresh's display every 2 second approx.


| Pic 16F716 pin  | Nokia 5110 LCD|
| ------ | ------ |
| RB0 6| LCD_CLK Pin 5 clock in |
| RB1 7| LCD_DIN Pin 4 data in |
| RB2 8| LCD_DC Pin 3 data/command|
| RB5 11| LCD_CE Pin 2 chip enable |
| RB4 10| LCD_RST Pin 1 reset|


Connect Nokia 5110 VCC(pin 6)to 3.3V, some may appear work at 5V
but they are not supposed to and may
overtime fail or work intermittent.
The user may have to adjust LCD contrast and bias settings in code
to the screen at hand for optimal display.
These settings are in the LCDinit function.
A resistor or pot can be connected between (Nokia LCD) GND(pin8) and LIGHT(pin7) to switch on backlight
and adjust it.




[Nokia 5110 LCD dataSheet ](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)

Schematic.

![SCH](https://github.com/gavinlyonsrepo/pic_16F716_projects/blob/master/images/5110.png)


**Practical Application as a battery tester**

The ADC is 8-bit resolution (0-255). 

So at 3.3V supply 3.3V/256 = 0.01289 volts per unit(VPU). 

So multiple the given ADC value(lets say 120 for example) by VPU to get voltage (0.01289 X 120) = 1.548V.

Replace the pot with battery connector and add a load and 10K pull down resistor.

This can then measure any battery below 3.3V. Where ADC 255= 3.3V.

![PIC](https://github.com/gavinlyonsrepo/pic_16F716_projects/blob/master/images/nokiapic.jpg)

