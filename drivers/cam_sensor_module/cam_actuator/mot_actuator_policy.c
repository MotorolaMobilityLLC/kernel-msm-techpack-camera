/*
 * Copyright (C) 2020 Motorola Mobility LLC.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/module.h>
#include <linux/types.h>
#include "cam_debug_util.h"
#include "mot_actuator_policy.h"

static atomic_t mot_actuator_ref_count = ATOMIC_INIT(0);
static unsigned long mot_actuator_consumers = ATOMIC_INIT(0);

int mot_actuator_get(mot_actuator_client user)
{
	if (user <= ACTUATOR_CLIENT_INVALID || user >= ACTUATOR_CLIENT_MAX) {
		CAM_ERR(CAM_ACTUATOR, "INVALID CLIENT!!!");
		return 0;
	}

	CAM_DBG(CAM_ACTUATOR, "Obtain actuator from: %d", user);

	test_and_set_bit(0x01 << user, &mot_actuator_consumers);

	return atomic_add_return(1, &mot_actuator_ref_count);
}
EXPORT_SYMBOL(mot_actuator_get);

int mot_actuator_put(mot_actuator_client user)
{
	if (user <= ACTUATOR_CLIENT_INVALID || user >= ACTUATOR_CLIENT_MAX) {
		CAM_ERR(CAM_ACTUATOR, "INVALID CLIENT!!!");
		return 0;
	}

	CAM_DBG(CAM_ACTUATOR, "Release actuator from: %d", user);

	test_and_clear_bit(0x01 << user, &mot_actuator_consumers);

	return atomic_sub_return(1, &mot_actuator_ref_count);
}
EXPORT_SYMBOL(mot_actuator_put);

int mot_actuator_get_ref_count(void)
{
	return atomic_read(&mot_actuator_ref_count);
}
EXPORT_SYMBOL(mot_actuator_get_ref_count);

unsigned int mot_actuator_get_consumers(void)
{
	return mot_actuator_consumers;
}
EXPORT_SYMBOL(mot_actuator_get_consumers);

ssize_t mot_actuator_dump(char *buf)
{
	int item_len = 0;
	unsigned int consumers = mot_actuator_get_consumers();

	item_len += sprintf(buf+item_len, "refCount: %d\n", mot_actuator_get_ref_count());
	item_len += sprintf(buf+item_len, "Consumers:\n");

	if (consumers & (0x01 << ACTUATOR_CLIENT_CAMERA)) {
		item_len += sprintf(buf+item_len, "camera");
	}

	if (consumers & (0x01 << ACTUATOR_CLIENT_VIBRATOR)) {
		item_len += sprintf(buf+item_len, " vibrator");
	}

	item_len += sprintf(buf+item_len, "\n");

	CAM_DBG(CAM_ACTUATOR, "ref cnt:%d, consumers:0x%x", mot_actuator_get_ref_count(), mot_actuator_get_consumers());
	return item_len;
}
EXPORT_SYMBOL(mot_actuator_dump);
