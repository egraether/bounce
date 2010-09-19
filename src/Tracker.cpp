#include "Tracker.h"
#include "constants.h"

Tracker::Tracker(Infobox* i, PushButton* m, Console* c) : 
    infobox(i), menuButton(m), console(c),
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
    storeSize(5) {
    
    console->addRegulation("threshold", &threshold, 0, 255);
    console->addRegulation("hue", &hue, 0, 179);
    console->addRegulation("hueVariance", &hueVariance, 0, 25);
    console->addRegulation("value", &value, 0, 255);
    console->addRegulation("valueVariance", &valueVariance, 0, 50);
    console->addRegulation("saturation", &saturation, 0, 255);
    console->addRegulation("saturationVariance", &saturationVariance, 0, 50);
    console->addRegulation("minBlobSize", &minBlobSize, 0, WIDTH * HEIGHT);
    console->addRegulation("maxBlobSize", &maxBlobSize, 0, WIDTH * HEIGHT);
    
    console->addInformation("lastBlobSize", &lastBlobSize);
        
    console->addRegulation("storeSize", &storeSize, 1, 10);
    
    videoCapture.setVerbose(true);
    videoCapture.initGrabber(WIDTH,HEIGHT);

    colorImg.allocate(WIDTH,HEIGHT);
    grayImg.allocate(WIDTH,HEIGHT);
    grayBg.allocate(WIDTH,HEIGHT);
    grayDiff.allocate(WIDTH,HEIGHT);
    
    showColorImg = false;
    showGrayImg = false;
    showGrayDiff = false;
    
    numCorners = 0;
    
    screenCorner = new Vector[4];
    projCorner = new Vector[4];
}

Tracker::~Tracker() {
    delete [] screenCorner;
    delete [] projCorner;
}

void Tracker::update() {
    if (mode == COMPLETE)
        getNewImage();
}

void Tracker::reset() {
    for (int i = 0; i < 4; i++) {
        projCorner[i].set(0,0);
    }
    
    screenCorner[0].set(SCREEN_CORNER_DISTANCE, SCREEN_CORNER_DISTANCE);
    screenCorner[1].set(WIDTH - SCREEN_CORNER_DISTANCE, SCREEN_CORNER_DISTANCE);
    screenCorner[2].set(WIDTH - SCREEN_CORNER_DISTANCE, HEIGHT - SCREEN_CORNER_DISTANCE);
    screenCorner[3].set(SCREEN_CORNER_DISTANCE, HEIGHT - SCREEN_CORNER_DISTANCE);
    
    numCorners = 0;
    counter = 0;
    
    countChecked = false;
    
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
                        calibrationQuad.getEyePoints(screenCorner, projCorner);
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
        case COMPLETE: {
            PixelHSV pixHSV[WIDTH * HEIGHT];
            for (int i = 0; i < 50; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    pixHSV[i * WIDTH + j].set(hue, saturation, value);
                }
            }
            ofxCvBounceImage hsvImg;
            hsvImg.allocate(WIDTH, 50);
            hsvImg.setFromPixels((unsigned char*)(pixHSV), WIDTH, 50);
            hsvImg.setFromPixels(hsvImg.getPixelsRGB(), WIDTH, 50);
            ofSetColor(0xffffff);
            hsvImg.draw(0, HEIGHT - 50);
            
            if (contourFinder.nBlobs) {
                Vector camHitPoint(contourFinder.blobs[0].centroid.x, contourFinder.blobs[0].centroid.y);
                Vector hitPoint = calibrationQuad.getHitPoint(camHitPoint);
                
                ofSetColor(0, 255, 0);
                ofFill();
                ofCircle(hitPoint.x, hitPoint.y, 10);
            }
            
            calibrationQuad.draw();
            break;
        }
        default:
            break;
    }
}

bool Tracker::draw(bool hit, Vector hitPoint) {
    
    if (infobox->checkState(hit, hitPoint) != Infobox::ALIVE) {
        if (mode != CALIBRATION_NULL) {
            if (mode == COMPLETE) {
                getHueContour();
                
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
        getNewImage();
        menuButton->draw();
        
        if (menuButton->checkHit(hit, hitPoint))
            return false;
    }
    
    ofSetColor(255, 255, 255);
    if (showColorImg)
        colorImg.draw(0, 0);
    if (showGrayImg)
        grayImg.draw(0, 0);
    if (showGrayDiff)
        grayDiff.draw(0, 0);
    
    contourFinder.draw(0, 0);
    
    return true;
}

void Tracker::keyPressed(int key) {
	switch (key){
        case 'c':
            reset();
			break;
        case 'g':
            showColorImg = !showColorImg;
			break;
        case 'f':
            showGrayImg = !showGrayImg;
			break;
        case 'd':
            showGrayDiff = !showGrayDiff;
			break;
        default:
            break;
	}
}

bool Tracker::getHitPoint(Vector hitPoint) {
    if (mode == COMPLETE) {
        getHueContour();
        //getBrightnessContour(threshold);
        
        if (contourFinder.nBlobs) {
            Vector camHitPoint(contourFinder.blobs[0].centroid.x, contourFinder.blobs[0].centroid.y);
            hitPoint = calibrationQuad.getHitPoint(camHitPoint);
            return true;
        }
    }
    return false;
}

bool Tracker::getNewImage() {
    bool newFrame = false;
    videoCapture.grabFrame();
    newFrame = videoCapture.isFrameNew();
    
    if (newFrame) {
        colorImg.setFromPixels(videoCapture.getPixels(), WIDTH, HEIGHT);
        storeImg.push_back(colorImg);
        
        while (storeImg.size() > storeSize) {
            storeImg.pop_front();
        }
        
        return true;
    }
    return false;
}

void Tracker::getBrightnessContour(int threshold) {
    if (getNewImage()) {
        grayImg = colorImg;
        
        grayDiff.absDiff(grayBg, grayImg);
        grayDiff.threshold(threshold);
        
        contourFinder.findContours(grayDiff, 20, (WIDTH * HEIGHT) / 3, 10, true);
    }
}

void Tracker::getHueContour() {
    PixelRGB* pixRGB = (PixelRGB*)(storeImg.front().getPixels());
    PixelHSV* pixHSV = (PixelHSV*)(storeImg.front().getPixelsHSV());
//    PixelRGB* pixRGB = (PixelRGB*)(colorImg.getPixels());
//    PixelHSV* pixHSV = (PixelHSV*)(colorImg.getPixelsHSV());
    
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (pixHSV[i].h > hue + hueVariance || pixHSV[i].h < hue - hueVariance ||
            pixHSV[i].s > saturation + saturationVariance || pixHSV[i].s < saturation - saturationVariance)
            pixRGB[i].set(0,0,0);
    }
    colorImg.setFromPixels((unsigned char*)(pixRGB), WIDTH, HEIGHT);
    
    grayDiff = colorImg;
    grayDiff.threshold(1);
    grayDiff.dilate();
    grayDiff.erode();
    
    contourFinder.findContours(grayDiff, minBlobSize, maxBlobSize, 1, false);
    
    if (contourFinder.nBlobs)
        lastBlobSize = contourFinder.blobs[0].area;
}