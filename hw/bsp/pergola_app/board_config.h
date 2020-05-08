#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

// Basic Data
#define VENDOR_NAME "xil.se"
#define PRODUCT_NAME "Pergola App"
#define BOARD_ID "Pergola"

// W25Q128JVSIQ: 128Mb = 16MB.
// TODO: Why doesn't 0x1000000 work?
// #define BOARD_FLASH_SIZE 0x1000000
#define BOARD_FLASH_SIZE 0x800000

#define BOARD_FLASH_PAGE_SIZE 256

// LED
#define PIN_LED         IOMUXC_GPIO_03_GPIOMUX_IO03
#define LED_GPIO_PORT   GPIO1
#define LED_GPIO_PIN    3
#define LED_STATE_ON    0

// UART
#define UART_PORT       LPUART1
#define PIN_UART_RX     IOMUXC_GPIO_09_LPUART1_RXD
#define PIN_UART_TX     IOMUXC_GPIO_10_LPUART1_TXD

// QSPI
#define PINNING_QSPI_FLEXSPIA

/*
  - {pin_num: '12', peripheral: GPIO1, signal: 'gpiomux_io, 01', pin_signal: GPIO_01, identifier: LED_G, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '11', peripheral: GPIO1, signal: 'gpiomux_io, 02', pin_signal: GPIO_02, identifier: LED_R, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '10', peripheral: GPIO1, signal: 'gpiomux_io, 03', pin_signal: GPIO_03, identifier: LED_B, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '51', peripheral: GPIO1, signal: 'gpiomux_io, 21', pin_signal: GPIO_AD_07, identifier: FPGA_PROGRAMN, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '59', peripheral: GPIO1, signal: 'gpiomux_io, 15', pin_signal: GPIO_AD_01, identifier: FPGA_DONE, direction: INPUT, pull_keeper_enable: Disable}
  - {pin_num: '65', peripheral: GPIO2, signal: 'gpio_io, 10', pin_signal: GPIO_SD_10, identifier: FPGA_SPI_CLK, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '66', peripheral: GPIO2, signal: 'gpio_io, 09', pin_signal: GPIO_SD_09, identifier: FPGA_SPI_MOSI, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '68', peripheral: GPIO2, signal: 'gpio_io, 07', pin_signal: GPIO_SD_07, identifier: FPGA_SPI_MISO, direction: INPUT, pull_keeper_enable: Disable}
  - {pin_num: '69', peripheral: GPIO2, signal: 'gpio_io, 06', pin_signal: GPIO_SD_06, identifier: FPGA_SPI_CSN, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '60', peripheral: GPIO1, signal: 'gpiomux_io, 14', pin_signal: GPIO_AD_00, identifier: FPGA_SPI_HOLDN, direction: OUTPUT, gpio_init_state: 'true', pull_keeper_enable: Disable}
  - {pin_num: '64', peripheral: GPIO2, signal: 'gpio_io, 11', pin_signal: GPIO_SD_11, identifier: SPIFLASH_HOLDN, direction: OUTPUT, pull_keeper_enable: Disable}
  - {pin_num: '2', peripheral: GPIO1, signal: 'gpiomux_io, 10', pin_signal: GPIO_10, direction: INPUT, pull_keeper_enable: Disable}
  - {pin_num: '3', peripheral: GPIO1, signal: 'gpiomux_io, 09', pin_signal: GPIO_09, direction: INPUT, pull_keeper_enable: Disable}
*/



#endif
