/*
 * imu.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef SYS_NAV_IMU_H_
#define SYS_NAV_IMU_H_

#include "../types.h"

#define IMU_BUFFER		0x40
#define IMU_CAL_COUNT	0x40

bool_e imu_init();

bool_e imu_write(uint8_t reg, uint8_t *buffer, size_t size);

bool_e imu_read(uint8_t reg, uint8_t *buffer, size_t size);

#endif
