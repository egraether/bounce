#ifndef _OFX_CV_CVT_IMAGE_
#define _OFX_CV_CVT_IMAGE_

#include "ofxOpenCv.h"

class ofxCvBounceImage : public ofxCvColorImage {
public:
    unsigned char* getPixelsHSV() {
        cvCvtColor(cvImage, cvImageTemp, CV_RGB2HSV);
        //swapTemp();
        flagImageChanged();
        
        if (pixels == NULL) {
            pixels = new unsigned char[width*height*3];
            pixelsWidth = width;
            pixelsHeight = height;
        } 
        else if (pixelsWidth != width || pixelsHeight != height) {
            delete pixels;
            pixels = new unsigned char[width*height*3];
            pixelsWidth = width;
            pixelsHeight = height;
        }
        
        for( int i = 0; i < height; i++ ) {
            memcpy(
                pixels + (i*width*3),
                cvImageTemp->imageData + (i*cvImageTemp->widthStep),
                width*3
            );
        }
        
        return pixels;
    }
    
    unsigned char* getPixelsRGB() {
        cvCvtColor(cvImage, cvImageTemp, CV_HSV2RGB);
        flagImageChanged();
        
        if (pixels == NULL) {
            pixels = new unsigned char[width*height*3];
            pixelsWidth = width;
            pixelsHeight = height;
        } 
        else if (pixelsWidth != width || pixelsHeight != height) {
            delete pixels;
            pixels = new unsigned char[width*height*3];
            pixelsWidth = width;
            pixelsHeight = height;
        }
        
        for( int i = 0; i < height; i++ ) {
            memcpy(
                pixels + (i*width*3),
                cvImageTemp->imageData + (i*cvImageTemp->widthStep),
                width*3
            );
        }
        
        return pixels;
    }
};
#endif