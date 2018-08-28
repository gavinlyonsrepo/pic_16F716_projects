
Overview
--------------------------------------------
* Name: blink Led in c
* Description: A program to blink an LED attached to PORTA RP2 every second in c
* Author: Gavin Lyons.

Table of contents
---------------------------

  * [Overview](#overview)
  * [Features](#features)


Features
----------------------

In this folder the source code in c can be found in file blink_16f716.c.
LED attached to RA2
This code is based on using an RC external oscillator.
* Vdd = 5v
* Vih = Vdd * 0.9 = 4.5v
* Rext = 10k
* Cext = 22pf

The calculations show the charge time of the RC circuit to reach the Vih level is 506 nanoseconds.

* Time=(10k∗22pf)ln((5v/(5v−4.5))=506nanoseconds
* OscillatorFrequency(Fosc)=1/Time=1/506nanoseconds=1.976Mhz
* InstructionClock=Fosc/4=494Khz


![PIC](https://github.com/gavinlyonsrepo/pic_16F716_projects/blob/master/images/blink.jpg)

 
