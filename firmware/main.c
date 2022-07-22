/*
 * main.c
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
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Main file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.9
 * 
 * \date 2021/04/04
 * 
 * \defgroup main Main file
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include "devices/watchdog/watchdog.h"
#include "system/clocks.h"
#include "app/tasks/tasks.h"
#include "drivers/testbench/ina22x/ina22x.h"

void main(void)
{


    /* Watchdog device initialization */
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //int err = watchdog_init();

    /* System clocks configuration */
    clocks_config_t clk_conf = {0};

    clk_conf.mclk_hz    = 32000000UL;
    clk_conf.smclk_hz   = 32000000UL;
    clk_conf.aclk_hz    = 32768;

    int err = clocks_setup(clk_conf);

    ina22x_config_t config;
    config.i2c_port = I2C_PORT_1;
    config.i2c_conf.speed_hz = 100000;
    config.i2c_adr = 0x44;
    config.avg_mode = INA22X_AVERAGING_MODE_128;
    config.bus_voltage_conv_time = INA22X_BUS_VOLTAGE_CONV_TIME_588u;
    config.shunt_voltage_conv_time = INA22X_SHUNT_VOLTAGE_CONV_TIME_332u;
    config.op_mode = INA22X_MODE_SHUNT_BUS_TRIG;
    config.lsb_current = 5e-6;
    config.cal =10240;

    ina22x_id_t sensor_man_id = 0;
    ina22x_id_t sensor_die_id = 0;

    ina22x_config_t config_2;
    config_2.i2c_port = I2C_PORT_1;
    config_2.i2c_conf.speed_hz = 100000;
    config_2.i2c_adr = 0x44;
    config_2.avg_mode = INA22X_AVERAGING_MODE_128;
    config_2.bus_voltage_conv_time = INA22X_BUS_VOLTAGE_CONV_TIME_588u;
    config_2.shunt_voltage_conv_time = INA22X_SHUNT_VOLTAGE_CONV_TIME_588u;
    config_2.op_mode = INA22X_MODE_BUS_TRIG;
    config_2.lsb_current = 1024;
    config_2.cal = 5e-6;

    uint16_t config_reg_val = 0;
    uint16_t cal_reg_val = 0;

    uint16_t reg_power_val = 0xFFFF;
    uint16_t reg_current_val = 0xFFFF;
    uint16_t reg_bus_vol_val = 0xFFFF;
    uint16_t reg_shunt_vol_val = 0xFFFF;

    uint16_t raw_cur = 0xFFFF;
    uint16_t raw_volt_shunt = 0xFFFF;
    uint16_t raw_volt_bus = 0xFFFF;
    uint16_t raw_pwr = 0xFFFF;

    ina22x_current_t mcur = 0xFFFF;
    ina22x_voltage_t mvolt_shunt = 0xFFFF;
    ina22x_voltage_t mvolt_bus = 0xFFFF;
    ina22x_power_t mpwr = 0xFFFF;


    /* Initialize the ina22x */
    int err_ina = -2; // Error count

    err_ina = ina22x_init(config);


    /* Read Manufacturer ID */
    err_ina = ina22x_get_manufacturer_id(config, &sensor_man_id);

    /* Read Die ID */
    err_ina = ina22x_get_die_id(config, &sensor_die_id);


    /* Configure sensor */
    err_ina = ina22x_configuration(config);

    /* Check mask is correct */


    /* Read Configuration Register */
    err_ina = ina22x_read_reg(config, INA22X_REG_CONFIGURATION, &config_reg_val);


    /* Calibrate sensor to uC measurements*/
    ina22x_calibration(config);


    /* Read Calibration Register */
    ina22x_read_reg(config, INA22X_REG_CALIBRATION, &cal_reg_val);


    err_ina = ina22x_configuration(config); /*For triggered mode */



    /* Read all measurements Registers */

    ina22x_read_reg(config, INA22X_REG_POWER, &reg_power_val);

    ina22x_read_reg(config, INA22X_REG_CURRENT, &reg_current_val);

    ina22x_read_reg(config, INA22X_REG_BUS_VOLTAGE, &reg_bus_vol_val);

    ina22x_read_reg(config, INA22X_REG_SHUNT_VOLTAGE, &reg_shunt_vol_val);

    /* Read all raw measurements with functions */

    ina22x_get_current_raw(config, &raw_cur);

    ina22x_get_voltage_raw(config, INA22X_SHUNT_VOLTAGE, &raw_volt_shunt);

    ina22x_get_voltage_raw(config, INA22X_BUS_VOLTAGE, &raw_volt_bus);

    ina22x_get_power_raw(config, &raw_pwr);

    /* Read measurements in mili */

    ina22x_get_current_mA(config, &mcur);

    ina22x_get_voltage_mV(config, INA22X_SHUNT_VOLTAGE, &mvolt_shunt);

    ina22x_get_voltage_mV(config, INA22X_BUS_VOLTAGE, &mvolt_bus);

    ina22x_get_power_mW(config, &mpwr);





    /* Create all the tasks */
    create_tasks();

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Will only get here if there was insufficient memory to create the idle and/or timer task */
    while(1)
    {
    }
}

/** \} End of main group */
