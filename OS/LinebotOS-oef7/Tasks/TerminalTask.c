#include "TerminalTask.h"
#include "LooplichtTask.h" // Deze aanroepen om variabelen te declareren
#include "FifoTask.h" // Nodig voor de getter functie te kunnen aanroepen
#include <string.h> // Nodig om strcmp te gebruiken
#include <stdio.h> // Nodig om scanf te gebruiken 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Variables
static char Buffer[512]; // Buffer van 512 bytes groot

//Private function prototypes
static void WorkerTerminal(void *pvParameters);

//Function definitions
void InitTerminalTask()
{
	xTaskCreate( WorkerTerminal, "terminal", configMINIMAL_STACK_SIZE+512 , NULL, tskIDLE_PRIORITY+1, NULL );	
}

static void WorkerTerminal(void *pvParameters)
{
	while (1)
	{	
		scanf("%s", RichtingString); // Waarde inlezen en in Richting zetten
		vTaskGetRunTimeStats(Buffer); // Overzicht van alle tasks en de toegekende processortijd verkrijgen		
		if (strcmp(RichtingString, "looplicht_links") == 0) {
			RichtingInt = 1;
		} else if (strcmp(RichtingString,  "looplicht_rechts") == 0 ) {
			RichtingInt = 2;
		} else if (strcmp(RichtingString, "fifo") == 0) {
			printf("FIFO queue: %d\r", getter()); // FIFO queue afprinten
		} else {
			RichtingInt = 3;
		}
		//puts(Buffer); // Buffer afprinten
	}
}
