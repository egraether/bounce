#ifndef _SHOOTING_CANS_
#define _SHOOTING_CANS_

#include "Game.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "PushButton.h"
#include "Infobox.h"

#include <vector>

class ShootingCans : public Game {
private:
    class Can;
    vector<Can> cans;
    Texture numbers;
    
public:
    ShootingCans(Infobox* i, PushButton* m);
    //~Game()
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class ShootingCans::Can {
private:
    Vector pos, flight;
    bool flying;
    SpriteSheet sprite;
    float time;
    
public:
    Can(int x, int y, int w, int h, Texture* t, int n) :
        pos(x, y), sprite(t, w, h, n), flying(false) {
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
        if (hitPoint.x > pos.x - sprite.width / 2 && 
            hitPoint.x < pos.x + sprite.width / 2 &&
            hitPoint.y > pos.y - sprite.height / 2 &&
            hitPoint.y < pos.y + sprite.height / 2) {
            
            flying = true;
            flight = ((Vector(0, -sprite.height / 3 * 2) + pos) - hitPoint) / 10;
            sprite.startAnimation();
            time = ofGetElapsedTimef();
        }
    }
};

#endif

