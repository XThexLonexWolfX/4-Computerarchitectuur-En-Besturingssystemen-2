#include "hwconfig.h"

#include "FreeRTOS.h"
#include "task.h"

#include "DriverSysClk.h"
#include "DriverUSART.h"
#include "DriverPower.h"

#include "transfertask.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

int main(void)
{
	DriverSysClkXtalInit();	//Clock init
	DriverUSARTInit();		//USART init and link to stdio
	DriverPowerInit();		//Initialize aux power driver
	DriverPowerVccAuxSet(1);//Enable Auxillary power line

	//Enable interrupts
	PMIC.CTRL=0b111;		
	sei();

	_delay_ms(10);

	//Init tasks
	InitTransferTasks();

	vTaskStartScheduler();	//Start scheduler loop

	return 0;
}


