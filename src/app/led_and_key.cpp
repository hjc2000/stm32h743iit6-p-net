/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "sampleapp_common.h"
#include <bsp-interface/di/led.h>

/************************* Utilities ******************************************/

void app_set_led(uint16_t id, bool led_state)
{
	if (id == APP_DATA_LED_ID)
	{
		if (led_state)
		{
			DI_GreenDigitalLed().TurnOn();
		}
		else
		{
			DI_GreenDigitalLed().TurnOff();
		}
	}
	else if (id == APP_PROFINET_SIGNAL_LED_ID)
	{
		if (led_state)
		{
			DI_RedDigitalLed().TurnOn();
		}
		else
		{
			DI_RedDigitalLed().TurnOff();
		}
	}
}

bool app_get_button(uint16_t id)
{
	return false;
}
