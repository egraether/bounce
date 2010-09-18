#ifndef _SHOOTING_CANS_
#define _SHOOTING_CANS_

#include "Game.h"
#include "ofMain.h"
#include "Texture.h"
#include "PushButton.h"
#include "Infobox.h"

class ShootingCans : public Game {
private:
    Texture numbers;
    int counter, actual;
    
    
public:
    ShootingCans(Infobox* i, PushButton* m);
    //~Game()
    
    virtual void reset();
    virtual bool draw(bool hit, ofPoint hitPoint);
};

#endif

