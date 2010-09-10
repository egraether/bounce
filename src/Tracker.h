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
    
    ofPoint hitPoint;
    
    ofPoint screenCorner[4];
    ofPoint projCorner[4];
    
    int cornerIndex;
    
    ofPoint eyePoint[2];
    
    ofPoint measurePoint[2];
    ofPoint realLengthVector[2];
    int index[2];
    
    ofPoint camHitPoint;
    ofPoint calCoefficient;
    ofPoint dummyPoint;
    
    ofPoint getVector(ofPoint &a, ofPoint&b);
    double pointDistance(ofPoint &a, ofPoint&b);
    double getCutParameter(ofPoint &a, ofPoint &va, ofPoint &c, ofPoint &vc);
    ofPoint getCutPoint(ofPoint &a, ofPoint &b, ofPoint &c, ofPoint &d);
    
    void getEyePoints();
    void getHitPoint();
    
public:
    Tracker();
    ~Tracker();
    
    void reset();
    void draw();
    void keyPressed(int key);
};

#endif

