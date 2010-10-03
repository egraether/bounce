#include "RobotDefense.h"
#include "ofMain.h"

#include <cmath>

RobotDefense::RobotDefense(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName), cannon(WIDTH / 2, HEIGHT - 80) {
    
    robotTexture[0].load("robot_a.png", true, GL_CLAMP, GL_CLAMP);
    robotTexture[1].load("robot_b.png", true, GL_CLAMP, GL_CLAMP);
    cannonTexture.load("gun.png", true, GL_CLAMP, GL_CLAMP);
    laserTexture.load("laser.png", true, GL_CLAMP, GL_CLAMP);
    background = new Texture();
    background->load("robot_bg.png", false, GL_CLAMP, GL_CLAMP);
}

void RobotDefense::reset() {
    resetGame();
    
    robots.clear();
    counter = 0;
    period = 100;
    speed = 1;
    laserTime = 100;
    
    gameOver = false;
}
   
bool RobotDefense::draw(bool hit, Vector &hitPoint) {
    background->draw(0, 0, WIDTH, HEIGHT);
    
    switch (mode) {
        case INIT:
            startGame();
            break;
        case PLAY:
            // destroy hit robots
            if (hit) {
                laserTime = 0;
                for (int i = 0; i < robots.size(); i++) {
                    if (robots[i].checkHit(cannon, hitPoint)) {
                        points += 10;
                        signs.push_back(Sign("+10", robots[i].getPosition(), 2.0));
                    }
                }
            }
            
            // new robots
            counter++;
            if (counter == period && !gameOver) {
                float angle = float(rand() % 61 + 30) / 180 * PI;
                
                int x = cannon.x + cos(angle) * WIDTH * (rand() % 2 ? 1 : -1), 
                y = cannon.y - sin(angle) * WIDTH;
                
                robots.push_back(Robot(x, y, 100, 100, &robotTexture[rand() % 2], 1, 4));
                
                counter = 0;
                speed += 0.1;
                
                if (period > 10)
                    period--;
            }
            
            // move robots
            if (!gameOver) {
                for (int i = 0; i < robots.size(); i++) {
                    if (robots[i].move(cannon, speed) <= 50) {
                        gameOver = true;
                        int timeBonus = ofGetElapsedTimef() - startTime;
                        points += timeBonus;
                        signs.push_back(Sign("Timebonus: +" + ofToString(timeBonus), Vector(WIDTH / 2, HEIGHT / 2), 5.0));
                        
                        for (int j = 0; j < robots.size(); j++) {
                            robots[j].destroy();
                        }
                    }
                }
            }
            
            // draw robots
            for (int i = 0; i < robots.size(); i++) {
                if (!robots[i].draw(cannon)) {
                    robots.erase(robots.begin() + i);
                    i--;
                }
            }
            
            // draw cannon
            {
                float angle = Vector::angle(hitPoint - cannon, Vector(0, -1)) / PI * 180;
                if (hitPoint.x < cannon.x)
                    angle *= -1;
                
                ofPushMatrix();
                ofTranslate(cannon.x, cannon.y, 0.0);
                ofRotateZ(angle);
                ofTranslate(-cannon.x, -cannon.y, 0.0);
                
                // shoot
                laserTime++;
                if (laserTime < 20)
                    laserTexture.draw(cannon.x - 75, cannon.y - WIDTH, 150, WIDTH);
                
                cannonTexture.draw(cannon.x - 100, cannon.y - 100, 200, 200);
                
                ofPopMatrix();
            }
            
            ofSetColor(0, 0, 0);
            ofDrawBitmapString(ofToString(points), 20, 20);
            ofDrawBitmapString(ofToString(ofGetElapsedTimef() - startTime, 0), WIDTH - 30, 20);
            
            if (gameOver && robots.size() == 0)
                stopGame();
            break;
        case PANEL:
            drawPanel();
            
            if (hit && !insertName)
                return false;
            
            break;            
        default:
            break;
    }
    
    drawSigns();
    
    return true;
}


