#ifndef _SPRITE_ANIMATION_
#define _SPRITE_ANIMATION_

#include "Texture.h"

class SpriteAnimation {
private:
    Texture* texture;
    bool isAnimationSet, isLastFrame, repeat;
    
    unsigned int fromRow, fromColumn, toRow, toColumn;
    unsigned int actualRow, actualColumn;
    bool increaseRow, increaseColumn;
    
    unsigned int spritesPerRow, spritesPerColumn;
    
    unsigned int frameSpeed, frameCounter;
    
    unsigned int width, height;
    
    const static int animationSpeed = 7;
    
public:
    SpriteAnimation() : 
        isAnimationSet(false), isLastFrame(false), width(0), height(0) {
    }
    
    SpriteAnimation(Texture* _texture, unsigned int _width, unsigned int _height, 
                    unsigned int _spritesPerRow, unsigned int _spritesPerColumn) : 
    
        texture(_texture), isAnimationSet(false), isLastFrame(false), 
        width(_width), height(_height), 
        spritesPerRow(_spritesPerRow), spritesPerColumn(_spritesPerColumn) {
    }
    
    void load(Texture* _texture, unsigned int _width, unsigned int _height, 
              unsigned int _spritesPerRow, unsigned int _spritesPerColumn) { 
    
        texture = _texture;
        width = _width;
        height = _height; 
        
        spritesPerRow = _spritesPerRow;
        spritesPerColumn = _spritesPerColumn;
    }
    
    bool draw(int x, int y, bool white = true) {
        if (isAnimationSet) {
            frameCounter++;
            if (frameCounter == frameSpeed) {
                frameCounter = 0;
                
                if (actualRow < toRow && increaseRow)
                    actualRow++;
                else if (actualRow > toRow && !increaseRow)
                    actualRow--;
                
                if (actualColumn < toColumn && increaseColumn)
                    actualColumn++;
                else if (actualColumn > toColumn && !increaseColumn)
                    actualColumn--;
                
                if (isLastFrame) {
                    isLastFrame = false;
                    
                    if (repeat) {
                        actualRow = fromRow;
                        actualColumn = fromColumn;
                    }
                    else
                        isAnimationSet = false;
                }
                else if (actualRow == toRow && actualColumn == toColumn)
                    isLastFrame = true;
            }
            
            texture->draw(
                x - width / 2, y - height / 2, 
                width, height,
                actualColumn * (1.0 / spritesPerColumn), actualRow * (1.0 / spritesPerRow),
                1.0 / spritesPerColumn, 1.0 / spritesPerRow,
                white
            );
        }
        
        return isAnimationSet;
    }
    
    int getWidth() {
        return width;
    }
    
    int getHeight() {
        return height;
    }
    
    void setAnimation(unsigned int _fromRow, unsigned int _fromColumn, 
                      unsigned int _toRow, unsigned int _toColumn, 
                      bool _repeat = false, unsigned int _frameSpeed = animationSpeed) {
        
        actualRow = fromRow = _fromRow; 
        actualColumn = fromColumn = _fromColumn;
        
        toRow = _toRow; 
        toColumn = _toColumn;
        
        increaseRow = fromRow < toRow;
        increaseColumn = fromColumn < toColumn;
        
        repeat = _repeat;
        
        frameCounter = 0; 
        frameSpeed = _frameSpeed;
        
        isAnimationSet = true;
        isLastFrame = false;
    }
};

#endif