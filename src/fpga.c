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
#include "jtag.h"
#include "jtag_ecp5.h"
#include "lattice_cmds.h"

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

/* GPIO_AD_01 (number 59), FPGA_DONE */
#define BOARD_INITPINS_FPGA_DONE_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_FPGA_DONE_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_FPGA_DONE_CHANNEL                                     15U   /*!< GPIO1 gpiomux_io channel: 15 */

/* GPIO_AD_07 (number 51), FPGA_PROGRAMN */
#define BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL                            GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_FPGA_PROGRAMN_SIGNAL                           gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL                                 21U   /*!< GPIO1 gpiomux_io channel: 21 */

static void sleep(int n)
{
    while (n--)
        __asm volatile ("nop");
}

static void fpga_start_program(void)
{
    // printf("start\n");

    // Toggle PROGRAMN to reset the FPGA. Sleeps are needed.
    GPIO_PinWrite
        (BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL,
            BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL, 0);
    sleep(1000000);
    GPIO_PinWrite
        (BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL,
            BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL, 1);
    sleep(1000000);

    jtag_init();
    sleep(1000000);

    ecp5_jtag_read_idcode();
    sleep(1000000);

    ecp5_jtag_read_status_register();
    sleep(1000000);

    ecp_jtag_cmd8(ISC_ENABLE, 0);
    sleep(1000000);

    ecp_jtag_cmd8(ISC_ERASE, 1);
    sleep(1000000);

    ecp_jtag_cmd8(LSC_RESET_CRC, 0);
    sleep(1000000);

    ecp_jtag_cmd8(LSC_INIT_ADDRESS, 1);

    ecp_jtag_cmd(LSC_BITSTREAM_BURST);

    jtag_go_to_state(STATE_CAPTURE_DR);
}

static uint8_t bit_reverse(uint8_t in){

    uint8_t out =  (in & 0x01) ? 0x80 : 0x00;
            out |= (in & 0x02) ? 0x40 : 0x00;
            out |= (in & 0x04) ? 0x20 : 0x00;
            out |= (in & 0x08) ? 0x10 : 0x00;
            out |= (in & 0x10) ? 0x08 : 0x00;
            out |= (in & 0x20) ? 0x04 : 0x00;
            out |= (in & 0x40) ? 0x02 : 0x00;
            out |= (in & 0x80) ? 0x01 : 0x00;

    return out;
}

static int bytes_transferred;
void fpga_bitstream_write(uint8_t *src, uint32_t offset, uint32_t len)
{
    // printf("Got %p %ld %ld\r\n", src, offset, len);
    if (offset == 0) {
        fpga_start_program();
        bytes_transferred = 0;
    }

    for(uint32_t i = 0; i < len; i++){
        src[i] = bit_reverse(src[i]);
    }

    jtag_tap_shift(src, NULL, len * 8, false, false);

    bytes_transferred += len;
}

extern void fpga_bitstream_finish(void)
{
    // printf("finish\n");

    ecp_jtag_cmd(ISC_DISABLE);

    GPIO_PinWrite(BOARD_INITPINS_LED_B_PERIPHERAL,
                  BOARD_INITPINS_LED_B_CHANNEL, 1);

    ecp5_jtag_read_status_register();

    bytes_transferred = 0;
}

void fpga_task(void)
{
    // LED status
    // Red:   Not done
    // Green: Done (is on while JTAG is enabled)
    // Blue:  Transferring

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

    GPIO_PinWrite(BOARD_INITPINS_LED_B_PERIPHERAL,
                  BOARD_INITPINS_LED_B_CHANNEL,
                  ((bytes_transferred & 0x8000) < 0x4000));
}

void fpga_init_pins(void) {
  /* GPIO configuration of LED_G on GPIO_01 (pin 12) */
  gpio_pin_config_t LED_G_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 1U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_01 (pin 12) */
  GPIO_PinInit(GPIO1, 1U, &LED_G_config);

  /* GPIO configuration of LED_R on GPIO_02 (pin 11) */
  gpio_pin_config_t LED_R_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 1U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_02 (pin 11) */
  GPIO_PinInit(GPIO1, 2U, &LED_R_config);

  /* GPIO configuration of LED_B on GPIO_03 (pin 10) */
  gpio_pin_config_t LED_B_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 1U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_03 (pin 10) */
  GPIO_PinInit(GPIO1, 3U, &LED_B_config);

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
}

void fpga_init(void)
{
    fpga_init_pins();

    GPIO_PinWrite(BOARD_INITPINS_LED_R_PERIPHERAL,
                  BOARD_INITPINS_LED_R_CHANNEL, 1);
    GPIO_PinWrite(BOARD_INITPINS_LED_G_PERIPHERAL,
                  BOARD_INITPINS_LED_G_CHANNEL, 1);
    GPIO_PinWrite(BOARD_INITPINS_LED_B_PERIPHERAL,
                  BOARD_INITPINS_LED_B_CHANNEL, 1);
    GPIO_PinWrite(BOARD_INITPINS_FPGA_PROGRAMN_PERIPHERAL,
                  BOARD_INITPINS_FPGA_PROGRAMN_CHANNEL, 0);
}