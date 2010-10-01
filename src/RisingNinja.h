#ifndef _RISING_NINJA_
#define _RISING_NINJA_

#include "Game.h"
#include "Texture.h"
#include "SpriteAnimation.h"
#include "Infobox.h"
#include "Vector.h"

class RisingNinja : public Game {
private:
    Texture numbers;
    SpriteAnimation numberAnimation;

public:
    RisingNinja(const char* titel, Infobox* infobox, const char* scoresFileName);
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);

};

#endif