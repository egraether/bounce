#ifndef _INFOBOX_
#define _INFOBOX_

#include "ofMain.h"
#include "constants.h"

#include <vector>

class Infobox {
private:
    vector<string> message;
    ofTrueTypeFont font;
    
public:
    Infobox() {
        font.loadFont("microgme.ttf", 26);
    }

    void set(string _message) {
        clear();
        
        if (_message.length() > 60) {
            int pos = _message.find(' ', 50);
            
            while (pos != string::npos) {
                message.push_back(_message.substr(0, pos));
                _message = _message.substr(pos + 1);
                pos = _message.find(' ', 50);
            }
        }
        
        message.push_back(_message);
    }
    
    void clear() {
        message.clear();
    }

    void draw() {
        if (message.size()) {
            ofFill();
            ofEnableAlphaBlending();
            ofSetColor(220, 220, 255, 200);
            ofRect(0, HEIGHT - message.size() * 30 - 20, WIDTH, message.size() * 30 + 20);
            ofDisableAlphaBlending();
        
            ofSetColor(0x384585);
            for (int i = 0; i < message.size(); i++) {
                font.drawString(
                    message[i], 
                    WIDTH - 20 - font.stringWidth(message[i]), 
                    HEIGHT - 20 - font.stringHeight(message[i]) * (message.size() - i - 1)
                );
            }
        }
    }
};

#endif