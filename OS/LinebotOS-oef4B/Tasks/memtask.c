#include "memtask.h"
#include "hwconfig.h"

#include "util/delay.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//Private function prototypes
void WorkerMemTask(void *pvParameters);
void MemFunction();

//Function definitions
void InitMemTask()
{
	xTaskCreate( WorkerMemTask, "mem", 1024, NULL, tskIDLE_PRIORITY+1, NULL );
}

void WorkerMemTask(void *pvParameters)
{	
	while (1)
	{
		MemFunction();
		vTaskDelay(100);
	}
}

void MemFunction()
{
	int *num;
	int a;
	
	num=pvPortMalloc(sizeof(int) * 100);
	
	for (a=0;a<100;a++)
	{	
		num[a]=a;
	}
	printf("Resterende heap: %d\r", xPortGetFreeHeapSize()); // Deze lijn heb ik er zelf bijgezet om de oorzaak van het probleem te kunnen vinden
	printf ("Complete!\r\n");
	vPortFree(num); // Dit is de oplossing van het probleem, na elke keer dat de for loop doorlopen is het geheugen dat num alloceert terug leeg maken
}
