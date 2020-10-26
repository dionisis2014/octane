/*
 * macros.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_MACROS_H_
#define SYS_MACROS_H_

#include "types.h"

#define ONETIMER(x) static bool_e state_ ## x = BOOL_FALSE; \
		if (state_ ## x == BOOL_TRUE) \
			return 0; \
		state_ ## x = BOOL_TRUE;

#define ONETIME(x) static bool_e state_ ## x = BOOL_FALSE; \
		if (state_ ## x == BOOL_TRUE) \
			return; \
		state_ ## x = BOOL_TRUE;

#define TIMEOUTR(x, y, z) size_t z = 0; \
		while (x) { \
			z++; \
			if (z >= y) \
				return BOOL_FALSE; \
		}

#define TIMEOUT(x, y, z) size_t z = 0; \
		while (x) { \
			z++; \
			if (z >= y) \
				return; \
		}

#define TIMEOUTIF(x, y, z) size_t z = 0; \
		while (x) { \
			z++; \
			if (z >= y) \
				break; \
		} \
		if(z >= y)
#endif
