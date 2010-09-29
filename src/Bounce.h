#ifndef _BOUNCE_
#define _BOUNCE_

#include "ofMain.h"
#include "Vector.h"
#include "Tracker.h"
#include "Console.h"
#include "Infobox.h"
#include "PushButton.h"
#include "NameInput.h"
#include "ShootingCans.h"
#include "RobotDefense.h"
#include "BalloonHunt.h"

class Bounce : public ofBaseApp{
private:
    Vector hitPoint;
    bool hit;
    
    enum Mode {MENU, CALIBRATE, SHOOTING_CANS, ROBOT_DEFENSE, BALLOON_HUNT} mode;
    void changeMode(Mode m);
    
    Infobox infobox;
    Console console;
    Tracker tracker;
    NameInput nameInput;
    
    Game* activeGame;
    
    ShootingCans shootingCans;
    RobotDefense robotDefense;
    BalloonHunt balloonHunt;
    
    PushButton calibrateButton;
    PushButton menuButton;
    PushButton shootingCansButton;
    PushButton robotDefenseButton;
    PushButton balloonHuntButton;
    
    // audio
    void audioReceived(float* input, int bufferSize, int nChannels);
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
