/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

#ifndef _TRACKER_
#define _TRACKER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "Vector.h"
#include "Console.h"
#include "Infobox.h"
#include "PushButton.h"

#include <deque>

class Tracker {
private:    
    enum {CALIBRATION_NULL, BACKGROUND, COUNT, POINT, COMPLETE} mode;
    
    // image capture
    ofVideoGrabber videoCapture;
    ofxCvColorImage colorImg;
    
    deque<ofImage*> screenImgStore;
    int screenStoreSize;
    
    int camWaitFrames;
    int camWaitCounter;
    ofxCvColorImage camImg;
    
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    
    // contour
    ofxCvContourFinder contourFinder;
    
    int threshold;
    int minBlobSize, maxBlobSize;
    int lastBlobSize;
    
    bool getNewCamImage();
    void getNewScreenImage();
    void getBrightnessContour();
    
    // calibration
    Vector* screenCorner;
    Vector* projCorner;
    
    bool countChecked;
    
    int counter;
    int numCorners;
    const int calibrationWaitFrames;
    
    CvMat* homography;
    
    // audio
    deque<float> energyPlot;
    deque<float> energyDiff;
    deque<float> energyDiffPlot;
    
    int derivationWidth;
    int bangLevel;
    int bangCounter;
    
    void drawAudioPlots();
    
    const int audioPlotSize;
    
    // other
    bool showColorImg;
    bool showGrayImg;
    bool showGrayDiff;
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

