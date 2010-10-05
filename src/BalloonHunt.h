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
    Vector pos, balloonCenter;
    bool explode;
    SpriteAnimation sprite;
    float speed; 
    int size;
    
    int color;
    static int colors[6];

public:
    Balloon(Texture* tex, int rows, int columns) : explode(false) {
        switch (counter % 3) {
            case 0:
                sprite.load(tex, 300, 300, rows, columns);
                size = 120;
                points = 10;
                speed = 5;
                balloonCenter.set(0, -50);
                break;
            case 1:
                sprite.load(tex, 200, 200, rows, columns);
                size = 80;
                points = 50;
                speed = 2;
                balloonCenter.set(0, -36);
                break;
            case 2:
                sprite.load(tex, 100, 100, rows, columns);
                size = 40;
                points = 100;
                speed = 1;
                balloonCenter.set(0, -17);
                break;
            default:
                break;
        }
        
        color = colors[rand() % 6];
        pos.set(rand() % (WIDTH - 300) + 150, HEIGHT + size);
        sprite.setAnimation(0, 0, 0, 0, true);
        counter++;
    }
    
    bool draw() {
        if (!explode)
            pos.y -= speed;
        
        if (pos.y < -100)
            return false;
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y, 0.0);
        //ofCircle(balloonCenter.x, balloonCenter.y, size);
        ofSetColor(color);
        bool destroyed = sprite.draw(0, 0, false);
        ofPopMatrix();
        
        return destroyed;
    }
    
    bool checkHit(Vector &hitPoint) {
        if (!explode && Vector::distance(hitPoint, pos + balloonCenter) <= size) {
            explode = true;
            sprite.setAnimation(0, 2, 0, 5, false, 3);
            return true;
        }
        return false;
    }
    
    Vector getPosition() {
        return pos;
    }
};

#endif