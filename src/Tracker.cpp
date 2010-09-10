#include "Tracker.h"
#include "constants.h"

Tracker::Tracker() {
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);

    colorImg.allocate(320,240);
    hsvImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);

    bLearnBakground = true;
    threshold = BALL_HUE;
}

Tracker::~Tracker() {}

void Tracker::draw() {
    ofBackground(100,100,100);
    
    bool bNewFrame = false;
    
    vidGrabber.grabFrame();
    bNewFrame = vidGrabber.isFrameNew();
    
	if (bNewFrame) {
        colorImg.setFromPixels(vidGrabber.getPixels(), 320, 240);
        
        PixelRGB* pixRGB = (PixelRGB*)(colorImg.getPixels());
        ofxCvBounceImage temp = colorImg;
        PixelHSV* pixHSV = (PixelHSV*)(temp.getPixelsHSV());
        for (int i = 0; i < 320 * 240; i++) {
            if (pixHSV[i].h > threshold + 1 || pixHSV[i].h < threshold - 1)
                pixRGB[i].set(0,0,0);
        }
        hsvImg.setFromPixels((unsigned char*)(pixRGB), 320, 240);
        
        grayImage = hsvImg;
		if (bLearnBakground == true) {
			grayBg = grayImage;
			bLearnBakground = false;
		}
        
        grayDiff = grayImage;
		grayDiff.threshold(threshold);
        grayDiff.erode();
        
		contourFinder.findContours(grayDiff, 100, (340*240)/3, 1, false);
	}
    
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetColor(0xffffff);
    colorImg.draw(20,20);
    grayImage.draw(360,20);
    grayBg.draw(20,280);
    grayDiff.draw(360,280);
    hsvImg.draw(700, 20);

    // then draw the contours:
    ofFill();
    ofSetColor(0x333333);
    ofRect(360,540,320,240);
    ofSetColor(0xffffff);

    // we could draw the whole contour finder
    //contourFinder.draw(360,540);

    // or, instead we can draw each blob individually,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }

    ofSetColor(0xffffff);
    char reportStr[1024];
    sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
    ofDrawBitmapString(reportStr, 20, 600);
}

void Tracker::keyPressed(int key) {
	switch (key){
		case ' ':
			bLearnBakground = true;
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