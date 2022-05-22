#include "freertos.h" // Nodig om queue te kunnen gebruiken
#include "queue.h" // Nodig om queue te kunnen gebruiken
#include "DriverUSART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "hwconfig.h"

static int stdio_putchar(char c, FILE * stream);
static int stdio_getchar(FILE *stream);
static FILE UsartStdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar,_FDEV_SETUP_RW);

// Variables
QueueHandle_t FifoReceive;
QueueHandle_t FifoTransmit;

void DriverUSARTInit(void)
{
	USART_PORT.DIRSET=0b00001000;	
	USART_PORT.DIRCLR=0b00000100;
	
	USART.CTRLA=0b00010100; // Pagina 259
	USART.CTRLB=0b00011000; // Pagina 260
	USART.CTRLC=0b00000011;	
	
	USART.BAUDCTRLA=0xE5; //BSEL=3301, BSCALE=-5 19200 baud
	USART.BAUDCTRLB=0xBC; 
	
	stdout=&UsartStdio;
	stdin=&UsartStdio;
	
	FifoReceive = xQueueCreate(10, sizeof(char)); // USART gebruikt 10 bits zie periferie dia 78
	FifoTransmit = xQueueCreate(10, sizeof(char));
}


static int stdio_putchar(char c, FILE * stream)
{
	xQueueSend(FifoTransmit, &c, portMAX_DELAY); // Databyte op FIFO zetten
	if (USART.STATUS & 0b00100000) { // Kijken of UART data aan het verzenden is
		xQueueReceive(FifoTransmit, &USART.DATA, portMAX_DELAY); // Databyte van FIFO afhalen en rechtsreeks naar USART DATA register schrijven
	}
}
	
static int stdio_getchar(FILE *stream)
{
	char Data;
	xQueueReceive(FifoReceive, &Data, portMAX_DELAY); // Data inlezen
	return Data; // Data teruggeven
}

ISR(USART_RXC_vect) { // Receive Interrupt Vector
	xQueueSendFromISR(FifoReceive, &USART.DATA, NULL); // Data doorsturen
}

ISR(USART_TXC_vect) {
	xQueueReceiveFromISR(FifoTransmit, &USART.DATA, NULL); // Databyte van FIFO afhalen en naar USART.DATA register schrijven
}