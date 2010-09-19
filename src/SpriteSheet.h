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
    int width, height;
    
    SpriteSheet() : 
        actualPic(0), animate(false), counter(0) {
    }
    
    SpriteSheet(Texture* t, int w, int h, int n, int s = ANIMATION_SPEED) : 
        actualPic(0), animate(false), counter(0), 
        tex(t), width(w), height(h), numPics(n), speed(s) {
    }
    
    void load(Texture* t, int w, int h, int n, int s = ANIMATION_SPEED) { 
        tex = t;
        width = w;
        height = h;
        numPics = n;
        speed = s;
    }
    
    bool draw(int x, int y) {
        counter++;
        if (counter > speed)
            counter = 0;
        
        if (animate && counter == speed)
            actualPic++;
        
        if (actualPic == 10)
            return false;
        
        tex->draw(
            x - width / 2, y - height / 2, 
            width, height, 
            0.0, actualPic * 1.0 / numPics, 
            1.0, 1.0 / numPics
        );
        
        return true;
    }
    
    int getWidth() {
        return width;
    }
    
    int getHeight() {
        return height;
    }
    
    void startAnimation() {
        animate = true;
    }
};

#endif