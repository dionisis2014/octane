/*
 * systick.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef DRIVER_TIMER_SYSTICK_H_
#define DRIVER_TIMER_SYSTICK_H_

#include "../../sys/types.h"

#define SYSTICK_PERIOD		1 //milliseconds
#define SYSTICK_TICKS		(1000 / SYSTICK_PERIOD)

bool_e systick_init();

size_t millis();

void delay(size_t millis);

#endif
