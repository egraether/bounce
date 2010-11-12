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
    Robot(int x, int y, int width, int height, Texture* texture, int rows, int columns);
    
    bool draw(Vector &cannon);
    double move(Vector &goal, float speed);
    
    bool checkHit(Vector &cannon, Vector &hitPoint);
    Vector getPosition();
    void destroy();
};

#endif

