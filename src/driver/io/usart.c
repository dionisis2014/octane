/*
 * usart.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#include "usart.h"
#include "../../sys/macros.h"
#include "gpio.h"
#include "../../sys/isr_conf.h"
#include "../../sys/mem/buffer.h"
#include "../dma/dma.h"

static uint8_t usart_buffer_dma[USART_BUFFER_DMA];
static uint8_t usart_buffer[USART_BUFFER];
static buffer_circ_s usart_buffer_circ;

void usart_buffer_read() {
	static size_t index = 0, index_old = 0;

	index = USART_BUFFER_DMA - DMA1_Channel5->CNDTR;
	if (index != index_old) {
		if (index > index_old) {
			buffer_circ_push(&usart_buffer_circ, usart_buffer_dma + index_old, index - index_old);
		} else {
			buffer_circ_push(&usart_buffer_circ, usart_buffer_dma + index_old, USART_BUFFER_DMA - index_old);
			if (index > 0) {
				buffer_circ_push(&usart_buffer_circ, usart_buffer_dma, index);
			}
		}
		if (index >= USART_BUFFER_DMA)
			index_old = 0;
		else
			index_old = index;
	}
}

void DMA1_Channel5_IRQHandler(void) {
	if (DMA1->ISR & DMA_ISR_HTIF5) {
		DMA1->IFCR |= DMA_IFCR_CHTIF5;
		usart_buffer_read();
	}
	if (DMA1->ISR & DMA_ISR_TCIF5) {
		DMA1->IFCR |= DMA_IFCR_CTCIF5;
		usart_buffer_read();
	}
	if (DMA1->ISR & DMA_ISR_TEIF5) {
		DMA1->IFCR |= DMA_IFCR_CTEIF5;
	}
}

void DMA1_Channel4_IRQHandler(void) {
	DMA1->IFCR |= DMA_IFCR_CGIF4;
}

void USART1_IRQHandler(void) {
	if (USART1->SR & USART_SR_IDLE) {
		volatile uint32_t tmp = USART1->DR;
		usart_buffer_read();
	}
}

void usart_init() {
	ONETIME(usart)

	buffer_circ_init(&usart_buffer_circ, usart_buffer, USART_BUFFER);

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR1 |= USART_CR1_UE;
	USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
	USART1->BRR = USART_BRR;
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_IDLEIE;
	NVIC_SetPriority(USART1_IRQn, ISR_PRIORITY_USART);
	NVIC_EnableIRQ(USART1_IRQn);

	NVIC_SetPriority(DMA1_Channel4_IRQn, ISR_PRIORITY_USART);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);

	NVIC_SetPriority(DMA1_Channel5_IRQn, ISR_PRIORITY_USART);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	dma_disable(DMA_CHANNEL5);
	dma_setup(DMA_CHANNEL5, (uint32_t) usart_buffer_dma, (uint32_t) &(USART1->DR), USART_BUFFER_DMA,
			DMA_OPT_PRIORITY_LOW | DMA_OPT_MSIZE_8 | DMA_OPT_PSIZE_8 | DMA_OPT_MINC | DMA_OPT_CIRC | DMA_OPT_READ | DMA_OPT_HTI | DMA_OPT_TCI);
	dma_enable(DMA_CHANNEL5);

	gpio_mode(GPIO_A10, GPIO_MODE_IN, GPIO_TYPE_IN_PULL);
	gpio_mode(GPIO_A9, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_PUSHPULL_ALT);
}

bool_e usart_write(uint8_t *buffer, size_t size) {
	TIMEOUTR(!(USART1->SR & USART_SR_TXE), 10000, counter)
	DMA1->IFCR |= DMA_IFCR_CGIF4;
	dma_disable(DMA_CHANNEL4);
	dma_setup(DMA_CHANNEL4, (uint32_t) buffer, (uint32_t) &(USART1->DR), size, DMA_OPT_PRIORITY_LOW | DMA_OPT_MSIZE_8 | DMA_OPT_PSIZE_8 | DMA_OPT_MINC | DMA_OPT_WRITE | DMA_OPT_TCI);
	USART1->SR &= ~(USART_SR_TC | USART_SR_ORE | USART_SR_NE | USART_SR_NE | USART_SR_PE);
	dma_enable(DMA_CHANNEL4);
	return BOOL_TRUE;
}

size_t usart_read(uint8_t *buffer, size_t size) {
	TIMEOUTR(USART1->SR & USART_SR_RXNE, 10000, counter)
	size_t length = usart_buffer_circ.count;
	buffer_circ_pull(&usart_buffer_circ, buffer, size);
	return length - usart_buffer_circ.count;
}

size_t usart_pending() {
	return usart_buffer_circ.count;
}

