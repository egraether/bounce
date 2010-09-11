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

    learnBackground = false;
    newBackground = true;
    isCalibrating = false;
    
    showGrayImg = false;
    showGrayDiff = false;
    
    threshold = 80;//BALL_HUE;
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
    
    learnBackground = true;
    isCalibrating = true;
}

void Tracker::calibrate() {
    ofFill();
    ofSetColor(0, 0, 0);    
    for (int i = 0; i < numCorners; i++) {
        ofCircle(projCorner[i].x, projCorner[i].y, PROJ_CORNER_SIZE);
    }
    
    if (isCalibrating) {
        if (learnBackground) {
            if (contourFinder.nBlobs == 0) {
                counter++;
                
                if (counter == 30) {
                    counter = 0;
                    learnBackground = false;
                }
            }
            else {
                counter = 0;
                grayBg = grayImg;
            }
        }
        else {
            ofCircle(screenCorner[numCorners].x, screenCorner[numCorners].y, SCREEN_CORNER_SIZE);
            
            if (counter == 30) {
                counter = 0;
                
                if (contourFinder.nBlobs == 1) {
                    projCorner[numCorners].set(contourFinder.blobs[0].centroid.x, contourFinder.blobs[0].centroid.y);
                    numCorners++;
                    learnBackground = true;
                    
                    if (numCorners == 4) {
                        getEyePoints();
                        isCalibrating = false;
                    }
                }
                else if (contourFinder.nBlobs > 1)
                    reset();
            }
            counter++;
        }
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

void Tracker::getEyePoints() {
    
    eyePoint[0] = getCutPoint(projCorner[0], projCorner[1], projCorner[2], projCorner[3]);
    eyePoint[1] = getCutPoint(projCorner[0], projCorner[2], projCorner[1], projCorner[3]);
    
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

void Tracker::getHitPoint(ofPoint camHitPoint) {
    ofPoint edgeProjection[2] = {
        getCutPoint(eyePoint[0], camHitPoint, projCorner[index[1]], projCorner[cornerIndex]),
        getCutPoint(eyePoint[1], camHitPoint, projCorner[index[0]], projCorner[cornerIndex])
    };
    
    ofPoint projectionVector[2] = {
        getVector(measurePoint[1], edgeProjection[0]),
        getVector(measurePoint[0], edgeProjection[1])
    };
    
    double param[2] = {
        getCutParameter(measurePoint[0], projectionVector[0], projCorner[cornerIndex], realLengthVector[1]), 
        getCutParameter(measurePoint[1], projectionVector[1], projCorner[cornerIndex], realLengthVector[0])
    };
    
    if (projCorner[cornerIndex].y > projCorner[index[1]].y)
        hitPoint.y = screenCorner[2].y - param[0] * (screenCorner[2].y - screenCorner[1].y);// + calCoefficient.y;
    else
        hitPoint.y = screenCorner[1].y + param[0] * (screenCorner[2].y - screenCorner[1].y);// + calCoefficient.y;
    
    
    if (projCorner[cornerIndex].x > projCorner[index[0]].x)
        hitPoint.x = screenCorner[1].x - param[1] * (screenCorner[1].x - screenCorner[0].x);// + calCoefficient.x;
    else
        hitPoint.x = screenCorner[0].x + param[1] * (screenCorner[1].x - screenCorner[0].x);// + calCoefficient.x;
}