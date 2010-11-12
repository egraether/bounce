#ifndef _SHOOTING_CANS_
#define _SHOOTING_CANS_

#include "Game.h"
#include "Texture.h"
#include "SpriteAnimation.h"
#include "Infobox.h"
#include "Vector.h"

#include <vector>

class ShootingCans : public Game {
private:
    class Can;
    vector<Can> cans;
    vector<Can> shootedCans;
    
    Texture canTexture;
    Texture shelfTexture;
    
    bool gameStarted;
    
    void drawBg();
    
public:
    ShootingCans(const char* titel, Infobox* infobox, const char* scoresFileName);
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class ShootingCans::Can {
private:
    Vector pos, flight;
    bool flying;
    SpriteAnimation sprite;
    float time;
    
public:
    Can(int x, int y, Texture* t, int w, int h, int r, int c);
    
    bool draw();
    bool checkHit(Vector &hitPoint);
};

#endif

