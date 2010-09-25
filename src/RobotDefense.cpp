#include "RobotDefense.h"
#include "ofMain.h"

#include <cmath>

RobotDefense::RobotDefense(Infobox* i) : 
    Game(i), cannon(WIDTH / 2, HEIGHT) {
        
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
   
bool RobotDefense::draw(bool hit, Vector &hitPoint) {
    // destroy hit robots
    if (hit) {
        laserTime = 0;
        for (int i = 0; i < robots.size(); i++) {
            robots[i].checkHit(cannon, hitPoint);
        }
    }
    
    // new robots
    counter++;
    if (counter == period) {
        float angle = float(rand() % 61 + 30) / 180 * PI;
        
        int x = cannon.x + cos(angle) * WIDTH * (rand() % 2 ? 1 : -1), 
            y = cannon.y - sin(angle) * WIDTH;
        
        robots.push_back(Robot(x, y, 100, 100, &numbers, 32));
        
        counter = 0;
        speed += 0.1;
        //period -= 10;
    }
    
    // move and draw robots
    for (int i = 0; i < robots.size(); i++) {
        
        if (robots[i].move(cannon, speed) <= 50)
            return false;
        
        if (!robots[i].draw(cannon))
            robots.erase(robots.begin() + i);
    }
    
    // draw cannon
    float angle = Vector::angle(hitPoint - cannon, Vector(0, -1)) / PI * 180;
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


