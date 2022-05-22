#include "LooplichtTask.h"

#include "hwconfig.h" // Nodig voor _delay_ms() te kunnen gebruiken
#include <util/delay.h> // Nodig voor _delay_ms() te kunnen gebruiken
#include <stdio.h> 
#include <stdlib.h> // Nodig voor abs()

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "DriverLed.h"

// Variables
char RichtingString[] = "looplicht_links"; // Basisrichting is links
int RichtingInt = 1; // Richting als links initialiseren
uint8_t LedData = 0b0001; // LedData initialiseren met eerste LED die brand
TickType_t xTimeIncrement = pdMS_TO_TICKS(100); // Frequentie waarmee de taak loopt omgezet in ticks
TickType_t pxPreviousWakeTime;
uint32_t t_nu;
uint32_t t_vorige;
uint32_t t_interval = (32000000/1000)*100; // Delay omzetten naar processorcycli
uint32_t jiter;
uint32_t max_jiter = 0;
uint32_t teller; // Teller om delay te maken voor max_jiter

//Private function prototypes
static void WorkerLooplicht(void *pvParameters);

//Function definitions
void InitLooplichtTask()
{
	xTaskCreate( WorkerLooplicht, "looplicht", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );	
}

static void WorkerLooplicht(void *pvParameters)
{
	while (1)
	{
		//_delay_ms(500);
		t_vorige = t_nu;
		t_nu = portGET_RUN_TIME_COUNTER_VALUE();
		jiter = (t_nu - t_vorige) - t_interval; // jiter berekenen
		//printf("t_vorige: %ld, t_nu: %ld, t_interval: %ld\r", t_vorige, t_nu, t_interval); Dit was voor te testen
		if (abs(jiter) > abs(max_jiter) && teller > 2) {
			max_jiter = abs(jiter);
		} else {
			teller ++;
		}
		printf("Jiter: %ld, max jiter: %ld\r", jiter, max_jiter); //ld want jitter kan ook negatief zijn
		
		vTaskDelayUntil(&pxPreviousWakeTime, xTimeIncrement); // Pointer nodig van pxPreviousWakeTime want vTaskDelayUntil moet deze variabele kunnen aanpassen
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

}
