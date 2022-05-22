#include "DriverMotor.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

volatile uint16_t Encoder1 = 0; // Volatile laat de compiler weten dat de variabele elk moment kan veranderen, zonder een taak van de source code
volatile uint16_t Encoder2 = 0;

void DriverMotorInit(void)
{
	//GPIO init
	// Pinnen als output schakelen
	PORTF.DIRSET = 0b00111111; // Eerste 4 bits voor CTRL pinnen (PF0-3), 5de bit voor Sleep pin (PF4) en 6de bit voor Fault pin (PF5)
	PORTF.OUT = 0b00010000; //PF4 sleep pin, 1 betekent een 0 doorsturen, dus dat de motor niet in slaaptoestand zit
	//Timer init, hbridge
	TCF0.CTRLA = 0b00000001; // Laatste 4 bits zijn gereserveerd (zie pg. 154) en eerste 4 bits 0001 om prescaler te delen door 1, voor de hoogst mogelijke frequentie, anders draait motor niet bij lagere getallen
	TCF0.CTRLB = 0b11110011; // Eerste 3 bits bepalen Waveform Generation Mode dus 011 voor single slope (pg.155), 4de bit is gereserveerd dus een 0 zetten (pg. 154) en laatste 3 bits moeten op 1 staan om CCy kanalen in te schakelen (pg.154 en dia 58).
	TCF0.CTRLD = 0b00000000; // Eerste 3 bits 0 want Event Action moet uitstaan (dia 58 en pg. 156), de andere bits zetten we ook gewoon uit.
	TCF0.PER = 4095; // Periode (zie parameters DriverMotorSet)
	TCF0.CCA = 0; // Initiele waarde
	TCF0.CCB = 0; // Initiele waarde
	TCF0.CCC = 0; // Initiele waarde
	TCF0.CCD = 0; // Initiele waarde
	// Enable Interrupts
	SREG = 0b10000000; // Zet Global Interrupt Enable (GIE) aan (pg.17).
	PMIC.CTRL = 0b00000111; // Bit 0 is om Low-level Interrupts aan te zetten, bit 1 voor Medium-level Interrupts, bit 2 voor High-level Interrupts, bit 3-5 zijn gereserveerd dus 0 en van bit 6-7 blijven we af (pg.118)
	//Encoder 1
	PORTC.DIRSET = 0b11000000; // PC6 en PC7 als uitgang zetten, heb hier DIRSET ipv DIR gebruikt, want deze verandert alleen de bits waar een 1 op komt te staan en DIR verandert alles waardoor ineens men LEDS niet meer werkten
	PORTC.INTCTRL = 0b00001111; // Eerste 4 bits tonen het interrupt level aan (11 staat voor High-level interrupt pg.115), laatste 4 bits zijn gereserveerd dus moeten op 0 (pg.131)
	PORTC.INT0MASK = 0b01000000; // Duid aan welke poort er voor deze interrupt gebruikt wordt, hier de 6de bit dus PC6
	PORTC.INT1MASK = 0b10000000; // Duid aan welke poort er voor deze interrupt gebruikt wordt, hier de 7de bit dus PC7
	PORTC.PIN6CTRL = 0b00000000; // Eerste 3 bits duiden aan op welke edge de interrupt sensed (000, pg.134), de volgende 3 bits duiden de output/pull configuration aan hier dus gewoon TOTEM (000, pg.133), bit 6 is voor te inverteren, maar dit willen we niet dus nemen we o (pg.133) en de laatste bit is gereserveerd dus ook 0 (pg.133).
	PORTC.PIN7CTRL = 0b00000000; // Eerste 3 bits duiden aan op welke edge de interrupt sensed (000, pg.134), de volgende 3 bits duiden de output/pull configuration aan hier dus gewoon TOTEM (000, pg.133), bit 6 is voor te inverteren, maar dit willen we niet dus nemen we o (pg.133) en de laatste bit is gereserveerd dus ook 0 (pg.133).
	//Encoder 2
	
	PORTE.DIRSET = 0b00110000; // PE4 en PE5 als uitgang zetten, heb hier DIRSET ipv DIR gebruikt, want deze verandert alleen de bits waar een 1 op komt te staan en DIR verandert alles waardoor ineens men printf niet meer werkte
	PORTE.INTCTRL = 0b00001111; // Eerste 4 bits tonen het interrupt level aan (11 staat voor High-level interrupt pg.115), laatste 4 bits zijn gereserveerd dus moeten op 0 (pg.131)
	PORTE.INT0MASK = 0b00010000; // Duid aan welke poort er voor deze interrupt gebruikt wordt, hier de 4de bit dus PE4
	PORTE.INT1MASK = 0b00100000; // Duid aan welke poort er voor deze interrupt gebruikt wordt, hier de 5de bit dus PE5
	PORTE.PIN4CTRL = 0b00000000; // Eerste 3 bits duiden aan op welke edge de interrupt sensed (000, pg.134), de volgende 3 bits duiden de output/pull configuration aan hier dus gewoon TOTEM (000, pg.133), bit 6 is voor te inverteren, maar dit willen we niet dus nemen we o (pg.133) en de laatste bit is gereserveerd dus ook 0 (pg.133).
	PORTE.PIN5CTRL = 0b00000000; // Eerste 3 bits duiden aan op welke edge de interrupt sensed (000, pg.134), de volgende 3 bits duiden de output/pull configuration aan hier dus gewoon TOTEM (000, pg.133), bit 6 is voor te inverteren, maar dit willen we niet dus nemen we o (pg.133) en de laatste bit is gereserveerd dus ook 0 (pg.133).
	
}

