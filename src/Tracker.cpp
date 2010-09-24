#include "Tracker.h"
#include "constants.h"

Tracker::Tracker(Infobox* i, PushButton* m, Console* c) : 
    infobox(i), 
    menuButton(m), 
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
    storeSize(3) {
    
    console->addRegulation("threshold", &threshold, 0, 255);
        
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
    
    console->addInformation("lastBlobSize", &lastBlobSize);
        
    console->addRegulation("storeSize", &storeSize, 1, 10);
    
    videoCapture.setVerbose(true);
    videoCapture.initGrabber(WIDTH,HEIGHT);

    colorImg.allocate(WIDTH,HEIGHT);
    camImg.allocate(WIDTH,HEIGHT);
    grayImg.allocate(WIDTH,HEIGHT);
    grayBg.allocate(WIDTH,HEIGHT);
    grayDiff.allocate(WIDTH,HEIGHT);
        
    screenImgStore.push_back(screenImg);
    
    showColorImg = false;
    showGrayImg = false;
    showGrayDiff = false;
    showScreenImg = false;
    showCamImg = false;
    
    numCorners = 0;
    
    projCorner = new Vector[4];
    screenCorner = new Vector[4];
    
    screenCorner[0].set(SCREEN_CORNER_DISTANCE, SCREEN_CORNER_DISTANCE);
    screenCorner[1].set(WIDTH - SCREEN_CORNER_DISTANCE, SCREEN_CORNER_DISTANCE);
    screenCorner[2].set(WIDTH - SCREEN_CORNER_DISTANCE, HEIGHT - SCREEN_CORNER_DISTANCE);
    screenCorner[3].set(SCREEN_CORNER_DISTANCE, HEIGHT - SCREEN_CORNER_DISTANCE);
    
    homography = cvCreateMat(3, 3, CV_32FC1);
    
    equalize = false;
}

Tracker::~Tracker() {
    delete [] screenCorner;
    delete [] projCorner;
}

void Tracker::reset() {
    for (int i = 0; i < 4; i++) {
        projCorner[i].set(0,0);
    }
    
    numCorners = 0;
    counter = 0;
    
    countChecked = false;
    
    threshold = 25;
    
    mode = BACKGROUND;
}

void Tracker::calibrate() {
    ofFill();
    ofSetColor(0, 0, 0);
    for (int i = 0; i < numCorners; i++) {
        ofCircle(projCorner[i].x, projCorner[i].y, PROJ_CORNER_SIZE);
    }
    
    switch (mode) {
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
                    infobox->set("counting of corners failed$check if camera is directed to screen", Infobox::CHECK);
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
            break;
        default:
            break;
    }
}

bool Tracker::draw(bool hit, Vector hitPoint) {
    
    if (infobox->checkState(hit, hitPoint) != Infobox::ALIVE) {
        if (mode != CALIBRATION_NULL) {
            if (mode == COMPLETE) {
                //getHueContour();
                
                menuButton->draw();
                if (menuButton->checkHit(hit, hitPoint))
                    return false;
            }
            else
                getBrightnessContour(threshold);
            
            calibrate();
        }
    }
    else {
        menuButton->draw();
        
        if (menuButton->checkHit(hit, hitPoint))
            return false;
    }
    
    drawPics();
    contourFinder.draw(0, 0);
    
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
    if (showScreenImg)
        screenImgStore.front().draw(0, 0);
    
    contourFinder.draw(0, 0);
}

void Tracker::keyPressed(int key) {
	switch (key){
        case 'r':
            reset();
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
    if (mode == COMPLETE) {
        
        grayImg = camImg;
        colorImg.setFromPixels(screenImgStore.front().getPixels(), WIDTH, HEIGHT);
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
    return false;
}

void Tracker::getPics() {
    if (getNewImage() && mode == COMPLETE) {
        
        screenImg.grabScreen(0, 0, WIDTH, HEIGHT);
        screenImgStore.push_back(screenImg);
        
        while (screenImgStore.size() > storeSize) {
            screenImgStore.pop_front();
        }
    }
}

bool Tracker::getNewImage() {
    bool newFrame = false;
    videoCapture.grabFrame();
    newFrame = videoCapture.isFrameNew();
    
    if (newFrame)
        camImg.setFromPixels(videoCapture.getPixels(), WIDTH, HEIGHT);
        
    return newFrame;
}

void Tracker::getBrightnessContour(int threshold) {
    if (getNewImage()) {
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