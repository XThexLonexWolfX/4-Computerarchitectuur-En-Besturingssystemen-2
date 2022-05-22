#include "FifoTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "DriverCursorStick.h" // Nodig om cursorstick uit te kunnen lezen

//Variables
QueueHandle_t FifoTaskQueue;
TickType_t xTimeIncrement2 = pdMS_TO_TICKS(100); // Frequentie waarmee de taak loopt omgezet in ticks, een 2 achtergzet omdat anders het dezelfde naam heeft als de variabele in LooplichtTask.c
TickType_t pxPreviousWakeTime;
static uint8_t VorigeWaarde=0b00000000;
uint8_t IngelezenWaarde;
uint8_t GetterWaarde;

//Private function prototypes
static void WorkerFifo(void *pvParameters);

//Function definitions
void InitFifoTask()
{
	xTaskCreate( WorkerFifo, "fifo", 256, NULL, tskIDLE_PRIORITY+1, NULL );
	FifoTaskQueue = xQueueCreate(5, sizeof(uint8_t)); // Creer FIFO met lengte 5
}

static void WorkerFifo(void *pvParameters)
{
	while (1)
	{
		vTaskDelayUntil(&pxPreviousWakeTime, xTimeIncrement2); // Pointer nodig van pxPreviousWakeTime want vTaskDelayUntil moet deze variabele kunnen aanpassen
		IngelezenWaarde = DriverCursorstickGet(); // Cursorstickwaarde inlezen
		if (VorigeWaarde != IngelezenWaarde) { // Actie detecteren door te zien of de waarde verandert
			xQueueSend(FifoTaskQueue, &IngelezenWaarde, portMAX_DELAY); // Actie wegschrijven naar de FIFO queue
		}
		VorigeWaarde = IngelezenWaarde;
	}
}

uint8_t getter() {
	if (xQueueReceive(FifoTaskQueue, &GetterWaarde, 0) == pdPASS) { // Waarde van FIFO queue inlezen en zien of er effectief data is uitgelezen
		return GetterWaarde;
	}
}
