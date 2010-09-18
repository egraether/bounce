#ifndef _TRACKER_
#define _TRACKER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBounceImage.h"
#include "Vector.h"
#include "CalibrationQuad.h"
#include "Console.h"
#include "Infobox.h"
#include "PushButton.h"

#include <deque>

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
    
    deque<ofxCvBounceImage> storeImg;
    int storeSize;
    
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    
    // contour
    ofxCvContourFinder contourFinder;
    
    int threshold;
    int hue, hueVariance;
    int saturation, saturationVariance;
    int value, valueVariance;
    
    int minBlobSize, maxBlobSize;
    int lastBlobSize;
    
    bool getNewImage();
    void getBrightnessContour(int threshold);
    void getHueContour();
    
    // calibration
    Vector* screenCorner;
    Vector* projCorner;
    
    CalibrationQuad calibrationQuad;
    
    bool countChecked;
    
    int counter;
    int numCorners;
    
    // other
    bool newBackground;
    bool showColorImg;
    bool showGrayImg;
    bool showGrayDiff;
    
    Infobox* infobox;
    Console* console;
    
    PushButton* menuButton;
    
public:
    Tracker(Infobox* i, PushButton* m, Console* c);
    //~Tracker();
    
    void update();
    void reset();
    void calibrate();
    bool draw(bool hit, Vector hitPoint);
    void keyPressed(int key);
    bool getHitPoint(Vector hitPoint);
};

#endif

