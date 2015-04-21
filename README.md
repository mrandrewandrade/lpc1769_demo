# WheelCare: Proximity Sensor and Danger Feedback For Wheechairs 

This project was part of EE3032 while on a year aboard at the National Univesity of Singapore.

This is the code running a on the [NXP LPC1769 LPCEXPRESSO microcontroller development board](http://www.embeddedartists.com/products/lpcxpresso/lpc1769_xpr.php) created by Embedded Artists connected to a custom designed and manufactured daughter PCBas

## Project Summary

Given the open ended problem of building an assistive device for the elderly, a danger feedback system using proximity sensing was designed, prototyped and manufactured. The solution to reducing wheelchair collisions is to provide passive and active feedback through the 3 main senses (visual, audio, touch).  The initial sketch of the product is shown below.

![alt text](https://github.com/mrandrewandrade/proximity-sensor-and-danger-feedback-for-wheelchairs/raw/master/images/system_sketch.jpeg "Initial Sketch")

The product consists of three sonar proximity sensors, an LCD screen, LED alert dashboard, three haptic actuators and three buzzers. The microcontroller constantly polls the sensors to collect distance readings.  After filtering the readers and determining if objects are approaching for three locations (left, center and right) feedback is given in the form of visual distance measurements on the LCD display, flashing lights on the led display, audio buzzes and physical vibration provided by surround feedback around the user. 



## About this project
We were given the open ended task of creating an "Assistive Device for the Elderly" to aide their quality of life.  We were to design a custom circuit board, send the board for manufacturing to China and program a microcontroller and present our solution.

## Problem

### Constraints

