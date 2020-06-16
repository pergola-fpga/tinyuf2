#pragma once

extern void jtag_io_init(void);
extern void jtag_io_tms(bool value);
extern void jtag_io_tck(void);
extern uint32_t jtag_io_tdo(void);
extern void jtag_io_tdi(bool value);
