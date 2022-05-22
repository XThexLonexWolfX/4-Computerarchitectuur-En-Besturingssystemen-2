#include "hwconfig.h"

#include "DriverSysClk.h"
#include "DriverUSART.h"
#include "DriverCursorStick.h"
#include "DriverPower.h"
#include "DriverTwiMaster.h"
#include "DriverAdc.h"
#include "DriverLed.h"
#include <string.h>

#include <util/delay.h>

#include <stdio.h>

char text[11];
char a, b; // b is voor LoopLicht functie for loop, Richting is voor LoopLicht functie en we starten met links
char RichtingString[] = "looplicht_links"; // Basisrichting is links
int	RichtingInt = 1; // Basisrichting is links
uint8_t LedData = 0b0001; // LedData initialiseren met eerste LED die brand

void SimpleFunction(void);	//A simple function: print a counter (0 to 9) to the terminal
void LoopLicht(void); // Start een looplicht

int main(void)
{
	//###1###
	
	//Initialize drivers
	DriverSysClkXtalInit();	//Clock init
	DriverUSARTInit();		//USART init and link to stdio
	DriverTWIMInit();		//Initialize TWI in master mode
	DriverCursorstickInit();//Initialize cursor stick
	DriverLedInit();		//Initialize LED's
	DriverPowerInit();		//Initialize aux power driver
	DriverAdcInit();		//Initialize ADC driver
	DriverMotorInit();		//Initialize motor driver

	DriverPowerVccAuxSet(1);//Enable Auxillary power line
	
	_delay_ms(500);
	
	//###2###
	//SimpleFunction();
	
	// Interrupt initialiseren
	SREG = 0b10000000; // Zet Global Interrupt Enable (GIE) aan (pg.17).
	PMIC.CTRL = 0b00000111; // Bit 0 is om Low-level Interrupts aan te zetten, bit 1 voor Medium-level Interrupts, bit 2 voor High-level Interrupts, bit 3-5 zijn gereserveerd dus 0 en van bit 6-7 blijven we af (pg.118)
	TCC0.CTRLA = 0b00000111; // Laatste 4 bits zijn gereserveerd (zie pg. 154) en eerste 4 bits 0111 om prescaler te delen door 1024
	TCC0.CTRLB = 0b00000000; // Eerste 3 bits bepalen WaveForm Generation Mode dus 000 voor normal (pg.155), 4de bit is gereserveerd dus een 0 zetten en laatste 3 bits dien voor CCy in te schakelen, maar dit is enkel nodig bij single slope dus 0
	TCC0.CTRLD = 0b00000000; // Eerste 3 bits 0 want Event Action moet uitstaan (dia 58 en pg. 156), de andere bits zetten we ook gewoon uit.
	TCC0.INTCTRLA = 0b00000011; // Eerste 2 bits bepalen TimerOverFlow/UnderFlowInterruptLevel dus 11 voor High, volgende 2 bits bepalen TimerErrorInterruptLevel dus 00 voor uit te zetten en laatste 4 bits zijn gereserveerd dus ook 0 (pg.115 en 157)
	TCC0.PER = 15625; // We willen dat het om de 500ms interrupt

	while(1)
	{
		LoopLicht();
	}

	return 0;
}

void SimpleFunction(void)
{
	for (a=0;a<10;a++)
	{
		sprintf(text,"Counter:%d\r",a);
		puts(text);
	}
}


void LoopLicht(void)
{	
	while(1) {
		scanf("%s", RichtingString); // Waarde inlezen en in Richting zetten
		if (strcmp(RichtingString, "looplicht_links") == 0) {
			RichtingInt = 1;
		} else if (strcmp(RichtingString,  "looplicht_rechts") == 0 ) {
			RichtingInt = 2;
		} else {
			RichtingInt = 3;
		}
	}
}

// Als de timer overflowt wordt de interrupt geactiveerd
ISR(TCC0_OVF_vect) {
	DriverLedWrite(LedData); // LedData doorgeven om LED's aan te sturen
	if (RichtingInt == 1) {
		if (LedData == 0b1000) {
			LedData = 0b0001; // Laatste Led is bereikt dus we gaan terug naar de eerste
		} else {
			LedData = LedData<<1; // Naar de volgende LED gaan, door een 0 rechts in te voegen (links shiften)
		}
	} else if (RichtingInt == 2){
		if (LedData == 0b0001) {
			LedData = 0b1000; // Laatste Led is bereikt dus we gaan terug naar de eerste
		} else {
			LedData = LedData>>1; // Naar de volgende LED gaan, door een 0 links in te voegen (rechts shiften)
		} 
	} else { // Zo kan ik makkelijk zien of er een error is
		LedData = 0b1111;
	}
}
