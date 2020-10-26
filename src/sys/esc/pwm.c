/*
 * pwm.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#include "pwm.h"
#include "../../driver/timer/timer.h"
#include "../../driver/io/gpio.h"

static esc_val_t delta;
static esc_val_t min;

void esc_pwm_init(pwm_mode_e mode) {
	timer_setup(TIMER_3, TIM_OPT_ARR_BUFF | TIM_OPT_ALIGN_EDGE | TIM_OPT_UPCOUNTER);
	timer_channel_setup(TIMER_3, TIMER_CHANNEL_1, TIM_CHAN_OPT_ACTIVE_HIGH | TIM_CHAN_OPT_CCEN | TIM_CHAN_OPT_CCMODE_PWM_HIGH | TIM_CHAN_OPT_CCBUFF);
	timer_channel_setup(TIMER_3, TIMER_CHANNEL_2, TIM_CHAN_OPT_ACTIVE_HIGH | TIM_CHAN_OPT_CCEN | TIM_CHAN_OPT_CCMODE_PWM_HIGH | TIM_CHAN_OPT_CCBUFF);
	timer_channel_setup(TIMER_3, TIMER_CHANNEL_3, TIM_CHAN_OPT_ACTIVE_HIGH | TIM_CHAN_OPT_CCEN | TIM_CHAN_OPT_CCMODE_PWM_HIGH | TIM_CHAN_OPT_CCBUFF);
	timer_channel_setup(TIMER_3, TIMER_CHANNEL_4, TIM_CHAN_OPT_ACTIVE_HIGH | TIM_CHAN_OPT_CCEN | TIM_CHAN_OPT_CCMODE_PWM_HIGH | TIM_CHAN_OPT_CCBUFF);

	gpio_mode(GPIO_A6, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_PUSHPULL_ALT);
	gpio_mode(GPIO_A7, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_PUSHPULL_ALT);
	gpio_mode(GPIO_B0, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_PUSHPULL_ALT);
	gpio_mode(GPIO_B1, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_PUSHPULL_ALT);

	esc_pwm_mode(mode);
	esc_pwm_set(0, 0, 0, 0);

	timer_enable(TIMER_3);
}

void esc_pwm_set(esc_val_t v1, esc_val_t v2, esc_val_t v3, esc_val_t v4) {
	if (v1 > PWM_RESOLUTION_MAX)
		v1 = PWM_RESOLUTION_MAX;
	if (v2 > PWM_RESOLUTION_MAX)
		v2 = PWM_RESOLUTION_MAX;
	if (v3 > PWM_RESOLUTION_MAX)
		v3 = PWM_RESOLUTION_MAX;
	if (v4 > PWM_RESOLUTION_MAX)
		v4 = PWM_RESOLUTION_MAX;

	v1 = min + (v1 * delta / PWM_RESOLUTION_MAX);
	v2 = min + (v2 * delta / PWM_RESOLUTION_MAX);
	v3 = min + (v3 * delta / PWM_RESOLUTION_MAX);
	v4 = min + (v4 * delta / PWM_RESOLUTION_MAX);

	timer_cc_set(TIMER_3, TIMER_CHANNEL_1, v1);
	timer_cc_set(TIMER_3, TIMER_CHANNEL_2, v2);
	timer_cc_set(TIMER_3, TIMER_CHANNEL_3, v3);
	timer_cc_set(TIMER_3, TIMER_CHANNEL_4, v4);
}

void esc_pwm_mode(pwm_mode_e mode) {
	switch (mode) {
	case PWM_MODE_PPM:
		timer_modify(TIMER_3, PWM_PPM_PSC, PWM_PPM_AR);
		delta = PWM_PPM_MAX - PWM_PPM_MIN;
		min = PWM_PPM_MIN;
		break;
	case PWM_MODE_ONESHOT125:
		timer_modify(TIMER_3, PWM_OS125_PSC, PWM_OS125_AR);
		delta = PWM_OS125_MAX - PWM_OS125_MIN;
		min = PWM_OS125_MIN;
		break;
	case PWM_MODE_ONESHOT42:
		timer_modify(TIMER_3, PWM_OS42_PSC, PWM_OS42_AR);
		delta = PWM_OS42_MAX - PWM_OS42_MIN;
		min = PWM_OS42_MIN;
		break;
	case PWM_MODE_MULTISHOT:
		timer_modify(TIMER_3, PWM_MS_PSC, PWM_MS_AR);
		delta = PWM_MS_MAX - PWM_MS_MIN;
		min = PWM_MS_MIN;
		break;
	}
}
