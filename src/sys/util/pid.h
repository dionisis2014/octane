/*
 * pid.h
 *
 *  Created on: Oct 26, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_UTIL_PID_H_
#define SYS_UTIL_PID_H_

#include "../types.h"

typedef struct {
	f32_t kp, ki, kd;
	f32_t current, target, output;
	f32_t integral, error_prev;
	f32_t output_max;
} pid_s;

void pid_init(pid_s *pid, f32_t kp, f32_t ki, f32_t kd, f32_t max);

void pid_update(pid_s *pid);

#endif
