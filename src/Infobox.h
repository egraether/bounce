#ifndef _INFOBOX_
#define _INFOBOX_

#include "ofMain.h"
#include "constants.h"

#include <vector>

class Infobox {
private:
    //vector<string> message;
    string message;
    //unsigned int width, height;
    
public:
    Infobox() {}

    void set(const char* m) {
        clear();
        message = m;
    }
    
    void clear() {
        message.clear();
    }

    void draw() {
        ofSetColor(0, 0, 0);
        //for (int i = 0; i < message.size(); i++) {
            ofDrawBitmapString(message, WIDTH - message.length() * 8 - 10, HEIGHT - 20);
        //}
    }
};

#endif

