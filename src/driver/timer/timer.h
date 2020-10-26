/*
 * timer.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef DRIVER_TIMER_TIMER_H_
#define DRIVER_TIMER_TIMER_H_

#include "../../sys/types.h"

typedef enum {
	TIMER_1,
	TIMER_2,
	TIMER_3,
	TIMER_4
} timer_e;

typedef enum {
	TIMER_CHANNEL_1,
	TIMER_CHANNEL_2,
	TIMER_CHANNEL_3,
	TIMER_CHANNEL_4,
} timer_chan_e;

typedef enum {
	TIM_OPT_ARR_BUFF = TIM_CR1_ARPE,
	TIM_OPT_ALIGN_EDGE = 0,
	TIM_OPT_ALIGN_CENTER1 = TIM_CR1_CMS_0,
	TIM_OPT_ALIGN_CENTER2 = TIM_CR1_CMS_1,
	TIM_OPT_ALIGN_CENTER3 = TIM_CR1_CMS_0 | TIM_CR1_CMS_1,
	TIM_OPT_UPCOUNTER = 0,
	TIM_OPT_DOWNCOUNTER = TIM_CR1_DIR,
	TIM_OPT_ONEPULSE = TIM_CR1_OPM
} timer_opt_e;

typedef enum {
	TIM_CHAN_OPT_ACTIVE_HIGH = 0,
	TIM_CHAN_OPT_ACTIVE_LOW = 2 << 8,
	TIM_CHAN_OPT_CCEN = 1 << 8,
	TIM_CHAN_OPT_CCMODE_FROZEN = 0,
	TIM_CHAN_OPT_CCMODE_MATCH = 1 << 4,
	TIM_CHAN_OPT_CCMODE_TOGGLE = 2 << 4,
	TIM_CHAN_OPT_CCMODELOW = 3 << 4,
	TIM_CHAN_OPT_CCMODE_HIGH = 5 << 4,
	TIM_CHAN_OPT_CCMODE_PWM_HIGH = 6 << 4,
	TIM_CHAN_OPT_CCMODE_PWM_LOW = 7 << 4,
	TIM_CHAN_OPT_CCBUFF = 1 << 3
} timer_chan_opt_e;

void timer_setup(timer_e timer, timer_opt_e option);

void timer_channel_setup(timer_e timer, timer_chan_e channel, timer_chan_opt_e option);

void timer_disable(timer_e timer);

void timer_enable(timer_e timer);

void timer_modify(timer_e timer, uint16_t psc, uint16_t ar);

void timer_cc_set(timer_e timer, timer_chan_e channel, uint16_t value);

#endif
