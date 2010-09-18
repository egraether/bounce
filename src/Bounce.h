#ifndef _BOUNCE_
#define _BOUNCE_

#include "ofMain.h"
#include "Vector.h"
#include "Tracker.h"
#include "Console.h"
#include "Infobox.h"
#include "PushButton.h"
#include "ShootingCans.h"
#include "RobotDefense.h"

class Bounce : public ofBaseApp{
private:
    Vector hitPoint;
    bool hit;
    
    enum Mode {MENU, CALIBRATE, SHOOTING_CANS, ROBOT_DEFENSE, BALLOON_HUNT} mode;
    void changeMode(Mode m);
    
    Infobox infobox;
    Console console;
    Tracker tracker;
    
    ShootingCans shootingCans;
    RobotDefense robotDefense;
    
    PushButton calibrateButton;
    PushButton menuButton;
    PushButton shootingCansButton;
    PushButton robotDefenseButton;
    
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
