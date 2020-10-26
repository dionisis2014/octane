/*
 * buffer.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#include "buffer.h"

void buffer_circ_init(buffer_circ_s *cbuff, uint8_t *buffer, size_t size) {
	cbuff->size = size;
	cbuff->count = 0;
	cbuff->head = 0;
	cbuff->tail = 0;
	cbuff->ptr = buffer;
}

void buffer_circ_push(buffer_circ_s *cbuff, uint8_t *data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		cbuff->ptr[cbuff->head] = data[i];
		cbuff->head = (cbuff->head + 1) % cbuff->size;
		if (cbuff->count < cbuff->size)
			cbuff->count++;
		else
			cbuff->tail = (cbuff->tail + 1) % cbuff->size;
	}
}

void buffer_circ_pull(buffer_circ_s *cbuff, uint8_t *buffer, size_t size) {
	if (size > cbuff->count)
		size = cbuff->count;

	for (size_t i = 0; i < size; i++) {
		buffer[i] = cbuff->ptr[cbuff->tail];
		cbuff->tail = (cbuff->tail + 1) % cbuff->size;
		cbuff->count--;
	}
}
