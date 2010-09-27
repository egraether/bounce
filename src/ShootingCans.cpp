#include "ShootingCans.h"
#include "ofMain.h"

ShootingCans::ShootingCans(const char* titel, Infobox* infobox, const char* scoresFileName) : 
    Game(titel, infobox, scoresFileName) {
        
    ofImage image;
    bool result = image.loadImage("numbers.png");
    if (!result) {
        cout << "Failed to load " << "numbers.png" << "\n";
        throw "Failed to load numberImage";
    }
    numbers.load(image, GL_CLAMP, GL_CLAMP);
}

void ShootingCans::reset() {
    resetGame();
    
    cans.clear();
    
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can((3 + i * 2) * WIDTH / 10, HEIGHT / 4, &numbers, 100, 100, 1, 32));
    }
    for (int i = 1; i <= 4; i++) {
        cans.push_back(Can(i * WIDTH / 5, HEIGHT / 2, &numbers, 100, 100, 1, 32));
    }
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can((3 + i * 2) * WIDTH / 10, HEIGHT / 4 * 3, &numbers, 100, 100, 1, 32));
    }
}
   
bool ShootingCans::draw(bool hit, Vector &hitPoint) {
    // check hit
    if (hit) {
        for (int i = 0; i < cans.size(); i++) {
            if (cans[i].checkHit(hitPoint)) {
                points += 100;
                infobox->set(ofToString(points).c_str());
            }
        }
    }
    
    // draw
    for (int i = 0; i < cans.size(); i++) {
        if (!cans[i].draw())
            cans.erase(cans.begin() + i);
    }
    
    if (gameOver) {
        drawPanel();
        
        if (hit && !insertName)
            return false;
    }
    else if (cans.size() == 0)
        stopGame();
    
    return true;
}


