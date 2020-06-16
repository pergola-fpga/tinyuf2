#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "jtag.h"
#include "jtag_ecp5.h"
#include "lattice_cmds.h"

struct ecp_device_id {
	const char* device_name;
	uint32_t    device_id;
};

const struct ecp_device_id ecp_devices[] =
{
	{"LFE5U-12"   , 0x21111043 },
	{"LFE5U-25"   , 0x41111043 },
	{"LFE5U-45"   , 0x41112043 },
	{"LFE5U-85"   , 0x41113043 },
	{"LFE5UM-25"  , 0x01111043 },
	{"LFE5UM-45"  , 0x01112043 },
	{"LFE5UM-85"  , 0x01113043 },
	{"LFE5UM5G-25", 0x81111043 },
	{"LFE5UM5G-45", 0x81112043 },
	{"LFE5UM5G-85", 0x81113043 }
};

static void print_idcode(uint32_t idcode){
	for (size_t i = 0; i < sizeof(ecp_devices) / sizeof(struct ecp_device_id); i++) {
		if (idcode == ecp_devices[i].device_id) {
			// printf("IDCODE: 0x%08lx (%s)\n", idcode, ecp_devices[i].device_name);
			printf("%s\n", ecp_devices[i].device_name);
			return;
		}
	}
	printf("IDCODE: 0x%08lx does not match :(\n", idcode);
}

void jtag_ecp5_read_idcode() {

	uint8_t data[4] = {READ_ID};

	jtag_go_to_state(STATE_SHIFT_IR);
	jtag_tap_shift(data, data, 8, true, true);

	data[0] = 0;
	jtag_go_to_state(STATE_SHIFT_DR);
	jtag_tap_shift(data, data, 32, true, true);

	uint32_t idcode = 0;
	
	/* Format the IDCODE into a 32bit value */
	for(int i = 0; i< 4; i++)
		idcode = data[i] | idcode << 8;

	print_idcode(idcode);
}


static void print_status_register(uint32_t status){	
	// printf("ECP5 Status Register: 0x%08lx\n", status);
	printf("SR=0x%08lx\n", status);
    int verbose = 0;
	if(verbose){
		printf("  Transparent Mode:   %s\n",  status & (1 << 0)  ? "Yes" : "No" );
		printf("  Config Target:      %s\n",  status & (7 << 1)  ? "eFuse" : "SRAM" );
		printf("  JTAG Active:        %s\n",  status & (1 << 4)  ? "Yes" : "No" );
		printf("  PWD Protection:     %s\n",  status & (1 << 5)  ? "Yes" : "No" );
		printf("  Decrypt Enable:     %s\n",  status & (1 << 7)  ? "Yes" : "No" );
		printf("  DONE:               %s\n",  status & (1 << 8)  ? "Yes" : "No" );
		printf("  ISC Enable:         %s\n",  status & (1 << 9)  ? "Yes" : "No" );
		printf("  Write Enable:       %s\n",  status & (1 << 10) ? "Writable" : "Not Writable");
		printf("  Read Enable:        %s\n",  status & (1 << 11) ? "Readable" : "Not Readable");
		printf("  Busy Flag:          %s\n",  status & (1 << 12) ? "Yes" : "No" );
		printf("  Fail Flag:          %s\n",  status & (1 << 13) ? "Yes" : "No" );
		printf("  Feature OTP:        %s\n",  status & (1 << 14) ? "Yes" : "No" );
		printf("  Decrypt Only:       %s\n",  status & (1 << 15) ? "Yes" : "No" );
		printf("  PWD Enable:         %s\n",  status & (1 << 16) ? "Yes" : "No" );
		printf("  Encrypt Preamble:   %s\n",  status & (1 << 20) ? "Yes" : "No" );
		printf("  Std Preamble:       %s\n",  status & (1 << 21) ? "Yes" : "No" );
		printf("  SPIm Fail 1:        %s\n",  status & (1 << 22) ? "Yes" : "No" );
		
		uint8_t bse_error = (status & (7 << 23)) >> 23;
		switch (bse_error){
			case 0b000: printf("  BSE Error Code:     No Error (0b000)\n"); break;
			case 0b001: printf("  BSE Error Code:     ID Error (0b001)\n"); break;
			case 0b010: printf("  BSE Error Code:     CMD Error - illegal command (0b010)\n"); break;
			case 0b011: printf("  BSE Error Code:     CRC Error (0b011)\n"); break;
			case 0b100: printf("  BSE Error Code:     PRMB Error - preamble error (0b100)\n"); break;
			case 0b101: printf("  BSE Error Code:     ABRT Error - configuration aborted by the user (0b101)\n"); break;
			case 0b110: printf("  BSE Error Code:     OVFL Error - data overflow error (0b110)\n"); break;
			case 0b111: printf("  BSE Error Code:     SDM Error - bitstream pass the size of SRAM array (0b111)\n"); break;
		}

		printf("  Execution Error:    %s\n",  status & (1 << 26) ? "Yes" : "No" );
		printf("  ID Error:           %s\n",  status & (1 << 27) ? "Yes" : "No" );
		printf("  Invalid Command:    %s\n",  status & (1 << 28) ? "Yes" : "No" );
		printf("  SED Error:          %s\n",  status & (1 << 29) ? "Yes" : "No" );
		printf("  Bypass Mode:        %s\n",  status & (1 << 30) ? "Yes" : "No" );
		printf("  Flow Through Mode:  %s\n",  status & (1 << 31) ? "Yes" : "No" );
	}
}

void read_status_register(void) {

	uint8_t data[4] = {LSC_READ_STATUS};

	jtag_go_to_state(STATE_SHIFT_IR);
	jtag_tap_shift(data, data, 8, true, true);

	data[0] = 0;
	jtag_go_to_state(STATE_SHIFT_DR);
	jtag_tap_shift(data, data, 32, true, true);
	//jtag_go_to_state(STATE_PAUSE_DR);

	uint32_t status = 0;
	
	/* Format the IDCODE into a 32bit value */
	for(int i = 0; i< 4; i++)
		status = data[i] << 24 | status >> 8;

	print_status_register(status);
}

void ecp_jtag_cmd(uint8_t cmd){
    uint8_t data[1] = {cmd};

    jtag_go_to_state(STATE_SHIFT_IR);
    jtag_tap_shift(data, data, 8, true, true);

    jtag_go_to_state(STATE_RUN_TEST_IDLE);
    jtag_wait_time(32);	
}

void ecp_jtag_cmd8(uint8_t cmd, uint8_t param){
    uint8_t data[1] = {cmd};

    jtag_go_to_state(STATE_SHIFT_IR);
    jtag_tap_shift(data, data, 8, true, true);

    data[0] = param;
    jtag_go_to_state(STATE_SHIFT_DR);
    jtag_tap_shift(data, data, 8, true, true);

    jtag_go_to_state(STATE_RUN_TEST_IDLE);
    jtag_wait_time(32);	
}

void ecp_jtag_enter_spi_background_mode(void) {

	uint8_t data[4] = {0x3A};

	jtag_go_to_state(STATE_SHIFT_IR);
	jtag_tap_shift(data, data, 8, true, true);

	/* These bytes seem to be required to un-lock the SPI interface */
	data[0] = 0xFE;
	data[1] = 0x68;
	jtag_go_to_state(STATE_SHIFT_DR);
	jtag_tap_shift(data, data, 16, true, true);

	/* Entering IDLE is essential */
	jtag_go_to_state(STATE_RUN_TEST_IDLE);
}
