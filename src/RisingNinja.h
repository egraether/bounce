#ifndef _RISING_NINJA_
#define _RISING_NINJA_

#include "Game.h"
#include "Texture.h"
#include "SpriteAnimation.h"
#include "Infobox.h"
#include "Vector.h"

class RisingNinja : public Game {
private:
    Vector ninjaHitPoint;
    Vector ninjaPosition, ninjaMotion;
    Vector hookPosition, hookMotion;
    
    bool hook;
    bool line;
    bool started;
    
    int width;
    int ninjaSize;
    int hookSize;
    
    float gravitationTime;
    
    struct Platform {
        int width;
        Vector position;
        Platform(int _width, int x, int y) : width(_width), position(x,y) {}
        bool draw() {
            ofSetColor(0xaaaaaa);
            ofRect(position.x - width / 2, position.y, width, 30);
            
            if (position.y > HEIGHT) 
                return false;
            
            return true;
        }
    };
    vector<Platform> platforms;
    int platformDistance;
    
    //bool checkOnPlatform();

public:
    RisingNinja(const char* titel, Infobox* infobox, const char* scoresFileName);
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);

};

#endif