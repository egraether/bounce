#include "ShootingCans.h"

ShootingCans::ShootingCans(Infobox* i, PushButton* m) : Game(i, m) {
    ofImage image;
    bool result = image.loadImage("numbers.png");
    if (!result) {
        cout << "Failed to load " << "numbers.png" << "\n";
        throw "Failed to load numberImage";
    }
    numbers.load(image, GL_CLAMP, GL_CLAMP);
    sprite.load(&numbers, 32);
    
    counter = 0;
}

void ShootingCans::reset() {
    counter = 0;
}
   
bool ShootingCans::draw(bool hit, ofPoint hitPoint) {
    ofPushMatrix();
    ofTranslate(hitPoint.x + sprite.getWidth() / 2, hitPoint.y + sprite.getHeight() / 2, 0.0);
    ofRotateZ(3.6 * counter);
    ofTranslate(-hitPoint.x - sprite.getWidth() / 2, -hitPoint.y - sprite.getHeight() / 2, 0.0);
    sprite.draw(hitPoint.x, hitPoint.y);
    ofPopMatrix();
    
    counter++;
    if (counter == 100) {
        sprite.startAnimation();
        counter = 0;
    }
    return true;
}


