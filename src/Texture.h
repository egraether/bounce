#ifndef _TEXTURE_
#define _TEXTURE_

#include "ofMain.h"

class Texture {
public:
    unsigned int textureId;
    bool alpha;

public:
    unsigned int width, height;
    
    Texture() {
        glGenTextures(1, &textureId);
    }
    
    ~Texture() {
        glDeleteTextures(1, &textureId);
    }
    
    void draw(int x, int y, bool white = true) {
        draw(x, y, width, height, 0.0, 0.0, 1.0, 1.0, white);
    }
    
    void draw(int x, int y, int w, int h, bool white = true) {
        draw(x, y, w, h, 0.0, 0.0, 1.0, 1.0, white);
    }
    
    void draw(int x, int y, int w, int h, float tx, float ty, float tw, float th, bool white = true) {
        if (white)
            ofSetColor(0xffffff);
        
        ofEnableAlphaBlending();
        bind();
        
        glBegin(GL_QUADS);
        
        glTexCoord2f(tx, ty);
        glVertex2f(x, y);
        
        glTexCoord2f(tx, ty + th);
        glVertex2f(x, y + h);
        
        glTexCoord2f(tx + tw, ty + th);
        glVertex2f(x + w, y + h);
        
        glTexCoord2f(tx + tw, ty);
        glVertex2f(x + w, y);
        
        glEnd();
        
        unbind();
        ofDisableAlphaBlending();
    }
    
    void load(const char* imageFile, bool _alpha, int wrapS, int wrapT) {
        alpha = _alpha;
        
        ofImage image;
        if (!image.loadImage(imageFile))
            throw "Failed to load Image";
        
        width = image.getWidth();
        height = image.getHeight();
        
        bind();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        unsigned char *pixels = image.getPixels();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        if (alpha)
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                pixels
            );
        else
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                pixels
            );
//        gluBuild2DMipmaps(
//            GL_TEXTURE_2D,
//            GL_RGBA,
//            image.getWidth(),
//            image.getHeight(),
//            GL_RGBA,
//            GL_UNSIGNED_BYTE,
//            pixels
//        );
        
        unbind();
    }
    
    void bind() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
    
    void unbind() {
        glDisable(GL_TEXTURE_2D);
    }
};

#endif