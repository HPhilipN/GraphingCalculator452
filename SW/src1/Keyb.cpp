#include "Keyb.h"
#include "VGA.h"
#include "textbox.h"
#include <cstring>
#include <unordered_map>
#include "macros.h"
#include "interrupts.h"

#define KEY_WIDTH 24
#define KEY_HEIGHT 36
#define BORDER_X 10
#define BORDER_Y 10
#define TILE_STEP_X (24 + 6)
#define TILE_STEP_Y (36 + 10)
/*
*     Keyboard Layout:
*   |------------------|
*   [<--][ 7 ][ 8 ][ 9 ]
*   [-->][ 4 ][ 5 ][ 6 ][ 0 ][ + ][ * ][ [ ][ ] ]
*        [ 1 ][ 2 ][ 3 ][ . ][ - ][ / ][sin][log]
*        |--------------------------------------|
*
*/
//create the keyboard datastructure

std::unordered_map<std::string, Key> keybMap;

Cursor keybCursor(nullptr);

void initKeybGraph(){
    //initialize the keyboard layout
    keybMap["del"] = {"DEL", "<", 0, 0};
    keybMap["7"] = {"7", "7", 1, 0};
    keybMap["8"] = {"8", "8", 2, 0};
    keybMap["9"] = {"9", "9", 3, 0};
    keybMap["X"] = {"X", "X", 4, 0};
    keybMap[ "panL"] = {"PNAL", "-", 7,0};
    keybMap[ "panR"] = {"PANR", "+", 8,0};

    keybMap["ZOOMOUT"] = {"ZOOMOUT", "-", 10, 2};
    keybMap["ZOOMIN"] = {"ZOOMIN", "+", 10, 1};

    keybMap["enter"] = {"ENTER", ">", 0, 1};
    keybMap["4"] = {"4", "4", 1, 1};
    keybMap["5"] = {"5", "5", 2, 1};
    keybMap["6"] = {"6", "6", 3, 1};
    keybMap["0"] = {"0", "0", 4, 1};
    keybMap["+"] = {"+", "+", 5, 1};
    keybMap["*"] = {"*", "*", 6, 1};
    keybMap["["] = {"[", "[", 7, 1};
    keybMap["]"] = {"]", "]", 8, 1};

    keybMap["1"] = {"1", "1", 1, 2};
    keybMap["2"] = {"2", "2", 2, 2};
    keybMap["3"] = {"3", "3", 3, 2};
    keybMap["."] = {".", ".", 4, 2};
    keybMap["-"] = {"-", "-", 5, 2};
    keybMap["/"] = {"/", "/", 6, 2};
    keybMap["sin("] = {"SIN[", "|", 7, 2};
    keybMap["log2("] = {"LOG2[", "&", 8, 2};

    //link the keys
    Key& del     = keybMap["del"];
    Key& panL   = keybMap["panL"];
    Key& panR   = keybMap["panR"];
    Key& zoomOut = keybMap["ZOOMOUT"];
    Key& zoomIn  = keybMap["ZOOMIN"];
    Key& key7    = keybMap["7"];
    Key& key8    = keybMap["8"];
    Key& key9    = keybMap["9"];
    Key& keyX    = keybMap["X"];
    Key& enter   = keybMap["enter"];
    Key& key4    = keybMap["4"];
    Key& key5    = keybMap["5"];
    Key& key6    = keybMap["6"];
    Key& key0    = keybMap["0"];
    Key& keyPlus = keybMap["+"];
    Key& keyMul  = keybMap["*"];
    Key& keyLBracket = keybMap["["];
    Key& keyRBracket = keybMap["]"];
    Key& key1    = keybMap["1"];
    Key& key2    = keybMap["2"];
    Key& key3    = keybMap["3"];
    Key& keyDot   = keybMap["."];
    Key& keyMinus = keybMap["-"];
    Key& keyDiv = keybMap["/"];
    Key& keySin = keybMap["sin("];
    Key& keyLog = keybMap["log2("];

    //Manually linked every node in the keyboard layout
    //link the keys
    // row/line 0 {del 7 8 9}
    
    del.right  = &key7;
    del.down   = &enter;

    key7.left  = &del;
    key7.right = &key8;
    key7.down  = &key4;

    key8.left  = &key7;
    key8.right = &key9;
    key8.down  = &key5;

    key9.left  = &key8;
    key9.down  = &key6;
    key9.right = &keyX;

    keyX.left  = &key9;
    keyX.down  = &key0;
    keyX.right = &keyPlus;
    
    enter.up   = &del; 
    enter.down = &key1;
    enter.right = &key4;
    // row/line 1 {enter 4 5 6 0 + * [ ]}
    key4.up    = &key7;
    key4.down  = &key1;
    key4.right = &key5;
    key4.left  = &enter;

    key5.up    = &key8;
    key5.down  = &key2;
    key5.right = &key6;
    key5.left  = &key4;

    key6.up    = &key9;
    key6.down  = &key3;
    key6.right = &key0;
    key6.left  = &key5;

    key0.up    = &keyX;
    key0.down  = &keyDot;
    key0.right = &keyPlus;
    key0.left  = &key6;

    keyPlus.down = &keyMinus;
    keyPlus.left = &key0;
    keyPlus.right = &keyMul;
    keyPlus.up = &keyX;

    keyMul.down = &keyDiv;
    keyMul.left = &keyPlus;
    keyMul.right = &keyLBracket;

    keyLBracket.up = &panL;
    keyLBracket.down = &keySin;
    keyLBracket.left = &keyMul;
    keyLBracket.right = &keyRBracket;

    keyRBracket.up = &panR;
    keyRBracket.down = &keyLog;
    keyRBracket.left = &keyLBracket;

    keyRBracket.right = &zoomIn;
    keyLog.right = &zoomOut;

    zoomOut.left = &keyLog;
    zoomOut.up = &zoomIn;
    zoomIn.down = &zoomOut;
    zoomIn.left = &keyRBracket;
    
    panL.right = &panR;
    panL.down = &keyLBracket;

    panR.left = &panL;
    panR.down = &keyRBracket;

    // Line/row 2 {1 2 3 . - / sin log}
    key1.up    = &key4;
    key1.left  = &enter;
    key1.right  = &key2;

    key2.up    = &key5;
    key2.left  = &key1;
    key2.right = &key3;

    key3.up    = &key6;
    key3.left  = &key2;
    key3.right = &keyDot;

    keyDot.up    = &key0;
    keyDot.left  = &key3;   
    keyDot.right = &keyMinus;

    keyMinus.up    = &keyPlus;
    keyMinus.left  = &keyDot;
    keyMinus.right = &keyDiv;

    keyDiv.up    = &keyMul;
    keyDiv.left  = &keyMinus;
    keyDiv.right = &keySin;

    keySin.up    = &keyLBracket;
    keySin.left  = &keyDiv;
    keySin.right = &keyLog;

    keyLog.up    = &keyRBracket;
    keyLog.left  = &keySin;

    keybCursor = Cursor(&keybMap["enter"]);
}

