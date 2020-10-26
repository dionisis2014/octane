/*
 * octane.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Dionisis Toulatos
 */

#include "driver/io/usart.h"
#include "driver/timer/systick.h"
#include "sys/nav/nav.h"
#include "sys/util/string.h"

#include "sys/esc/pwm.h"

uint8_t buffer[100];
size_t s;

void main() {
	systick_init();
	delay(2000);
	usart_init();
	usart_write("Calibrating gyro ...\n", 21);
	nav_init();
	usart_write("Done.\n", 6);
	delay(5000);

	while (1) {
		nav_update();

		s = uint2str(nav_esc()[0], buffer);
		usart_write(buffer, s);
		delay(1);
		usart_write(" ", 1);
		delay(1);

		s = uint2str(nav_esc()[1], buffer);
		usart_write(buffer, s);
		delay(1);
		usart_write(" ", 1);
		delay(1);

		s = uint2str(nav_esc()[2], buffer);
		usart_write(buffer, s);
		delay(1);
		usart_write(" ", 1);
		delay(1);

		s = uint2str(nav_esc()[3], buffer);
		usart_write(buffer, s);
		delay(1);
		usart_write("\n", 1);
		delay(1);
	}
}