ISR(PORTC_INT0_vect) { // Deze wordt opgeroepen als PC6/ENC1A verandert
	if ((PORTC.IN & 0b01000000) == 0) { // Door de AND functie kunnen we zien of bit6/ENC1A/PC6 op 0 staat
		if ((PORTC.IN & 0b10000000) == 0) { // Door de AND functie kunnen we zien of bit7/ENC1B/PC7 op 0 staat
			// Als ENC1A op 0 staat en ENC1B op 0 moeten we aftrekken
			Encoder1--;
		} 
		// Als ENC1A op 0 staat en ENC1B op 1 moeten we optellen
		else {
			Encoder1++;
		}
	}
	else {
		if ((PORTC.IN & 0b10000000) == 0) { // Door de AND functie kunnen we zien of bit7/ENC1B/PC7 op 0 staat
			// Als ENC1A op 1 staat en ENC1B op 0 moeten we optellen
			Encoder1++;	
		}
		// Als ENC1A op 1 staat en ENC1B op 1 moeten we aftrekken
		else {
			Encoder1--;	
		}
	}
}

ISR(PORTC_INT1_vect) { // Deze wordt opgeroepen als PC7/ENC1B verandert
	if ((PORTC.IN & 0b10000000) == 0) { // Door de AND functie kunnen we zien of bit7/ENC1B/PC7 op 0 staat
		if ((PORTC.IN & 0b01000000) == 0) { // Door de AND functie kunnen we zien of bit6/ENC1A/PC6 op 0 staat
			// Als ENC1A op 0 staat en ENC1B op 0 moeten we optellen
			Encoder1++;
		} 
		// Als ENC1A op 0 staat en ENC1B op 1 moeten we aftrekken
		else {
			Encoder1--;
		}
	}
	else {
		if ((PORTC.IN & 0b10000000) == 0) { // Door de AND functie kunnen we zien of bit7/ENC1B/PC7 op 0 staat
			// Als ENC1A op 1 staat en ENC1B op 0 moeten we aftrekken
			Encoder1--;	
		}
		// Als ENC1A op 1 staat en ENC1B op 1 moeten we optellen
		else {
			Encoder1++;	
		}
	}
}

