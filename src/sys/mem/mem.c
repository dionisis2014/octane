/*
 * mem.c
 *
 *  Created on: Oct 23, 2020
 *      Author: Dionisis Toulatos
 */

#include "mem.h"

void memcpy(void *src, void *dst, size_t size, mem_align_e align) {
	switch (align) {
	case MEM_ALIGN_32:
		for (size_t i = 0; i < size; i++)
			((uint32_t*) dst)[i] = ((uint32_t*) src)[i];
		break;
	case MEM_ALIGN_16:
		for (size_t i = 0; i < size; i++)
			((uint16_t*) dst)[i] = ((uint16_t*) src)[i];
		break;
	default:
		for (size_t i = 0; i < size; i++)
			((uint8_t*) dst)[i] = ((uint8_t*) src)[i];
		break;
	}
}
