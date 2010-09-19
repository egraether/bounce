#ifndef _BALLOON_HUNT_
#define _BALLOON_HUNT_

#include "Game.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "PushButton.h"
#include "Infobox.h"

#include <vector>

class BalloonHunt : public Game {
private:
    class Balloon;
    vector<Balloon> balloons;
    Texture numbers;
    float startTime;
    int counter;
    
public:
    BalloonHunt(Infobox* i, PushButton* m);
    //~Game()
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class BalloonHunt::Balloon {
public:
    enum Type {BIG, MEDIUM, SMALL};
private:
    Type type;
    Vector pos;
    bool explode;
    SpriteSheet sprite;
    float speed; 
    int size;
    
public:
    Balloon(Type t, int x, int y, Texture* tex, int n) :
        type(t), pos(x, y), sprite(tex, 50, 50, n), explode(false) {
        
        switch (type) {
            case BIG:
                sprite.width = 200;
                sprite.height = 200;
                size = 100;
                speed = 2;
                break;
            case MEDIUM:
                sprite.width = 100;
                sprite.height = 100;
                size = 50;
                speed = 1;
                break;
            case SMALL:
                sprite.width = 50;
                sprite.height = 50;
                size = 25;
                speed = .5;
                break;
            default:
                break;
        }
    }
    
    bool draw() {
        if (!explode)
            pos.y -= speed;
        
        if (pos.y < -100)
            return false;
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y, 0.0);
        //ofRotateZ();
        bool destroyed = sprite.draw(0, 0);
        ofPopMatrix();
        
        return destroyed;
    }
    
    bool checkHit(Vector &hitPoint) {
        if (Vector::distance(hitPoint, pos) <= size) {
            explode = true;
            sprite.startAnimation();
        }
    }
};

#endif

