/*
 * buffer.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_MEM_BUFFER_H_
#define SYS_MEM_BUFFER_H_

#include "../types.h"

typedef struct {
	size_t size;
	size_t count;
	size_t head;
	size_t tail;
	uint8_t *ptr;
} buffer_circ_s;

void buffer_circ_init(buffer_circ_s *cbuff, uint8_t *buffer, size_t size);

void buffer_circ_push(buffer_circ_s *cbuff, uint8_t *data, size_t size);

void buffer_circ_pull(buffer_circ_s *cbuff, uint8_t *buffer, size_t size);

#endif
