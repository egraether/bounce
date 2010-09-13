#include "CalibrationQuad.h"
#include "utilities.h"
#include "constants.h"

//CalibrationQuad::CalibrationQuad() {}

void CalibrationQuad::draw() {    
    ofSetColor(100, 100, 100);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            ofLine(eyePoint[j].x, eyePoint[j].y, projCorner[i].x, projCorner[i].y);
        }
    }
    
    ofSetColor(255, 100, 0);
    for (int i = 0; i < 2; i++) {
        ofLine(measurePoint[i].x, measurePoint[i].y, 
               projCorner[cornerIndex].x + realLengthVector[i].x, projCorner[cornerIndex].y + realLengthVector[i].y);
        ofLine(projCorner[cornerIndex].x , projCorner[cornerIndex].y, 
               projCorner[cornerIndex].x + realLengthVector[i].x, projCorner[cornerIndex].y + realLengthVector[i].y);
        ofCircle(measurePoint[i].x, measurePoint[i].y, PROJ_CORNER_SIZE);
        ofCircle(projCorner[index[i]].x, projCorner[index[i]].y, PROJ_CORNER_SIZE);
        ofSetColor(255, 0, 255);
    }
    
    ofSetColor(200, 200, 0);
    for (int i = 0; i < 2; i++) {
        ofLine(projCorner[cornerIndex].x , projCorner[cornerIndex].y, 
               projCorner[cornerIndex].x + realParam[i] * realLengthVector[i].x, 
               projCorner[cornerIndex].y + realParam[i] * realLengthVector[i].y);
        ofLine(measurePoint[i].x , measurePoint[i].y, 
               projCorner[cornerIndex].x + realParam[i] * realLengthVector[i].x, 
               projCorner[cornerIndex].y + realParam[i] * realLengthVector[i].y);
        ofCircle(projCorner[cornerIndex].x + realParam[i] * realLengthVector[i].x, 
                 projCorner[cornerIndex].y + realParam[i] * realLengthVector[i].y, PROJ_CORNER_SIZE);
        ofSetColor(0, 200, 200);
    }
    
    if (camHitPoint.x) {
        ofSetColor(255, 0, 0);
        ofCircle(camHitPoint.x, camHitPoint.y, PROJ_CORNER_SIZE);
        ofSetColor(0, 255, 0);
        ofCircle(hitPoint.x, hitPoint.y, PROJ_CORNER_SIZE);
        
        ofSetColor(255, 0, 0);
        ofLine(eyePoint[0].x, eyePoint[0].y, edgeProjection[0].x, edgeProjection[0].y);
        ofCircle(edgeProjection[0].x, edgeProjection[0].y, PROJ_CORNER_SIZE);
        ofSetColor(0, 100, 255);
        ofLine(eyePoint[1].x, eyePoint[1].y, edgeProjection[1].x, edgeProjection[1].y);
        ofCircle(edgeProjection[1].x, edgeProjection[1].y, PROJ_CORNER_SIZE);
    }
}

