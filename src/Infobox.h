#ifndef _INFOBOX_
#define _INFOBOX_

#include "ofMain.h"

class Infobox {
private:
    string message;
    int width, height;
    bool isAlive;
    
public:
    Infobox();
    //~Infobox();
    
    void set(string m);
    int draw(bool hit, ofPoint hitPoint);
    void kill();
};

#endif

