/*
 * blink.c
 *
 *  Created on: Sep 7, 2022
 *      Author: admin
 */
#include <string.h>
#include "blink.h"

void blink_init(blink_t *blink, write_pin_t write, tick_t tick, uint32_t interval)
{
	memset(blink, 0, sizeof(blink_t));
	blink->write=write;
	blink->get_tick=tick;
	blink->interval = interval;
}

void blink_process(blink_t *blink)
{
	if(blink->get_tick() - blink->tick > blink->interval)
	{
	  blink->state = !blink->state;
	  blink->write(blink->state);
	  blink->tick = blink->get_tick();
	}
}
