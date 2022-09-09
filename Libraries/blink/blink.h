/*
 * blink.h
 *
 *  Created on: Sep 7, 2022
 *      Author: admin
 */

#ifndef BLINK_H_
#define BLINK_H_

#include <stdint.h>

typedef void (* write_pin_t)(uint8_t level);
typedef uint32_t (* tick_t)(void);

typedef struct{
	uint8_t state;
	uint32_t tick;
	write_pin_t write;
	tick_t get_tick;
	uint32_t interval;
}blink_t;

void blink_init(blink_t *blink, write_pin_t write, tick_t tick, uint32_t interval);
void blink_process(blink_t *blink);

#endif /* BLINK_H_ */
