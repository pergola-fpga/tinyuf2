#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fsl_device_registers.h"
#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#include "bsp.h"
#include "tusb.h"
#include "hid.h"
#include "fpga.h"



/*




*************************
*************************

2020-06-15

Clean up this mess!

But this works for now.

*************************
*************************





*/



/* GPIO_01 (number 12), LED_G */
#define BOARD_INITPINS_LED_G_PERIPHERAL                                    GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_LED_G_SIGNAL                                   gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_LED_G_CHANNEL                                          1U   /*!< GPIO1 gpiomux_io channel: 01 */

/* GPIO_02 (number 11), LED_R */
#define BOARD_INITPINS_LED_R_PERIPHERAL                                    GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_LED_R_SIGNAL                                   gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_LED_R_CHANNEL                                          2U   /*!< GPIO1 gpiomux_io channel: 02 */

/* GPIO_03 (number 10), LED_B */
#define BOARD_INITPINS_LED_B_PERIPHERAL                                    GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_LED_B_SIGNAL                                   gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_LED_B_CHANNEL                                          3U   /*!< GPIO1 gpiomux_io channel: 03 */

/* GPIO_AD_07 (number 51), FPGA_PROGRAMN */
#define BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL                            GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_FPGA_PROGRAMN_SIGNAL                           gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL                                 21U   /*!< GPIO1 gpiomux_io channel: 21 */

/* GPIO_AD_01 (number 59), FPGA_DONE */
#define BOARD_INITPINS_FPGA_DONE_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_FPGA_DONE_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_FPGA_DONE_CHANNEL                                     15U   /*!< GPIO1 gpiomux_io channel: 15 */

/* GPIO_SD_10 (number 65), FlexSPI_CLK_A/U13[6] */
#define BOARD_INITPINS_FPGA_SPI_CLK_GPIO                                   GPIO2   /*!< GPIO device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_CLK_PORT                                   GPIO2   /*!< PORT device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_CLK_PIN                                      10U   /*!< GPIO2 pin index: 10 */

/* GPIO_SD_09 (number 66), FlexSPI_D0_A/U13[5] */
#define BOARD_INITPINS_FPGA_SPI_MOSI_GPIO                                  GPIO2   /*!< GPIO device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_MOSI_PORT                                  GPIO2   /*!< PORT device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_MOSI_PIN                                      9U   /*!< GPIO2 pin index: 9 */

/* GPIO_SD_07 (number 68), FlexSPI_D1_A/U13[2] */
#define BOARD_INITPINS_FPGA_SPI_MISO_GPIO                                  GPIO2   /*!< GPIO device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_MISO_PORT                                  GPIO2   /*!< PORT device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_MISO_PIN                                      7U   /*!< GPIO2 pin index: 7 */

/* GPIO_SD_06 (number 69), FlexSPI_SS0/U13[1] */
#define BOARD_INITPINS_FPGA_SPI_CSN_GPIO                                   GPIO2   /*!< GPIO device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_CSN_PORT                                   GPIO2   /*!< PORT device name: GPIO2 */
#define BOARD_INITPINS_FPGA_SPI_CSN_PIN                                       6U   /*!< GPIO2 pin index: 6 */

/* GPIO_AD_00 (number 60), USB_OTG1_PWR */
#define BOARD_INITPINS_FPGA_SPI_HOLDN_PERIPHERAL                           GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_FPGA_SPI_HOLDN_SIGNAL                          gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_FPGA_SPI_HOLDN_CHANNEL                                14U   /*!< GPIO1 gpiomux_io channel: 14 */

/* GPIO_SD_11 (number 64), FlexSPI_D3_A/U13[7] */
#define BOARD_INITPINS_SPIFLASH_HOLDN_GPIO                                 GPIO2   /*!< GPIO device name: GPIO2 */
#define BOARD_INITPINS_SPIFLASH_HOLDN_PORT                                 GPIO2   /*!< PORT device name: GPIO2 */
#define BOARD_INITPINS_SPIFLASH_HOLDN_PIN                                    11U   /*!< GPIO2 pin index: 11 */

/* GPIO_10 (number 2), LPUART1_TXD/J56[4] */
#define BOARD_INITPINS_UART1_TXD_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_UART1_TXD_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_UART1_TXD_CHANNEL                                     10U   /*!< GPIO1 gpiomux_io channel: 10 */

