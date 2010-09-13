#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

#include "ofMain.h"

class PushButton {
private:
    int posX, posY, width, height;
    string name;
public:
    PushButton(int x, int y, int w, int h, string n);
    void draw();
    bool isHit(ofPoint hitPoint);
};

#endif

