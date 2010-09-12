#ifndef _TRACKER_
#define _TRACKER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBounceImage.h"
#include "CalibrationQuad.h"

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
    
    enum {CALIBRATION_NULL, BACKGROUND, COUNT, POINT, TEST, COMPLETE} mode;
    
    ofVideoGrabber videoCapture;
    ofxCvBounceImage colorImg, hsvImg;
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    ofxCvContourFinder contourFinder;
    
    int threshold;
    
    ofPoint hitPoint;
    ofPoint* screenCorner;
    ofPoint* projCorner;
    
    CalibrationQuad calibrationQuad;
    
    bool countChecked;
    bool newBackground;
    
    bool showGrayImg;
    bool showGrayDiff;
    
    int counter;
    int numCorners;
    
    ofPoint dummyPoint;
    
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

