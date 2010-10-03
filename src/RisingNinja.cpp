#include "RisingNinja.h"
#include "ofMain.h"

RisingNinja::RisingNinja(const char* titel, Infobox* infobox, const char* scoresFileName) : 
Game(titel, infobox, scoresFileName) {
    
    numbers.load("numbers.png", true, GL_CLAMP, GL_CLAMP);
    numberAnimation.load(&numbers, 100, 100, 32, 1);
}

void RisingNinja::reset() {
    resetGame();
}

bool RisingNinja::draw(bool hit, Vector &hitPoint) {
    
    switch (mode) {
        case INIT:
            if (hit) {
                startGame();
                infobox->set("This is RisingNinja have fun, bounce to stop");
                numberAnimation.setAnimation(10, 0, 0, 0);
            }
            break;
        case PLAY:
            if (!numberAnimation.draw(100, 100)) {
                signs.push_back(Sign("Dave sucks!", Vector(100, 100), 3.0));
                stopGame();
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