void updateKeyb(int Xpos, int Ypos){

    std::unordered_map<std::string, Key>::iterator it;

    for (it = keybMap.begin(); it != keybMap.end(); ++it) { //if current key
        //save key state
        Key saveKey = it->second;

        int drawX = Xpos + saveKey.col * (TILE_STEP_X);
        int drawY = Ypos + saveKey.row * (TILE_STEP_Y);

        uint32_t fillColor = saveKey.cursorOn ? colorHex(1, 248, 124, 250) : colorHex(0, 101, 247, 99); // highlight vs normal

        // drop shadow
        drawRect(drawX, drawY, 24, 36, colorHex(0, 0, 100, 0));
        // Highlighted color
        drawRect(drawX + 2, drawY + 3, 24, 36, fillColor);
        // Label character
        if (!saveKey.val.empty()) {
            drawChar(saveKey.id[0], drawX + 4, drawY + 6, colorHex(1, 0, 0, 0));
        }

        if(btn_value == 0b00001 && saveKey.cursorOn){ // if selected and pressed

            // drop shadow
            drawRect(drawX, drawY, 24, 36, colorHex(0, 200, 200, 0));
            // Highlighted color
            drawRect(drawX + 2, drawY + 3, 24, 36, fillColor);
            drawChar(saveKey.id[0], drawX + 4, drawY + 6, colorHex(1, 255, 255, 255));
        }

    }
}

// Keyboard Cursor to track current key position

Cursor::Cursor(Key* startKey) : current(startKey) {
    if(current) current->cursorOn = true;
}

void Cursor::moveTo(Key* next){
    if(!next){
        ERRORBNDS = true;
        return;
    };

    current->cursorOn = false; 
    next->cursorOn = true;
    current = next;
}

