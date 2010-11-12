#include "Tracker.h"

Tracker::Tracker(Infobox* i, Console* c) : 
    infobox(i),  
    console(c),

    mode(CALIBRATION_NULL),
    threshold(33),

    minBlobSize(500),
    maxBlobSize(3000),
    lastBlobSize(0),

    screenStoreSize(6),

    derivationWidth(2),
    bangLevel(7), 
    bangCounter(0),
    
    calibrationWaitFrames(20),
                
    camWaitFrames(4),
    camWaitCounter(10),
                
    audioPlotSize(500) {
    
    console->addInformation("lastBlobSize", &lastBlobSize);
    console->addRegulation("screenStoreSize", &screenStoreSize, 1, 10);    
    console->addRegulation("camWaitFrames", &camWaitFrames, 0, 30);
    
    console->addRegulation("threshold", &threshold, 0, 255);
    console->addRegulation("derivationWidth", &derivationWidth, 0, 10);
    console->addRegulation("bangLevel", &bangLevel, 0, 50);
    
    videoCapture.setVerbose(true);
    videoCapture.initGrabber(ofGetWidth(), ofGetHeight());

    colorImg.allocate(ofGetWidth(), ofGetHeight());
    camImg.allocate(ofGetWidth(), ofGetHeight());
    grayImg.allocate(ofGetWidth(), ofGetHeight());
    grayBg.allocate(ofGetWidth(), ofGetHeight());
    grayDiff.allocate(ofGetWidth(), ofGetHeight());
    
    showColorImg = false;
    showGrayImg = false;
    showGrayDiff = false;
    showCamImg = false;
    showContours = true;
    
    numCorners = 0;
    
    projCorner = new Vector[4];
    screenCorner = new Vector[4];
    
    screenCorner[0].set(30, 30);
    screenCorner[1].set(ofGetWidth() - 30, 30);
    screenCorner[2].set(ofGetWidth() - 30, ofGetHeight() - 30);
    screenCorner[3].set(30, ofGetHeight() - 30);
    
    homography = cvCreateMat(3, 3, CV_32FC1);
    
    energyPlot.push_back(.0f);
}

Tracker::~Tracker() {
    delete [] screenCorner;
    delete [] projCorner;
    
    cvReleaseMat(&homography);
}

void Tracker::reset() {
    for (int i = 0; i < 4; i++) {
        projCorner[i].set(0,0);
    }
    
    numCorners = 0;
    counter = 0;
    
    countChecked = false;
    
    threshold = 25;
    
    mode = CALIBRATION_NULL;
    showColorImg = true;
    
    infobox->set("direct the camera to the screen, then click to start the calibration");
}

void Tracker::calibrate() {
    ofFill();
    ofSetColor(0, 0, 0);
    for (int i = 0; i < numCorners; i++) {
        ofCircle(projCorner[i].x, projCorner[i].y, 5);
    }
    
    switch (mode) {
        case CALIBRATION_NULL:
            drawAudioPlots();
            break;
        case BACKGROUND:
            if (contourFinder.nBlobs == 0) {
                counter++;
                
                if (counter == calibrationWaitFrames) {
                    counter = 0;
                    
                    if (countChecked)
                        mode = POINT;
                    else
                        mode = COUNT;
                }
            }
            else {
                counter = 0;
                grayBg = grayImg;
            }
            break;
        case COUNT:
            for (int i = 0; i < 4; i++) {
                ofCircle(screenCorner[i].x, screenCorner[i].y, 15);
            }
            counter++;
            
            if (counter == calibrationWaitFrames) {
                counter = 0;
                mode = BACKGROUND;
                
                if (contourFinder.nBlobs == 4)
                    countChecked = true;
                else
                    reset();
                    infobox->set("counting of corners failed check if camera is directed to screen");
            }
            break;
        case POINT:
            ofCircle(screenCorner[numCorners].x, screenCorner[numCorners].y, 15);
            
            if (counter == calibrationWaitFrames) {
                counter = 0;
                
                if (contourFinder.nBlobs == 1) {
                    projCorner[numCorners].set(contourFinder.blobs[0].centroid.x, contourFinder.blobs[0].centroid.y);
                    numCorners++;
                    
                    if (numCorners == 4) {
                        
                        CvMat* projPoints = cvCreateMat(4, 2, CV_32FC1);
                        CvMat* screenPoints = cvCreateMat(4, 2, CV_32FC1);
                        
                        for (int i = 0; i < 4; i++) {
                            cvmSet(projPoints, i, 0, projCorner[i].x);
                            cvmSet(projPoints, i, 1, projCorner[i].y);
                            
                            cvmSet(screenPoints, i, 0, screenCorner[i].x);
                            cvmSet(screenPoints, i, 1, screenCorner[i].y);
                        }
                        
                        cvFindHomography(projPoints, screenPoints, homography);
                        
                        cvReleaseMat(&projPoints);
                        cvReleaseMat(&screenPoints);
                        
                        threshold = 125;
                        
                        mode = COMPLETE;
                        
                        infobox->set("calibration finished, press 'm' for returning to menu");
                    }
                    else
                        mode = BACKGROUND;
                }
                else if (contourFinder.nBlobs > 1)
                    reset();
            }
            counter++;
            break;
        case COMPLETE:
            drawAudioPlots();
            break;
        default:
            break;
    }
}

bool Tracker::draw(bool hit, Vector hitPoint) {
    
    if (mode == CALIBRATION_NULL) {
        if (hit) {
            mode = BACKGROUND;
            showColorImg = false;
            infobox->clear();
        }
    }
    else if (mode != COMPLETE)
        getBrightnessContour();
    
    calibrate();
    
    return true;
}

