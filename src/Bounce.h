#ifndef _BOUNCE_
#define _BOUNCE_

#include "ofMain.h"
#include "Tracker.h"

class Bounce : public ofBaseApp{
private:
    Tracker tracker;

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
};

#endif
