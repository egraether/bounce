#ifndef _CALIBRATION_QUAD_
#define _CALIBRATION_QUAD_

#include "Vector.h"

class CalibrationQuad {
private:
    Vector camHitPoint;
    Vector* projCorner;
    Vector* screenCorner;
    int cornerIndex;
    
    Vector eyePoint[2];
    Vector measurePoint[2];
    Vector realLengthVector[2];
    int index[2];
    double realParam[2];
    Vector edgeProjection[2];
    
public:
    CalibrationQuad() {}
    //~CalibrationQuad() {}
    
    void draw();
    void getEyePoints(Vector* sCorners, Vector* pCorners);
    Vector getHitPoint(Vector camHitPoint);
};

#endif

