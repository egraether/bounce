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
    Texture canTexture;
    Texture shelfTexture;
    
    bool gameStarted;
    
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
    Can(int x, int y, Texture* t, int w, int h, int r, int c) :
        pos(x, y), flying(false),
        sprite(t, w, h, r, c) {
            
        sprite.setAnimation(0, 0, 0, 0, true);
    }
    
    bool draw() {
        pos = flight + pos;
        
        if (flying)
            flight.y += (ofGetElapsedTimef() - time) * 10;
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y, 0.0);
        if (flying)
            ofRotateZ((ofGetElapsedTimef() - time) * pos.x);
        sprite.draw(0, 0);
        ofPopMatrix();
        
        return pos.y < HEIGHT + 100;
    }
    
    bool checkHit(Vector &hitPoint) {
        if (!flying && 
            hitPoint.x > pos.x - sprite.getWidth() / 2 && 
            hitPoint.x < pos.x + sprite.getWidth() / 2 &&
            hitPoint.y > pos.y - sprite.getHeight() / 2 &&
            hitPoint.y < pos.y + sprite.getHeight() / 2) {
            
            flying = true;
            flight = ((Vector(0, -sprite.getHeight() / 3 * 2) + pos) - hitPoint) / 5;
            sprite.setAnimation(0, 1, 0, 3, true);
            time = ofGetElapsedTimef();
            
            return true;
        }
        
        return false;
    }
};

#endif

