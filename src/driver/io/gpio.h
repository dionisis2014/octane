/*
 * gpio.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef DRIVER_IO_GPIO_H_
#define DRIVER_IO_GPIO_H_

#include "../../sys/types.h"

typedef enum {
	GPIO_A0, GPIO_A1, GPIO_A2, GPIO_A3, GPIO_A4, GPIO_A5, GPIO_A6, GPIO_A7, GPIO_A8, GPIO_A9, GPIO_A10, GPIO_A11, GPIO_A12, GPIO_A13, GPIO_A14, GPIO_A15,
	GPIO_B0, GPIO_B1, GPIO_B2, GPIO_B3, GPIO_B4, GPIO_B5, GPIO_B6, GPIO_B7, GPIO_B8, GPIO_B9, GPIO_B10, GPIO_B11, GPIO_B12, GPIO_B13, GPIO_B14, GPIO_B15
} gpio_e;

typedef enum {
	GPIO_MODE_IN,
	GPIO_MODE_OUT_10MHZ,
	GPIO_MODE_OUT_2MHZ,
	GPIO_MODE_OUT_50MHZ
} gpio_mode_e;

typedef enum {
	GPIO_TYPE_IN_ANALOG,
	GPIO_TYPE_IN_FLOATING,
	GPIO_TYPE_IN_PULL,
	GPIO_TYPE_OUT_PUSHPULL = 0,
	GPIO_TYPE_OUT_OPENDRAIN,
	GPIO_TYPE_OUT_PUSHPULL_ALT,
	GPIO_TYPE_OUT_OPENDRAIN_ALT
} gpio_type_e;

void gpio_mode(gpio_e gpio, gpio_mode_e mode, gpio_type_e type);

void gpio_set(gpio_e gpio, bool_e state);

#endif
