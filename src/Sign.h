#ifndef _SIGN_
#define _SIGN_

#include "ofMain.h"
#include "Vector.h"

class Sign {
private:
    string content;
    Vector position;
    float stopTime;
    
public:
    Sign(string _content, Vector _position, float time) : 
        content(_content), position(_position), stopTime(ofGetElapsedTimef() + time) {
    }
    
    bool draw(ofTrueTypeFont* font) {
        if (stopTime <= ofGetElapsedTimef())
            return false;
        
        ofSetColor(0x384585);
        font->drawString(content, position.x, position.y);
        return true;
    }
};

#endif