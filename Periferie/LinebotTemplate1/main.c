#include "hwconfig.h"

#include "DriverSysClk.h"
#include "DriverUSART.h"
#include "DriverCursorStick.h"
#include "DriverPower.h"
#include "DriverTwiMaster.h"
#include "DriverAdc.h"
#include "DriverLed.h"
#include "DriverMotor.h" // Deze moet hier staan om EncodrStruct te kunnen oproepen

#include <util/delay.h>

#include <stdio.h>

// Joystick Constants
static uint8_t Center=0b00000001; // static ervoor gezet anders geeft hij problemen door dat ik een variabele met dezelfde naam heb in DriverCursorstick.c
static uint8_t Right=0b00000010;
static uint8_t Down=0b00000100;
static uint8_t Left=0b00001000;
static uint8_t Up=0b00010000;
// Motor Constants
EncoderStruct EncoderVariable;

char text[11]; // Naar 11 gezet om het probleem bij SimpleFunction op te lossen
char a, b; // b is voor LoopLicht functie for loop
uint8_t LedData = 0b0001; // LedData initialiseren met eerste LED die brand

// Functions
void SimpleFunction(void);	//A simple function: print a counter (0 to 9) to the terminal
void LoopLicht(void); // Start een looplicht
void Joystick(void); // Leest de joystick waarde uit en print de richting
void Test_Motor(void); // Leest een motorsnelheid waarde in en laat de motors aan deze snelheid draaien
void Test_Encoder(void); // Print Encoder waardes af
void Test_USART(void);

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
		
	while(1)
	{
		//LoopLicht();
		//Joystick();
		//Test_Encoder();
		//Test_Motor();
		//Test_USART(); // Om deze snel genoeg achter elkaar te sturen kan je best de andere functies uitzetten, want deze zorgen voor een grote delay
	}

	return 0;
}

void SimpleFunction(void)
{
	for (a=0;a<10;a++)
	{
		sprintf(text,"Counter:%d\r",a); // De variabele staat op grootte 10, maar Counter:%d\r neemt al 10 plaatsen (%d telt voor 1 plek net zoals \r) waardoor uw einde \0 van een string op de volgende lijn komt, dus eigenlijk op de plaats van a waardoor a gelijk wordt aan 0, dit kunnen we oplossen door de grootte van de variabele aan te passen (C declareert het einde van een string met \0)
		puts(text);
	}
}

void LoopLicht(void)
{	
	for (b=0;b<4;b++)
	{
		DriverLedWrite(LedData); // LedData doorgeven om LED's aan te sturen
		LedData = LedData<<1; // Naar de volgende LED gaan, door een 0 rechts in te voegen (links shiften)
		_delay_ms(500); // Delay aanmaken anders zie je de LED's niet wisselen
	}
	LedData = 0b0001; // LedData terug resetten voor de volgende keer
}

void Joystick(void)
{
	uint8_t JoystickOutput = DriverCursorstickGet();
	if (JoystickOutput == Center) {
		printf("Center\r");
	} else if (JoystickOutput == Left) {
		printf("Left\r");
	} else if (JoystickOutput == Down) {
		printf("Down\r");
	} else if (JoystickOutput == Right) {
		printf("Right\r");
	} else if (JoystickOutput == Up) {
		printf("Up\r");
	}
}

void Test_Motor(void) {
	uint16_t LinkerSnelheid = 0;
	uint16_t RechterSnelheid= 0;
	printf("Geef snelheid LinkerMotor: \r"); // Snelheid linker motor wordt gevraagd
	scanf("%d", &LinkerSnelheid); // Ingevoerde waarde in LinkerSnelheid variabele steken
	printf("Je hebt snelheid %d gegeven.\r", LinkerSnelheid);
	printf("Geef snelheid RechterMotor: \r"); // Snelheid rechter motor wordt gevraagd
	scanf("%d", &RechterSnelheid); // Ingevoerde waarde in RechterSnelheid variabele steken
	printf("Je hebt snelheid %d gegeven.\r", RechterSnelheid);
	DriverMotorSet(LinkerSnelheid, RechterSnelheid); // Waarden doorgeven aan de motors
}

void Test_Encoder(void) {
	EncoderVariable = DriverMotorGetEncoder();
	printf("Encoder1: %d\r", EncoderVariable.Cnt1);
	printf("Encoder2: %d\r", EncoderVariable.Cnt2);		
}

void Test_USART(void) {
	_delay_ms(1);
	putchar(0x55);	
}