#pragma once

extern void fpga_bitstream_write(uint8_t *src, uint32_t offset, uint32_t len);
extern void fpga_bitstream_finish(void);
extern void fpga_task(void);
extern void fpga_init(void);
