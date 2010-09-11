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
    
    enum {CALIBRATION_NULL, BACKGROUND, POINT, TEST, COMPLETE} mode;
    
    ofVideoGrabber videoCapture;
    ofxCvBounceImage colorImg, hsvImg;
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    ofxCvContourFinder contourFinder;
    
    int threshold;
    
    ofPoint hitPoint;
    
    bool newBackground;
    
    bool showGrayImg;
    bool showGrayDiff;
    
    int counter;
    int numCorners;
    
    ofPoint screenCorner[4];
    ofPoint projCorner[4];
    int cornerIndex;
    
    ofPoint eyePoint[2];
    ofPoint measurePoint[2];
    ofPoint realLengthVector[2];
    int index[2];
    
    ofPoint dummyPoint;
    
    void getEyePoints();
    void getHitPoint(ofPoint camHitPoint);
    
public:
    Tracker();
    //~Tracker();
    
    void reset();
    void calibrate();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y);
};

#endif

