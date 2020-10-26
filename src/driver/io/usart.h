/*
 * usart.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef DRIVER_IO_USART_H_
#define DRIVER_IO_USART_H_

#include "../../sys/types.h"

#define USART_BAUD			115200
#define USART_BUFFER_DMA	0x10
#define USART_BUFFER		0x40
#define USART_BRR			SystemCoreClock / USART_BAUD

void usart_init();

bool_e usart_write(uint8_t *buffer, size_t size);

size_t usart_read(uint8_t *buffer, size_t size);

size_t usart_pending();

#endif
