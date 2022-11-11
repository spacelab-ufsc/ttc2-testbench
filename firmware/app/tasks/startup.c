/*
 * startup.c
 * 
 * Copyright The TTC 2.0 Contributors.
 * 
 * This file is part of TTC 2.0.
 * 
 * TTC 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TTC 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Startup task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.3
 * 
 * \date 2019/12/04
 * 
 * \addtogroup startup
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <system/clocks.h>
#include <devices/watchdog/watchdog.h>
#include <devices/leds/leds.h>
#include <devices/radio/radio.h>
#include <devices/power_sensor/power_sensor.h>
#include <devices/temp_sensor/temp_sensor.h>
#include <devices/antenna/antenna.h>
#include <devices/media/media.h>

#include <ngham/ngham.h>

#include "startup.h"


xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void)
{
    unsigned int error_counter = 0;



    /* Logger device initialization */
    sys_log_init();




    /* Print the FreeRTOS version */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "FreeRTOS ");
    sys_log_print_msg(tskKERNEL_VERSION_NUMBER);
    sys_log_new_line();

    /* Print the hardware version */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Hardware revision is ");
    sys_log_print_uint(system_get_hw_version());
    sys_log_new_line();

    /* Print the system clocks */
    clocks_config_t clks = clocks_read();
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "System clocks: MCLK=");
    sys_log_print_uint(clks.mclk_hz);
    sys_log_print_msg(" Hz, SMCLK=");
    sys_log_print_uint(clks.smclk_hz);
    sys_log_print_msg(" Hz, ACLK=");
    sys_log_print_uint(clks.aclk_hz);
    sys_log_print_msg(" Hz");
    sys_log_new_line();

    /* Print last reset cause (code) */
    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Last reset cause: ");
    sys_log_print_hex(system_get_reset_cause());
    sys_log_new_line();


    voltage_t ind_volt_uc_shunt = 0;
    voltage_t ind_volt_uc_bus = 0;

    voltage_t ind_volt_radio_shunt = 0;
    voltage_t ind_volt_radio_bus = 0;

    current_t ind_curr_uc = 0;
    current_t ind_curr_radio = 0;

    power_t   ind_power_uc = 0;
    power_t   ind_power_radio = 0;

    power_sensor_data_t data_uc;
    power_sensor_data_t data_radio;

    /* Initialize power sensor */
    error_counter = power_sensor_init();

    /*Check individual voltage */
    error_counter = power_sensor_read_voltage(POWER_SENSOR_UC, POWER_SENSOR_MILI_SCALE, POWER_SENSOR_MILI_SCALE, &ind_volt_uc_shunt, &ind_volt_uc_bus);
    error_counter = power_sensor_read_voltage(POWER_SENSOR_RADIO, POWER_SENSOR_MILI_SCALE, POWER_SENSOR_MILI_SCALE, &ind_volt_radio_shunt, &ind_volt_radio_bus);
    vTaskDelay(500);
    /* Check individual curr */
    error_counter = power_sensor_read_current(POWER_SENSOR_UC, POWER_SENSOR_MILI_SCALE, &ind_curr_uc);
    error_counter = power_sensor_read_current(POWER_SENSOR_RADIO, POWER_SENSOR_MILI_SCALE, &ind_curr_radio);
    vTaskDelay(500);
    /* Check individual power */
    error_counter = power_sensor_read_power(POWER_SENSOR_UC, POWER_SENSOR_MILI_SCALE, &ind_power_uc);
    error_counter = power_sensor_read_power(POWER_SENSOR_RADIO, POWER_SENSOR_MILI_SCALE, &ind_power_radio);
    vTaskDelay(500);
    /* Check read function */
    power_sensor_read(POWER_SENSOR_UC, &data_uc);
    power_sensor_read(POWER_SENSOR_RADIO, &data_radio);

    /* Internal non-volatile memory initialization */
    if (media_init(MEDIA_INT_FLASH) != 0)
    {
        error_counter++;
    }

    /* LEDs device initialization */
#if defined(CONFIG_DEVICE_LEDS_ENABLED) && (CONFIG_DEVICE_LEDS_ENABLED)
    if (leds_init() != 0)
    {
        error_counter++;
    }
#endif /* CONFIG_DEVICE_LEDS_ENABLED */

    /* Power sensor device initialization */
    if (power_sensor_init() != 0)
    {
        error_counter++;
    }

    /* Temperature sensor device initialization */
    if (temp_sensor_init() != 0)
    {
        error_counter++;
    }

    /* Radio device initialization */
    if (radio_init() != 0)
    {
        error_counter++;
    }

    /* NGHam initialization */
    ngham_init_arrays();
    ngham_init();

    /* Antenna device initialization */
    if (antenna_init() != 0)
    {
        error_counter++;
    }

    if (error_counter > 0U)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME, "Boot completed with ");
        sys_log_print_uint(error_counter);
        sys_log_print_msg(" ERROR(S)!");
        sys_log_new_line();

        led_set(LED_FAULT);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "Boot completed with SUCCESS!");
        sys_log_new_line();

        led_clear(LED_FAULT);
    }

    /* Startup task status = Done */
    xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

    vTaskSuspend(xTaskStartupHandle);
}

/** \} End of startup group */
