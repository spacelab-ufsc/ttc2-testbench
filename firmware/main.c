#include <msp430.h>
#include <stdint.h>

#include <hal/dma.h>
#include <hal/usci_a_spi.h>
#include <hal/gpio.h>
#include <hal/wdt_a.h>

uint8_t tx_data[5] = {'a','b','c','d'};   // 0x61, 0x62, 0x63, 0x64

int main(void)
{
    WDT_A_hold(WDT_A_BASE);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN4);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN3);

    USCI_A_SPI_initSlave(USCI_A2_BASE, USCI_A_SPI_MSB_FIRST, USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT, USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW);

    HWREG8(USCI_A2_BASE + OFS_UCAxCTL0) |= UCMODE_2;  /* 4-pin SPI with UCxSTE active low: Slave enabled when UCxSTE = 0 */

    USCI_A_SPI_enable(USCI_A2_BASE);

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
        case DMAIV_DMA1IFG: break; // DMA1IFG = DMA Channel 1
        case DMAIV_DMA2IFG: break; // DMA2IFG = DMA Channel 2
        case 8: break; // Reserved
        case 10: break; // Reserved
        case 12: break; // Reserved
        case 14: break; // Reserved
        case 16: break; // Reserved
        default: break;
    }
}
