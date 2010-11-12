/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

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
    Balloon(Texture* tex, int rows, int columns);
    
    bool draw();
    bool checkHit(Vector &hitPoint);
    Vector getPosition();
};

#endif