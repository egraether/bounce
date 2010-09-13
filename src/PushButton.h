#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

#include "ofMain.h"
#include "constants.h"

class PushButton {
public:
    enum Size {SMALL, BIG};

private:
    int posX, posY;
    string name;
    Size size;
    bool isHit(ofPoint &hitPoint);
    
public:
    PushButton();
    void set(string n, int x, int y, Size s);
    bool draw(bool hit, ofPoint &hitPoint);
    
    static ofPoint measure[2];
};

#endif