void Cursor::moveUp(){
    moveTo(current->up); 
    AUDIOCLICK = true;
    if (current) printf("Moved Up to: %s\n\r", current->val.c_str());
}
void Cursor::moveDown(){
    moveTo(current->down); 
    AUDIOCLICK = true;
    if (current) printf("Moved Down to: %s\n\r", current->val.c_str());
}
void Cursor::moveLeft(){
    moveTo(current->left); 
    AUDIOCLICK = true;
    if (current) printf("Moved Left to: %s\n\r", current->val.c_str());
}
void Cursor::moveRight(){
    moveTo(current->right); 
    AUDIOCLICK = true;
    if (current) printf("Moved Right to: %s\n\r", current->val.c_str());
}

std::string Cursor::getCurrentKeyValue() const {
    AUDIOSEL = true;
    return current ? current->val : "";
}

std::string Cursor::pressCurrentKey() const {
    AUDIOSEL = true;
    return current ? current->id : "";
}

// This function serves to test and configure to initial keyboard layout and rendering style
void renderKeyb(int xPos, int yPos){

    //backspace
    drawRect(xPos , yPos, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 2, yPos + 3, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('<', xPos + 4, yPos + 3, colorHex(1, 0, 0, 0));
    // 7
    drawRect(xPos + 30, yPos, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 32, yPos + 3, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('7', xPos + 34, yPos + 3, colorHex(1, 0, 0, 0));
    // 8
    drawRect(xPos + 60, yPos, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 62, yPos + 3, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('8', xPos + 64, yPos + 3, colorHex(1, 0, 0, 0));
    // 9
    drawRect(xPos + 90, yPos, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 92, yPos + 3, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('9', xPos + 94, yPos + 3, colorHex(1, 0, 0, 0));
    //enter
    drawRect(xPos , yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 2, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('>', xPos + 4, yPos + 49, colorHex(1, 0, 0, 0));
    // 4
    drawRect(xPos + 30, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 32, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('4', xPos + 34, yPos + 49, colorHex(1, 0, 0, 0));
    // 5
    drawRect(xPos + 60, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 62, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('5', xPos + 64, yPos + 49, colorHex(1, 0, 0, 0));
    // 6
    drawRect(xPos + 90, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 92, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('6', xPos + 94, yPos + 49, colorHex(1, 0, 0, 0));
    // 0
    drawRect(xPos + 120, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 122, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('0', xPos + 124, yPos + 49, colorHex(1, 0, 0, 0));
    // +
    drawRect(xPos + 150, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 152, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('+', xPos + 154, yPos + 51, colorHex(1, 0, 0, 0));
    // *
    drawRect(xPos + 180, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 182, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('*', xPos + 184, yPos + 51, colorHex(1, 0, 0, 0));
    // [
    drawRect(xPos + 210, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 212, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('[', xPos + 214, yPos + 51, colorHex(1, 0, 0, 0));
    // ]
    drawRect(xPos + 240, yPos + 46, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 242, yPos + 49, 24, 36, colorHex(0, 101, 247, 99));
    drawChar(']', xPos + 244, yPos + 51, colorHex(1, 0, 0, 0));
    //Row 2
    // 1
    drawRect(xPos + 30, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 32, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('1', xPos + 34, yPos + 95, colorHex(1, 0, 0, 0));
    // 2
    drawRect(xPos + 60, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 62, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('2', xPos + 64, yPos + 95, colorHex(1, 0, 0, 0));
    // 3
    drawRect(xPos + 90, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 92, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('3', xPos + 94, yPos + 95, colorHex(1, 0, 0, 0));
    // .
    drawRect(xPos + 120, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 122, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('.', xPos + 124, yPos + 95, colorHex(1, 0, 0, 0));
    // -
    drawRect(xPos + 150, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 152, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('-', xPos + 154, yPos + 97, colorHex(1, 0, 0, 0));
    // /
    drawRect(xPos + 180, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 182, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('/', xPos + 184, yPos + 97, colorHex(1, 0, 0, 0));
    // sin
    drawRect(xPos + 210, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 212, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('|', xPos + 214, yPos + 95, colorHex(1, 0, 0, 0));
    // log
    drawRect(xPos + 240, yPos + 92, 24, 36, colorHex(0, 0, 100, 0));
    drawRect(xPos + 242, yPos + 95, 24, 36, colorHex(0, 101, 247, 99));
    drawChar('&', xPos + 244, yPos + 95, colorHex(1, 0, 0, 0));
}
