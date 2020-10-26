/*
 * gpio.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#include "gpio.h"

void gpio_mode(gpio_e gpio, gpio_mode_e mode, gpio_type_e type) {
	GPIO_TypeDef *port;
	if (gpio < GPIO_B0) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		port = GPIOA;
	} else {
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		port = GPIOB;
		gpio %= 16;
	}
	size_t offset;
	if (gpio < 8) {
		offset = gpio << 2;
		port->CRL &= ~(0xF << offset);
		port->CRL |= ((type << 2) | mode) << offset;
	} else {
		gpio -= 8;
		offset = gpio << 2;
		port->CRH &= ~(0xF << offset);
		port->CRH |= ((type << 2) | mode) << offset;
	}
}

void gpio_set(gpio_e gpio, bool_e state) {
	GPIO_TypeDef *port;
	if (gpio < GPIO_B0)
		port = GPIOA;
	else
		port = GPIOB;
	gpio %= 16;
	if (state)
		port->ODR |= 1 << gpio;
	else
		port->BRR |= 1 << gpio;
}
