#include "ShootingCans.h"
#include "ofMain.h"

ShootingCans::ShootingCans(Infobox* i, PushButton* m) : Game(i, m) {
    ofImage image;
    bool result = image.loadImage("numbers.png");
    if (!result) {
        cout << "Failed to load " << "numbers.png" << "\n";
        throw "Failed to load numberImage";
    }
    numbers.load(image, GL_CLAMP, GL_CLAMP);
}

void ShootingCans::reset() {
    cans.clear();
    
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can(200 + i * 100, 150, 50, 50, &numbers, 32));
    }
    for (int i = 0; i < 4; i++) {
        cans.push_back(Can(150 + i * 100, 250, 50, 50, &numbers, 32));
    }
    for (int i = 0; i < 3; i++) {
        cans.push_back(Can(200 + i * 100, 350, 50, 50, &numbers, 32));
    }
}
   
bool ShootingCans::draw(bool hit, Vector &hitPoint) {
    // check hit
    if (hit) {
        for (int i = 0; i < cans.size(); i++) {
            cans[i].checkHit(hitPoint);
        }
    }
    
    // draw
    for (int i = 0; i < cans.size(); i++) {
        if (!cans[i].draw())
            cans.erase(cans.begin() + i);
    }
    
    if (cans.size() == 0)
        return false;
    
    return true;
}


