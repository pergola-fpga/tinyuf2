
/*
 * Code for interacting with the FPGA via JTAG.
 * This file is part of LUNA.
 *
 * This JTAG driver is intended to be as simple as possible in order to facilitate
 * configuration and debugging of the attached FPGA. It is not intended to be a general-
 * purpose JTAG link.
 */

#ifndef __JTAG_H__
#define __JTAG_H__

typedef enum e_TAPState
{
	STATE_TEST_LOGIC_RESET =  0,
	STATE_RUN_TEST_IDLE    =  1,
	STATE_SELECT_DR_SCAN   =  2,
	STATE_CAPTURE_DR       =  3,
	STATE_SHIFT_DR         =  4,
	STATE_EXIT1_DR         =  5,
	STATE_PAUSE_DR         =  6,
	STATE_EXIT2_DR         =  7,
	STATE_UPDATE_DR        =  8,
	STATE_SELECT_IR_SCAN   =  9, 
	STATE_CAPTURE_IR       = 10,
	STATE_SHIFT_IR         = 11,
	STATE_EXIT1_IR         = 12,
	STATE_PAUSE_IR         = 13,
	STATE_EXIT2_IR         = 14,
	STATE_UPDATE_IR        = 15
} jtag_tap_state_t;


/**
 * Performs the start-of-day tasks necessary to talk JTAG to our FPGA.
 */
void jtag_init(void);

/**
 * De-inits the JTAG connection, so the JTAG chain. is no longer driven.
 */
void jtag_deinit(void);


/**
 * Moves to a given JTAG state.
 */
void jtag_goto_state(int state);


/**
 * Performs a raw TAP scan.
 */
void jtag_tap_shift(
	uint8_t *input_data,
	uint8_t *output_data,
	uint32_t data_bits,
	bool must_end);

void jtag_error(int status);

void jtag_wait_time(uint32_t microseconds);

void jtag_go_to_state(unsigned state);

uint8_t jtag_current_state(void);

#endif