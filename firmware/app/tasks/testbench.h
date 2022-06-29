/*
 * testbench.h
 *
 * Copyright (C) 2021, SpaceLab.
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
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief TestBench definition.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2020/01/11
 *
 * \defgroup testbench Test Bench
 * \ingroup tasks
 * \{
 */

#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <FreeRTOS.h>
#include <task.h>

#define TASK_TESTBENCH_NAME                         "TestBench"         /**< Task name. */
#define TASK_TESTBENCH_STACK_SIZE                   150                 /**< Stack size in bytes. */
#define TASK_TESTBENCH_PRIORITY                     1                   /**< Task priority. */
#define TASK_TESTBENCH_PERIOD_MS                    100                 /**< Task period in milliseconds. */
#define TASK_TESTBENCH_INITIAL_DELAY_MS             0                   /**< Delay, in milliseconds, before the first execution. */

/**
 * \brief TestBench handle.
 */
extern xTaskHandle xTaskTestBenchHandle;

/**
 * \brief TestBench task.
 *
 * \return None.
 */
void vTaskTestBench(void);

#endif /* TESTBENCH_H_ */

/** \} End of testbench group */
