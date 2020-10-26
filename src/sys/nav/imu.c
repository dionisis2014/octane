/*
 * imu.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#include "imu.h"
#include "../../driver/io/i2c.h"
#include "../mem/mem.h"
#include "../../driver/timer/systick.h"

bool_e imu_init() {
	i2c_init();
	uint8_t commands[] = { 0x00, 0xE8, 0x08 };
	uint8_t buffer[6];
	ssize_t cal[3] = { 0, 0, 0 };
	ssize_t val[3] = { 0, 0, 0 };
	size_t i;
	f32_t perc;

	if (!imu_write(0x6B, commands, 1)) //wake up
		return BOOL_FALSE;

	//gyroscope calibration
	if (!imu_write(0x1B, commands + 1, 1))
		return BOOL_FALSE;
	for (i = 0; i < IMU_CAL_COUNT / 2; i++) {
		if (!imu_read(0x43, buffer, 6))
			return BOOL_FALSE;
		cal[0] += (ssize_t) ((int16_t) ((buffer[0] << 8) | buffer[1]));
		cal[1] += (ssize_t) ((int16_t) ((buffer[2] << 8) | buffer[3]));
		cal[2] += (ssize_t) ((int16_t) ((buffer[4] << 8) | buffer[5]));
		delay(5);
	}
	cal[0] /= IMU_CAL_COUNT;
	cal[1] /= IMU_CAL_COUNT;
	cal[2] /= IMU_CAL_COUNT;
	if (!imu_write(0x1B, commands + 2, 1))
		return BOOL_FALSE;
	for (i = 0; i < IMU_CAL_COUNT / 2; i++) {
		if (!imu_read(0x43, buffer, 6))
			return BOOL_FALSE;
		val[0] += (ssize_t) ((int16_t) ((buffer[0] << 8) | buffer[1]));
		val[1] += (ssize_t) ((int16_t) ((buffer[2] << 8) | buffer[3]));
		val[2] += (ssize_t) ((int16_t) ((buffer[4] << 8) | buffer[5]));
		delay(5);
	}
	val[0] /= IMU_CAL_COUNT;
	val[1] /= IMU_CAL_COUNT;
	val[2] /= IMU_CAL_COUNT;
	for (i = 0; i < 3; i++) {
		perc = (f32_t) (cal[i] - val[i]) / 32768.0f;
		if (perc < -0.2f || perc > 0.2f)
			return BOOL_FALSE;
	}

	//accelerometer calibration
	if (!imu_write(0x1C, commands + 1, 1))
		return BOOL_FALSE;
	for (i = 0; i < IMU_CAL_COUNT / 2; i++) {
		if (!imu_read(0x3B, buffer, 6))
			return BOOL_FALSE;
		cal[0] += (ssize_t) ((int16_t) ((buffer[0] << 8) | buffer[1]));
		cal[1] += (ssize_t) ((int16_t) ((buffer[2] << 8) | buffer[3]));
		cal[2] += (ssize_t) ((int16_t) ((buffer[4] << 8) | buffer[5]));
		delay(5);
	}
	cal[0] /= IMU_CAL_COUNT;
	cal[1] /= IMU_CAL_COUNT;
	cal[2] /= IMU_CAL_COUNT;
	if (!imu_write(0x1C, commands + 2, 1))
		return BOOL_FALSE;
	for (i = 0; i < IMU_CAL_COUNT / 2; i++) {
		if (!imu_read(0x3B, buffer, 6))
			return BOOL_FALSE;
		val[0] += (ssize_t) ((int16_t) ((buffer[0] << 8) | buffer[1]));
		val[1] += (ssize_t) ((int16_t) ((buffer[2] << 8) | buffer[3]));
		val[2] += (ssize_t) ((int16_t) ((buffer[4] << 8) | buffer[5]));
		delay(5);
	}
	val[0] /= IMU_CAL_COUNT;
	val[1] /= IMU_CAL_COUNT;
	val[2] /= IMU_CAL_COUNT;
	for (i = 0; i < 3; i++) {
		perc = (f32_t) (cal[i] - val[i]) / 32768.0f;
		if (perc < -0.2f || perc > 0.2f)
			return BOOL_FALSE;
	}

	return BOOL_TRUE;
}

bool_e imu_write(uint8_t reg, uint8_t *buffer, size_t size) {
	if (size >= IMU_BUFFER)
		return BOOL_FALSE;

	uint8_t buff[IMU_BUFFER];
	buff[0] = reg;
	memcpy(buffer, buff + 1, size, MEM_ALIGN_8);
	return i2c_write(buff, size + 1, 0x68);
}

bool_e imu_read(uint8_t reg, uint8_t *buffer, size_t size) {
	if (!i2c_write(&reg, 1, 0x68))
		return BOOL_FALSE;
	return i2c_read(buffer, size, 0x68);
}
