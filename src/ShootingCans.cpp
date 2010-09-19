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
    sprite.load(&numbers, 100, 100, 32);
    
    counter = 0;
}

void ShootingCans::reset() {
    counter = 0;
}
   
bool ShootingCans::draw(bool hit, Vector hitPoint) {
    ofPushMatrix();
    ofTranslate(hitPoint.x, hitPoint.y, 0.0);
    
    Vector a(0, 1), b(WIDTH / 2, HEIGHT);
    float angle = Vector::angle(b - hitPoint, a) / PI * 180 + 180;
    
    if (hitPoint.x < WIDTH / 2)
        angle *= -1;
    
    ofRotateZ(angle);
    ofTranslate(-hitPoint.x, -hitPoint.y, 0.0);
    sprite.draw(hitPoint.x, hitPoint.y);
    ofPopMatrix();
    
    counter++;
    if (counter == 100) {
        sprite.startAnimation();
        counter = 0;
    }
    return true;
}


