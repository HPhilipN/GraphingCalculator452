#include "main.h"
#include "macros.h"
#include "interrupts.h"

/* ---------------------------------------------------------------------------- *
 * 								MAIN Audio Handler								*
 * ---------------------------------------------------------------------------- */

int main(void) {
    xil_printf("CPU 0: Entering Main\r\n");
    
    int status;
    IicConfig(XPAR_XIICPS_0_DEVICE_ID);                // Configure the IIC data structure
    AudioPllConfig();                                  // Configure the Audio Codec's PLL
    AudioConfigureJacks();                             // Configure line in/out ports, configure HP jack
    
    // Disable caching on the shared memory addresses
    Xil_SetTlbAttributes((u32)(&TASKCMPLT), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&ERRORBNDS), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&AUDIOERROR), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&AUDIOCLICK), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&AUDIOSEL), 0x11DE2);
    
    Xil_SetTlbAttributes((u32)(&BTNUP), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&BTNDOWN), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&BTNLEFT), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&BTNRIGHT), 0x11DE2);
    Xil_SetTlbAttributes((u32)(&BTNSEL), 0x11DE2);
    xil_printf("CPU Shared Addresses initialized\n\r");

    //Wake up CPU 1
    Xil_OUT32(ARM1_STARTADDR, ARM1_BASEADDR);
    dmb();
    sev();

    //set flags
    COMM_VAL = 0;
    TASKCMPLT = false;
    ERRORBNDS = false;
    AUDIOERROR = false;
    AUDIOCLICK = false;
    AUDIOSEL = false;

    BTNUP = false;
    BTNDOWN = false;
    BTNLEFT = false;
    BTNRIGHT = false;
    BTNSEL = false;

    // GPIO Init 
    gpio_init();
    status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
    if (status != XST_SUCCESS) return XST_FAILURE;

    XGpio_SetDataDirection(&BTNInst, 1, 0xFF);
    status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
    if (status != XST_SUCCESS) return XST_FAILURE;
    xil_printf("GPIO peripheral configured\r\n");

    // Initialize SD card and load WAV files
    sd_card_initialization();

    xil_printf("Listening for memory triggers...\r\n");
    COMM_VAL = 1;
    xil_printf("CPU 0: Finished setup\r\n");

    while (!XUartPs_IsReceiveData(UART_BASEADDR)) {
        //
        switch (btn_value) {
            case 0b10000:
                    // BTNUP = true;
                break;
            case 0b00010:
                    // BTNDOWN = true;
                break;
            case 0b00100:
                    // BTNLEFT = true;
                break;
            case 0b01000:
                    // BTNRIGHT = true;
                break;
            case 0b00001:
                    // BTNSEL = true;
                break;
            default:
                break;
            }

        // Code cluster borrowed and modified from Nick
        // listens for audio flags and plays the corresponding audio file
        if(TASKCMPLT){
            playWavFile(1);
            xil_printf("task Complete!");
            TASKCMPLT = false;
        }
        if(ERRORBNDS){
            playWavFile(2);
            xil_printf("ERROR OUT OF BOUNDs!");
            ERRORBNDS = false;
        }
        if(AUDIOERROR){
            playWavFile(3);
            xil_printf("ERROR!");
            AUDIOERROR = false;
        }
        if(AUDIOCLICK){
            playWavFile(4);
            xil_printf("MOVE!");
            AUDIOCLICK = false;
        }
        if(AUDIOSEL){
            playWavFile(5);
            xil_printf("SELECTING!");
            AUDIOSEL = false;
        }


    }




    return 0;
}
