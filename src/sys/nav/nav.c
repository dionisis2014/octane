/*
 * nav.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#include "nav.h"
#include "imu.h"
#include "../esc/pwm.h"
#include "../util/string.h"
#include "../../driver/io/usart.h"
#include "../../driver/timer/systick.h"
#include "../util/math.h"
#include "../util/pid.h"

static f32_t accel[3]; //x y z
static f32_t gyro[3]; //yaw pitch roll
static f32_t offset_gyro[3] = { 0.0f, 0.0f, 0.0f };
static f32_t direction[3] = { 0.0f, 0.0f, 0.0f }; //yaw pitch roll
static pid_s pid_pitch, pid_roll, pid_yaw;
static ssize_t output[4] = { 0, 0, 0, 0 }; //front left, front right, back left, back right
static f32_t throttle = 0.0f;

static uint8_t buffer_accel[6];
static uint8_t buffer_gyro[6];

bool_e get_accel() {
	if (!imu_read(0x3B, buffer_accel, 6))
		return BOOL_FALSE;

	accel[0] = ((f32_t) ((int16_t) ((buffer_accel[0] << 8) | buffer_accel[1]))) * 0.0001220740379;
	accel[1] = ((f32_t) ((int16_t) ((buffer_accel[2] << 8) | buffer_accel[3]))) * 0.0001220740379;
	accel[2] = ((f32_t) ((int16_t) ((buffer_accel[4] << 8) | buffer_accel[5]))) * -0.0001220740379;

	return BOOL_TRUE;
}

bool_e get_gyro() {
	if (!imu_read(0x43, buffer_gyro, 6))
		return BOOL_FALSE;

	gyro[1] = (f32_t) ((int16_t) ((buffer_gyro[0] << 8) | buffer_gyro[1])) * -0.0002663242366f;
	gyro[2] = (f32_t) ((int16_t) ((buffer_gyro[2] << 8) | buffer_gyro[3])) * 0.0002663242366f;
	gyro[0] = (f32_t) ((int16_t) ((buffer_gyro[4] << 8) | buffer_gyro[5])) * -0.0002663242366f;

	return BOOL_TRUE;
}

bool_e nav_init() {
	if (!imu_init())
		return BOOL_FALSE;

	for (size_t i = 0; i < NAV_CAL_COUNT; i++) {
		if (!get_gyro())
			return BOOL_FALSE;
		offset_gyro[0] += gyro[0];
		offset_gyro[1] += gyro[1];
		offset_gyro[2] += gyro[2];
		delay(16);
	}
	offset_gyro[0] /= NAV_CAL_COUNT;
	offset_gyro[1] /= NAV_CAL_COUNT;
	offset_gyro[2] /= NAV_CAL_COUNT;

	esc_pwm_init(PWM_MODE_ONESHOT125);

	pid_init(&pid_pitch, NAV_PID_PITCH_KP, NAV_PID_PITCH_KI, NAV_PID_PITCH_KD, PWM_RESOLUTION_MAX);
	pid_init(&pid_roll, NAV_PID_ROLL_KP, NAV_PID_ROLL_KI, NAV_PID_ROLL_KD, PWM_RESOLUTION_MAX);
	pid_init(&pid_yaw, NAV_PID_YAW_KP, NAV_PID_YAW_KI, NAV_PID_YAW_KD, PWM_RESOLUTION_MAX);

	return BOOL_TRUE;
}

void esc_calculate() {
	if (pid_pitch.output < 0.0f) {
		output[3] = output[2] = -pid_pitch.output;
		output[1] = output[0] = 0;
	} else {
		output[1] = output[0] = pid_pitch.output;
		output[3] = output[2] = 0;
	}
//	if (pid_roll.output < 0.0f) {
//		output[0] -= pid_roll.output;
//		output[2] -= pid_roll.output;
//	} else {
//		output[1] += pid_roll.output;
//		output[3] += pid_roll.output;
//	}
//	if (pid_yaw.output < 0.0f) {
//		output[0] += pid_yaw.output;
//		output[1] -= pid_yaw.output;
//		output[2] -= pid_yaw.output;
//		output[3] += pid_yaw.output;
//	} else {
//		output[0] -= pid_yaw.output;
//		output[1] += pid_yaw.output;
//		output[2] += pid_yaw.output;
//		output[3] -= pid_yaw.output;
//	}

	if (output[0] > PWM_RESOLUTION_MAX)
		output[0] = PWM_RESOLUTION_MAX;
	if (output[0] < 0)
		output[0] = 0;

	if (output[1] > PWM_RESOLUTION_MAX)
		output[1] = PWM_RESOLUTION_MAX;
	if (output[1] < 0)
		output[1] = 0;

	if (output[2] > PWM_RESOLUTION_MAX)
		output[2] = PWM_RESOLUTION_MAX;
	if (output[2] < 0)
		output[2] = 0;

	if (output[3] > PWM_RESOLUTION_MAX)
		output[3] = PWM_RESOLUTION_MAX;
	if (output[3] < 0)
		output[3] = 0;
}

bool_e nav_update() {
	if (!get_accel())
		return BOOL_FALSE;
	if (!get_gyro())
		return BOOL_FALSE;

	static size_t millis_prev = 0;
	static f32_t delta = 0.0f;
	static f32_t gyroi[3] = { 0.0f, 0.0f, 0.0f };
	static bool_e init = BOOL_FALSE;

	delta = ((f32_t) (millis() - millis_prev)) / 1000.0f;
	millis_prev = millis();

	gyroi[0] += (gyro[0] - offset_gyro[0]) * delta;
	gyroi[1] += (gyro[1] - offset_gyro[1]) * delta;
	gyroi[2] += (gyro[2] - offset_gyro[2]) * delta;

	f32_t mag = sqrt(square(accel[0]) + square(accel[1]) + square(accel[2]));

	f32_t pitcha = asin(accel[1] / mag);
	f32_t rolla = asin(accel[0] / mag);

	complementary(direction[0], gyroi[0], 0.1f);
	if (init) {
		complementary(direction[1], gyroi[1] * 0.9f + pitcha * 0.1f, 0.1f);
		complementary(direction[2], gyroi[2] * 0.9f + rolla * 0.1f, 0.1f);
	} else {
		init = BOOL_TRUE;
		direction[0] = gyroi[0];
		direction[1] = pitcha;
		direction[2] = rolla;
	}

	pid_yaw.current = direction[0];
	pid_pitch.current = direction[1];
	pid_roll.current = direction[2];
	pid_update(&pid_yaw);
	pid_update(&pid_pitch);
	pid_update(&pid_roll);

	esc_calculate();
	esc_pwm_set(output[0], output[1], output[2], output[3]);

	return BOOL_TRUE;
}

f32_t nav_yaw() {
	return direction[0];
}

f32_t nav_pitch() {
	return direction[1];
}

f32_t nav_roll() {
	return direction[2];
}

void nav_set_throttle(f32_t value) {
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;
	throttle = value;
}

const size_t* nav_esc() {
	return output;
}
