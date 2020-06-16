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
#include "jtag_io.h"

/* GPIO_AD_03 (number 57), JTAG_TDO */
#define BOARD_INITPINS_JTAG_TDO_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_JTAG_TDO_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_JTAG_TDO_CHANNEL                                     17U   /*!< GPIO1 gpiomux_io channel: 17 */

/* GPIO_AD_04 (number 56), JTAG_TDI */
#define BOARD_INITPINS_JTAG_TDI_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_JTAG_TDI_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_JTAG_TDI_CHANNEL                                     18U   /*!< GPIO1 gpiomux_io channel: 18 */

/* GPIO_AD_05 (number 55), JTAG_TMS */
#define BOARD_INITPINS_JTAG_TMS_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_JTAG_TMS_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_JTAG_TMS_CHANNEL                                     19U   /*!< GPIO1 gpiomux_io channel: 19 */

/* GPIO_AD_06 (number 52), JTAG_TCK */
#define BOARD_INITPINS_JTAG_TCK_PERIPHERAL                                GPIO1   /*!< Device name: GPIO1 */
#define BOARD_INITPINS_JTAG_TCK_SIGNAL                               gpiomux_io   /*!< GPIO1 signal: gpiomux_io */
#define BOARD_INITPINS_JTAG_TCK_CHANNEL                                     20U   /*!< GPIO1 gpiomux_io channel: 20 */

static void _sleep(int n)
{
	while (n--)
		__asm volatile ("nop");
}


void jtag_io_init(void)
{
    /* GPIO configuration of JTAG_TDO on GPIO_AD_03 (pin 57) */
    gpio_pin_config_t JTAG_TDO_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_NoIntmode
    };
    /* Initialize GPIO functionality on GPIO_AD_03 (pin 57) */
    GPIO_PinInit(GPIO1, 17U, &JTAG_TDO_config);

    /* GPIO configuration of JTAG_TDI on GPIO_AD_04 (pin 56) */
    gpio_pin_config_t JTAG_TDI_config = {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_NoIntmode
    };
    /* Initialize GPIO functionality on GPIO_AD_04 (pin 56) */
    GPIO_PinInit(GPIO1, 18U, &JTAG_TDI_config);

    /* GPIO configuration of JTAG_TMS on GPIO_AD_05 (pin 55) */
    gpio_pin_config_t JTAG_TMS_config = {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_NoIntmode
    };
    /* Initialize GPIO functionality on GPIO_AD_05 (pin 55) */
    GPIO_PinInit(GPIO1, 19U, &JTAG_TMS_config);

    /* GPIO configuration of JTAG_TCK on GPIO_AD_06 (pin 52) */
    gpio_pin_config_t JTAG_TCK_config = {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_NoIntmode
    };
    /* Initialize GPIO functionality on GPIO_AD_06 (pin 52) */
    GPIO_PinInit(GPIO1, 20U, &JTAG_TCK_config);

}

void jtag_io_tms(bool value)
{
    GPIO_PinWrite(BOARD_INITPINS_JTAG_TMS_PERIPHERAL,
              BOARD_INITPINS_JTAG_TMS_CHANNEL, !!value);
}

void jtag_io_tck(void)
{
    GPIO_PinWrite(BOARD_INITPINS_JTAG_TCK_PERIPHERAL,
              BOARD_INITPINS_JTAG_TCK_CHANNEL, 0);

    _sleep(10);

    GPIO_PinWrite(BOARD_INITPINS_JTAG_TCK_PERIPHERAL,
              BOARD_INITPINS_JTAG_TCK_CHANNEL, 1);

    _sleep(10);

}

void jtag_io_tdi(bool value)
{
    GPIO_PinWrite(BOARD_INITPINS_JTAG_TDI_PERIPHERAL,
              BOARD_INITPINS_JTAG_TDI_CHANNEL, !!value);
}

uint32_t jtag_io_tdo(void)
{
    return GPIO_PinRead(BOARD_INITPINS_JTAG_TDO_PERIPHERAL,
              BOARD_INITPINS_JTAG_TDO_CHANNEL);
}

uint32_t pulse_clock_and_read_tdo(void)
{
    GPIO_PinWrite(BOARD_INITPINS_JTAG_TCK_PERIPHERAL,
              BOARD_INITPINS_JTAG_TCK_CHANNEL, 0);
    _sleep(10);

    uint32_t out = jtag_io_tdo();

    GPIO_PinWrite(BOARD_INITPINS_JTAG_TCK_PERIPHERAL,
              BOARD_INITPINS_JTAG_TCK_CHANNEL, 1);
    _sleep(10);

    return out;
}
