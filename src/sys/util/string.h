/*
 * string.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_UTIL_STRING_H_
#define SYS_UTIL_STRING_H_

#include "../types.h"

size_t uint2str(size_t number, uint8_t *buffer);

size_t int2str(ssize_t number, uint8_t *buffer);

size_t float2str(f32_t num, uint8_t *buffer, size_t decimals);

#endif
