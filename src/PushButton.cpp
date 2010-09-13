#include "PushButton.h"
#include "ofMain.h"

ofPoint PushButton::measure[2] = {ofPoint(WIDTH / 5, HEIGHT / 5), ofPoint(WIDTH / 3, HEIGHT / 3)};

PushButton::PushButton() :
    name(""), posX(0), posY(0), size(SMALL) {
}

void PushButton::set(string n, int x, int y, Size s) {
    name = n;
    posX = x; 
    posY = y;
    size = s;
}

bool PushButton::draw(bool hit, ofPoint &hitPoint) {
    if (hit && isHit(hitPoint))
        return true;
    
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofRect(posX, posY, measure[size].x, measure[size].y);
    ofDrawBitmapString(name, posX + 10, posY + 20);
    return false;
}

bool PushButton::isHit(ofPoint &hitPoint) {
    if (hitPoint.x > posX && hitPoint.x < posX + measure[size].x &&
        hitPoint.y > posY && hitPoint.y < posY + measure[size].y)
        return true;
    else
        return false;
}
