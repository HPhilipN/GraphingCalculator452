#ifndef MACROS_H
#define MACROS_H
/*
*
*
*
*
*
*
*
*/

#include <stdio.h>
#include <xil_io.h>
#include <sleep.h>
#include "xiicps.h"
#include <xil_printf.h>
#include <xparameters.h>
#include "xgpio.h"
#include "xuartps.h"
#include "stdlib.h"
#include "xil_mmu.h"
#include "stdbool.h"


//Included IP Libraries 
#include "sdcard.h"
#include "audio.h"
//GPIO interrupt definitions
//XGpio Gpio; // Gpio instance for buttons and switches
//XGpio BTNInst;
//XScuGic INTCInst;
#define UART_BASEADDR XPAR_PS7_UART_1_BASEADDR

#define BUTTON_SWITCH_BASE XPAR_GPIO_1_BASEADDR
//#define LED_BASE XPAR_LED_CONTROLLER_0_S00_AXI_BASEADDR
#define BUTTON_SWITCH_ID XPAR_GPIO_1_DEVICE_ID
#define BUTTON_CHANNEL 1
#define SWITCH_CHANNEL 2

//wake up processor 1
#define sev() __asm__("sev")
#define ARM1_STARTADDR 0xFFFFFFF0
#define ARM1_BASEADDR 0x10080000
#define COMM_VAL (*(volatile unsigned long*)(0xFFFF0034))
extern u32 MMUTable;

//Audio Handling
#define TASKCMPLT  (*(volatile bool*)(0xFFFF0000))
#define ERRORBNDS  (*(volatile bool*)(0xFFFF0004))
#define AUDIOERROR (*(volatile bool*)(0xFFFF0008))
#define AUDIOCLICK (*(volatile bool*)(0xFFFF000c))
#define AUDIOSEL   (*(volatile bool*)(0xFFFF0010))



//Button Flags
#define BTNUP (*(volatile bool*)(0xFFFF0020))
#define BTNDOWN (*(volatile bool*)(0xFFFF0024))
#define BTNLEFT (*(volatile bool*)(0xFFFF0028))
#define BTNRIGHT (*(volatile bool*)(0xFFFF002c))
#define BTNSEL   (*(volatile bool*)(0xFFFF0030))

#endif
