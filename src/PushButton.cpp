#include "PushButton.h"
#include "ofMain.h"
#include "constants.h"

PushButton::PushButton() :
    name(""), posX(0), posY(0) {
}

void PushButton::set(string n, int x, int y, int w, int h) {
    name = n;
    posX = x; 
    posY = y;
    width = w;
    height = h;
}

void PushButton::draw() {
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofRect(posX, posY, width, height);
    ofDrawBitmapString(name, posX + 10, posY + 20);
}

bool PushButton::checkHit(bool hit, Vector &hitPoint) {
    if (hit && 
        hitPoint.x > posX && hitPoint.x < posX + width &&
        hitPoint.y > posY && hitPoint.y < posY + height)
        return true;
    else
        return false;
}
