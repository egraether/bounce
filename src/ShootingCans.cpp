#include "ShootingCans.h"
#include "ofMain.h"

ShootingCans::ShootingCans(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
    
    numbers.load("numbers.png", GL_CLAMP, GL_CLAMP);
}

void ShootingCans::reset() {
    resetGame();
    
    cans.clear();
    
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can((3 + i * 2) * WIDTH / 10, HEIGHT / 4, &numbers, 100, 100, 32, 1));
    }
    for (int i = 1; i <= 4; i++) {
        cans.push_back(Can(i * WIDTH / 5, HEIGHT / 2, &numbers, 100, 100, 32, 1));
    }
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can((3 + i * 2) * WIDTH / 10, HEIGHT / 4 * 3, &numbers, 100, 100, 32, 1));
    }
    
    gameStarted = false;
}
   
bool ShootingCans::draw(bool hit, Vector &hitPoint) {
    
    switch (mode) {
        case INIT:
            startGame();
            break;
        case PLAY:
            // check hit
            if (hit) {
                if (!gameStarted) {
                    gameStarted = true;
                    startTime = ofGetElapsedTimef() + 120;
                }
                
                for (int i = 0; i < cans.size(); i++) {
                    if (cans[i].checkHit(hitPoint)) {
                        points += 100;
                        signs.push_back(Sign("+100", hitPoint, 2.0));
                        
                        if (points == 1000) {
                            int timeBonus = (startTime - ofGetElapsedTimef()) * 10;
                            points += timeBonus;
                            signs.push_back(Sign("Timebonus: +" + ofToString(timeBonus), Vector(WIDTH / 2, HEIGHT / 2), 5.0));
                        }
                    }
                }
            }
            
            // draw
            for (int i = 0; i < cans.size(); i++) {
                if (!cans[i].draw())
                    cans.erase(cans.begin() + i);
            }
            
            ofSetColor(0, 0, 0);
            if (gameStarted)
                ofDrawBitmapString(ofToString(startTime - ofGetElapsedTimef(), 0), WIDTH - 30, 20);
            else
                ofDrawBitmapString("120", WIDTH - 30, 20);
            
            ofDrawBitmapString(ofToString(points), 20, 20);
            
            if (cans.size() == 0 || (gameStarted && startTime - ofGetElapsedTimef() < 0))
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


