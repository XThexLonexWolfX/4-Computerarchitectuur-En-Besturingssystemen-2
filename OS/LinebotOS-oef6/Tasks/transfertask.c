#include "transfertask.h"
#include "hwconfig.h"

#include "util/delay.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // Nodig voor semaphore


// Variables
volatile static uint32_t Data=0;
SemaphoreHandle_t TransferTaskMutex;
uint32_t Startwaarde; 

//Private function prototypes
void WorkerSendTask(void *pvParameters);
void WorkerReceiveTask(void *pvParameters);

//Function definitions
void InitTransferTasks()
{
	xTaskCreate( WorkerSendTask, "sendtask", 256, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( WorkerReceiveTask, "receivetask", 256, NULL, tskIDLE_PRIORITY+2, NULL );
	TransferTaskMutex = xSemaphoreCreateMutex(); // Creer mutex
}

void WorkerSendTask(void *pvParameters)
{	
	while (1)
	{	
		xSemaphoreTake(TransferTaskMutex, portMAX_DELAY); // Nemen de mutex voor onbepaalde duur portMAX_DELAY = oneindig
		Data=0x10101010;
		xSemaphoreGive(TransferTaskMutex); // Geven de mutex terug
		xSemaphoreTake(TransferTaskMutex, portMAX_DELAY); // Nemen de mutex voor onbepaalde duur portMAX_DELAY = oneindig
		Data=0x20202020;
		xSemaphoreGive(TransferTaskMutex); // Geven de mutex terug
	}
}

void WorkerReceiveTask(void *pvParameters)
{
	
	while(1)
	{
		Startwaarde = portGET_RUN_TIME_COUNTER_VALUE();
		xSemaphoreTake(TransferTaskMutex, portMAX_DELAY); // Nemen de mutex voor onbepaalde duur portMAX_DELAY = oneindig
		if (Data!=0x10101010 && Data!=0x20202020) printf ("Invalid Data value:%lx \r\n",Data);
		xSemaphoreGive(TransferTaskMutex); // Geven de mutex terug
		printf("Tijd nodig: %d\r", portGET_RUN_TIME_COUNTER_VALUE() - Startwaarde); // Tijd nodig voor dit gedeelte afprinten
		vTaskDelay(1);
	}
}