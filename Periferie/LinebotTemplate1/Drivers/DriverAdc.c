#include "DriverAdc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void DriverAdcInit(void)
{
	// Pg.290 en 299
	ADCA.CTRLA= 0b00000111 // Bit 0 enabled ADC, bit 1 flushed de ADC, bit 2 start een ADC conversie en laatste 5 bits zijn gereserveerd dus 0.
	ADCA.CTRLB= 0b00010110 // Bit 0 is gereserveerd dus 0, bit 1 en 2 bepalen de resolutie (12 voor het grootste bereik ppt), bit 3 bepaalt free run mode, bit 4 de conversie modus (signed voor grootste bereik ppt), bit 5-6 00 voor geen limiet en bit 7 is gereserveerd
	ADCA.REFCTRL= 0b00000010 // Bit 0 op 0 want geen temperatuur sensor, bit 1 enables bandgap, bit 2-3 zijn gereserveerd dus 0, bit 4-6 bepalen de referentie dus 000 (bandgap) en bit 7 is gereserveerd dus 0
	ADCA.PRESCALER= 0b00000011 // Bit 0-2 bepaalt de clock van ADC mag max 1,8MHz zijn dus we delen door 32 om hier onder te zitten en toch een redelijk grote frequentie te hebben, de andere bits zijn gereserveerd dus 0
	ADCA.CH0.INTCTRL= 0b00000000 // Bit 0-1 bepaalt interrupt niveau dus 00 voor off, bit 2-3 de interrupt mode 00 voor complete en bit 4-7 zijn gereserveerd dus 0
}

int16_t DriverAdcGetCh(uint8_t PinPos,uint8_t PinNeg)
{

}

