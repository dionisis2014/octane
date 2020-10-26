/*
 * string.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#include "string.h"

size_t uint2str(size_t number, uint8_t *buffer) {
	if (number == 0) {
		buffer[0] = '0';
		return 1;
	}

	size_t size = 0;
	while (number) {
		buffer[size] = '0' + number % 10;
		number /= 10;
		size++;
	}
	uint8_t tmp;
	for (size_t i = 0; i < size >> 1; i++) {
		tmp = buffer[i];
		buffer[i] = buffer[size - i - 1];
		buffer[size - i - 1] = tmp;
	}
	return size;
}

size_t int2str(ssize_t number, uint8_t *buffer) {
	if (number < 0) {
		buffer[0] = '-';
		return uint2str(-number, buffer + 1) + 1;
	}
	return uint2str(number, buffer);
}

size_t float2str(f32_t num, uint8_t *buffer, size_t decimals) {
	size_t i, s;
	bool_e z = BOOL_FALSE;
	if (num > -1.0f && num < 0.0f) {
		num -= 1.0f;
		z = BOOL_TRUE;
	} else if (num > 0.0f && num < 1.0f) {
		num += 1.0f;
		z = BOOL_TRUE;
	}
	for (i = 0; i < decimals; i++)
		num *= 10.0f;
	s = int2str((int32_t) num, buffer);
	for (i = 0; i < decimals; i++)
		buffer[s - i] = buffer[s - i - 1];
	buffer[s - decimals] = '.';
	if (z){
		if (num < 0.0f)
			buffer[1] = '0';
		else
			buffer[0] = '0';
	}
	return s + 1;
}
