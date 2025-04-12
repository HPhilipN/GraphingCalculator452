#include "vga.h"
#include "macros.h"
#include "ASCII.h"

//Video Buffer Sizes
int VIDEO_SIZE = 5242880;  // 1280 * 1024 * 4 bytes

//main
int * vidyaBuffer      = (int *)0x10980000;

//other Buffers
int * drawBuffer        = (int *)0x119D2008;

//display parameters
int displayWidth = 1280;
int displayHeight = 1024;
int barOffset = 0;
int barWidth = 256;

//Function to handle RGBA values and put them into a 32-bit hex-code.
uint32_t colorHex(int A,int R, int B, int G){
	return (A << 24) | (G << 16) |(B << 8) | R;
}

// changes a signle pixel in the drawBuffer to a color
// the offeset is calculated by multiplying the y coordinate by the width of the screen and adding the x coordinate
void drawPx(int x, int y, uint32_t color) {
    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
        int offset = y * displayWidth + x;
        drawBuffer[offset] = color;
    }
}

void drawRect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            drawPx(x + j, y + i, color);
        }
    }
}

//handles drawing the ASCII chars onto the screen
//takes the char input, compares it to an ASCII map and draws the corresponding pixels
void drawChar(char c, int xPos, int yPos, uint32_t color){

    int tile = GetTileIndex(c);

    for(int i = 0; i < Tile_Height; i++){
        for(int j = 0; j < Tile_Width; j++){

        	int x = xPos + j;
        	int y = yPos + i;

            if(ASCII_TILES[tile][i][j] == 1){

                drawBuffer[y * RWidth + x] = color;
            }
        }
    }
}

//makes the entire screen a single color
void clearScreen(){
    for(int x=0; x < RWidth; x++){
        for(int y=0; y < RHeight; y++){
            drawBuffer[x+1280*y] = colorHex(255, 25, 25, 25);
        }
    }
}

//Scrapped idea for Color map struct 

//const ColorPack Darkmode{
//    Darkmode.text0 = colorHex(1, 230, 230, 230),
//    Darkmode.text1 = colorHex(1, 100, 100, 100),
//    Darkmode.key0 = colorHex(1, 0, 101, 247),
//    Darkmode.key1 = colorHex(1, 0, 0, 0),
//    Darkmode.selected0 = colorHex(1, 0, 101, 247),
//    Darkmode.selected1 = colorHex(1, 0, 0, 0),
//    Darkmode.dimmed0 = colorHex(1, 0, 0, 0),
//    Darkmode.dimmed1 = colorHex(1, 0, 101, 247),
//    Darkmode.bgterminal = colorHex(1, 0, 0, 0),
//    Darkmode.border = colorHex(1, 0, 101, 247),
//    Darkmode.bgmain = colorHex(1, 0, 0, 0),
//    Darkmode.grid = colorHex(1, 0, 101, 247),
//    Darkmode.plot = colorHex(1, 0, 101, 247),
//};
