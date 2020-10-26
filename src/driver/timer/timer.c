/*
 * timer.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Dionisis Toulatos
 */

#include "timer.h"
#include "stm32f10x.h"

void timer_setup(timer_e timer, timer_opt_e option) {
	switch (timer) {
	case TIMER_1:
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		TIM1->CR1 |= option;
		break;
	case TIMER_2:
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		TIM2->CR1 |= option;
		break;
	case TIMER_3:
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		TIM3->CR1 |= option;
		break;
	default:
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		TIM4->CR1 |= option;
		break;
	}
}

void timer_channel_setup(timer_e timer, timer_chan_e channel, timer_chan_opt_e option) {
	TIM_TypeDef *tim;
	switch (timer) {
	case TIMER_1:
		tim = TIM1;
		break;
	case TIMER_2:
		tim = TIM2;
		break;
	case TIMER_3:
		tim = TIM3;
		break;
	default:
		tim = TIM4;
		break;
	}
	tim->CCER |= (option >> 8) << (channel << 2);
	if (channel < TIMER_CHANNEL_3)
		tim->CCMR1 |= (option & 0xFF) << (channel << 3);
	else
		tim->CCMR2 |= (option & 0xFF) << ((channel - 2) << 3);
}

void timer_disable(timer_e timer) {
	switch (timer) {
	case TIMER_1:
		TIM1->CR1 &= ~TIM_CR1_CEN;
		break;
	case TIMER_2:
		TIM2->CR1 &= ~TIM_CR1_CEN;
		break;
	case TIMER_3:
		TIM3->CR1 &= ~TIM_CR1_CEN;
		break;
	default:
		TIM4->CR1 &= ~TIM_CR1_CEN;
		break;
	}
}

void timer_enable(timer_e timer) {
	switch (timer) {
	case TIMER_1:
		TIM1->CR1 |= TIM_CR1_CEN;
		TIM1->EGR |= TIM_EGR_UG;
		break;
	case TIMER_2:
		TIM2->CR1 |= TIM_CR1_CEN;
		TIM2->EGR |= TIM_EGR_UG;
		break;
	case TIMER_3:
		TIM3->CR1 |= TIM_CR1_CEN;
		TIM3->EGR |= TIM_EGR_UG;
		break;
	default:
		TIM4->CR1 |= TIM_CR1_CEN;
		TIM4->EGR |= TIM_EGR_UG;
		break;
	}
}

void timer_modify(timer_e timer, uint16_t psc, uint16_t ar) {
	switch (timer) {
	case TIMER_1:
		TIM1->PSC = psc;
		TIM1->ARR = ar;
		break;
	case TIMER_2:
		TIM2->PSC = psc;
		TIM2->ARR = ar;
		break;
	case TIMER_3:
		TIM3->PSC = psc;
		TIM3->ARR = ar;
		break;
	default:
		TIM4->PSC = psc;
		TIM4->ARR = ar;
		break;
	}
}

void timer_cc_set(timer_e timer, timer_chan_e channel, uint16_t value) {
	TIM_TypeDef *tim;
	switch (timer) {
	case TIMER_1:
		tim = TIM1;
		break;
	case TIMER_2:
		tim = TIM2;
		break;
	case TIMER_3:
		tim = TIM3;
		break;
	default:
		tim = TIM4;
		break;
	}
	switch (channel) {
	case TIMER_CHANNEL_1:
		tim->CCR1 = value;
		break;
	case TIMER_CHANNEL_2:
		tim->CCR2 = value;
		break;
	case TIMER_CHANNEL_3:
		tim->CCR3 = value;
		break;
	case TIMER_CHANNEL_4:
		tim->CCR4 = value;
		break;
	}
}
