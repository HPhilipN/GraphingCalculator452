#ifndef VGA_H
#define VGA_H
#pragma once

/* Main Graphics Handler
*  All graphical functions are defined here
*  Handles any animations or graphical elements
*  Memory addresses for video are defined here
*  Parts of the implementation are credited to Manraj Specifically the
*  method for implementing the ASCII characters and drawing assets onto the screen
*/
//Libraries
#include "macros.h"
#include "ASCII.h"
#include <string>

//resolution definitions
#define RWidth 1280
#define RHeight 1024

//Address definitions for fast access
#define VGA_BASE_ADDR 0x10980000
#define DRAW_PX_ADDR 0x119D2008
// #define ASCII_ADDR 0x018D2008
 
extern int VIDEO_SIZE;  // 1280 * 1024 * 4 bytes

//main 
extern int * vidyaBuffer;
//other Buffers
extern int * drawBuffer;
extern int * animationBuffer0;
extern int * KeyboardBuffer0;

//resource pack for different color schemes
struct ColorPack{
    //text color scheme
    uint32_t text0;
    uint32_t text1;
    //color scheme for keyboard/buttons
    uint32_t key0;
    uint32_t key1;
    uint32_t selected0;
    uint32_t selected1;
    uint32_t dimmed0;
    uint32_t dimmed1;
    //color scheme for background
    uint32_t bgterminal;
    uint32_t border;
    uint32_t bgmain;
    //color scheme for graph
    uint32_t grid;
    uint32_t plot;
    
};

extern const ColorPack Darkmode;
extern const ColorPack Lightmode;
extern const ColorPack Redmode;
extern const ColorPack Greenmode;
extern const ColorPack Bluemode;

//function prototypes
uint32_t colorHex(int A,int R, int G, int B);
void drawPx(int x, int y, uint32_t color);
void drawChar(char c, int xPos, int yPos, uint32_t color);
void drawstring(int xPos, int yPos, uint32_t color);
void drawRect(int x, int y, int width, int height, uint32_t color);
void clearScreen();
void RenderStart();




#endif
