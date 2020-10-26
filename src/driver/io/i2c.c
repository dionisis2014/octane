/*
 * i2c.c
 *
 *  Created on: Oct 23, 2020
 *      Author: Dionisis Toulatos
 */

#include "i2c.h"
#include "gpio.h"
#include "../../sys/macros.h"

static size_t i2c_state = 0;
static size_t i2c_state_prev = 0;

void i2c_init() {
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR1 &= ~I2C_CR1_PE; //disable peripheral

	//timings
	I2C1->CR2 |= 36; //set frequency of APB clock
	I2C1->CCR = I2C_CCR_FS | 15; //set full speed mode, duty 2, CCR 15??
	I2C1->TRISE = 11; //11.8 -> 11

	gpio_mode(GPIO_B6, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_OPENDRAIN_ALT);
	gpio_mode(GPIO_B7, GPIO_MODE_OUT_50MHZ, GPIO_TYPE_OUT_OPENDRAIN_ALT);

	I2C1->CR1 |= I2C_CR1_PE; //enable peripheral
}

bool_e i2c_read(uint8_t *buffer, size_t size, uint8_t address) {
	if (i2c_state)
		return BOOL_FALSE;

	TIMEOUTIF(I2C1->SR2 & I2C_SR2_BUSY, 100000, busy) {
		i2c_state = 0;
		return BOOL_FALSE;
	}
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 &= ~I2C_CR1_POS;
	i2c_state = 1;

	I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_START;
	TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_SB), 100000, sb) {
		i2c_state = 0;
		return BOOL_FALSE;
	}
	I2C1->DR = (address << 1) | 1;
	TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_ADDR), 100000, addr) {
		//		if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
		//			I2C1->CR1 |= I2C_CR1_STOP;
		i2c_state = 0;
		return BOOL_FALSE;
	}

	volatile uint32_t tmp;
	switch (size) {
	case 0:
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		I2C1->CR1 |= I2C_CR1_STOP;
		break;
	case 1:
		I2C1->CR1 &= ~I2C_CR1_ACK;
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		I2C1->CR1 |= I2C_CR1_STOP;
		break;
	case 2:
		I2C1->CR1 &= ~I2C_CR1_ACK;
		I2C1->CR1 |= I2C_CR1_POS;
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		break;
	default:
		I2C1->CR1 |= I2C_CR1_ACK;
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		break;
	}

	tmp = 0;
	while (tmp < size) {
		switch (size - tmp) {
		case 1:
			;
			TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_RXNE), 100000, rxne_1)
			{
				if (I2C1->SR1 & I2C_SR1_STOPF) {
					I2C1->SR1 &= ~I2C_SR1_STOPF;
				}
				i2c_state = 0;
				return BOOL_FALSE;
			}
			buffer[tmp] = I2C1->DR;
			tmp++;
			break;
		case 2:
			;
			TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_BTF), 100000, btf_2)
			{
				if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
					I2C1->CR1 |= I2C_CR1_STOP;
				i2c_state = 0;
				return BOOL_FALSE;
			}
			I2C1->CR1 |= I2C_CR1_STOP;
			buffer[tmp] = I2C1->DR;
			tmp++;
			buffer[tmp] = I2C1->DR;
			tmp++;
			break;
		case 3:
			;
			TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_BTF), 100000, btf_31)
			{
				if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
					I2C1->CR1 |= I2C_CR1_STOP;
				i2c_state = 0;
				return BOOL_FALSE;
			}
			I2C1->CR1 &= ~I2C_CR1_ACK;
			buffer[tmp] = I2C1->DR;
			tmp++;
			TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_BTF), 100000, btf_32)
			{
				if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
					I2C1->CR1 |= I2C_CR1_STOP;
				i2c_state = 0;
				return BOOL_FALSE;
			}
			I2C1->CR1 |= I2C_CR1_STOP;
			buffer[tmp] = I2C1->DR;
			tmp++;
			buffer[tmp] = I2C1->DR;
			tmp++;
			break;
		default:
			;
			TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_RXNE), 100000, rxne)
			{
				if (I2C1->SR1 & I2C_SR1_STOPF) {
					I2C1->SR1 &= ~I2C_SR1_STOPF;
				}
				i2c_state = 0;
				return BOOL_FALSE;
			}
			buffer[tmp] = I2C1->DR;
			tmp++;
			if (I2C1->SR1 & I2C_SR1_BTF) {
				buffer[tmp] = I2C1->DR;
				tmp++;
			}
			break;
		}
	}
	i2c_state = 0;

	return BOOL_TRUE;
}

bool_e i2c_write(uint8_t *buffer, size_t size, uint8_t address) {
	if (i2c_state)
		return BOOL_FALSE;

	TIMEOUTIF(I2C1->SR2 & I2C_SR2_BUSY, 100000, busy) {
		i2c_state = 0;
		return BOOL_FALSE;
	}
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 &= ~I2C_CR1_POS;
	i2c_state = 1;

	I2C1->CR1 |= I2C_CR1_START;
	TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_SB), 100000, sb) {
		i2c_state = 0;
		return BOOL_FALSE;
	}
	I2C1->DR = address << 1;
	TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_ADDR), 100000, addr) {
//		if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
//			I2C1->CR1 |= I2C_CR1_STOP;
		i2c_state = 0;
		return BOOL_FALSE;
	}
	volatile uint32_t tmp = I2C1->SR1;
	tmp = I2C1->SR2;
	tmp = 0;
	while (tmp < size) {
		TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_TXE), 100000, txe) {
			if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
				I2C1->CR1 |= I2C_CR1_STOP;
			i2c_state = 0;
			return BOOL_FALSE;
		}
		I2C1->DR = buffer[tmp];
		tmp++;
		if ((I2C1->SR1 & I2C_SR1_BTF) && (tmp < size - 1)) {
			I2C1->DR = buffer[tmp];
			tmp++;
		}
		TIMEOUTIF(!(I2C1->SR1 & I2C_SR1_BTF), 100000, btf)
		{
			if (I2C1->SR1 & I2C_SR1_AF) //ACK failed
				I2C1->CR1 |= I2C_CR1_STOP;
			i2c_state = 0;
			return BOOL_FALSE;
		}
	}
	I2C1->CR1 |= I2C_CR1_STOP;
	i2c_state = 0;

	return BOOL_TRUE;
}