/* GPIO_09 (number 3), LPUART1_RXD/J56[2] */
#define BOARD_INITPINS_UART1_RXD_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_UART1_RXD_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_UART1_RXD_CHANNEL                                      9U   /*!< GPIO1 gpiomux_io channel: 09 */



static uint8_t buf[64];
static int state;

static void sleep(int n)
{
	while (n--)
		__asm volatile ("nop");
}

static void
fakespi_xfer(const uint8_t *data_out, uint8_t *data_in, int len, int toggle_cs)
{
    if (toggle_cs) {
        GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CSN_GPIO,
                  BOARD_INITPINS_FPGA_SPI_CSN_PIN, 0);
    }
    // Transmit data, configuration should be so that "Each clock shifts one bit of data at the clock falling edge"
    for (int i = 0; i < len; i++) {
        uint8_t byte_out = data_out[i];
        uint8_t byte_in = 0;
        for (int j = 0; j < 8; j++) {
            uint8_t bit = byte_out & (1 << (7 - j));
            GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CLK_GPIO,
                      BOARD_INITPINS_FPGA_SPI_CLK_PIN, 0);
            GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_MOSI_GPIO,
                      BOARD_INITPINS_FPGA_SPI_MOSI_PIN,
                      bit);
            GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CLK_GPIO,
                      BOARD_INITPINS_FPGA_SPI_CLK_PIN, 1);
            uint8_t bit_in =
                GPIO_PinRead(BOARD_INITPINS_FPGA_SPI_MISO_GPIO,
                     BOARD_INITPINS_FPGA_SPI_MISO_PIN);
            byte_in = (bit_in ? 1 : 0) | (byte_in << 1);
        }
        if (data_in) {
            data_in[i] = byte_in;
        }
    }
    if (toggle_cs) {
        GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CSN_GPIO,
                  BOARD_INITPINS_FPGA_SPI_CSN_PIN, 1);
    }
}

static void fakespi_xfer_cmd(uint8_t cmd, int len, int toggle_cs)
{
    memset(buf, 0x0, sizeof(buf));
    buf[0] = cmd;
    fakespi_xfer(buf, buf, sizeof(buf), toggle_cs);
}

static void fpga_start_program(void)
{
    printf("fpga_start_program\n");

    // Toggle PROGRAMN to reset the FPGA. Sleeps are needed.
    GPIO_PinWrite
        (BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL,
            BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL, 0);
    sleep(1000000);
    GPIO_PinWrite
        (BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL,
            BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL, 1);
    sleep(1000000);

    // READ_ID *must* be sent
    fakespi_xfer_cmd(0xE0, 8, 1);
    printf("READ_ID: ");
    for (int a = 0; a < 4 + 4; a++)
        printf("%02x ", buf[a]);
    printf("\r\n");

    // unknown(0xc6) *must* be sent
    fakespi_xfer_cmd(0xC6, 4, 1);	// Unknown

    // Keep CS low during the LSC_BITSTREAM_BURST command and bit stream
    GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CSN_GPIO,
                BOARD_INITPINS_FPGA_SPI_CSN_PIN, 0);
    fakespi_xfer_cmd(0x7A, 4, 0);	// LSC_BITSTREAM_BURST
}

static int bytes_transferred;
void fpga_bitstream_write(const uint8_t *src, uint32_t lba, uint32_t num_blocks)
{
    printf("Got %p %ld %ld\r\n", src, lba, num_blocks);
    if (lba == 0) {
        fpga_start_program();
        bytes_transferred = 0;
    }

    fakespi_xfer(src, NULL, num_blocks, 0);
    // Blink to indicate transfer
    GPIO_PinWrite(BOARD_INITPINS_LED_B_PERIPHERAL,
                BOARD_INITPINS_LED_B_CHANNEL,
                (bytes_transferred & 0x10000) > 0x8000);

    bytes_transferred += num_blocks * BOARD_FLASH_PAGE_SIZE;
}

extern void fpga_bitstream_finish(void)
{
    printf("fpga_bitstream_finish\n");

    GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CSN_GPIO,
                BOARD_INITPINS_FPGA_SPI_CSN_PIN, 1);
    GPIO_PinWrite(BOARD_INITPINS_LED_B_PERIPHERAL,
                BOARD_INITPINS_LED_B_CHANNEL, 1);
    sleep(10000);

    fakespi_xfer_cmd(0x26, 4, 1);	// ISC_DISABLE
    fakespi_xfer_cmd(0xFF, 4, 1);	// ISC_NOOP

    state = 0;
}

