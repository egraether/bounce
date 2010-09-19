#ifndef _GAME_
#define _GAME_

#include "Vector.h"
#include "PushButton.h"
#include "Infobox.h"

class Game {
protected:
    Infobox* infobox;
    PushButton* menuButton;
    
public:
    Game(Infobox* i, PushButton* m) : infobox(i), menuButton(m) {}
    //~Game()
    
    virtual void reset() = 0;
    virtual bool draw(bool hit, Vector &hitPoint) = 0;
};

#endif

