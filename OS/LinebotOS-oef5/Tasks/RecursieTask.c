#include "RecursieTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h> // Voor printf

//Private function prototypes
static void WorkerRecursie(void *pvParameters);

//Function definitions
void InitRecursieTask()
{
	xTaskCreate( WorkerRecursie, "recursie", configMINIMAL_STACK_SIZE+512, NULL, tskIDLE_PRIORITY+2, NULL );	
}

static void WorkerRecursie(void *pvParameters)
{
	int LocaleVariabele;
	TaskStatus_t pxTaskStatus;
	TaskHandle_t xTask;
	while (1)
	{	
		LocaleVariabele = 1;
		xTask = xTaskGetCurrentTaskHandle(); // Pointer naar begin TCB
		vTaskGetTaskInfo(xTask, &pxTaskStatus, pdTRUE, eInvalid);
		printf("Hoeveelheid vrije heap: %d pointer variabele: %d pointer TCB: %d \r", pxTaskStatus.usStackHighWaterMark, &LocaleVariabele, xTask);
		WorkerRecursie(pvParameters); // Functie terug oproepen om erin te blijven
	}

}
