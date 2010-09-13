#include "PushButton.h"
#include "ofMain.h"

PushButton::PushButton(int x, int y, int w, int h, string n) :
    posX(x), posY(y), width(w), height(h), name(n) {
}

void PushButton::draw() {
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofRect(posX, posY, width, height);
    ofDrawBitmapString(name, posX + 10, posY + 10);
}

bool PushButton::isHit(ofPoint hitPoint) {
    if (hitPoint.x > posX && hitPoint.x < posX + width &&
        hitPoint.y > posY && hitPoint.y < posY + height)
        return true;
    else
        return false;
}
