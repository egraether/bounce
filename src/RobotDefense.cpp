#include "RobotDefense.h"
#include "ofMain.h"

#include <cmath>

RobotDefense::RobotDefense(Infobox* i, PushButton* m) : 
    Game(i, m), cannon(WIDTH / 2, HEIGHT) {
        
    ofImage image;
    bool result = image.loadImage("numbers.png");
    if (!result) {
        cout << "Failed to load " << "numbers.png" << "\n";
        throw "Failed to load numberImage";
    }
    numbers.load(image, GL_CLAMP, GL_CLAMP);
}

void RobotDefense::reset() {
    robots.clear();
    counter = 0;
    period = 100;
    speed = 1;
    laserTime = 100;
}
   
bool RobotDefense::draw(bool hit, Vector hitPoint) {
    // destroy hit robots
    if (hit) {
        laserTime = 0;
        Vector laser = Vector::getVector(hitPoint, cannon);
        for (int i = 0; i < robots.size(); i++) {
            if (robots[i].pos.distanceToLine(hitPoint, laser) < 50) {
                robots[i].sprite.startAnimation();
                robots[i].stop = true;
            }
        }
    }
    
    // new robots
    counter++;
    if (counter == period) {
        float angle = float(rand() % 61 + 30) / 180 * PI;
        Vector pos(cannon.x + cos(angle) * WIDTH * (rand() % 2 ? 1 : -1), cannon.y - sin(angle) * WIDTH);
        
        SpriteSheet s;
        s.load(&numbers, 100, 100, 32);
        
        robots.push_back(Robot(pos, s));
        counter = 0;
        
        speed += 0.1;
        //period -= 10;
    }
    
    // move and draw robots
    for (int i = 0; i < robots.size(); i++) {
        Vector toGoal = Vector::getVector(robots[i].pos, cannon);
        
        if (!robots[i].stop) {
            robots[i].pos = Vector::add(robots[i].pos, Vector::toLength(toGoal, speed));
            if (Vector::norm(toGoal) <= 50)
                return false;
        }
        
        float angle = Vector::angle(toGoal, Vector(0, 1)) / PI * 180 + 180;
        if (robots[i].pos.x < cannon.x)
            angle *= -1;
        
        ofPushMatrix();
        ofTranslate(robots[i].pos.x, robots[i].pos.y, 0.0);
        ofRotateZ(angle);
        ofTranslate(-robots[i].pos.x, -robots[i].pos.y, 0.0);
        
        if (!robots[i].sprite.draw(robots[i].pos.x, robots[i].pos.y))
            robots.erase(robots.begin() + i);
        
        ofPopMatrix();
    }
    
    // draw cannon
    float angle = Vector::angle(Vector::getVector(cannon, hitPoint), Vector(0, -1)) / PI * 180;
    if (hitPoint.x < cannon.x)
        angle *= -1;
    
    ofPushMatrix();
    ofTranslate(cannon.x, cannon.y, 0.0);
    ofRotateZ(angle);
    ofTranslate(-cannon.x, -cannon.y, 0.0);
    
    // shoot
    laserTime++;
    if (laserTime < 20) {
        ofSetColor(100, 200, 0);
        ofRect(cannon.x - 10, cannon.y - WIDTH, 20, WIDTH);
    }
    
    ofSetColor(150, 150, 150);
    ofRect(cannon.x - 20, cannon.y - 80, 40, 60);
    ofSetColor(200, 200, 200);
    ofCircle(cannon.x, cannon.y, 50);
    
    ofPopMatrix();
    
    return true;
}


