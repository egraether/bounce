#include "BalloonHunt.h"
#include "ofMain.h"

int BalloonHunt::Balloon::counter = 0;
int BalloonHunt::Balloon::colors[6] = {
    0xff8888, 0xffcc88, 0x8888ff, 0xffff88, 0xff88ff, 0x88ffff
};

BalloonHunt::BalloonHunt(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
    
    balloonTexture.load("balloon.png", true, GL_CLAMP, GL_CLAMP);
    background.load("balloon_bg.png", false, GL_CLAMP, GL_CLAMP);
}

void BalloonHunt::reset() {
    resetGame();
    
    balloons.clear();
    counter = 0;
    
    Balloon::counter = 1;
    screenTime = 60;
}
   
bool BalloonHunt::draw(bool hit, Vector &hitPoint) {
    background.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    switch (mode) {
        case INIT:
            startGame();
            startTime = ofGetElapsedTimef() + 60;
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

BalloonHunt::Balloon::Balloon(Texture* tex, int rows, int columns) : 
    explode(false) {
        
    switch (counter % 3) {
        case 0:
            sprite.load(tex, 300, 300, rows, columns);
            size = 120;
            points = 10;
            speed = 5;
            balloonCenter.set(0, -50);
            break;
        case 1:
            sprite.load(tex, 200, 200, rows, columns);
            size = 80;
            points = 50;
            speed = 2;
            balloonCenter.set(0, -36);
            break;
        case 2:
            sprite.load(tex, 120, 120, rows, columns);
            size = 60;
            points = 100;
            speed = 1;
            balloonCenter.set(0, -17);
            break;
        default:
            break;
    }
    
    color = colors[rand() % 6];
    pos.set(rand() % (ofGetWidth() - 300) + 150, ofGetHeight() + size);
    sprite.setAnimation(0, 0, 0, 1, true);
    counter++;
}

bool BalloonHunt::Balloon::draw() {
    if (!explode)
        pos.y -= speed;
    
    if (pos.y < -100)
        return false;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, 0.0);
    ofCircle(balloonCenter.x, balloonCenter.y, size);
    ofSetColor(color);
    bool destroyed = sprite.draw(0, 0, false);
    ofPopMatrix();
    
    return destroyed;
}

bool BalloonHunt::Balloon::checkHit(Vector &hitPoint) {
    if (!explode && Vector::distance(hitPoint, pos + balloonCenter) <= size) {
        explode = true;
        sprite.setAnimation(0, 2, 0, 5, false, 3);
        return true;
    }
    return false;
}

Vector BalloonHunt::Balloon::getPosition() {
    return pos;
}


