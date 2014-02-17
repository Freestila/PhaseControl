/**** A P P L I C A T I O N   N O T E   ************************************
*
* Title			: 8ch forward phase control dimmer
* Version		: v2.3_Arduino
* Last updated	: 24.04.07
* Target		: Transceiver Rev.3.01 [ATmega8515]
* Clock			: 8MHz
* MainsFreq		: 50Hz
*
* original written by hendrik hoelscher, www.hoelscher-hi.de
* transformed for Arduino by Helge Stasch
* Feel free to change e.g. maximum number of channels and pinmapping
***************************************************************************
 This program is free software; you can redistribute it and/or 
 modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation; either version2 of 
 the License, or (at your option) any later version. 

 This program is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 General Public License for more details. 

 If you have no copy of the GNU General Public License, write to the 
 Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 

 For other license models, please contact the author.

;***************************************************************************/

//#include <Wiring.h>
#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#else
 #include <WProgram.h>
#endif
#include "lib_phase_cntrl.h"
#include <avr/pgmspace.h>

volatile uint8_t	 DimmerField[5];	//array of DMX vals

// ********************* local definitions *********************
		uint8_t 	gDimmCount;

const uint16_t C3Curve[] PROGMEM =	{
0x2710, 0x2472, 0x2346, 0x228F, 0x2204, 0x2190, 0x212C, 0x20D2, 0x2081, 0x2037, 0x1FF2, 0x1FB1, 0x1F74, 0x1F3A, 0x1F03, 0x1ECE, 
0x1E9B, 0x1E6B, 0x1E3C, 0x1E0E, 0x1DE2, 0x1DB8, 0x1D8E, 0x1D66, 0x1D3E, 0x1D18, 0x1CF2, 0x1CCE, 0x1CAA, 0x1C86, 0x1C64, 0x1C42, 
0x1C21, 0x1C00, 0x1BE0, 0x1BC0, 0x1BA1, 0x1B82, 0x1B64, 0x1B46, 0x1B28, 0x1B0B, 0x1AEE, 0x1AD2, 0x1AB6, 0x1A9A, 0x1A7E, 0x1A63, 
0x1A48, 0x1A2E, 0x1A13, 0x19F9, 0x19DF, 0x19C6, 0x19AC, 0x1993, 0x197A, 0x1961, 0x1949, 0x1930, 0x1918, 0x1900, 0x18E8, 0x18D1, 
0x18B9, 0x18A2, 0x188A, 0x1873, 0x185C, 0x1846, 0x182F, 0x1818, 0x1802, 0x17EB, 0x17D5, 0x17BF, 0x17A9, 0x1793, 0x177E, 0x1768, 
0x1752, 0x173D, 0x1727, 0x1712, 0x16FD, 0x16E8, 0x16D2, 0x16BD, 0x16A9, 0x1694, 0x167F, 0x166A, 0x1655, 0x1641, 0x162C, 0x1618, 
0x1603, 0x15EF, 0x15DB, 0x15C6, 0x15B2, 0x159E, 0x158A, 0x1576, 0x1562, 0x154E, 0x153A, 0x1526, 0x1512, 0x14FE, 0x14EA, 0x14D6, 
0x14C2, 0x14AE, 0x149B, 0x1487, 0x1473, 0x1460, 0x144C, 0x1438, 0x1424, 0x1411, 0x13FD, 0x13EA, 0x13D6, 0x13C2, 0x13AF, 0x139B, 
0x1388, 0x1374, 0x1360, 0x134D, 0x1339, 0x1325, 0x1312, 0x12FE, 0x12EB, 0x12D7, 0x12C3, 0x12AF, 0x129C, 0x1288, 0x1274, 0x1261, 
0x124D, 0x1239, 0x1225, 0x1211, 0x11FD, 0x11E9, 0x11D5, 0x11C1, 0x11AD, 0x1199, 0x1185, 0x1171, 0x115D, 0x1149, 0x1134, 0x1120, 
0x110C, 0x10F7, 0x10E3, 0x10CE, 0x10BA, 0x10A5, 0x1090, 0x107B, 0x1066, 0x1052, 0x103D, 0x1027, 0x1012, 0x0FFD, 0x0FE8, 0x0FD2, 
0x0FBD, 0x0FA7, 0x0F91, 0x0F7C, 0x0F66, 0x0F50, 0x0F3A, 0x0F24, 0x0F0D, 0x0EF7, 0x0EE0, 0x0EC9, 0x0EB3, 0x0E9C, 0x0E85, 0x0E6D, 
0x0E56, 0x0E3E, 0x0E27, 0x0E0F, 0x0DF7, 0x0DDF, 0x0DC6, 0x0DAE, 0x0D95, 0x0D7C, 0x0D63, 0x0D49, 0x0D30, 0x0D16, 0x0CFC, 0x0CE1, 
0x0CC7, 0x0CAC, 0x0C91, 0x0C75, 0x0C59, 0x0C3D, 0x0C21, 0x0C04, 0x0BE7, 0x0BC9, 0x0BAB, 0x0B8D, 0x0B6E, 0x0B4F, 0x0B2F, 0x0B0F, 
0x0AEE, 0x0ACD, 0x0AAB, 0x0A89, 0x0A65, 0x0A41, 0x0A1D, 0x09F7, 0x09D1, 0x09A9, 0x0981, 0x0957, 0x092D, 0x0901, 0x08D3, 0x08A4, 
0x0874, 0x0841, 0x080C, 0x07D5, 0x079B, 0x075E, 0x071D, 0x06D8, 0x068E, 0x063D, 0x05E3, 0x057F, 0x050B, 0x0480, 0x03C9, 0x029D 
};

