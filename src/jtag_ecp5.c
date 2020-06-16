#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "jtag.h"
#include "jtag_ecp5.h"
#include "lattice_cmds.h"


static void print_idcode(uint32_t idcode){
	for (size_t i = 0; i < sizeof(ecp_devices) / sizeof(struct ecp_device_id); i++) {
		if (idcode == ecp_devices[i].device_id) {
			printf("IDCODE: 0x%08lx (%s)\n", idcode, ecp_devices[i].device_name);
			return;
		}
	}
	printf("IDCODE: 0x%08lx does not match :(\n", idcode);
}

void jtag_ecp5_read_idcode() {

	uint8_t data[4] = {READ_ID};

	jtag_go_to_state(STATE_SHIFT_IR);
	jtag_tap_shift(data, data, 8, true);

	data[0] = 0;
	jtag_go_to_state(STATE_SHIFT_DR);
	jtag_tap_shift(data, data, 32, true);

	uint32_t idcode = 0;
	
	/* Format the IDCODE into a 32bit value */
	for(int i = 0; i< 4; i++)
		idcode = data[i] << 24 | idcode >> 8;

	print_idcode(idcode);
}