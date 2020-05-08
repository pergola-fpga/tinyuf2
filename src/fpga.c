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

static uint32_t blink_interval_ms = BOARD_BLINK_INTERVAL;

void fpga_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (board_millis() - start_ms < blink_interval_ms)
        return;

    start_ms += blink_interval_ms;

    led_state = !led_state;
    // GPIO_PinWrite(LED_GPIO_PORT, LED_GPIO_PIN - 1, led_state ? LED_STATE_ON : (1-LED_STATE_ON));
}
