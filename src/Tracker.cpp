#include "Tracker.h"
#include "utilities.h"
#include "constants.h"

Tracker::Tracker() {
    videoCapture.setVerbose(true);
    videoCapture.initGrabber(WIDTH,HEIGHT);

    colorImg.allocate(WIDTH,HEIGHT);
    hsvImg.allocate(WIDTH,HEIGHT);
    grayImg.allocate(WIDTH,HEIGHT);
    grayBg.allocate(WIDTH,HEIGHT);
    grayDiff.allocate(WIDTH,HEIGHT);

    newBackground = true;
    
    showGrayImg = false;
    showGrayDiff = false;
    
    threshold = 80;//BALL_HUE;
    
    numCorners = 0;
    
    screenCorner = new ofPoint[4];
    projCorner = new ofPoint[4];
    
    mode = CALIBRATION_NULL;
}

//Tracker::~Tracker() {}

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
    
    hitPoint.set(0,0);
    dummyPoint.set(0,0);
    
    countChecked = false;
    
    mode = BACKGROUND;
}

void Tracker::calibrate() {
    ofFill();
    ofSetColor(0, 0, 0);
    if (mode == BACKGROUND || mode == POINT) {
        for (int i = 0; i < numCorners; i++) {
            ofCircle(projCorner[i].x, projCorner[i].y, PROJ_CORNER_SIZE);
        }
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
                    cout << "count failed\n";
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
                        mode = TEST;
                    }
                    else
                        mode = BACKGROUND;
                }
                else if (contourFinder.nBlobs > 1)
                    reset();
            }
            counter++;
            break;
        case TEST:
            if (dummyPoint.x) {
                ofSetColor(0, 0, 0);
                ofCircle(dummyPoint.x, dummyPoint.y, 10);
            }
            
            if (contourFinder.nBlobs) {
                ofPoint camHitPoint(contourFinder.blobs[0].centroid.x, contourFinder.blobs[0].centroid.y);
                hitPoint = calibrationQuad.getHitPoint(camHitPoint);
            }
            
            calibrationQuad.draw();
            break;
        default:
            break;
    }
}

void Tracker::draw() {
    
    bool newFrame = false;
    videoCapture.grabFrame();
    newFrame = videoCapture.isFrameNew();
    
    if (newFrame) {
        colorImg.setFromPixels(videoCapture.getPixels(), WIDTH, HEIGHT);
        grayImg = colorImg;
        
        if (newBackground) {
            grayBg = grayImg;
            newBackground = false;
        }
        
        grayDiff.absDiff(grayBg, grayImg);
        //grayDiff.blur();
        grayDiff.threshold(threshold);
        //grayDiff.erode();
        
        contourFinder.findContours(grayDiff, 20, (WIDTH * HEIGHT) / 3, 10, true);
    }
    calibrate();
    
    ofSetColor(255, 255, 255);
    if (showGrayImg)
        grayImg.draw(0, 0);
    if (showGrayDiff)
        grayDiff.draw(0, 0);
    
    contourFinder.draw(0, 0);

//    ofBackground(100,100,100);
//    
//    bool bNewFrame = false;
//    
//    vidGrabber.grabFrame();
//    bNewFrame = vidGrabber.isFrameNew();
//    
//	if (bNewFrame) {
//        colorImg.setFromPixels(vidGrabber.getPixels(), WIDTH, HEIGHT);
//        
//        PixelRGB* pixRGB = (PixelRGB*)(colorImg.getPixels());
//        ofxCvBounceImage temp = colorImg;
//        PixelHSV* pixHSV = (PixelHSV*)(temp.getPixelsHSV());
//        for (int i = 0; i < WIDTH * HEIGHT; i++) {
//            if (pixHSV[i].h > threshold + 1 || pixHSV[i].h < threshold - 1)
//                pixRGB[i].set(0,0,0);
//        }
//        hsvImg.setFromPixels((unsigned char*)(pixRGB), WIDTH, HEIGHT);
//        
//        grayImage = hsvImg;
//		if (bLearnBakground == true) {
//			grayBg = grayImage;
//			bLearnBakground = false;
//		}
//        
//        grayDiff = grayImage;
//		grayDiff.threshold(threshold);
//        grayDiff.erode();
//        
//		contourFinder.findContours(grayDiff, 100, (WIDTH*HEIGHT)/3, 1, false);
//	}
//    
//    ofSetColor(0xffffff);
//    colorImg.draw(20,20);
//    grayImage.draw(360,20);
//    grayBg.draw(20,280);
//    grayDiff.draw(360,280);
//    hsvImg.draw(700, 20);
//    
//    ofFill();
//    ofSetColor(0x333333);
//    ofRect(360,540,WIDTH,HEIGHT);
//    ofSetColor(0xffffff);
//
//    //contourFinder.draw(360,540);
//    for (int i = 0; i < contourFinder.nBlobs; i++) {
//        contourFinder.blobs[i].draw(360,540);
//    }
//
//    ofSetColor(0xffffff);
//    char reportStr[1024];
//    sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
//    ofDrawBitmapString(reportStr, 20, 600);
}

void Tracker::keyPressed(int key) {
	switch (key){
		case ' ':
			newBackground = true;
			break;
        case 'c':
            reset();
			break;
        case 'g':
            showGrayImg = !showGrayImg;
			break;
        case 'd':
            showGrayDiff = !showGrayDiff;
			break;
		case '+':
			threshold++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold--;
			if (threshold < 0) threshold = 0;
			break;
	}
}

void Tracker::mousePressed(int x, int y) {
    dummyPoint.set(x, y);
}