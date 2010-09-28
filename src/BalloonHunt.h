#ifndef _BALLOON_HUNT_
#define _BALLOON_HUNT_

#include "Game.h"
#include "Texture.h"
#include "SpriteAnimation.h"
#include "PushButton.h"
#include "Infobox.h"

#include <vector>

class BalloonHunt : public Game {
private:
    class Balloon;
    vector<Balloon> balloons;
    Texture numbers;
    int counter;
    
public:
    BalloonHunt(const char* titel, Infobox* infobox, const char* scoresFileName);
    //~Game()
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class BalloonHunt::Balloon {
public:
    enum Type {BIG, MEDIUM, SMALL};
    int points;
    
private:
    Type type;
    Vector pos;
    bool explode;
    SpriteAnimation sprite;
    float speed; 
    int size;
    
public:
    Balloon(Type t, int x, int y, Texture* tex, int rows, int columns) :
        type(t), pos(x, y), explode(false) {
        
        switch (type) {
            case BIG:
                sprite.load(tex, 300, 300, rows, columns);
                size = 100;
                points = 10;
                speed = 2;
                break;
            case MEDIUM:
                sprite.load(tex, 100, 100, rows, columns);
                size = 50;
                points = 50;
                speed = 1;
                break;
            case SMALL:
                sprite.load(tex, 50, 50, rows, columns);
                size = 25;
                points = 100;
                speed = .5;
                break;
            default:
                break;
        }
            
        sprite.setAnimation(0, 0, 0, 0, true);
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
            sprite.setAnimation(5, 0, 0, 0, false);
            return true;
        }
        return false;
    }
    
    Vector getPosition() {
        return pos;
    }
};

#endif

