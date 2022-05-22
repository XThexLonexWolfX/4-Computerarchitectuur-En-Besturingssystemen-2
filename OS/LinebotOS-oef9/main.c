#include "hwconfig.h"

#include "FreeRTOS.h"
#include "task.h"

#include "DriverSysClk.h"
#include "DriverUSART.h"
#include "DriverCursorStick.h"
#include "DriverPower.h"
#include "DriverTwiMaster.h"
#include "DriverAdc.h"
#include "DriverLed.h"
#include "DriverMPU6050.h"
#include "LooplichtTask.h" // Header Looplicht initialiseren
#include "TerminalTask.h" // Header Terminal initialiseren
#include "RGBLEDTask.h" // Header RGBLED initialiseren
#include "Driverpl9823.h" // Nodig om RGB LEDs aan te kunnen sturen

#include <util/delay.h>

#include <stdio.h>

int main(void)
{
	DriverSysClkXtalInit();	//Clock init
	DriverUSARTInit();		//USART init and link to stdio
	DriverTWIMInit();		//Initialize TWI in master mode
	DriverCursorstickInit();//Initialize cursor stick
	DriverLedInit();		//Initialize LED's
	DriverPowerInit();		//Initialize aux power driver
	DriverAdcInit();		//Initialize ADC driver
	DriverPowerVccAuxSet(1);//Enable Auxillary power line
	DriverPL9823Init(); // Nodig om RGB LEDs aan te kunnen sturen
	//Enable interrupts
	PMIC.CTRL=0b111;		
	sei();

	_delay_ms(10);

	//Init tasks
	InitLooplichtTask(); // Looplicht task oproepen
	InitTerminalTask(); // Terminal task oproepen
	InitRGBLEDTask(); // RGBLED task oproepen
	vTaskStartScheduler();	//Start scheduler loop

	return 0;
}


