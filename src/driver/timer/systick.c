/*
 * systick.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#include "systick.h"
#include "../../sys/macros.h"
#include "../../sys/isr_conf.h"

static volatile size_t milliseconds = 0;

void SysTick_Handler() {
	milliseconds += SYSTICK_PERIOD;
}

bool_e systick_init() {
	ONETIMER(systick)

	NVIC_SetPriority(SysTick_IRQn, ISR_PRIORITY_SYSTICK);
	return SysTick_Config(SystemCoreClock / SYSTICK_TICKS) ? BOOL_FALSE : BOOL_TRUE;
}

size_t millis() {
	return milliseconds;
}

void delay(size_t millis) {
	millis += milliseconds;
	while (milliseconds <= millis)
		;
}
