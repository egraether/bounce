#ifndef _SPRITE_SHEET_
#define _SPRITE_SHEET_

#include "Texture.h"
#include "constants.h"

class SpriteSheet {
private:
    Texture* tex;
    int numPics, actualPic;
    int speed, counter;
    bool animate;
    
public:
    SpriteSheet() : 
        actualPic(0), animate(false), counter(0) {
    }
    
    void load(Texture* t, int n, int s = ANIMATION_SPEED) { 
        tex = t;
        numPics = n;
        speed = s;
    }
    
    void draw(int x, int y) {
        counter++;
        if (counter > speed)
            counter = 0;
        
        if (animate && actualPic < 10/*numPics - 1*/ && counter == speed)
            actualPic++;
        
        tex->draw(
            x - tex->width / 2, y - tex->height / numPics / 2, 
            tex->width, tex->height / numPics, 
            0.0, actualPic * 1.0 / numPics, 
            1.0, 1.0 / numPics
        );
    }
    
    int getWidth() {
        return tex->width;
    }
    
    int getHeight() {
        return tex->height / numPics;
    }
    
    void startAnimation() {
        animate = true;
    }
};

#endif