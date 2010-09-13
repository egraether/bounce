#include "Infobox.h"
#include "constants.h"

Infobox::Infobox() : isAlive(false) {}

void Infobox::set(string m) {
    message = m;
    width = message.length() * 8 + 40;
    height = 50;
    isAlive = true;
}

int Infobox::draw(bool hit, ofPoint hitPoint) {
    if (hit)
        isAlive = false;
    else if (isAlive) {
        ofFill();
        ofSetColor(220, 220, 220);
        ofRect((WIDTH - width) / 2, (HEIGHT - height) / 2, width, height);
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(message, (WIDTH - width) / 2 + 20, HEIGHT / 2 + 5);
        return 1;
    }
    
    return 0;
}
