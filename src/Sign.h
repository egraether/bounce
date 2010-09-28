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
    Sign(string _content, int x, int y, float time) : 
        content(_content), position(x, y), stopTime(ofGetElapsedTimef() + time) {
    }
    
    bool draw() {
        if (stopTime <= ofGetElapsedTimef())
            return false;
        
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(content, position.x, position.y);
        return true;
    }
};

#endif