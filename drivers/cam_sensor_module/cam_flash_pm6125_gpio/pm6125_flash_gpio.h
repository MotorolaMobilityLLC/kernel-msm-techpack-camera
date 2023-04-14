#ifndef _PM6125_FLASH_GPIO_H_
#define _PM6125_FLASH_GPIO_H_

#include "../cam_sensor_utils/cam_sensor_cmn_header.h"

#define PM6125_FLASH_PRINT pr_info //printk

/* DTS state */
typedef enum {
	PM6125_FLASH_GPIO_STATE_ACTIVE,
	PM6125_FLASH_GPIO_STATE_SUSPEND,
	PM6125_FLASH_GPIO_STATE_MAX,	/* for array size */
} PM6125_FLASH_GPIO_STATE;

#define PM6125_PWM_PERIOD 50000
#define FLASH_FIRE_HIGH_MAXCURRENT 1200
#define FLASH_FIRE_LOW_MAXCURRENT 150

/*****************************************************************************
 * Function Prototype
 *****************************************************************************/
extern void pm6125_flash_gpio_select_state(PM6125_FLASH_GPIO_STATE s, enum camera_flash_opcode opcode, u64 flash_current);
int pm6125_flash_gpio_init_module(void);
void pm6125_flash_gpio_exit_module(void);
#endif /* _PM6125_FLASH_GPIO_H_*/
