/*
 * dma.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef DRIVER_DMA_DMA_H_
#define DRIVER_DMA_DMA_H_

#include "stm32f10x.h"

typedef enum {
	DMA_CHANNEL1,
	DMA_CHANNEL2,
	DMA_CHANNEL3,
	DMA_CHANNEL4,
	DMA_CHANNEL5,
	DMA_CHANNEL6,
	DMA_CHANNEL7
} dma_e;

typedef enum {
	DMA_OPT_M2M = 1 << 14,
	DMA_OPT_PRIORITY_LOW = 0,
	DMA_OPT_PRIORITY_MED = 1 << 12,
	DMA_OPT_PRIORITY_HI = 1 << 13,
	DMA_OPT_PRIORITY_VHI = 3 << 12,
	DMA_OPT_MSIZE_8 = 0,
	DMA_OPT_MSIZE_16 = 1 << 10,
	DMA_OPT_MSIZE_32 = 1 << 11,
	DMA_OPT_PSIZE_8 = 0,
	DMA_OPT_PSIZE_16 = 1 << 8,
	DMA_OPT_PSIZE_32 = 1 << 9,
	DMA_OPT_MINC = 1 << 7,
	DMA_OPT_PINC = 1 << 6,
	DMA_OPT_CIRC = 1 << 5,
	DMA_OPT_READ = 0,
	DMA_OPT_WRITE = 1 << 4,
	DMA_OPT_TEI = 1 << 3,
	DMA_OPT_HTI = 1 << 2,
	DMA_OPT_TCI = 1 << 1
} dma_opt_e;

void dma_setup(dma_e dma, uint32_t mptr, uint32_t pptr, uint16_t size, dma_opt_e options);

void dma_enable(dma_e dma);

void dma_disable(dma_e dma);

#endif
