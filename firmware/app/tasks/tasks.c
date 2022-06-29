/*
 * tasks.c
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
 * \brief Tasks implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.20
 * 
 * \date 2019/11/02
 * 
 * \addtogroup tasks
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <config/config.h>

#include "tasks.h"
#include "startup.h"
#include "watchdog_reset.h"
#include "testbench.h"

void create_tasks(void)
{
    /* Startup task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskStartup, TASK_STARTUP_NAME, TASK_STARTUP_STACK_SIZE, NULL, TASK_STARTUP_PRIORITY, &xTaskStartupHandle);

    if (xTaskStartupHandle == NULL)
    {
        /* Error creating the startup task */
    }
#endif /* CONFIG_TASK_STARTUP_ENABLED */

    /* TestBench task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskTestBench, TASK_TESTBENCH_NAME, TASK_TESTBENCH_STACK_SIZE, NULL, TASK_TESTBENCH_PRIORITY, &xTaskTestBenchHandle);

    if (xTaskTestBenchHandle == NULL)
    {
        /* Error creating the time control task */
    }
#endif /* CONFIG_TASK_BEACON_ENABLED */

    /* Watchdog reset task */
#if defined(CONFIG_TASK_STARTUP_ENABLED) && (CONFIG_TASK_STARTUP_ENABLED == 1)
    xTaskCreate(vTaskWatchdogReset, TASK_WATCHDOG_RESET_NAME, TASK_WATCHDOG_RESET_STACK_SIZE, NULL, TASK_WATCHDOG_RESET_PRIORITY, &xTaskWatchdogResetHandle);

    if (xTaskWatchdogResetHandle == NULL)
    {
        /* Error creating the watchdog reset task */
    }
#endif /* CONFIG_TASK_WATCHDOG_RESET_ENABLED */
}
void create_event_groups(void)
{
    task_startup_status = xEventGroupCreate();

    if (task_startup_status == NULL)
    {
        /* Error creating the startup status event group */
    }
}

/** \} End of tasks group */
