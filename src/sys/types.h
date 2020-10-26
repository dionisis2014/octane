/*
 * types.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_

#include "stm32f10x_i2c.h"

typedef enum {
	BOOL_FALSE,
	BOOL_TRUE
} bool_e;

typedef uint32_t size_t;
typedef int32_t ssize_t;
typedef float f32_t;
typedef double f64_t;

#endif
