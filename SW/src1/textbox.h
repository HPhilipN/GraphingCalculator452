#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "UART.h"
#include "ASCII.h"
#include "vga.h"
#include "macros.h"

#define Tile_Width 20
#define Tile_Height 32
#define Padding 10
#define Terminal_W 400
#define Terminal_H 288
#define Screen_Width 1280
#define Screen_Height 1024
#define CharLimit 160

#define Max_rows (Terminal_H/Tile_Height)
#define Max_cols (Terminal_W/Tile_Width)

extern char CharBuffer[];
extern int CharIndex;

struct TextBox{

    //Dimensions w/ padding
    int width = Terminal_W;
    int height  = Terminal_H;
    int padding = Padding;
    //where to put the textbox and all characters are drawn wrt the provided coordinates
    int posX, posY;
    //track current 'carat' location on the textbox
    int row = Max_rows; 
    int col = Max_cols;
    //stores the text data
    char text[CharLimit] = {0};
    //carat for string position
    int carat = 0;
    //constructors ~ methods
    void draw(uint32_t textColor, uint32_t bgColor);
    void addChar(char c);
    void backspace();
    void clear();
};

// extern TextBox terminal;

#endif
