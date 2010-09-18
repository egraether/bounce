#include "ShootingCans.h"

ShootingCans::ShootingCans(Infobox* i, PushButton* m) : Game(i, m) {
    ofImage image;
    bool result = image.loadImage("numbers.png");
    if (!result) {
        cout << "Failed to load " << "numbers.png" << "\n";
        throw "Failed to load numberImage";
    }
    numbers.load(image, GL_CLAMP, GL_CLAMP);
    
    counter = 0;
    actual = 0;
}

void ShootingCans::reset() {
    counter = 0;
    actual = 0;
}
   
bool ShootingCans::draw(bool hit, ofPoint hitPoint) {
    ofPushMatrix();
    ofTranslate(hitPoint.x + numbers.width / 2, hitPoint.y + numbers.height / 64, 0.0);
    ofRotateZ(3.6 * counter);
    ofTranslate(-hitPoint.x - numbers.width / 2, -hitPoint.y - numbers.height / 64, 0.0);
    numbers.draw(hitPoint.x, hitPoint.y, numbers.width, numbers.height / 32, 0.0, actual * 1.0 / 32, 1.0, 1.0 / 32);
    ofPopMatrix();
    
    counter++;
    if (counter == 100) {
        counter = 0;
        actual++;
        if (actual == 10) {
            actual = 0;
        }
    }
    return true;
}


