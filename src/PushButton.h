#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

#include "ofMain.h"
#include "Vector.h"
#include "Texture.h"

class PushButton {
private:
    int posX, posY;
    int width, height;
    Texture texture;
    
public:
    PushButton() : posX(0), posY(0) {}
    
    void set(const char* imageFile, int x, int y, int w, int h) {
        texture.load(imageFile, true, GL_CLAMP, GL_CLAMP);
        posX = x; 
        posY = y;
        width = w;
        height = h;
    }
    
    void draw() {
        texture.draw(posX, posY, width, height);
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

