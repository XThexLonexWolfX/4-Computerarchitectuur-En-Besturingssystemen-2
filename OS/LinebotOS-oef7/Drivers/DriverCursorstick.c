#include "DriverCursorstick.h"

// Constants
static uint8_t Center=0b10000000;
static uint8_t Right=0b01000000;
static uint8_t Down=0b00100000;
static uint8_t Left=0b00010000;
static uint8_t Up=0b00001000;
static uint8_t CursorstickOutput=0b00000000;

void DriverCursorstickInit(void)
{
	// Pagina 129
	PORTB.DIRCLR=0b00000111; // Pin 2-7 als ingang zetten
	// Als pullup instellen zie pagina 133 en inverteren, zodat als de switch wordt gebruikt er een 1 komt ipv 0
	PORTB.PIN3CTRL=0b01011000;
	PORTB.PIN4CTRL=0b01011000;
	PORTB.PIN5CTRL=0b01011000;
	PORTB.PIN6CTRL=0b01011000;
	PORTB.PIN7CTRL=0b01011000;
}

uint8_t DriverCursorstickGet(void)
{	
	uint8_t Opslag = PORTB.IN & 0b11111000;// We willen enkel de cursorstickwaarden en niet degene van de LED's
	if (Opslag == Center) {
		CursorstickOutput = 0b00000001;
	} else if (Opslag == Right) {
		CursorstickOutput = 0b00000010;
	} else if (Opslag == Down) {
		CursorstickOutput = 0b00000100;
	} else if (Opslag == Left) {
		CursorstickOutput = 0b00001000;
	} else if (Opslag == Up) {
		CursorstickOutput = 0b00010000;
	} 
	return CursorstickOutput;
}