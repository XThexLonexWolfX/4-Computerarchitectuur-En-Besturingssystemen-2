#include "DriverLed.h"

void DriverLedInit(void)
{
	PORTB.DIRSET=0b00000111; // PB0-2 als uitgang zetten
	// Logica omdraaien zodat een 1 sturen naar de LED's ze laat branden ipv een 0
	// Zie pagina 133
	PORTB.PIN0CTRL=0b01000000;
	PORTB.PIN1CTRL=0b01000000;
	PORTB.PIN2CTRL=0b01000000;
	
	PORTA.DIRSET=0b10000000; // PA7 als uitgang zetten
	// Logica omdraaien zodat een 1 sturen naar de LED's ze laat branden ipv een 0
	// Zie pagina 133
	PORTA.PIN7CTRL=0b01000000;
}

void DriverLedWrite(uint8_t LedData)
{	
	PORTB.OUT = LedData & 0b00000111;	// Alleen de de waarden van PB0-2 overhouden
	LedData = LedData<<4; // 4 nullen langs rechts inschuiven om de 4de bit op plaats 8 te krijgen voor PA7
	PORTA.OUT = LedData & 0b10000000; // Alleen waarde van PA7 overhouden
}

void DriverLedSet(uint8_t LedData)
{

}

void DriverLedClear(uint8_t LedData)
{

}

void DriverLedToggle(uint8_t LedData)
{

}