#include "PushButton.h"
#include "ofMain.h"
#include "constants.h"

ofPoint PushButton::size(WIDTH / 5, HEIGHT / 5);

PushButton::PushButton() :
    name(""), posX(0), posY(0) {
}

void PushButton::set(string n, int x, int y) {
    name = n;
    posX = x; 
    posY = y;
}

void PushButton::draw() {
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofRect(posX, posY, size.x, size.y);
    ofDrawBitmapString(name, posX + 10, posY + 20);
}

bool PushButton::checkHit(bool hit, ofPoint &hitPoint) {
    if (hit && 
        hitPoint.x > posX && hitPoint.x < posX + size.x &&
        hitPoint.y > posY && hitPoint.y < posY + size.y)
        return true;
    else
        return false;
}