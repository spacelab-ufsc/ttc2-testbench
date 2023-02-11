from pyftdi.spi import SpiController, SpiIOError

ctrl = SpiController()

# Configure the first interface (IF/1) of the FTDI device as a SPI master
ctrl.configure('ftdi://ftdi:232h:FT5VZFRA/1')

slave = ctrl.get_port(cs=0, freq=100000, mode=1)

print(slave.exchange([0x40, 0x80, 0xC1, 0x01, 0x41, 0x81, 0xC2, 0x02, 0x42, 0x82, 0xC3, 0x03, 0x43, 0xFF]))


