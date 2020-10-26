/*
 * dma.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#include "dma.h"
#include "stm32f10x.h"

void dma_setup(dma_e dma, uint32_t mptr, uint32_t pptr, uint16_t size, dma_opt_e options) {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA_Channel_TypeDef *channel = (DMA_Channel_TypeDef*) ((uint32_t) DMA1_Channel1 + 0x14 * dma);
	channel->CCR = options;
	channel->CNDTR = size;
	channel->CMAR = mptr;
	channel->CPAR = pptr;
}

void dma_enable(dma_e dma) {
	DMA_Channel_TypeDef *channel = (DMA_Channel_TypeDef*) ((uint32_t) DMA1_Channel1 + 0x14 * dma);
	channel->CCR |= 1;
}

void dma_disable(dma_e dma) {
	DMA_Channel_TypeDef *channel = (DMA_Channel_TypeDef*) ((uint32_t) DMA1_Channel1 + 0x14 * dma);
	channel->CCR &= ~1;
}
