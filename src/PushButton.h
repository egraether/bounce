#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

//#include "ofMain.h"
#include "Vector.h"
#include <cstring>

using namespace std;

class PushButton {
private:
    int posX, posY;
    int width, height;
    string name;
    
public:
    PushButton();
    void set(string n, int x, int y, int w, int h);
    void draw();
    bool checkHit(bool hit, Vector &hitPoint);
};

#endif