void Tracker::drawPics() {
    
    ofSetColor(255, 255, 255);
    if (showColorImg)
        camImg.draw(0, 0);
    if (showGrayImg)
        grayBg.draw(0, 0);
    if (showGrayDiff)
        grayDiff.draw(0, 0);
    if (showCamImg)
        grayImg.draw(0, 0);
    if (showContours)
        contourFinder.draw(0, 0);
}

void Tracker::keyPressed(int key) {
	switch (key) {
        // set all to false when returning to menu
        case 'm':
            showContours = false;
            showColorImg = false;
            showGrayImg = false;
            showGrayDiff = false;
            showCamImg = false;
            break;
            
        // delete calibration
        case '0':
            reset();
			break;
        
        // draw images
        case '1':
            showColorImg = !showColorImg;
			break;
        
        case '2':
            showContours = !showContours;
			break;

        case '3':
            showGrayImg = !showGrayImg;
			break;
            
        case '4':
            showGrayDiff = !showGrayDiff;
			break;
            
        case '5':
            showCamImg = !showCamImg;
            break;
        default:
            break;
	}
}

bool Tracker::getHitPoint(Vector &hitPoint) {
    camWaitCounter++;
    if (camWaitFrames == camWaitCounter) {
        if (mode == COMPLETE) {
            
            grayImg = camImg;
            colorImg.setFromPixels(screenImgStore.front()->getPixels(), ofGetWidth(), ofGetHeight());
            grayBg = colorImg;
            
            cvWarpPerspective(grayImg.getCvImage(), grayDiff.getCvImage(), homography);
            
            grayImg = grayDiff;
            grayDiff.absDiff(grayBg);
            grayBg = grayDiff;
            
            grayDiff.threshold(threshold);
            grayDiff.erode();
            grayDiff.erode();
            grayDiff.dilate();
            grayDiff.dilate();
            
            contourFinder.findContours(grayDiff, minBlobSize, maxBlobSize, 1, false);
            
            if (contourFinder.nBlobs) {
                lastBlobSize = contourFinder.blobs[0].area;
                hitPoint.set(contourFinder.blobs[0].centroid.x, contourFinder.blobs[0].centroid.y);
                return true;
            }
        }
    }
    
    getNewCamImage();
    getNewScreenImage();
    
    return false;
}

bool Tracker::getNewCamImage() {
    bool newFrame = false;
    videoCapture.grabFrame();
    newFrame = videoCapture.isFrameNew();
    
    if (newFrame)
        camImg.setFromPixels(videoCapture.getPixels(), ofGetWidth(), ofGetHeight());
        
    return newFrame;
}

void Tracker::getNewScreenImage() {
    ofImage* screenImg = new ofImage();
    screenImg->grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    screenImgStore.push_back(screenImg);
    
    while (screenImgStore.size() > screenStoreSize) {
        screenImg = screenImgStore.front();
        screenImgStore.pop_front();
        delete screenImg;
    }
}

void Tracker::getBrightnessContour() {
    if (getNewCamImage()) {
        grayImg = camImg;
        
        grayDiff.absDiff(grayBg, grayImg);
        grayDiff.threshold(threshold);
        
        contourFinder.findContours(grayDiff, 20, (ofGetWidth() * ofGetHeight()) / 3, 10, true);
    }
}

void Tracker::audioInput(float energy) {
    energyDiff.push_back(energy - energyPlot[energyPlot.size() - 1]);
    while (energyDiff.size() > derivationWidth) {
        energyDiff.pop_front();
    }
    
    energyPlot.push_back(energy);
    if (energyPlot.size() > audioPlotSize)
        energyPlot.pop_front();
    
    float averageEnergyDiff = 0;
    for (int i = 0; i < energyDiff.size(); i++) {
        averageEnergyDiff += energyDiff[i];
    }
    
    energyDiffPlot.push_back(averageEnergyDiff);
    if (energyDiffPlot.size() > audioPlotSize)
        energyDiffPlot.pop_front();
    
    bangCounter++;
    if (averageEnergyDiff >= bangLevel && bangCounter > 20) {
        bangCounter = 0;
        camWaitCounter = 0;
    }
}

void Tracker::drawAudioPlots() {
    ofSetColor(255, 0, 0);
    ofLine((ofGetWidth() - audioPlotSize) / 2, ofGetHeight() / 2 + 50 - bangLevel, 
           (ofGetWidth() + audioPlotSize) / 2, ofGetHeight() / 2 + 50 - bangLevel);
    
    ofSetColor(0, 0, 0);
    for (int i = 0; i < energyPlot.size() - 1; i++) {
        ofLine((ofGetWidth() - audioPlotSize) / 2 + i, ofGetHeight() / 2 - 50 - energyPlot[i], 
               (ofGetWidth() - audioPlotSize) / 2 + 1 + i, ofGetHeight() / 2 - 50 - energyPlot[i + 1]);
    }
    for (int i = 0; i < energyDiffPlot.size() - 1; i++) {
        ofLine((ofGetWidth() - audioPlotSize) / 2 + i, ofGetHeight() / 2 + 50 - energyDiffPlot[i], 
               (ofGetWidth()- audioPlotSize) / 2 + 1 + i, ofGetHeight() / 2 + 50 - energyDiffPlot[i + 1]);
    }
}

void Tracker::drawPauseScreen() {
    ofSetColor(255, 255, 255);
    screenImgStore.back()->draw(0, 0);
}