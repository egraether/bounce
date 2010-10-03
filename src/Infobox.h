#ifndef _INFOBOX_
#define _INFOBOX_

#include "ofMain.h"
#include "constants.h"

#include <vector>

class Infobox {
private:
    string message;
    ofTrueTypeFont font;
    
public:
    Infobox() {
        font.loadFont("microgme.ttf", 26);
    }

    void set(const char* _message) {
        clear();
        message = _message;
    }
    
    void clear() {
        message.clear();
    }

    void draw() {
        ofSetColor(0x384585);
        font.drawString(message, WIDTH - 20 - font.stringWidth(message), HEIGHT - 20);
    }
};

#endif

