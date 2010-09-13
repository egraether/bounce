#ifndef _INFOBOX_
#define _INFOBOX_

#include "ofMain.h"
#include "PushButton.h"

class Infobox {
public:
    enum Type {CHECK, CHOICE};
    
private:
    string message;
    int width, height;
    Type type;
    PushButton a, b;
    
public:
    bool isAlive;
    
    Infobox();
    //~Infobox();
    
    void set(string m, Type t);
    int draw(bool hit, ofPoint &hitPoint);
};

#endif