void clearOutput(){
digitalWrite(5,HIGH);
digitalWrite(6,HIGH);
digitalWrite(7,HIGH);
digitalWrite(8,HIGH);
digitalWrite(9,HIGH);
}


// ************** Timer synchronisation **************
//ISR (INT1_vect)
void INT1vect(){

TCNT1  = 0;											//sync T1 with mains zc
gDimmCount= 0xFF;									//reset dimmer counter
OCR1A  = pgm_read_word(C3Curve +0xFF); 				//set first compare angle (full brightness)
TIMSK |= (1<<OCIE1A);								//enable compare irq
TIFR   = (1<<OCF1A);								//clear Timer flag
GICR  &= ~(1<<INT1);								//clear self (avoid multi-triggering by noise)	
}

// *************** phase control Initialisation ****************
void init_phase_cntrl()
{
//Output
//DDRA = 0xFF;
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
//PORTA= 0xFF;										//high Output
clearOutput();

//ZC Input
//DDRD  &= ~(1<<PD3);									//input with pull up

//PORTD |=  (1<<PD3);

//Timer1
TCCR1A= 0;
TCCR1B= (1<<CS11);									//set T1 @clk/8

//ext. Int1. (zc)
//MCUCR  = (1<<ISC10);								//irq on logical change
//GICR  |= (1<<INT1);									//enable INT1 for zc
attachInterrupt(1, INT1vect, CHANGE);
GIFR   = (1<<INTF1);								//clear INT1 flag
}





// ****************** phase control ISR ********************
ISR (TIMER1_COMPA_vect)
{
uint8_t DimmCount= gDimmCount;						//just load once!

//PORTA= 0xFF;										//reset port
clearOutput();

if (DimmCount <= DimmerField[0])					//compare ch1-8
	{
	//PORTA &= ~(1<<0);								//fire TRIAC if DimmCount < ch val
	digitalWrite(5,LOW);
	}
if (DimmCount <= DimmerField[1])
	{
	//PORTA &= ~(1<<1);
	digitalWrite(6,LOW);
	}
if (DimmCount <= DimmerField[2])
	{
	//PORTA &= ~(1<<2);
	digitalWrite(7,LOW);
	}
if (DimmCount <= DimmerField[3])
	{
	//PORTA &= ~(1<<3);
	digitalWrite(8,LOW);
	}
if (DimmCount <= DimmerField[4])
	{
	//PORTA &= ~(1<<4);
	digitalWrite(9,LOW);
	}

//set up next angle to compare
uint16_t NxtAngle;
do	{
	DimmCount--;
	if (DimmCount == 0)
		{
		gDimmCount= DimmCount;
	//	PORTA  = 0xFF;								//quench TRIACs
		clearOutput();
		TIMSK &= ~(1<<OCIE1A);						//disable self
		GIFR   = (1<<INTF1);						//clear zc flag
		GICR  |= (1<<INT1);							//enable zc irq
		return;
		}
	NxtAngle= pgm_read_word(C3Curve +DimmCount); 	//corresponding firing angle
	}
while ((TCNT1 +5) >= NxtAngle);
gDimmCount= DimmCount;
OCR1A= NxtAngle;
}