void CalibrationQuad::getEyePoints(ofPoint* sCorners, ofPoint* pCorners) {
    screenCorner = sCorners;
    projCorner = pCorners;
    
    eyePoint[0] = getCutPoint(projCorner[0], projCorner[1], projCorner[2], projCorner[3]);
    eyePoint[1] = getCutPoint(projCorner[1], projCorner[2], projCorner[0], projCorner[3]);
    
    ofPoint eyeVector = getVector(eyePoint[1], eyePoint[0]);
    
    float hypotenuse = sqrt(eyeVector.x * eyeVector.x + eyeVector.y * eyeVector.y),
    length = 0;
    
    for (int i = 0; i < 4; i++) {
        double newLength = pointDistance(projCorner[i], eyePoint[0]) + pointDistance(projCorner[i], eyePoint[1]);
        
        if ( newLength > length ) {
            length = newLength;
            cornerIndex = i;
        }
    }
    
    double x = ((eyeVector.x * (projCorner[cornerIndex].x - eyePoint[0].x)) + 
                (eyeVector.y * (projCorner[cornerIndex].y - eyePoint[0].y))) / hypotenuse;
    
    x = x < 0 ? -x : x;
    
    if (x > hypotenuse )
        x = hypotenuse - 10;
    
    double cathetus[2];
    cathetus[0] = sqrt(x * hypotenuse);
    cathetus[1] = sqrt(hypotenuse * hypotenuse - x * hypotenuse);
    
    measurePoint[0].set(
        eyePoint[0].x + eyeVector.x / hypotenuse * cathetus[0] * -1, 
        eyePoint[0].y + eyeVector.y / hypotenuse * cathetus[0] * -1
    );
    measurePoint[1].set(
        eyePoint[1].x + eyeVector.x / hypotenuse * cathetus[1], 
        eyePoint[1].y + eyeVector.y / hypotenuse * cathetus[1]
    );
    
    for (int i = 0; i < 2; i++) {
        length = hypotenuse;
        
        for (int j = 0; j < 4; j++) {
            double newLength = pointDistance(projCorner[j], eyePoint[i]);
            
            if (newLength < length) {
                length = newLength;
                index[i] = j;
            }
        }
    }
    
    ofPoint measureVector[2] = {
        getVector(measurePoint[0], projCorner[index[0]]),
        getVector(measurePoint[1], projCorner[index[1]])
    };
    
    double param[2] = {
        getCutParameter(measurePoint[0], measureVector[0], projCorner[cornerIndex], eyeVector), 
        getCutParameter(measurePoint[1], measureVector[1], projCorner[cornerIndex], eyeVector)
    };
    
    realLengthVector[0].set(param[0] * eyeVector.x, param[0] * eyeVector.y);
    realLengthVector[1].set(param[1] * eyeVector.x, param[1] * eyeVector.y);
}

ofPoint CalibrationQuad::getHitPoint(ofPoint cHitPoint) {
    camHitPoint = cHitPoint;
    
    edgeProjection[0] = getCutPoint(eyePoint[0], camHitPoint, projCorner[index[1]], projCorner[cornerIndex]);
    edgeProjection[1] = getCutPoint(eyePoint[1], camHitPoint, projCorner[index[0]], projCorner[cornerIndex]);
    
    ofPoint projectionVector[2] = {
        getVector(measurePoint[1], edgeProjection[0]),
        getVector(measurePoint[0], edgeProjection[1])
    };
    
    realParam[0] = getCutParameter(measurePoint[0], projectionVector[1], projCorner[cornerIndex], realLengthVector[0]); 
    realParam[1] = getCutParameter(measurePoint[1], projectionVector[0], projCorner[cornerIndex], realLengthVector[1]);
    
    if (screenCorner[cornerIndex].x > screenCorner[index[0]].x)
        hitPoint.x = screenCorner[1].x - realParam[0] * (screenCorner[1].x - screenCorner[0].x);
    else if (screenCorner[cornerIndex].x < screenCorner[index[0]].x)
        hitPoint.x = screenCorner[0].x + realParam[0] * (screenCorner[1].x - screenCorner[0].x);
    else if (screenCorner[cornerIndex].x > screenCorner[index[1]].x)
        hitPoint.x = screenCorner[1].x - realParam[1] * (screenCorner[1].x - screenCorner[0].x);
    else if (screenCorner[cornerIndex].x < screenCorner[index[0]].x)
        hitPoint.x = screenCorner[0].x + realParam[1] * (screenCorner[1].x - screenCorner[0].x);
    
    if (screenCorner[cornerIndex].y > screenCorner[index[0]].y)
        hitPoint.y = screenCorner[2].y - realParam[0] * (screenCorner[2].y - screenCorner[1].y);
    else if (screenCorner[cornerIndex].y < screenCorner[index[0]].y)
        hitPoint.y = screenCorner[1].y + realParam[0] * (screenCorner[2].y - screenCorner[1].y);
    else if (screenCorner[cornerIndex].y > screenCorner[index[1]].y)
        hitPoint.y = screenCorner[2].y - realParam[1] * (screenCorner[2].y - screenCorner[1].y);
    else if (screenCorner[cornerIndex].y < screenCorner[index[0]].y)
        hitPoint.y = screenCorner[1].y + realParam[1] * (screenCorner[2].y - screenCorner[1].y);
    
    return hitPoint;
}
