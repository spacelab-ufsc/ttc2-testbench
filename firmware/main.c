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

#include <drivers/spi_slave/spi_slave.h>
#include <drivers/uart/uart.h>

void main(void)
{
    int err = -1;
    /* Watchdog device initialization */
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* System clocks configuration */
    clocks_config_t clk_conf = {0};

    clk_conf.mclk_hz    = 32000000UL;
    clk_conf.smclk_hz   = 32000000UL;
    clk_conf.aclk_hz    = 32768;

    err = clocks_setup(clk_conf);

    spi_config_t spi_config;
    spi_config.mode = SPI_MODE_0;
    spi_config.speed_hz = 100000; /*TODO: Remove speed param (not used for configuration) */

    uint16_t av_bytes = 0;
    uint16_t len = 14;
    uint8_t data[14] = {42};
    uint8_t data_write[14];
    data_write[0] = 0x01;
    data_write[1] = 0x02;
    data_write[2] = 0x03;
    data_write[3] = 0x04;
    data_write[4] = 0x05;
    data_write[5] = 0x06;
    data_write[6] = 0x07;
    data_write[7] = 0x08;
    data_write[8] = 0x09;
    data_write[9] = 0x0A;
    data_write[10] = 0x0B;
    data_write[11] = 0x0C;
    data_write[12] = 0x0D;
    data_write[13] = 0x0E;


    /* 1. Initialize the spi with spi_slave_init() */
       err = spi_slave_init(SPI_PORT_2, spi_config); // A2_VECTOR

       err = spi_slave_write(SPI_PORT_2, data_write, 14);

    /* 2. Enable interruption using spi_slave_enable() */
       err = spi_slave_enable_isr(SPI_PORT_2);

       /*while(av_bytes <= 3)
       {
           av_bytes = spi_slave_read_available(SPI_PORT_2);
       }*/
       //spi_slave_read(SPI_PORT_2, data, len);

       while(1)
       {

           av_bytes = spi_slave_read_available(SPI_PORT_2);
           if (av_bytes == 14)
           {
               spi_slave_read(SPI_PORT_2, data, 14);
               while(1);
           }

       }
        /* 3. Check ISR (3-4 times)*/
       //while (spi_slave_read_available(SPI_PORT_2) < 4);

       /*  4. Check number of available bytes with spi_read_available() */
       //av_bytes = spi_slave_read_available(SPI_PORT_2);

       /*  5. Read the received data with spi_slave_read() */
       //spi_slave_read(SPI_PORT_2, data, len);

       /* 6. Check number of available bytes with spi_read_available() */
       //av_bytes = spi_slave_read_available(SPI_PORT_2);

       /* 7. Write to master (3-4 times) with spi_slave_write() */
       /*data[0] = 0;
       data[0] = 1;
       data[0] = 2;
       data[0] = 3;
       spi_slave_write(SPI_PORT_2, data, 4);*/

       /* 8. Disable interruption (spi_slave_disable()) */
       //spi_slave_disable_isr(SPI_PORT_2);

       /* 9. Try interruption */

       /* 10. Enable interruption (spi_slave_enable()) */
       //spi_slave_enable_isr(SPI_PORT_2);

       /* 11. Fill the RX_buffer to max */

       /* 12. Try (spi_flush()) and check with spi_read_available() */
       //spi_slave_flush(SPI_PORT_2);
       //av_bytes = spi_slave_read_available(SPI_PORT_2);

       /* 13. Write to master buffer and check spi_unread_bytes() */
       //spi_slave_write(SPI_MODE_3, data, 3);
       //av_bytes = spi_slave_bytes_not_sent(SPI_PORT_2);
    /*uart_config_t config;

    config.baudrate     = 115200;
    config.data_bits    = 8;
    config.parity       = UART_NO_PARITY;
    config.stop_bits    = UART_ONE_STOP_BIT;

    err = uart_init(UART_PORT_1, config);
    uint8_t msg[] = {'H', 'e', 'l', 'l', 'o', '!'};

    err = uart_write(UART_PORT_1, msg, 6);

    err = uart_rx_enable(UART_PORT_1);

    while(uart_read_available(UART_PORT_1) == 0)
    {
        uart_write(UART_PORT_1, msg, 6);
    }*/

    /* Will only get here if there was insufficient memory to create the idle and/or timer task */

}

/** \} End of main group */
