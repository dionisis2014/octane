/*
 * pid.c
 *
 *  Created on: Oct 26, 2020
 *      Author: Dionisis Toulatos
 */

#include "pid.h"

void pid_init(pid_s *pid, f32_t kp, f32_t ki, f32_t kd, f32_t max) {
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->error_prev = pid->integral = pid->output = pid->target = pid->current = 0.0f;
	pid->output_max = max;
}

void pid_update(pid_s *pid) {
	f32_t error = pid->target - pid->current;
	pid->integral += error;
	pid->output = pid->kp * error + pid->ki * pid->integral + pid->kd * (error - pid->error_prev);
	if (pid->output > pid->output_max)
		pid->output = pid->output_max;
	pid->error_prev = error;
}
