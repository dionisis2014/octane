/*
 * nav.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_NAV_NAV_H_
#define SYS_NAV_NAV_H_

#include "../types.h"

#define NAV_CAL_COUNT	0x100
#define NAV_FILTER_VAL	0.1f

#define NAV_PID_PITCH_KP	50000.0f
#define NAV_PID_PITCH_KI	1000.0f
#define NAV_PID_PITCH_KD	1000.0f
#define NAV_PID_ROLL_KP		50000.0f
#define NAV_PID_ROLL_KI		0.0f
#define NAV_PID_ROLL_KD		0.0f
#define NAV_PID_YAW_KP		1.0f
#define NAV_PID_YAW_KI		0.0f
#define NAV_PID_YAW_KD		0.0f

bool_e nav_init();

bool_e nav_update();

f32_t nav_yaw();

f32_t nav_pitch();

f32_t nav_roll();

void nav_set_throttle(f32_t value);

const size_t* nav_esc();

#endif
