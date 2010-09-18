#include "Infobox.h"
#include "ofMain.h"
#include "constants.h"

Infobox::Infobox() : state(DEAD) {}

void Infobox::set(string m, Type t) {
    message.clear();
    
    int pos1 = 0,
        pos2 = -1;
    
    while (true) {
        pos1 = pos2 + 1;
        pos2 = m.find("$", pos1);
        message.push_back(m.substr(pos1, pos2 - pos1));
        
        if (pos2 >= m.length())
            break;
    }
    
    type = t;
    state = ALIVE;
    
    switch (type) {
        case CHECK: {
            int maxLength = 0, length = 0;
            for (int i = 0; i < message.size(); i++) {
                if (message[i].length() > length) {
                    maxLength = i;
                    length = message[i].length();
                }
            }
            width = 40 + message[maxLength].length() * 8;
            height = 40 + message.size() * 20 + PushButton::size.y;
            one.set("OK", (WIDTH - PushButton::size.x) / 2, HEIGHT / 2);
            break;
        }
        default:
            break;
    }
}

void Infobox::draw() {
    if (state == ALIVE) {
        switch (type) {
            case CHECK:
                ofFill();
                ofSetColor(220, 220, 255);
                ofRect((WIDTH - width) / 2, HEIGHT / 2 - 20 - message.size() * 20, width, height);
                
                ofSetColor(0, 0, 0);
                for (int i = 0; i < message.size(); i++) {
                    ofDrawBitmapString(
                        message[i], 
                        (WIDTH - width) / 2 + 20, 
                        HEIGHT / 2 - message.size() * 20 + i * 20
                    );
                }
                
                one.draw();
                break;
            default:
                break;
        }
    }
}

Infobox::State Infobox::checkState(bool hit, Vector &hitPoint) {
    if (state == ALIVE) {
        switch (type) {
            case CHECK:
                if (one.checkHit(hit, hitPoint)) {
                    state = DEAD;
                    return ONE_HIT;
                }
                break;
            default:
                break;
        }
        
        return ALIVE;
    }
    else
        return DEAD;
}

void Infobox::kill() {
    state = DEAD;
}
