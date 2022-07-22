/*
 * testbench.c
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
 * \brief TestBench task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.0.1
 *
 * \date 2020/01/11
 *
 * \addtogroup testbench
 * \{
 */


#include "testbench.h"
#include <drivers/testbench/ina22x/ina22x.h>
xTaskHandle xTaskTestBenchHandle;

void vTaskTestBench(void)
{
    while(1)
    {

    }
}
    /*ina22x_config_t config;
    config.i2c_port = INA22X_I2C_PORT;
    config.i2c_conf.speed_hz = INA22X_I2C_CLOCK_HZ;
    config.avg_mode = INA22X_AVERAGING_MODE_1;
    config.bus_voltage_conv_time = INA22X_BUS_VOLTAGE_CONV_TIME_140u;
    config.shunt_voltage_conv_time = INA22X_SHUNT_VOLTAGE_CONV_TIME_140u;
    config.op_mode = INA22X_MODE_SHUNT_BUS_TRIG;
    config.lsb_current = 1024;
    config.cal = 5e-6;

    ina22x_id_t sensor_man_id;
    ina22x_id_t sensor_die_id;

    ina22x_config_t config_2;
    config_2.i2c_port = INA22X_I2C_PORT;
    config_2.i2c_conf.speed_hz = INA22X_I2C_CLOCK_HZ;
    config_2.avg_mode = INA22X_AVERAGING_MODE_128;
    config_2.bus_voltage_conv_time = INA22X_BUS_VOLTAGE_CONV_TIME_588u;
    config_2.shunt_voltage_conv_time = INA22X_SHUNT_VOLTAGE_CONV_TIME_588u;
    config_2.op_mode = INA22X_MODE_SHUNT_TRIG;
    config_2.lsb_current = 1024;
    config_2.cal = 5e-6;

    uint16_t config_reg_val = 0xFFFF;
    uint16_t cal_reg_val = 0xFFFF;

    uint16_t power_reg_val = 0xFFFF;
    uint16_t current_reg_val = 0xFFFF;
    uint16_t bus_vol_reg_val = 0xFFFF;
    uint16_t shunt_vol_reg_val = 0xFFFF;

    uint16_t cur = 0xFFFF;
    uint16_t volt = 0xFFFF;
    uint16_t pwr = 0xFFFF;

    ina22x_current_t mcur = 0xFFFF;
    ina22x_voltage_t mvolt = 0xFFFF;
    ina22x_power_t mpwr = 0xFFFF;


    while(1)
    {*/
        /* Read Manufacturer ID */
      // ina22x_get_manufacturer_id(config, &sensor_man_id);

       /* Read Die ID */
       //ina22x_get_die_id(config, &sensor_die_id);

       /* Initialize the ina22x */
       //ina22x_init(config);

       /* Configure sensor */
       //ina22x_configuration(config);

       //ina22x_configuration(config_2);

       /* Calibrate sensor to uC measurements*/
       //ina22x_calibration(config_2);

       /* Read Configuration Register */
       //ina22x_read_reg(config_2, INA22X_REG_CONFIGURATION, &config_reg_val);

       /* Read Calibration Register */
       //ina22x_read_reg(config_2, INA22X_REG_CALIBRATION, &cal_reg_val);

       /* Read all measurements Registers */

       //ina22x_read_reg(config_2, INA22X_REG_POWER, &power_reg_val);

       //ina22x_read_reg(config_2, INA22X_REG_CURRENT, &current_reg_val);

       //ina22x_read_reg(config_2, INA22X_REG_BUS_VOLTAGE, &bus_vol_reg_val);

       //ina22x_read_reg(config_2, INA22X_REG_SHUNT_VOLTAGE, &shunt_vol_reg_val);

       /* Read all raw measurements with functions */

       //ina22x_get_current_raw(config, &cur);

       //ina22x_get_voltage_raw(config_2, INA22X_SHUNT_VOLTAGE, &volt);

       //ina22x_get_power_raw(config_2, &pwr);

       /* Read measurements in mili */

       //ina22x_get_current_mA(config_2, &mcur);

       //ina22x_get_voltage_mV(config_2, INA22X_SHUNT_VOLTAGE, &mvolt);

       //ina22x_get_power_mW(config_2, &mpwr);

    //}
//}

/** \} End of testbench group */
