#pragma once

extern void fpga_bitstream_write(const uint8_t *src, uint32_t lba, uint32_t num_blocks);
extern void fpga_bitstream_finish(void);
extern void fpga_task(void);
extern void fpga_init(void);
