#include "transfertask.h"
#include "hwconfig.h"

#include "util/delay.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // Nodig voor semaphore


// Variables
static uint32_t Data=0; // Volatile weggehaald want dit zorgde voor problemen bij xQueueCreate()
static uint32_t Data2=0; // Nieuwe variable om queuepeek in te zetten
uint32_t Startwaarde; 
QueueHandle_t TransferTaskQueue; // Creer variabele voor de queue
//Private function prototypes
void WorkerSendTask(void *pvParameters);
void WorkerReceiveTask(void *pvParameters);

//Function definitions
void InitTransferTasks()
{
	xTaskCreate( WorkerSendTask, "sendtask", 256, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( WorkerReceiveTask, "receivetask", 256, NULL, tskIDLE_PRIORITY+2, NULL );
	TransferTaskQueue = xQueueCreate(1, sizeof(uint32_t)); // Creer de queue, er moet telkens maar 1 variabele in dus daarom lengte 1
}

void WorkerSendTask(void *pvParameters)
{	
	while (1)
	{	
		Data=0x10101010;
		xQueueOverwrite(TransferTaskQueue, &Data); // Data wegschrijven naar de queue
		Data=0x20202020;
		xQueueOverwrite(TransferTaskQueue, &Data); // Data wegschrijven naar de queue
	}
}

void WorkerReceiveTask(void *pvParameters)
{
	while(1)
	{
		Startwaarde = portGET_RUN_TIME_COUNTER_VALUE();
		xQueuePeek(TransferTaskQueue, &Data2, portMAX_DELAY); // Queuewaarde in Data2 steken
		if (Data2!=0x10101010 && Data2!=0x20202020) printf ("Invalid Data value:%lx \r\n",Data);
		printf("Tijd nodig: %d\r", portGET_RUN_TIME_COUNTER_VALUE() - Startwaarde); // Tijd nodig voor dit gedeelte afprinten
		vTaskDelay(1);
	}
}