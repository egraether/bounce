#ifndef _ROBOT_DEFENSE_
#define _ROBOT_DEFENSE_

#include "Game.h"
#include "Texture.h"
#include "SpriteAnimation.h"
#include "PushButton.h"
#include "Infobox.h"

#include <vector>

class RobotDefense : public Game {
private:
    class Robot;
    vector<Robot> robots;
    int counter, period, laserTime;
    float speed;
    Vector cannon;
    Texture robotTexture[3];
    Texture cannonTexture;
    Texture laserTexture;
    
    bool gameOver;
    
public:
    RobotDefense(const char* titel, Infobox* infobox, const char* scoresFileName);
    virtual void reset();
    virtual bool draw(bool hit, Vector &hitPoint);
};

class RobotDefense::Robot {
private:
    Vector pos;
    SpriteAnimation sprite;
    bool stop;
    
public:
    Robot(int x, int y, int width, int height, Texture* texture, int rows, int columns) : 
        pos(x, y), sprite(texture, width, height, rows, columns), stop(false) {
            
        sprite.setAnimation(0, 0, 0, 0, true);
    }
    
    bool draw(Vector &cannon) {
        float angle = Vector::angle(cannon - pos, Vector(0, 1)) / PI * 180 + 180;
        if (pos.x < cannon.x)
            angle *= -1;
        
        angle += 180;
        
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
    
    bool checkHit(Vector &cannon, Vector &hitPoint) {
        if (!stop && pos.distanceToLine(hitPoint, cannon - hitPoint) < 50) {
            destroy();
            return true;
        }
        return false;
    }
    
    Vector getPosition() {
        return pos;
    }
    
    void destroy() {
        sprite.setAnimation(0, 1, 0, 13, false, 3);
        stop = true;
    }
};

#endif

