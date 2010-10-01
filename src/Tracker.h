#ifndef _TRACKER_
#define _TRACKER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBounceImage.h"
#include "Vector.h"
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
    ofxCvColorImage colorImg;
    
    deque<ofImage*> screenImgStore;
    int screenStoreSize;
    int screenStoreIndex;
    
    int camWaitFrames;
    int camWaitCounter;
    ofxCvColorImage camImg;
    
    bool imageSwitch;
    bool trackStop;
    
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    
    // contour
    ofxCvContourFinder contourFinder;
    
    int threshold;
    int hue, hueVariance;
    int saturation, saturationVariance;
    int value, valueVariance;
    
    int minBlobSize, maxBlobSize;
    int lastBlobSize;
    
    bool getNewCamImage();
    void getNewScreenImage();
    void getBrightnessContour();
    void getHueContour();
    
    // calibration
    Vector* screenCorner;
    Vector* projCorner;
    
    bool countChecked;
    
    int counter;
    int numCorners;
    
    CvMat* homography;
    
    bool equalize;
    
    // audio
    deque<float> energyPlot;
    deque<float> energyDiff;
    deque<float> energyDiffPlot;
    
    int derivationWidth;
    int bangLevel;
    int bangCounter;
    
    void drawAudioPlots();
    
    // other
    bool showColorImg;
    bool showGrayImg;
    bool showGrayDiff;
    bool showScreenImg;
    bool showCamImg;
    bool showContours;
    
    Infobox* infobox;
    Console* console;
    
public:
    Tracker(Infobox* i, Console* c);
    ~Tracker();
    
    void reset();
    void calibrate();
    bool draw(bool hit, Vector hitPoint);
    void drawPics();
    void drawPauseScreen();
    void keyPressed(int key);
    bool getHitPoint(Vector &hitPoint);
    void audioInput(float energy);
};

#endif

