#ifndef _BOUNCE_
#define _BOUNCE_

#include "ofMain.h"
#include "Vector.h"
#include "Tracker.h"
#include "Console.h"
#include "Infobox.h"
#include "PushButton.h"
#include "Texture.h"
#include "ShootingCans.h"
#include "RobotDefense.h"
#include "BalloonHunt.h"
#include "RisingNinja.h"

class Bounce : public ofBaseApp{
private:
    Vector hitPoint;
    bool hit;
    
    enum Mode {MENU, CALIBRATE, SHOOTING_CANS, ROBOT_DEFENSE, BALLOON_HUNT, RISING_NINJA} mode;
    void changeMode(Mode m);
    
    Infobox infobox;
    Console console;
    Tracker tracker;
    
    Game* activeGame;
    bool gamePaused;
    
    ShootingCans shootingCans;
    RobotDefense robotDefense;
    BalloonHunt balloonHunt;
    RisingNinja risingNinja;
    
    PushButton calibrateButton;
    PushButton creditsButton;
    PushButton shootingCansButton;
    PushButton robotDefenseButton;
    PushButton balloonHuntButton;
    PushButton risingNinjaButton;
    
    // audio
    void audioReceived(float* input, int bufferSize, int nChannels);
    bool fullScreen;
    
    // menu
    Texture background;
    Texture bounceLogo;
    Texture fhLogo;
    Texture mmtLogo;
    Texture mmaLogo;
    
    ofTrueTypeFont font, font2;

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
