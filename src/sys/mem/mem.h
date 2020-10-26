/*
 * mem.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_MEM_MEM_H_
#define SYS_MEM_MEM_H_

#include "../types.h"

typedef enum {
	MEM_ALIGN_32,
	MEM_ALIGN_16,
	MEM_ALIGN_8
} mem_align_e;

void memcpy(void *src, void *dst, size_t size, mem_align_e align);

#endif
