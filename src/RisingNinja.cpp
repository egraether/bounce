// not finished, do not try to start

#include "RisingNinja.h"
#include "ofMain.h"

RisingNinja::RisingNinja(const char* titel, Infobox* infobox, const char* scoresFileName) : 
Game(titel, infobox, scoresFileName), hookSize(5), ninjaSize(10), width(600), platformDistance(200) {
    
}

void RisingNinja::reset() {
    resetGame();
    
    ninjaPosition.set(width / 2, HEIGHT - 50);
    ninjaMotion.set(0, 0);
    hook = false;
    line = false;
    started = false;
    
    int startY = HEIGHT - 10;
    while (startY > 0) {
        platforms.push_back(Platform(width, width / 2, startY));
        startY -= platformDistance;
    }
}

bool RisingNinja::draw(bool hit, Vector &hitPoint) {
    
    ofSetColor(0x777777);
    ofRect((WIDTH - width) / 2 - 5, 0, 5, HEIGHT);
    ofRect((WIDTH - width) / 2 + width, 0, 5, HEIGHT);
    
    switch (mode) {
        case INIT:
            startGame();
            break;
        case PLAY:
            ofPushMatrix();
            ofTranslate((WIDTH - width) / 2, 0, 0);
            
            if (hit) {
                started = true;
                ninjaHitPoint = hitPoint;
                ninjaHitPoint.x -= (WIDTH - width) / 2;
                
                if (ninjaHitPoint.y < ninjaPosition.y - 50) {
                    hook = true;
                    hookPosition = ninjaPosition;
                    hookMotion = Vector::toLength(ninjaHitPoint - ninjaPosition, 50);
                }
            }
            
            // draw Ninja
            ninjaMotion.x--;
            if (ninjaMotion.x < 0)
                ninjaMotion.x = 0;
            
            ninjaPosition = ninjaPosition + ninjaMotion;
            if (ninjaPosition.x > width - ninjaSize || ninjaPosition.x < ninjaSize)
                ninjaMotion.x *= -1;
            
            if (!line)
                ninjaPosition.y += ofGetElapsedTimef() - gravitationTime;
            
            ofSetColor(0x999999);
            ofRect(ninjaPosition.x - ninjaSize, ninjaPosition.y - ninjaSize, ninjaSize * 2, ninjaSize * 2);
            
            if (ninjaPosition.y > HEIGHT)
                stopGame();
            
            // draw Hook
            if (hook) {
                hookPosition = hookPosition + hookMotion;
                if (hookPosition.x > width - hookSize || hookPosition.x < hookSize)
                    hookMotion.x *= -1;
                if (hookPosition.y < 0)
                    hook = false;
                
                ofRect(hookPosition.x - hookSize, hookPosition.y - hookSize, hookSize * 2, hookSize * 2);
            }
            
            // draw platforms
            for (int i = 0; i < platforms.size(); i++) {
                if (!platforms[i].draw()) {
                    platforms.erase(platforms.begin() + i);
                    platforms.push_back(Platform(width, width / 2, --(platforms.begin() + platforms.size())->position.y - platformDistance));
                }
            }
            
            ofPopMatrix();
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

//bool RisingNinja::checkOnPlatform() {
//    for (int i = 0; i < platforms.size(); i++) {
//        if (ninjaPosition.x - ninjaSize < platforms[i].x + platforms[i].width / 2 &&
//            ninjaPosition.x + ninjaSize > platforms[i].x - platforms[i].width / 2 &&)
//    }
//}
//
//bool RisingNinja::checkHookHit(Vector &lineEnd) {
//    for (int i = 0; i < platforms.size(); i++) {
//        if ()
//    }
//}