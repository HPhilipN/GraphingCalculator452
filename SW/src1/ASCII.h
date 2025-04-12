#ifndef ASCII_H
#define ASCII_H

#include <cstdint>

#define ASCII_Width 200
#define ASCII_Height 160                                                        
#define Tile_Width 20
#define Tile_Height 32
#define Tile_count 50

#define ASCII_IMAGE_BASE 0x018D2008

// Char matrix mapping (index-based lookup)
// const char CharMatrix[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ|&:~+-*/[]=!<>1234567890 ";
// Tile struct to hold the pixel data of each ASCII disaplay character
// Included a mapping function that takes the Char value and returns the tile index 

//Define the ASCII tiles
// 50 tiles, 32 rows, 20 columns, 4 bytes per pixel
extern const uint8_t ASCII_TILES[50][32][20];
extern char stringBuffer[];
// function prototypes
//Returns index of the tile in the ASCII array
int GetTileIndex(char c);

#endif // ASCII_H
