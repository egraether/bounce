#ifndef _INFOBOX_
#define _INFOBOX_

#include "ofMain.h"
#include "PushButton.h"

#include <vector>

class Infobox {
public:
    enum Type {CHECK, CHOICE};
    enum State {ALIVE, ONE_HIT, TWO_HIT, DEAD};
    
private:
    vector<string> message;
    int width, height;
    Type type;
    State state;
    PushButton one, two;
    
public:
    Infobox();
    //~Infobox();
    
    void set(string m, Type t);
    void draw();
    State checkState(bool hit, ofPoint &hitPoint);
    void kill();
};

#endif