void fpga_task(void)
{
    if (GPIO_PinRead(BOARD_INITPINS_FPGA_DONE_PERIPHERAL,
                BOARD_INITPINS_FPGA_DONE_CHANNEL)) {
        GPIO_PinWrite(BOARD_INITPINS_LED_R_PERIPHERAL,
                    BOARD_INITPINS_LED_R_CHANNEL, 1);
        GPIO_PinWrite(BOARD_INITPINS_LED_G_PERIPHERAL,
                    BOARD_INITPINS_LED_G_CHANNEL, 0);
    } else {
        GPIO_PinWrite(BOARD_INITPINS_LED_R_PERIPHERAL,
                    BOARD_INITPINS_LED_R_CHANNEL, 0);
        GPIO_PinWrite(BOARD_INITPINS_LED_G_PERIPHERAL,
                    BOARD_INITPINS_LED_G_CHANNEL, 1);
    }
}

void fpga_InitPins(void) {
  /* GPIO configuration of LED_G on GPIO_01 (pin 12) */
  gpio_pin_config_t LED_G_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_01 (pin 12) */
  GPIO_PinInit(GPIO1, 1U, &LED_G_config);

  /* GPIO configuration of LED_R on GPIO_02 (pin 11) */
  gpio_pin_config_t LED_R_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_02 (pin 11) */
  GPIO_PinInit(GPIO1, 2U, &LED_R_config);

  /* GPIO configuration of LED_B on GPIO_03 (pin 10) */
  gpio_pin_config_t LED_B_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_03 (pin 10) */
  GPIO_PinInit(GPIO1, 3U, &LED_B_config);

  /* GPIO configuration of UART1_RXD on GPIO_09 (pin 3) */
  gpio_pin_config_t UART1_RXD_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_09 (pin 3) */
  GPIO_PinInit(GPIO1, 9U, &UART1_RXD_config);

  /* GPIO configuration of UART1_TXD on GPIO_10 (pin 2) */
  gpio_pin_config_t UART1_TXD_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_10 (pin 2) */
  GPIO_PinInit(GPIO1, 10U, &UART1_TXD_config);

  /* GPIO configuration of FPGA_SPI_HOLDN on GPIO_AD_00 (pin 60) */
  gpio_pin_config_t FPGA_SPI_HOLDN_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 1U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_00 (pin 60) */
  GPIO_PinInit(GPIO1, 14U, &FPGA_SPI_HOLDN_config);

  /* GPIO configuration of FPGA_DONE on GPIO_AD_01 (pin 59) */
  gpio_pin_config_t FPGA_DONE_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_01 (pin 59) */
  GPIO_PinInit(GPIO1, 15U, &FPGA_DONE_config);

  /* GPIO configuration of FPGA_PROGRAMN on GPIO_AD_07 (pin 51) */
  gpio_pin_config_t FPGA_PROGRAMN_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_07 (pin 51) */
  GPIO_PinInit(GPIO1, 21U, &FPGA_PROGRAMN_config);

  /* GPIO configuration of FPGA_SPI_CSN on GPIO_SD_06 (pin 69) */
  gpio_pin_config_t FPGA_SPI_CSN_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_06 (pin 69) */
  GPIO_PinInit(GPIO2, 6U, &FPGA_SPI_CSN_config);

  /* GPIO configuration of FPGA_SPI_MISO on GPIO_SD_07 (pin 68) */
  gpio_pin_config_t FPGA_SPI_MISO_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_07 (pin 68) */
  GPIO_PinInit(GPIO2, 7U, &FPGA_SPI_MISO_config);

  /* GPIO configuration of FPGA_SPI_MOSI on GPIO_SD_09 (pin 66) */
  gpio_pin_config_t FPGA_SPI_MOSI_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_09 (pin 66) */
  GPIO_PinInit(GPIO2, 9U, &FPGA_SPI_MOSI_config);

  /* GPIO configuration of FPGA_SPI_CLK on GPIO_SD_10 (pin 65) */
  gpio_pin_config_t FPGA_SPI_CLK_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_10 (pin 65) */
  GPIO_PinInit(GPIO2, 10U, &FPGA_SPI_CLK_config);

  /* GPIO configuration of SPIFLASH_HOLDN on GPIO_SD_11 (pin 64) */
  gpio_pin_config_t SPIFLASH_HOLDN_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_11 (pin 64) */
  GPIO_PinInit(GPIO2, 11U, &SPIFLASH_HOLDN_config);

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_01_GPIOMUX_IO01,            /* GPIO_01 is configured as GPIOMUX_IO01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_02_GPIOMUX_IO02,            /* GPIO_02 is configured as GPIOMUX_IO02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_03_GPIOMUX_IO03,            /* GPIO_03 is configured as GPIOMUX_IO03 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_09_GPIOMUX_IO09,            /* GPIO_09 is configured as GPIOMUX_IO09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_10_GPIOMUX_IO10,            /* GPIO_10 is configured as GPIOMUX_IO10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_00_GPIOMUX_IO14,         /* GPIO_AD_00 is configured as GPIOMUX_IO14 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_01_GPIOMUX_IO15,         /* GPIO_AD_01 is configured as GPIOMUX_IO15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_07_GPIOMUX_IO21,         /* GPIO_AD_07 is configured as GPIOMUX_IO21 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_06_GPIO2_IO06,           /* GPIO_SD_06 is configured as GPIO2_IO06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_07_GPIO2_IO07,           /* GPIO_SD_07 is configured as GPIO2_IO07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_09_GPIO2_IO09,           /* GPIO_SD_09 is configured as GPIO2_IO09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_10_GPIO2_IO10,           /* GPIO_SD_10 is configured as GPIO2_IO10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_11_GPIO2_IO11,           /* GPIO_SD_11 is configured as GPIO2_IO11 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(IOMUXC_GPR_GPR26_GPIO_SEL_MASK)))      /* Mask bits to zero which are setting */
      | IOMUXC_GPR_GPR26_GPIO_SEL(0x00U)      /* Select GPIO1 or GPIO2: 0x00U */
    );
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_01_GPIOMUX_IO01,            /* GPIO_01 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_02_GPIOMUX_IO02,            /* GPIO_02 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_03_GPIOMUX_IO03,            /* GPIO_03 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_09_GPIOMUX_IO09,            /* GPIO_09 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_10_GPIOMUX_IO10,            /* GPIO_10 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_00_GPIOMUX_IO14,         /* GPIO_AD_00 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_01_GPIOMUX_IO15,         /* GPIO_AD_01 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_07_GPIOMUX_IO21,         /* GPIO_AD_07 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_06_GPIO2_IO06,           /* GPIO_SD_06 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_07_GPIO2_IO07,           /* GPIO_SD_07 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_09_GPIO2_IO09,           /* GPIO_SD_09 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_10_GPIO2_IO10,           /* GPIO_SD_10 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_11_GPIO2_IO11,           /* GPIO_SD_11 PAD functional properties : */
      0xA0U);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/4
                                                 Speed Field: fast(150MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
}

