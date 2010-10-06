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
    
    bool draw(ofTrueTypeFont &font) {
        position.y -= 0.5;
        
        float leftTime = stopTime - ofGetElapsedTimef();
        if (leftTime <= 0)
            return false;
        
        ofEnableAlphaBlending();
        
        int alpha = leftTime < 1 ? 255 * leftTime : 255;
        ofSetColor(56, 69, 133, alpha);
        font.drawString(content, position.x, position.y);
        
        ofDisableAlphaBlending();
        return true;
    }
    
    void update(string _content) {
        content = _content;
    }
};

#endif