/*
 * main.c
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
 * \brief SPI slave DMA test code.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2023/06/05
 * 
 * \defgroup spi_slave_dma SPI Slave DMA
 * \{
 */

#include <msp430.h>
#include <stdint.h>

#include <hal/dma.h>
#include <hal/usci_a_spi.h>
#include <hal/gpio.h>
#include <hal/wdt_a.h>

uint8_t tx_data[5] = {'a','b','c','d'};   // 0x61, 0x62, 0x63, 0x64
uint8_t rx_data[5] = {0};

int main(void)
{
    WDT_A_hold(WDT_A_BASE);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN4);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN3);

    USCI_A_SPI_initSlave(USCI_A2_BASE, USCI_A_SPI_MSB_FIRST, USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT, USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW);

    HWREG8(USCI_A2_BASE + OFS_UCAxCTL0) |= UCMODE_2;  /* 4-pin SPI with UCxSTE active low: Slave enabled when UCxSTE = 0 */

    USCI_A_SPI_enable(USCI_A2_BASE);

    /* SPI Slave TX */
    DMA_initParam param = {0};

    param.channelSelect       = DMA_CHANNEL_0;
    param.transferModeSelect  = DMA_TRANSFER_REPEATED_SINGLE;
    param.transferSize        = 4;
    param.triggerSourceSelect = DMA_TRIGGERSOURCE_13;   // UCA2TXIFG
    param.transferUnitSelect  = DMA_SIZE_SRCBYTE_DSTBYTE;
    param.triggerTypeSelect   = DMA_TRIGGER_HIGH;

    DMA_init(&param);

    DMA_setSrcAddress(DMA_CHANNEL_0, (uint32_t)(uintptr_t)tx_data, DMA_DIRECTION_INCREMENT);

    DMA_setDstAddress(DMA_CHANNEL_0, USCI_A_SPI_getTransmitBufferAddressForDMA(USCI_A2_BASE), DMA_DIRECTION_UNCHANGED);

    DMA_clearInterrupt(DMA_CHANNEL_0);

    DMA_enableInterrupt(DMA_CHANNEL_0);

    DMA_enableTransfers(DMA_CHANNEL_0);

    /* SPI Slave RX */
    DMA_initParam param_rx = {0};

    param_rx.channelSelect       = DMA_CHANNEL_1;
    param_rx.transferModeSelect  = DMA_TRANSFER_REPEATED_SINGLE;
    param_rx.transferSize        = 4;
    param_rx.triggerSourceSelect = DMA_TRIGGERSOURCE_12;   // UCA2RXIFG
    param_rx.transferUnitSelect  = DMA_SIZE_SRCBYTE_DSTBYTE;
    param_rx.triggerTypeSelect   = DMA_TRIGGER_HIGH;

    DMA_init(&param_rx);

    DMA_setSrcAddress(DMA_CHANNEL_1, USCI_A_SPI_getReceiveBufferAddressForDMA(USCI_A2_BASE), DMA_DIRECTION_UNCHANGED);

    DMA_setDstAddress(DMA_CHANNEL_1, (uint32_t)(uintptr_t)rx_data, DMA_DIRECTION_INCREMENT);

    DMA_clearInterrupt(DMA_CHANNEL_1);

    DMA_enableInterrupt(DMA_CHANNEL_1);

    DMA_enableTransfers(DMA_CHANNEL_1);

    __bis_SR_register(LPM4_bits + GIE);
}

#pragma vector=DMA_VECTOR
__interrupt void DMA0_ISR(void)
{
    switch (__even_in_range(DMAIV, 16))
    {
        case DMAIV_NONE: break; // No interrupts
        case DMAIV_DMA0IFG: // DMA0IFG = DMA Channel 0
            DMA_clearInterrupt(DMA_CHANNEL_0);
            break;
        case DMAIV_DMA1IFG: // DMA1IFG = DMA Channel 1
            DMA_clearInterrupt(DMA_CHANNEL_1);
            break;
        case DMAIV_DMA2IFG: break; // DMA2IFG = DMA Channel 2
        case 8: break; // Reserved
        case 10: break; // Reserved
        case 12: break; // Reserved
        case 14: break; // Reserved
        case 16: break; // Reserved
        default: break;
    }
}

/** \} End of spi_slave_dma group */
