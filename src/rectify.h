#include "cmath"

unsigned char* rectifyVerticalDimension(unsigned char* image,int initResolution[2],int finalResolution[2], int pointList[16]) {
    
    float angle[2];
    float lineStartY[2];
    
    for (int i = 0; i < 2; i++) {
        angle[i] = atan(float(pointList[9 + (i * 4)] - pointList[11 + (i * 4)]) / (pointList[10 + (i * 4)] - pointList[8 + (i * 4)]));
        lineStartY[i] = (pointList[9 + (i * 4)] + tan(angle[i]) * pointList[8 + (i * 4)]);
    }
    
    float lineDistanceChange =  tan(angle[0]) -  tan(angle[1]);
    float sizeMod[initResolution[0]];
    unsigned char* tempImage = new unsigned char[initResolution[0] * finalResolution[1]];
    
    for (int i = 0; i < initResolution[0]; i++) {
        sizeMod[i] =  (lineStartY[1] - lineStartY[0] + lineDistanceChange * i) / (pointList[5] - pointList[1]);
        
        for (int j = 0; j < finalResolution[1]; j++) {
            int sourceIndex = int(j * sizeMod[i] + lineStartY[0] - pointList[1] * sizeMod[i] - tan(angle[0]) * i) * initResolution[0] + i;
            
            if (sourceIndex > 0 && sourceIndex < initResolution[0] * finalResolution[1]) {
                tempImage[j * initResolution[0] + i] = image[sourceIndex];
            }
            else {
                tempImage[j * initResolution[0] + i] = 100;
            }
        }
    }
    
    return tempImage;
}

unsigned char* rectifyImage(unsigned char* image,int initResolution[2],int finalResolution[2], int pointList[16]) {
    
    unsigned char* tempImage = new unsigned char[initResolution[1] * initResolution[0]];
    for (int i = 0; i < initResolution[0]; i++) {
        for (int j = 0; j < initResolution[1]; j++) {
            tempImage[j + (i * initResolution[1])] = image[i + (j*initResolution[0])];
        }
    }
    
    int tempPointList[16] = {pointList[1], pointList[0], pointList[5], pointList[4],
                            pointList[3], pointList[2], pointList[7], pointList[6],
                            pointList[9], pointList[8], pointList[13], pointList[12],
                            pointList[11], pointList[10], pointList[15], pointList[14]};
    int tempInitResolution[2] = {initResolution[1],initResolution[0]};
    int tempFinalResolution[2] = {finalResolution[1],finalResolution[0]};
    
    tempImage = rectifyVerticalDimension(tempImage, tempInitResolution, tempFinalResolution, tempPointList);
    
    for (int i = 0; i < initResolution[0]; i++) {
        for (int j = 0; j < initResolution[1]; j++) {
            image[i + (j * initResolution[0])] = tempImage[j + (i * initResolution[1])];
        }
    }
    delete[] tempImage;
    
    image = rectifyVerticalDimension(image, initResolution, finalResolution, pointList);
    
    return image;
}