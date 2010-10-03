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
    Texture balloonTexture;
    int counter;
    
public:
    BalloonHunt(const char* titel, Infobox* infobox, const char* scoresFileName);
    //~Game()
    
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class BalloonHunt::Balloon {
public:
    int points;
    static int counter;
    
private:
    Vector pos;
    bool explode;
    SpriteAnimation sprite;
    float speed; 
    int size;
    int color[3];

public:
    Balloon(Texture* tex, int rows, int columns) : explode(false) {
        switch (counter % 3) {
            case 0:
                sprite.load(tex, 300, 300, rows, columns);
                size = 100;
                points = 10;
                speed = 5;
                break;
            case 1:
                sprite.load(tex, 200, 200, rows, columns);
                size = 75;
                points = 50;
                speed = 2;
                break;
            case 2:
                sprite.load(tex, 100, 100, rows, columns);
                size = 50;
                points = 100;
                speed = 1;
                break;
            default:
                break;
        }
        
        color[0] = rand() % 125 + 125;
        color[1] = rand() % 125 + 125;
        color[2] = rand() % 125 + 125;
            
        pos.set(rand() % (WIDTH - 300) + 150, HEIGHT + size);
        sprite.setAnimation(0, 0, 0, 1, true);
        counter++;
    }
    
    bool draw() {
        if (!explode)
            pos.y -= speed;
        
        if (pos.y < -100)
            return false;
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y, 0.0);
        //ofCircle(0, 0, size);
        ofSetColor(color[0], color[1], color[2]);
        bool destroyed = sprite.draw(0, 0, false);
        ofPopMatrix();
        
        return destroyed;
    }
    
    bool checkHit(Vector &hitPoint) {
        if (Vector::distance(hitPoint, pos) <= size) {
            explode = true;
            sprite.setAnimation(0, 2, 0, 3, false);
            return true;
        }
        return false;
    }
    
    Vector getPosition() {
        return pos;
    }
};

#endif