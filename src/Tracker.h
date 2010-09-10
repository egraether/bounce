#ifndef _TRACKER_
#define _TRACKER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBounceImage.h"

class Tracker {
private:
    struct PixelRGB {
        unsigned char r, g, b;
        void set(unsigned char r, unsigned char g, unsigned char b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }
    };
    
    struct PixelHSV {
        unsigned char h, s, v;
        void set(unsigned char h, unsigned char s, unsigned char v) {
            this->h = h;
            this->s = s;
            this->v = v;
        }
    };
    
    ofVideoGrabber vidGrabber;
    ofxCvBounceImage colorImg, hsvImg;
    ofxCvGrayscaleImage grayImage, grayBg, grayDiff;
    ofxCvContourFinder contourFinder;
    
    int threshold;
    bool bLearnBakground;
    
public:
    Tracker();
    ~Tracker();
    
    void draw();
    void keyPressed(int key);
};

#endif

