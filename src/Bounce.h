#ifndef _BOUNCE_
#define _BOUNCE_

#include "ofMain.h"
#include "Tracker.h"
#include "Console.h"
#include "Infobox.h"
#include "PushButton.h"

class Bounce : public ofBaseApp{
private:
    ofPoint hitPoint;
    bool hit;
    
    enum Mode {MENU, CALIBRATE, TARGET} mode;
    void changeMode(Mode m);
    
    Infobox infobox;
    Console console;
    Tracker tracker;
    
    PushButton calibrateButton;
    PushButton menuButton;
    PushButton targetButton;
    
    // audio
    void audioReceived(float* input, int bufferSize, int nChannels);
    int bangLevel;
    int lastBang;
    bool bang;

public:
    Bounce();
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
};

#endif
