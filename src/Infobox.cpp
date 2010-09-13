#include "Infobox.h"
#include "constants.h"

Infobox::Infobox() : isAlive(false) {}

void Infobox::set(string m, Type t) {
    message = m;
    type = t;
    isAlive = true;
    
    switch (type) {
        case CHECK:
            width = message.length() * 8 + 20;
            width = width < WIDTH / 3 ? WIDTH / 3 : width;
            height = HEIGHT / 2;
            a.set(
                "OK", 
                (WIDTH - PushButton::measure[PushButton::BIG].x) / 2, 
                (HEIGHT - PushButton::measure[PushButton::BIG].y) / 2 + height / 6, 
                PushButton::BIG
            );
            break;
        default:
            break;
    }
}

int Infobox::draw(bool hit, ofPoint &hitPoint) {
    if (isAlive) {
        switch (type) {
            case CHECK:
                ofFill();
                ofSetColor(220, 220, 255);
                ofRect((WIDTH - width) / 2, (HEIGHT - height) / 2, width, height);
                
                ofSetColor(0, 0, 0);
                ofDrawBitmapString(message, (WIDTH - width) / 2 + 20, HEIGHT / 2 - height / 4);
                
                if (a.draw(hit, hitPoint))
                    isAlive = false;
                break;
            default:
                break;
        }
    }
    
    return 0;
}
