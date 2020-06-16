#pragma once

extern void jtag_ecp5_read_idcode(void);
extern void read_status_register(void);
extern void ecp_jtag_cmd(uint8_t cmd);
extern void ecp_jtag_cmd8(uint8_t cmd, uint8_t param);
