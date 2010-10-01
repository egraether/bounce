#include "Tracker.h"
#include "constants.h"

Tracker::Tracker(Infobox* i, Console* c) : 
    infobox(i),  
    console(c),

    mode(CALIBRATION_NULL),
    threshold(THRESHOLD),

    hue(HUE),
    hueVariance(HUE_VARIANCE),
    saturation(SATURATION),
    saturationVariance(SATURATION_VARIANCE),
    value(VALUE),
    valueVariance(VALUE_VARIANCE),

    minBlobSize(MIN_BLOB_SIZE),
    maxBlobSize(MAX_BLOB_SIZE),
    lastBlobSize(0),

    screenStoreSize(6),
    trackStop(false),

    derivationWidth(2),
    bangLevel(3), 
    bangCounter(0),

    camWaitFrames(3),
    camWaitCounter(10) {
    
    console->addInformation("lastBlobSize", &lastBlobSize);
    console->addRegulation("screenStoreSize", &screenStoreSize, 1, 10);    
    console->addRegulation("camWaitFrames", &camWaitFrames, 0, 30);
    
    console->addRegulation("threshold", &threshold, 0, 255);
    console->addRegulation("derivationWidth", &derivationWidth, 0, 10);
    console->addRegulation("bangLevel", &bangLevel, 0, 50);
        
//    console->addRegulation("hue", &hue, 0, 179);
//    console->addRegulation("hueVariance", &hueVariance, 0, 25);
//    
//    console->addRegulation("saturation", &saturation, 0, 255);
//    console->addRegulation("saturationVariance", &saturationVariance, 0, 50);
//        
//    console->addRegulation("value", &value, 0, 255);
//    console->addRegulation("valueVariance", &valueVariance, 0, 50);
//        
//    console->addRegulation("minBlobSize", &minBlobSize, 0, WIDTH * HEIGHT);
//    console->addRegulation("maxBlobSize", &maxBlobSize, 0, WIDTH * HEIGHT);
    
    videoCapture.setVerbose(true);
    videoCapture.initGrabber(WIDTH,HEIGHT);

    colorImg.allocate(WIDTH,HEIGHT);
    camImg.allocate(WIDTH, HEIGHT);
    grayImg.allocate(WIDTH,HEIGHT);
    grayBg.allocate(WIDTH,HEIGHT);
    grayDiff.allocate(WIDTH,HEIGHT);
    
    showColorImg = false;
    showGrayImg = false;
    showGrayDiff = false;
    showScreenImg = false;
    showCamImg = false;
    showContours = true;
    
    numCorners = 0;
    
    projCorner = new Vector[4];
    screenCorner = new Vector[4];
    
    screenCorner[0].set(SCREEN_CORNER_DISTANCE, SCREEN_CORNER_DISTANCE);
    screenCorner[1].set(WIDTH - SCREEN_CORNER_DISTANCE, SCREEN_CORNER_DISTANCE);
    screenCorner[2].set(WIDTH - SCREEN_CORNER_DISTANCE, HEIGHT - SCREEN_CORNER_DISTANCE);
    screenCorner[3].set(SCREEN_CORNER_DISTANCE, HEIGHT - SCREEN_CORNER_DISTANCE);
    
    homography = cvCreateMat(3, 3, CV_32FC1);
    
    equalize = false;
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
        ofCircle(projCorner[i].x, projCorner[i].y, PROJ_CORNER_SIZE);
    }
    
    switch (mode) {
        case CALIBRATION_NULL:
            drawAudioPlots();
            break;
        case BACKGROUND:
            if (contourFinder.nBlobs == 0) {
                counter++;
                
                if (counter == CALIBRATION_WAIT_FRAMES) {
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
                ofCircle(screenCorner[i].x, screenCorner[i].y, SCREEN_CORNER_SIZE);
            }
            counter++;
            
            if (counter == CALIBRATION_WAIT_FRAMES) {
                counter = 0;
                mode = BACKGROUND;
                
                if (contourFinder.nBlobs == 4)
                    countChecked = true;
                else
                    reset();
                    //infobox->set("counting of corners failed$check if camera is directed to screen", Infobox::CHECK);
            }
            break;
        case POINT:
            ofCircle(screenCorner[numCorners].x, screenCorner[numCorners].y, SCREEN_CORNER_SIZE);
            
            if (counter == CALIBRATION_WAIT_FRAMES) {
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
        //camImg.draw(0, 0);
    if (showScreenImg)
        screenImgStore.front()->draw(0, 0);
    if (showContours)
        contourFinder.draw(0, 0);
}

void Tracker::keyPressed(int key) {
	switch (key){
        case 'r':
            reset();
			break;
        case 'm':
            showContours = false;
            showColorImg = false;
            showGrayImg = false;
            showGrayDiff = false;
            showScreenImg = false;
            showCamImg = false;
            break;
        case 'x':
            showContours = !showContours;
			break;
        case 'c':
            showColorImg = !showColorImg;
			break;
        case 'v':
            showGrayImg = !showGrayImg;
			break;
        case 'b':
            showGrayDiff = !showGrayDiff;
			break;
        case 's':
            showScreenImg = !showScreenImg;
            break;
        case 'a':
            showCamImg = !showCamImg;
            break;
        case 'e':
            equalize = !equalize;
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
            colorImg.setFromPixels(screenImgStore.front()->getPixels(), WIDTH, HEIGHT);
            grayBg = colorImg;
            
            int pointList[16] = {
                screenCorner[0].x, screenCorner[0].y, screenCorner[1].x, screenCorner[1].y,
                screenCorner[3].x, screenCorner[3].y, screenCorner[2].x, screenCorner[2].y,
                projCorner[0].x, projCorner[0].y, projCorner[1].x, projCorner[1].y, 
                projCorner[3].x, projCorner[3].y, projCorner[2].x, projCorner[2].y
            };
            int resolution[2] = {WIDTH, HEIGHT};
            
            cvWarpPerspective(grayImg.getCvImage(), grayDiff.getCvImage(), homography);
            
            //        if (equalize)
            //            cvEqualizeHist(grayDiff.getCvImage(), grayDiff.getCvImage());
            
            grayImg = grayDiff;
            grayDiff.absDiff(grayBg);
            grayBg = grayDiff;
            
            grayDiff.threshold(threshold);
            grayDiff.erode();
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
        camImg.setFromPixels(videoCapture.getPixels(), WIDTH, HEIGHT);
        
    return newFrame;
}

void Tracker::getNewScreenImage() {
    ofImage* screenImg = new ofImage();
    screenImg->grabScreen(0, 0, WIDTH, HEIGHT);
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
        
        contourFinder.findContours(grayDiff, 20, (WIDTH * HEIGHT) / 3, 10, true);
    }
}

void Tracker::getHueContour() {
//    PixelRGB* pixRGB = (PixelRGB*)(colorImg.getPixels());
//    PixelHSV* pixHSV = (PixelHSV*)(colorImg.getPixelsHSV());
//    
//    for (int i = 0; i < WIDTH * HEIGHT; i++) {
//        if (pixHSV[i].h > hue + hueVariance || pixHSV[i].h < hue - hueVariance ||
//            pixHSV[i].s > saturation + saturationVariance || pixHSV[i].s < saturation - saturationVariance ||
//            pixHSV[i].s > value + valueVariance || pixHSV[i].s < value - valueVariance)
//            pixRGB[i].set(0,0,0);
//    }
//    ofxCvColorImage c;
//    c.allocate(WIDTH, HEIGHT);
//    c.setFromPixels((unsigned char*)(pixRGB), WIDTH, HEIGHT);
//    
//    grayDiff = c;
//    grayDiff.threshold(1);
//    grayDiff.dilate();
//    grayDiff.erode();
//    
//    contourFinder.findContours(grayDiff, minBlobSize, maxBlobSize, 1, false);
//    
//    if (contourFinder.nBlobs)
//        lastBlobSize = contourFinder.blobs[0].area;
}

void Tracker::audioInput(float energy) {
    energyDiff.push_back(energy - energyPlot[energyPlot.size() - 1]);
    while (energyDiff.size() > derivationWidth) {
        energyDiff.pop_front();
    }
    
    energyPlot.push_back(energy);
    if (energyPlot.size() > PLOT_SIZE)
        energyPlot.pop_front();
    
    float averageEnergyDiff = 0;
    for (int i = 0; i < energyDiff.size(); i++) {
        averageEnergyDiff += energyDiff[i];
    }
    
    energyDiffPlot.push_back(averageEnergyDiff);
    if (energyDiffPlot.size() > PLOT_SIZE)
        energyDiffPlot.pop_front();
    
    bangCounter++;
    if (averageEnergyDiff >= bangLevel && bangCounter > 20) {
        bangCounter = 0;
        camWaitCounter = 0;
    }
}

void Tracker::drawAudioPlots() {
    ofSetColor(255, 0, 0);
    ofLine(WIDTH / 2 - 250, HEIGHT / 2 + 50 - bangLevel, 
           WIDTH / 2 + 250, HEIGHT / 2 + 50 - bangLevel);
    
    ofSetColor(0, 0, 0);
    for (int i = 0; i < energyPlot.size() - 1; i++) {
        ofLine((WIDTH - PLOT_SIZE) / 2 + i, HEIGHT / 2 - 50 - energyPlot[i], 
               (WIDTH - PLOT_SIZE) / 2 + 1 + i, HEIGHT / 2 - 50 - energyPlot[i + 1]);
    }
    for (int i = 0; i < energyDiffPlot.size() - 1; i++) {
        ofLine((WIDTH - PLOT_SIZE) / 2 + i, HEIGHT / 2 + 50 - energyDiffPlot[i], 
               (WIDTH - PLOT_SIZE) / 2 + 1 + i, HEIGHT / 2 + 50 - energyDiffPlot[i + 1]);
    }
}

void Tracker::drawPauseScreen() {
    ofSetColor(255, 255, 255);
    screenImgStore.back()->draw(0, 0);
}