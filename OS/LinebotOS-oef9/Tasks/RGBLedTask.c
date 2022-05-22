#include "FreeRTOS.h"
#include "RGBLEDTask.h"
#include "queue.h"
#include "Driverpl9823.h" // Nodig voor RGBLEDs

//Private function prototypes
static void WorkerRGBLED(void *pvParameters);

//Function definitions
void InitRGBLEDTask()
{
	xTaskCreate( WorkerRGBLED, "rgbled", configMINIMAL_STACK_SIZE+512 , NULL, tskIDLE_PRIORITY+1, NULL );	
}

static void WorkerRGBLED(void *pvParameters)
{
	while (1)
	{
		taskENTER_CRITICAL(); // Code atomair uitvoeren
		DriverPL9823Set(PL9823_RED, PL9823_GREEN, PL9823_BLUE, PL9823_WHITE); // Elke LED een kleur geven
		taskEXIT_CRITICAL();
	}
}