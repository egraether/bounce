#ifndef _ROBOT_DEFENSE_
#define _ROBOT_DEFENSE_

#include "Game.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "PushButton.h"
#include "Infobox.h"

#include <vector>

class RobotDefense : public Game {
private:
    class Robot;
    Texture numbers;
    vector<Robot> robots;
    int counter, period, laserTime;
    float speed;
    Vector cannon;
    
public:
    RobotDefense(const char* titel, Infobox* infobox, const char* scoresFileName);
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class RobotDefense::Robot {
private:
    Vector pos;
    SpriteSheet sprite;
    bool stop;
    
public:
    Robot(int x, int y, int w, int h, Texture* t, int n) : 
        pos(x, y), sprite(t, w, h, n), stop(false) {
    }
    
    bool draw(Vector &cannon) {
        float angle = Vector::angle(cannon - pos, Vector(0, 1)) / PI * 180 + 180;
        if (pos.x < cannon.x)
            angle *= -1;
        
        ofPushMatrix();
        
        ofTranslate(pos.x, pos.y, 0.0);
        ofRotateZ(angle);
        bool destroyed = sprite.draw(0, 0);
        
        ofPopMatrix();
        
        return destroyed;
    }
    
    double move(Vector &goal, float speed) {
        if (!stop)
            pos = Vector::toLength(goal - pos, speed) + pos;
        
        return Vector::distance(goal, pos);
    }
    
    void checkHit(Vector &cannon, Vector &hitPoint) {
        if (pos.distanceToLine(hitPoint, cannon - hitPoint) < 50) {
            sprite.startAnimation();
            stop = true;
        }
    }
};

#endif

