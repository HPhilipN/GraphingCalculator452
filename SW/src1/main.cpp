#include "macros.h"
#include "UART.h"
#include "ASCII.h"
#include "textbox.h"
#include "VGA.h"
#include "Keyb.h"
#include "interrupts.h"
#include "logic.h"

#include <cmath>    // For log()
#include <cstring>  // For memcpy, memset
#include <stack>

//Define Terminal position
TextBox TerminalMain = {
    TerminalMain.posX = 880,
    TerminalMain.posY = 530,
};

//removes caching from buffers
inline void flushFrame() {
    memcpy(vidyaBuffer, drawBuffer, VIDEO_SIZE);
    Xil_DCacheFlush();
}

//Wr2Terminal - handles center button behavior
//Enter: transfers terminal data into a parsing buffer to later be evaluated and drawn onto the screen
//Del: removes the current character from the buffer
//Default: adds current character to the buffer

void Wr2Terminal() {
    std::string keyVal = keybCursor.getCurrentKeyValue();
    xil_printf("Pressed: %s\n\r", keyVal.c_str());

    if (keyVal == "ENTER") {
        if (TerminalMain.text[0] != '\0') {
            strcpy(CharBuffer, TerminalMain.text);
            xil_printf("Evaluating: %s\n\r", CharBuffer);

            clearScreen();
            drawGrid();
            readAndDraw(CharBuffer);
            flushFrame();
            TerminalMain.clear();
        } else {
            xil_printf("Empty input — skipping evaluation\n\r");
        }
    } else if (keyVal == "DEL") {
        TerminalMain.backspace();
    } else {
        for (char c : keyVal)
            TerminalMain.addChar(c);
    }
}


int main(void){
    //status for GPIO and interrupts
    xil_printf("CPU1 Rendering Video....\n\r");
    int status;
    
    // GPIO Init 
    gpio_init();
    status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
    if (status != XST_SUCCESS) return XST_FAILURE;

    XGpio_SetDataDirection(&BTNInst, 1, 0xFF);
    status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
    if (status != XST_SUCCESS) return XST_FAILURE;
    //init terminal position
    xil_printf("CPU1 interrupts....\n\r");

    int x = 900;
    int y = 850;

//  Buffer and address initializaitons
    Xil_SetTlbAttributes(VGA_BASE_ADDR, 0x11DE2);
    Xil_SetTlbAttributes(DRAW_PX_ADDR, 0x11DE2);

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
    
    //set flags
    TASKCMPLT = false;
    ERRORBNDS = false;
    AUDIOERROR = false;
    AUDIOCLICK = false;
    AUDIOSEL = false;
    
    //Render initializations
    initKeybGraph();       // Build key layout
    clearScreen();         // Clear the screen
    drawGrid();            // Draw the graph grid

    flushFrame();

   while(1){

       //update keyboard status and terminal status
       updateKeyb(x, y);
       TerminalMain.draw(colorHex(1,0,255,0), colorHex(255,10,10,10));
       flushFrame();
       
//Flags triggered by core 0
       if(BTNUP){
           keybCursor.moveUp();
           xil_printf("CPU1 UP\n\r");
           BTNDOWN = false;
       }
       if(BTNDOWN){
           keybCursor.moveDown();
           xil_printf("CPU1 DOWN\n\r");
           BTNDOWN = false;
       }
       if(BTNRIGHT){
           keybCursor.moveRight();
           xil_printf("CPU1 RIGHT\n\r");
           BTNRIGHT = false;
       }
       if(BTNLEFT){
           keybCursor.moveLeft();
           xil_printf("CPU1 LEFT\n\r");
           BTNLEFT = false;
       }
       if(BTNSEL){
           Wr2Terminal();
           xil_printf("CPU1 SEL\n\r");
           BTNSEL = false;
       }

// Interrupts are being weird so i cant use this 
//             switch (btn_value) {
//                 case 0b10000:
//                          keybCursor.moveUp();
//                         xil_printf("CPU0 UP\n\r");
// //                        BTNUP = true;
//                     break;
//                 case 0b00010:
//                          keybCursor.moveDown();
//                         xil_printf("CPU0 DOWN\n\r");
// //                        BTNDOWN = true;
//                     break;
//                 case 0b00100:
//                          keybCursor.moveLeft();
//                         xil_printf("CPU0 LEFT\n\r");
// //                        BTNLEFT = true;
//                     break;
//                 case 0b01000:
//                          keybCursor.moveRight();
// //                        BTNRIGHT = true;
//                     break;
//                 case 0b00001:
//                          Wr2Terminal();
// //                        BTNSEL = true;
//                     break;
//                 default:
//                     break;
//                 }

        flushFrame();

   }

    return 0;
}
