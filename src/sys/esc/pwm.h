/*
 * pwm.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_ESC_PWM_H_
#define SYS_ESC_PWM_H_

#include "../types.h"

#define PWM_RESOLUTION		16 //bits
#define PWM_RESOLUTION_MAX	((1 << PWM_RESOLUTION) - 1)

#define PWM_PPM_PSC		2
#define PWM_PPM_AR		48000
#define PWM_PPM_MIN		24000
#define PWM_PPM_MAX		48000
#define PWM_OS125_PSC	1
#define PWM_OS125_AR	9000
#define PWM_OS125_MIN	4500
#define PWM_OS125_MAX	9000
#define PWM_OS42_PSC	1
#define PWM_OS42_AR		3024
#define PWM_OS42_MIN	1512
#define PWM_OS42_MAX	3024
#define PWM_MS_PSC		1
#define PWM_MS_AR		900
#define PWM_MS_MIN		180
#define PWM_MS_MAX		720

typedef size_t esc_val_t;

typedef enum {
	PWM_MODE_PPM,
	PWM_MODE_ONESHOT125,
	PWM_MODE_ONESHOT42,
	PWM_MODE_MULTISHOT
} pwm_mode_e;

void esc_pwm_init(pwm_mode_e mode);

void esc_pwm_set(esc_val_t v1, esc_val_t v2, esc_val_t v3, esc_val_t v4);

void esc_pwm_mode(pwm_mode_e mode);

#endif
