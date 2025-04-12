#include "interrupts.h"
#include "macros.h"

XGpio SWInst;
XGpio BTNInst;
XScuGic INTCInst;
XGpio Gpio;

int btn_value = 0;
volatile int btnFlag = 0;
int sw_val = 0;
volatile unsigned int swFlag = 0;

// listens for button presses and set the flag to 1

void BTN_Intr_Handler(void *InstancePtr){
   XGpio_InterruptDisable(&BTNInst, BTN_INT);
   if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT) return;

   btn_value = XGpio_DiscreteRead(&BTNInst, 1);
   btnFlag = 1;
   (void)XGpio_InterruptClear(&BTNInst, BTN_INT);
   XGpio_InterruptEnable(&BTNInst, BTN_INT);
}


int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	// Enable interrupt
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
	XGpio_InterruptGlobalEnable(&BTNInst);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}

// Initialise the interrupt controller and connect the GPIO interrupt handler
int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_INTERRUPT_ID,
					  	  	 (Xil_ExceptionHandler)BTN_Intr_Handler,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID);

	return XST_SUCCESS;
}

// Initialise the GPIO peripheral for the buttons and switches
unsigned char gpio_init()
{
	int Status;

	Status = XGpio_Initialize(&Gpio, BUTTON_SWITCH_ID);
	if(Status != XST_SUCCESS) return XST_FAILURE;

//	XGpio_SetDataDirection(&Gpio, SWITCH_CHANNEL, 0xFF);
	XGpio_SetDataDirection(&Gpio, BUTTON_CHANNEL, 0xFF);

	return XST_SUCCESS;
}

