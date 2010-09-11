#ifndef _UTILITIES_
#define _UTILITIES_

#include "ofMain.h"

ofPoint getVector(ofPoint &a, ofPoint&b);
double pointDistance(ofPoint &a, ofPoint&b);
double getCutParameter(ofPoint &a, ofPoint &va, ofPoint &c, ofPoint &vc);
ofPoint getCutPoint(ofPoint &a, ofPoint &b, ofPoint &c, ofPoint &d);

#endif