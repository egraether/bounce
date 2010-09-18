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
    struct Robot {
        Vector pos;
        SpriteSheet sprite;
        bool stop;
        Robot(Vector p, SpriteSheet s) : pos(p), sprite(s), stop(false) {}
    };
    
    Texture numbers;
    vector<Robot> robots;
    int counter, period, laserTime;
    float speed;
    Vector cannon;
    
public:
    RobotDefense(Infobox* i, PushButton* m);
    virtual void reset();
    virtual bool draw(bool hit, Vector hitPoint);
};

#endif

