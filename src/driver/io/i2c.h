/*
 * i2c.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Dionisis Toulatos
 */

#ifndef DRIVER_IO_I2C_H_
#define DRIVER_IO_I2C_H_

#include "../../sys/types.h"

void i2c_init();

bool_e i2c_read(uint8_t *buffer, size_t size, uint8_t address);

bool_e i2c_write(uint8_t *buffer, size_t size, uint8_t address);

#endif
