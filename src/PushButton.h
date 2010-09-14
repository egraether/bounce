#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

#include "ofMain.h"

class PushButton {
private:
    int posX, posY;
    string name;
    
public:
    PushButton();
    void set(string n, int x, int y);
    void draw();
    bool checkHit(bool hit, ofPoint &hitPoint);
    
    static ofPoint size;
};

#endif

