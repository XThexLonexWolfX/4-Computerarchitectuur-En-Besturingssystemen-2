#include "transfertask.h"
#include "hwconfig.h"

#include "util/delay.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // Nodig voor semaphore


// Variables
volatile static uint32_t Data=0;
uint32_t Startwaarde; 

//Private function prototypes
void WorkerSendTask(void *pvParameters);
void WorkerReceiveTask(void *pvParameters);

//Function definitions
void InitTransferTasks()
{
	xTaskCreate( WorkerSendTask, "sendtask", 256, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( WorkerReceiveTask, "receivetask", 256, NULL, tskIDLE_PRIORITY+2, NULL );
}

void WorkerSendTask(void *pvParameters)
{	
	while (1)
	{	
		taskENTER_CRITICAL(); // Zetten interrupts uit
		Data=0x10101010;
		taskEXIT_CRITICAL(); // Interrupts terug aanzetten
		taskENTER_CRITICAL(); // Zetten interrupts uit
		Data=0x20202020;
		taskEXIT_CRITICAL(); // Interrupts terug aanzetten
	}
}

void WorkerReceiveTask(void *pvParameters)
{
	
	while(1)
	{
		Startwaarde = portGET_RUN_TIME_COUNTER_VALUE();
		taskENTER_CRITICAL(); // Zetten interrupts uit
		if (Data!=0x10101010 && Data!=0x20202020) printf ("Invalid Data value:%lx \r\n",Data);
		taskEXIT_CRITICAL(); // Interrupts terug aanzetten
		printf("Tijd nodig: %d\r", portGET_RUN_TIME_COUNTER_VALUE() - Startwaarde); // Tijd nodig voor dit gedeelte afprinten
		vTaskDelay(1);
	}
}