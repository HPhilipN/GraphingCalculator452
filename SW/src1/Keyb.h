#ifndef KEYB_H
#define KEYB_H

#include <string>
#include <vector>

struct Key {
    std::string val;
    std::string id;

    int col, row;
    
    Key* up = nullptr;
    Key* down = nullptr;
    Key* left = nullptr;
    Key* right = nullptr;

    bool cursorOn = false;
};

//Cursor object to track the current key
struct Cursor{
    Key* current;

    Cursor(Key* startKey);
    void moveTo(Key* next);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    std::string getCurrentKeyValue() const;
    std::string pressCurrentKey() const;
   
};



//initialize keyboard data structure
void initKeybGraph();
//draw the keyboard
void renderKeyb(int xPos, int yPos);
void updateKeyb(int xPos, int yPos);
//
void handleArrowInput(char direction);
//load string into string buffer for parser
//load string from data structure with current key = 1
std::string pressCurrentKey();

extern Cursor keybCursor;

#endif
