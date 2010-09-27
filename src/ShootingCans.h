#ifndef _SHOOTING_CANS_
#define _SHOOTING_CANS_

#include "Game.h"
#include "Texture.h"
#include "SpriteAnimation.h"
#include "PushButton.h"
#include "Infobox.h"

#include <vector>

class ShootingCans : public Game {
private:
    class Can;
    vector<Can> cans;
    Texture numbers;
    
public:
    ShootingCans(const char* titel, Infobox* infobox, const char* scoresFileName);
    //~Game()
    
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
            
        sprite.setAnimation(1, 0, 5, 0, true);
    }
    
    bool draw() {
        pos = flight + pos;
        
        if (flying) {
            flight.y += (ofGetElapsedTimef() - time) / 5;
        }
        
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y, 0.0);
        //ofRotateZ();
        bool destroyed = sprite.draw(0, 0);
        ofPopMatrix();
        
        return destroyed;
    }
    
    bool checkHit(Vector &hitPoint) {
        if (hitPoint.x > pos.x - sprite.getWidth() / 2 && 
            hitPoint.x < pos.x + sprite.getWidth() / 2 &&
            hitPoint.y > pos.y - sprite.getHeight() / 2 &&
            hitPoint.y < pos.y + sprite.getHeight() / 2) {
            
            flying = true;
            flight = ((Vector(0, -sprite.getHeight() / 3 * 2) + pos) - hitPoint) / 10;
            sprite.setAnimation(10, 0, 0, 0);
            time = ofGetElapsedTimef();
            
            return true;
        }
        
        return false;
    }
};

#endif

