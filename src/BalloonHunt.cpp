#include "BalloonHunt.h"
#include "ofMain.h"

BalloonHunt::BalloonHunt(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
    
    numbers.load("numbers.png", GL_CLAMP, GL_CLAMP);
}

void BalloonHunt::reset() {
    resetGame();
    
    balloons.clear();
    counter = 0;
}
   
bool BalloonHunt::draw(bool hit, Vector &hitPoint) {
    
    switch (mode) {
        case INIT:
            if (hit) {
                startGame();
                startTime = ofGetElapsedTimef() + 120;
            }
            break;
        case PLAY:
            // new balloons
            counter++;
            if (counter == 100) {
                counter = 0;
                balloons.push_back(Balloon(Balloon::Type(rand() % 3), rand() % (WIDTH - 100) + 50, HEIGHT + 50, &numbers, 32, 1));
            }
            
            // check hit
            if (hit) {
                for (int i = 0; i < balloons.size(); i++) {
                    if (balloons[i].checkHit(hitPoint)) {
                        points += balloons[i].points;
                        signs.push_back(Sign("+" + ofToString(balloons[i].points), balloons[i].getPosition(), 2.0));
                    }
                }
            }
            
            // draw
            for (int i = 0; i < balloons.size(); i++) {
                if (!balloons[i].draw())
                    balloons.erase(balloons.begin() + i);
            }
            
            ofSetColor(0, 0, 0);
            ofDrawBitmapString(ofToString(points), 20, 20);
            ofDrawBitmapString(ofToString(startTime - ofGetElapsedTimef(), 0), WIDTH - 30, 20);
            
            if (startTime - ofGetElapsedTimef() <= 0)
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