void fpga_init(void)
{
    fpga_InitPins();

    GPIO_PinWrite(BOARD_INITPINS_LED_R_PERIPHERAL,
              BOARD_INITPINS_LED_R_CHANNEL, 1);
    GPIO_PinWrite(BOARD_INITPINS_LED_G_PERIPHERAL,
              BOARD_INITPINS_LED_G_CHANNEL, 1);
    GPIO_PinWrite(BOARD_INITPINS_LED_B_PERIPHERAL,
              BOARD_INITPINS_LED_B_CHANNEL, 1);

    // Assert D3/HOLDN that is connected to the spiflash
    GPIO_PinWrite(BOARD_INITPINS_SPIFLASH_HOLDN_GPIO,
              BOARD_INITPINS_SPIFLASH_HOLDN_PIN, 0);

    GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_HOLDN_PERIPHERAL,
              BOARD_INITPINS_FPGA_SPI_HOLDN_CHANNEL, 1);

    GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CSN_GPIO,
              BOARD_INITPINS_FPGA_SPI_CSN_PIN, 1);

    GPIO_PinWrite(BOARD_INITPINS_FPGA_SPI_CLK_GPIO,
              BOARD_INITPINS_FPGA_SPI_CLK_PIN, 0);

    GPIO_PinWrite(BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL,
              BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL, 1);

}