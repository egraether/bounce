#include "BalloonHunt.h"
#include "ofMain.h"

BalloonHunt::BalloonHunt(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
    
    ofImage image;
    bool result = image.loadImage("numbers.png");
    if (!result) {
        cout << "Failed to load " << "numbers.png" << "\n";
        throw "Failed to load numberImage";
    }
    numbers.load(image, GL_CLAMP, GL_CLAMP);
}

void BalloonHunt::reset() {
    resetGame();
    
    balloons.clear();
    counter = 0;
    startTime = ofGetElapsedTimef();
}
   
bool BalloonHunt::draw(bool hit, Vector &hitPoint) {
    // new balloons
    counter++;
    if (counter == 50) {
        counter = 0;
        balloons.push_back(Balloon(Balloon::Type(rand() % 3), rand() % (WIDTH - 100) + 50, HEIGHT + 50, &numbers, 32));
    }
    
    // check hit
    if (hit) {
        for (int i = 0; i < balloons.size(); i++) {
            balloons[i].checkHit(hitPoint);
        }
    }
    
    // draw
    for (int i = 0; i < balloons.size(); i++) {
        if (!balloons[i].draw())
            balloons.erase(balloons.begin() + i);
    }
    
    if (ofGetElapsedTimef() - startTime > 60)
        return false;
    
    return true;
}


