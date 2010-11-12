#include "ShootingCans.h"
#include "ofMain.h"

ShootingCans::ShootingCans(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
    
    textColor = 0xededed;
    canTexture.load("cans.png", true, GL_CLAMP, GL_CLAMP);
    shelfTexture.load("body.png", true, GL_CLAMP, GL_CLAMP);
    background.load("cans_bg.png", true, GL_CLAMP, GL_CLAMP);
}

void ShootingCans::reset() {
    resetGame();
    
    cans.clear();
    shootedCans.clear();
    
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can((4 + i * 2) * WIDTH / 12, HEIGHT / 4 + 5, &canTexture, 150, 125, 1, 3));
    }
    for (int i = 1; i <= 4; i++) {
        cans.push_back(Can(i * WIDTH / 5, HEIGHT / 2 - 20, &canTexture, 150, 125, 1, 3));
    }
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can((4 + i * 2) * WIDTH / 12, HEIGHT / 4 * 3 - 45, &canTexture, 150, 125, 1, 3));
    }
    
    gameStarted = false;
    screenTime = 120;
    
    ofBackground(0xe3, 0xcb, 0xcc);
}
   
bool ShootingCans::draw(bool hit, Vector &hitPoint) {
    
    switch (mode) {
        case INIT:
            startGame();
            infobox->set("bounce at the cans.");
            break;
        case PLAY:
            // check hit
            if (hit) {
                infobox->clear();
                ballHits++;
                
                if (!gameStarted) {
                    gameStarted = true;
                    startTime = ofGetElapsedTimef() + 120;
                }
                
                for (int i = 0; i < cans.size(); i++) {
                    if (cans[i].checkHit(hitPoint)) {
                        points += 100;
                        signs.push_back(Sign("+100", hitPoint, 2.0));
                        
                        shootedCans.push_back(cans[i]);
                        cans.erase(cans.begin() + i);
                        i--;
                        
                        if (points == 1000) {
                            int timeBonus = (startTime - ofGetElapsedTimef()) * 10;
                            points += timeBonus;
                            
                            string bonus = "timebonus: +" + ofToString(timeBonus);
                            signs.push_back(Sign(bonus, Vector((WIDTH - gameFont.stringWidth(bonus)) / 2, HEIGHT / 2), 3.0));
                            
                            points -= ballHits * 10;
                            string minus = "ballthrows: -" + ofToString(ballHits * 10);
                            signs.push_back(Sign(minus, Vector((WIDTH - gameFont.stringWidth(minus)) / 2, HEIGHT / 2 + 40), 4.0));
                        }
                    }
                }
            }
            
            if (gameStarted)
                screenTime = startTime - ofGetElapsedTimef();
            
            if ((cans.size() == 0 && signs.size() == 0) || screenTime <= 0)
                stopGame();
            
            for (int i = 0; i < shootedCans.size(); i++) {
                if (!shootedCans[i].draw()) {
                    shootedCans.erase(shootedCans.begin() + i);
                    i--;
                }
            }
            drawBg();
            break;
        case PANEL:
            drawBg();
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

void ShootingCans::drawBg() {
    shelfTexture.draw(150, 250, WIDTH - 300, HEIGHT - 300);
    background.draw(0, 0, WIDTH, HEIGHT);
    // draw
    for (int i = 0; i < cans.size(); i++) {
        if (!cans[i].draw()) {
            cans.erase(cans.begin() + i);
            i--;
        }
    }
}

ShootingCans::Can::Can(int x, int y, Texture* t, int w, int h, int r, int c) :
    pos(x, y), flying(false),
    sprite(t, w, h, r, c) {
    
    int canType = rand() % 3;
    sprite.setAnimation(0, canType, 0, canType, true);
}

bool ShootingCans::Can::draw() {
    pos = flight + pos;
    
    if (flying)
        flight.y += (ofGetElapsedTimef() - time) * 10;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, 0.0);
    if (flying) {
        ofRotateZ((ofGetElapsedTimef() - time) * flight.x * 20);
        ofScale(1.0 - (ofGetElapsedTimef() - time) / 3, 1.0 - (ofGetElapsedTimef() - time) / 3, 0);
    }
    ofSetColor(255, 255, 255);
    //        ofRect(-sprite.getWidth() / 2, -sprite.getHeight() * 2 / 3, sprite.getWidth(), sprite.getHeight() * 4 / 3);
    sprite.draw(0, 0);
    ofPopMatrix();
    
    return pos.y < HEIGHT + 100;
}

bool ShootingCans::Can::checkHit(Vector &hitPoint) {
    if (!flying && 
        hitPoint.x > pos.x - sprite.getWidth() / 2 && 
        hitPoint.x < pos.x + sprite.getWidth() / 2 &&
        hitPoint.y > pos.y - sprite.getHeight() * 2 / 3 &&
        hitPoint.y < pos.y + sprite.getHeight() * 2 / 3) {
        
        flying = true;
        flight = ((Vector(0, -sprite.getHeight() / 3 * 2) + pos) - hitPoint) / 5;
        time = ofGetElapsedTimef();
        
        return true;
    }
    
    return false;
}

