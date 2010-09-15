#ifndef _GAME_
#define _GAME_

#include "ofMain.h"
#include "PushButton.h"
#include "Infobox.h"

class Game {
protected:
    Infobox* infobox;
    PushButton* menuButton;
    
public:
    Game(Infobox* i, PushButton* m) : infobox(i), menuButton(m) {}
    //~Game()
    
    bool draw(bool hit, ofPoint hitPoint) = 0;
};

#endif

