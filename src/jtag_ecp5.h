#pragma once

#include <stdint.h>

extern void     ecp5_jtag_print_idcode(uint32_t idcode);
extern uint32_t ecp5_jtag_read_idcode(void);
extern uint32_t ecp5_jtag_read_status_register(void);
extern void     print_status_register(uint32_t status, unsigned verbose);
extern void     ecp_jtag_cmd(uint8_t cmd);
extern void     ecp_jtag_cmd8(uint8_t cmd, uint8_t param);
