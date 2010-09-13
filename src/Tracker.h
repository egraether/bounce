#ifndef _TRACKER_
#define _TRACKER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBounceImage.h"
#include "CalibrationQuad.h"
#include "Console.h"

class Tracker {
private:
    struct PixelRGB {
        unsigned char r, g, b;
        void set(unsigned char _r, unsigned char _g, unsigned char _b) {
            r = _r; g = _g; b = _b;
        }
    };
    
    struct PixelHSV {
        unsigned char h, s, v;
        void set(unsigned char _h, unsigned char _s, unsigned char _v) {
            h = _h; s = _s; v = _v;
        }
    };
    
    enum {CALIBRATION_NULL, BACKGROUND, COUNT, POINT, COMPLETE} mode;
    
    // image capture
    ofVideoGrabber videoCapture;
    ofxCvBounceImage colorImg;
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    
    // contour
    ofxCvContourFinder contourFinder;
    
    int threshold;
    int hue;
    int minBlobSize, maxBlobSize;
    
    // calibration
    ofPoint* screenCorner;
    ofPoint* projCorner;
    
    CalibrationQuad calibrationQuad;
    
    bool countChecked;
    
    int counter;
    int numCorners;
    
    // other
    bool newBackground;
    bool showGrayImg;
    bool showGrayDiff;
    
    ofPoint dummyPoint;
    
    Console* console;
    
    void getBrightnessContour(int threshold);
    void getHueContour(int hue);
    
public:
    Tracker(Console* c);
    //~Tracker();
    
    void reset();
    void calibrate();
    void draw();
    void keyPressed(int key);
    bool getHitPoint(ofPoint hitPoint);
};

#endif

