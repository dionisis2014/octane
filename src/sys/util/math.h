/*
 * math.h
 *
 *  Created on: Oct 25, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_UTIL_MATH_H_
#define SYS_UTIL_MATH_H_

#include "../types.h"

#define SQRT_ITERATIONS	5

#define PId4	0.7853981634f
#define PId2	1.570796327f
#define PId34	2.356194490f
#define PI		3.141592654f
#define PIm2	6.283185307f

#define deg2rad(x) (x * PI / 180.0f)
#define rad2deg(x) (x * 180.0f / PI)

#define square(x) (x * x)

#define complementary(o, n, a) (o = o * (1.0f - a) + (n) * a)

f32_t modulo(f32_t value, f32_t mod);

f32_t sin(f32_t angle);

f32_t cos(f32_t angle);

f32_t asin(f32_t value);

f32_t acos(f32_t value);

f32_t sqrt(f32_t value);

#endif
