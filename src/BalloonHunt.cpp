#include "BalloonHunt.h"
#include "ofMain.h"

int BalloonHunt::Balloon::counter = 0;
int BalloonHunt::Balloon::colors[6] = {
    0xff8888, 0xffcc88, 0x8888ff, 0xffff88, 0xff88ff, 0x88ffff
};

BalloonHunt::BalloonHunt(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
    
    balloonTexture.load("balloon.png", true, GL_CLAMP, GL_CLAMP);
    background = new Texture();
    background->load("balloon_bg.png", false, GL_CLAMP, GL_CLAMP);
}

void BalloonHunt::reset() {
    resetGame();
    
    balloons.clear();
    counter = 0;
    Balloon::counter = 1;
    screenTime = 120;
}
   
bool BalloonHunt::draw(bool hit, Vector &hitPoint) {
    background->draw(0, 0, WIDTH, HEIGHT);
    
    switch (mode) {
        case INIT:
            startGame();
            startTime = ofGetElapsedTimef() + 120;
            counter = 99;
            infobox->set("bounce at the balloons.");
            break;
        case PLAY:
            // new balloons
            counter++;
            if (counter == 100) {
                counter = 0;
                balloons.push_back(Balloon(&balloonTexture, 1, 6));
            }
            
            // check hit
            if (hit) {
                infobox->clear();
                for (int i = 0; i < balloons.size(); i++) {
                    if (balloons[i].checkHit(hitPoint)) {
                        points += balloons[i].points;
                        signs.push_back(Sign("+" + ofToString(balloons[i].points), balloons[i].getPosition(), 2.0));
                    }
                }
            }
            
            // draw
            for (int i = 0; i < balloons.size(); i++) {
                if (!balloons[i].draw()) {
                    balloons.erase(balloons.begin() + i);
                    i--;
                }
            }
            
            screenTime = startTime - ofGetElapsedTimef();
            if (screenTime <= 0) {
                stopGame();
                screenTime = 0;
            }
            
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