ISR(PORTE_INT0_vect) { // Deze wordt opgeroepen als PE4/ENC2A verandert
	if ((PORTE.IN & 0b00010000) == 0) { // Door de AND functie kunnen we zien of bit4/ENC2A/PE4 op 0 staat
		if ((PORTE.IN & 0b00100000) == 0) { // Door de AND functie kunnen we zien of bit4/ENC2B/PE5 op 0 staat
			// Als ENC2A op 0 staat en ENC2B op 0 moeten we aftrekken
			Encoder1--;
		} 
		// Als ENC2A op 0 staat en ENC2B op 1 moeten we optellen
		else {
			Encoder1++;
		}
	}
	else {
		if ((PORTE.IN & 0b0010000) == 0) { // Door de AND functie kunnen we zien of bit5/ENC2B/PE5 op 0 staat
			// Als ENC2A op 1 staat en ENC2B op 0 moeten we optellen
			Encoder1++;	
		}
		// Als ENC2A op 1 staat en ENC2B op 1 moeten we aftrekken
		else {
			Encoder1--;	
		}
	}
}

ISR(PORTE_INT1_vect) { // Deze wordt opgeroepen als PE5/ENC2B verandert
	if ((PORTE.IN & 0b00100000) == 0) { // Door de AND functie kunnen we zien of bit5/ENC2B/PE5 op 0 staat
		if ((PORTE.IN & 0b00010000) == 0) { // Door de AND functie kunnen we zien of bit4/ENC2A/PE4 op 0 staat
			// Als ENC2A op 0 staat en ENC2B op 0 moeten we optellen
			Encoder1++;
		} 
		// Als ENC2A op 0 staat en ENC2B op 1 moeten we aftrekken
		else {
			Encoder1--;
		}
	}
	else {
		if ((PORTE.IN & 0b00010000) == 0) { // Door de AND functie kunnen we zien of bit4/ENC2B/PE4 op 0 staat
			// Als ENC2A op 1 staat en ENC2B op 0 moeten we aftrekken
			Encoder1--;	
		}
		// Als ENC2A op 1 staat en ENC2B op 1 moeten we optellen
		else {
			Encoder1++;	
		}
	}
}

void DriverMotorSet(int16_t MotorLeft,int16_t MotorRight)
{	
	// Positief is tegenwijzerzin (voorwaarts), negatief is wijzerzin (achterwaarts), als je kijkt naar de rechtermotor, linkermotor is omgekeerd
	if (MotorRight >= 0) { // Tegenwijzerzin
		TCF0.CCA = MotorRight; // Dutycycle voor MOTOR1-CTRL1
		TCF0.CCB = 0; // Dutycycle voor MOTOR12-CTRL2
	} else if (MotorRight < 0) { // Tegenwijzerzin
		TCF0.CCA = 0; // Dutycycle voor MOTOR1-CTRL1
		TCF0.CCB = -MotorRight; // Dutycycle voor MOTOR12-CTRL2 en als de waarde negatief is moeten we die wel positief maken voor de dutycycle
	}
	// RechterMotor moet omgekeerd tov LinkerMotor, anders gaat de robot draaien ipv in een rechte lijn rijden
	if (MotorLeft >= 0) { // Tegenwijzerzin
		TCF0.CCC = 0; // Dutycycle voor MOTOR2-CTRL1
		TCF0.CCD = MotorLeft; // DutyCycle voor MOTOR2-CTRL2
	} else if (MotorLeft < 0) { // Wijzerzin
		TCF0.CCC = -MotorLeft; // Dutycycle voor MOTOR2-CTRL1 en als de waarde negatief is moeten we die wel positief maken voor de dutycycle
		TCF0.CCD = 0; // DutyCycle voor MOTOR2-CTRL2
	}
}

EncoderStruct DriverMotorGetEncoder(void)
{
	EncoderStruct Encoder; // Encoder verwijst naar de struct waarin het benoemd wordt
	Encoder.Cnt1 = Encoder1; // Variabele doorgeven
	Encoder.Cnt2 = Encoder2; // Variabele doorgeven
	return Encoder;
}

