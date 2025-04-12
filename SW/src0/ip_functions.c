/*
 * ip_functions.c
 *
 * Contains all functions which pertain to setup and use of IP periperals.
 */

#include "main.h"


/* ---------------------------------------------------------------------------- *
 * 								audio_stream()									*
 * ---------------------------------------------------------------------------- *
 * This function performs audio loopback streaming by sampling the input audio
 * from the codec and then immediately passing the sample to the output of the
 * codec.
 *
 * The main menu can be accessed by entering 'q' on the keyboard.
 * ---------------------------------------------------------------------------- */
void audio_stream(){
	u32  in_left, in_right;

	while (1){
		// Read audio input from codec
		in_left = Xil_In32(I2S_DATA_RX_L_REG);
		in_right = Xil_In32(I2S_DATA_RX_R_REG);
		// Write audio output to codec
		Xil_Out32(I2S_DATA_TX_L_REG, in_left);
		Xil_Out32(I2S_DATA_TX_R_REG, in_right);
	}

	/* If input from the terminal is 'q', then return to menu.
	 * Else, continue streaming. */

} // audio_stream()


/* ---------------------------------------------------------------------------- *
 * 								gpio_initi()									*
 * ---------------------------------------------------------------------------- *
 * Initialises the GPIO driver for the push buttons and switches.
 * ---------------------------------------------------------------------------- */




