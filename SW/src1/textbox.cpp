#include "textbox.h"
#include "VGA.h"
#include "UART.h"
#include <string>

// TextBox terminal;
char CharBuffer[CharMax] = {0};
int CharIndex = 0;

//Draws the textbox and characters: 
void TextBox::draw(uint32_t textColor, uint32_t bgColor){
    int textWidth = col * Tile_Width;
    int textHeight = row * Tile_Height;
    drawRect(posX - 10, posY -10, textWidth + 10, textHeight + 10, colorHex(0, 101, 247, 99));
    drawRect(posX - 5 , posY - 5, textWidth, textHeight, bgColor);


    for(int i = 0; i < carat; i++){
        int rows = i / col;
        int cols = i % col;

        int xPos = posX - 5  + cols * Tile_Width;
        int yPos = posY - 5 + rows * Tile_Height;
        
        drawChar(text[i], xPos, yPos, textColor);
    }
}

//adds char to buffer and moves the carat position
void TextBox::addChar(char c){
    if(carat < col * row && c >= 32 && c <= 126){
        text[carat++] = c;
        text[carat] = '\0';
    }
}

//removes char from buffer
void TextBox::backspace(){
    if(carat > 0){
        carat--;
        text[carat] = '\0';
    }
}

//clears the buffer and resets the carat position
void TextBox::clear(){
    std::memset(text, 0, sizeof(text));
    carat = 0;
}
