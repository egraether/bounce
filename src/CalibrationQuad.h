#ifndef _CALIBRATION_QUAD_
#define _CALIBRATION_QUAD_

#include "ofMain.h"

class CalibrationQuad {
private:
    ofPoint hitPoint;
    ofPoint camHitPoint;
    ofPoint* projCorner;
    ofPoint* screenCorner;
    int cornerIndex;
    
    ofPoint eyePoint[2];
    ofPoint measurePoint[2];
    ofPoint realLengthVector[2];
    int index[2];
    
public:
    CalibrationQuad() {}
    //~CalibrationQuad() {}
    
    void draw();
    void getEyePoints(ofPoint* sCorners, ofPoint* pCorners);
    ofPoint getHitPoint(ofPoint camHitPoint);
};

#endif

