#include "RobotDefense.h"
#include "ofMain.h"

#include <cmath>

RobotDefense::RobotDefense(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName), cannon(WIDTH / 2, HEIGHT - 80) {
    
    robotTexture[0].load("robot_a.png", true, GL_CLAMP, GL_CLAMP);
    robotTexture[1].load("robot_b.png", true, GL_CLAMP, GL_CLAMP);
    robotTexture[2].load("robot_c.png", true, GL_CLAMP, GL_CLAMP);
    cannonTexture.load("gun.png", true, GL_CLAMP, GL_CLAMP);
    laserTexture.load("laser.png", true, GL_CLAMP, GL_CLAMP);
    background.load("robot_bg.png", false, GL_CLAMP, GL_CLAMP);
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
    background.draw(0, 0, WIDTH, HEIGHT);
    
    switch (mode) {
        case INIT:
            startGame();
            counter = 99;
            infobox->set("bounce at the robots.");
            break;
        case PLAY:
            // destroy hit robots
            if (hit && !gameOver) {
                infobox->clear();
                int comboCounter = 0;
                laserTime = 0;
                
                for (int i = 0; i < robots.size(); i++) {
                    if (robots[i].checkHit(cannon, hitPoint)) {
                        points += 10;
                        comboCounter++;
                        signs.push_back(Sign("+10", robots[i].getPosition(), 2.0));
                    }
                }
                
                if (comboCounter > 1) {
                    string combo = "+" + ofToString(10 * comboCounter);
                    points += comboCounter * (comboCounter - 1) * 10;
                    
                    for (int i = signs.size() - comboCounter; i < signs.size(); i++) {
                        signs[i].update(combo);
                    }
                    
                    combo = "combo: x" + ofToString(comboCounter);
                    signs.push_back(Sign(combo, Vector((WIDTH - gameFont.stringWidth(combo)) / 2, HEIGHT / 2), 2.0));
                }
            }
            
            // new robots
            counter++;
            if (counter == period && !gameOver) {
                float angle = float(rand() % 61 + 30) / 180 * PI;
                int x = cannon.x + cos(angle) * WIDTH * 3 / 4 * (rand() % 2 ? 1 : -1), 
                y = cannon.y - sin(angle) * WIDTH * 3 / 4;
                
                robots.push_back(Robot(x, y, 100, 100, &robotTexture[rand() % 3], 1, 14));
                
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
                        
                        string bonus = "timebonus: +" + ofToString(timeBonus);
                        signs.push_back(Sign(bonus, Vector((WIDTH - gameFont.stringWidth(bonus)) / 2, HEIGHT / 2), 3.0));
                        
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
                if (laserTime < 20) {
                    ofEnableAlphaBlending();
                    if (laserTime < 3)
                        ofSetColor(255, 255, 255, 255 * laserTime / 3);
                    else if (laserTime < 10)
                        ofSetColor(255, 255, 255, 255);
                    else
                        ofSetColor(255, 255, 255, (1.0 - (laserTime - 10) / 10.0) * 255);
                    laserTexture.draw(cannon.x - 75, cannon.y - WIDTH - laserTime * 5 + 50, 150, WIDTH, false);
                    ofDisableAlphaBlending();
                    
                }
                
                cannonTexture.draw(cannon.x - 82, cannon.y - 140, 164, 280);
                
                ofPopMatrix();
            }
            
            screenTime = ofGetElapsedTimef() - startTime;
            
            if (gameOver && robots.size() == 0 && signs.size() == 0)
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

RobotDefense::Robot::Robot(int x, int y, int width, int height, Texture* texture, int rows, int columns) : 
    pos(x, y), 
    sprite(texture, width, height, rows, columns), 
    stop(false) {
    
    sprite.setAnimation(0, 0, 0, 0, true);
}

bool RobotDefense::Robot::draw(Vector &cannon) {
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

double RobotDefense::Robot::move(Vector &goal, float speed) {
    if (!stop)
        pos = Vector::toLength(goal - pos, speed) + pos;
    
    return Vector::distance(goal, pos);
}

bool RobotDefense::Robot::checkHit(Vector &cannon, Vector &hitPoint) {
    if (!stop && pos.distanceToLine(hitPoint, cannon - hitPoint) < 50) {
        destroy();
        return true;
    }
    return false;
}

Vector RobotDefense::Robot::getPosition() {
    return pos;
}

void RobotDefense::Robot::destroy() {
    sprite.setAnimation(0, 1, 0, 13, false, 3);
    stop = true;
}

