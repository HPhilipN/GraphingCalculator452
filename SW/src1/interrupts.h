#ifndef INTERRUPTS_H
#define INTERRUPTS_H
/*

This Interrupt file is used from the Audio 452 tutorial
main changes were to remove unused functions and variables

*/



#include "macros.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "sleep.h"

// Parameter definitions
#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID		XPAR_AXI_GPIO_1_DEVICE_ID
#define BUTTON_SWITCH_BASE XPAR_GPIO_1_BASEADDR


#define INTC_GPIO_INTERRUPT_ID XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR

#define BTN_INT 			XGPIO_IR_CH1_MASK

#define SWITCH_CHANNEL 2

extern XGpio SWInst;
extern XGpio BTNInst;
extern XGpio SWInst;
extern XScuGic INTCInst;
extern int btn_value;
extern volatile int btnFlag;
//volatile int btnFlag;
//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
unsigned char sw_init();
unsigned int read_switches();

void BTN_Intr_Handler(void *baseaddr_p);
int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);
unsigned char gpio_init();


#endif
