#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

#include "ofMain.h"
#include "Vector.h"
#include <cstring>

using namespace std;

class PushButton {
private:
    int posX, posY;
    int width, height;
    string name;
    
public:
    PushButton() : name(""), posX(0), posY(0) {}
    
    void set(string n, int x, int y, int w, int h) {
        name = n;
        posX = x; 
        posY = y;
        width = w;
        height = h;
    }
    
    void draw() {
        ofFill();
        ofSetColor(0xffffff);
        ofRect(posX, posY, width, height);
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofRect(posX, posY, width, height);
        ofDrawBitmapString(name, posX + 10, posY + 20);
    }
    
    bool checkHit(bool hit, Vector &hitPoint) {
        if (hit && 
            hitPoint.x > posX && hitPoint.x < posX + width &&
            hitPoint.y > posY && hitPoint.y < posY + height)
            return true;
        else
            return false;
    }
};

#endif

