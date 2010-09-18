#ifndef _SHOOTING_CANS_
#define _SHOOTING_CANS_

#include "Game.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "PushButton.h"
#include "Infobox.h"

class ShootingCans : public Game {
private:
    Texture numbers;
    SpriteSheet sprite;
    int counter, actual;
    
    
public:
    ShootingCans(Infobox* i, PushButton* m);
    //~Game()
    
    virtual void reset();
    virtual bool draw(bool hit, Vector hitPoint);
};

#endif

