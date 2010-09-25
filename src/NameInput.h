#ifndef _NAME_INPUT_
#define _NAME_INPUT_

#include "ofMain.h"
#include "PushButton.h"
#include "constants.h"

class NameInput {
private:
    PushButton backspace, enter, alphabet;
    string name, left, mid, right;
    char actualChar;
    ofTrueTypeFont verdanaSmall, verdanaBig;
    int counter;

public:
    NameInput() {
        alphabet.set("", WIDTH / 5, HEIGHT / 5, WIDTH * 3 / 5, HEIGHT / 4);
        backspace.set("remove", WIDTH / 5, HEIGHT / 3 * 2, WIDTH / 7, HEIGHT / 7);
        enter.set("OK", WIDTH * 23 / 35, HEIGHT / 3 * 2, WIDTH / 7, HEIGHT / 7);
        
        verdanaSmall.loadFont("verdana.ttf", 100, false, true);
        verdanaBig.loadFont("verdana.ttf", 140, false, true);
        
        reset();
    }
    
    void reset() {
        name = "";
        actualChar = 'A';
        counter = 0;
        setChars();
    }
    
    bool draw(bool hit, Vector &hitPoint) {
        if (hit) {
            if (alphabet.checkHit(hit, hitPoint))
                name += actualChar;
            else if (backspace.checkHit(hit, hitPoint))
                name = name.substr(0, name.length() - 1);
            else if (enter.checkHit(hit, hitPoint))
                return false;
        }
        
        ofFill();
        ofSetColor(255, 255, 255);
        ofRect(WIDTH / 6, HEIGHT / 6, WIDTH * 2 / 3, HEIGHT * 2 / 3);
        
        ofNoFill();
        ofSetColor(0, 0, 0);
        ofRect(WIDTH / 6, HEIGHT / 6, WIDTH * 2 / 3, HEIGHT * 2 / 3);
        
        alphabet.draw();
        backspace.draw();
        enter.draw();
        
        counter++;
        if (counter == 100) {
            counter = 0;
            actualChar++;
            
            if (actualChar > 'Z')
                actualChar = 'A';
            
            setChars();
        }
        
        verdanaSmall.drawString(left, WIDTH * 5 / 16, HEIGHT / 2.5);
        verdanaBig.drawString(mid, WIDTH * 5 / 11, HEIGHT / 2.5);
        verdanaSmall.drawString(right, WIDTH * 5 / 8, HEIGHT / 2.5);
        
        verdanaSmall.drawString(name, WIDTH / 2 - WIDTH / 25 * name.length(), HEIGHT / 1.6);
        
        return true;
    }
    
    void setChars() {
        mid = actualChar;
        left = actualChar == 'A' ? 'Z' : actualChar - 1;
        right = actualChar == 'Z' ? 'A' : actualChar + 1;
    }
};

#endif