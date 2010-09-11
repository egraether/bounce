#include "utilities.h"

ofPoint getVector(ofPoint &a, ofPoint&b) {
    return ofPoint(b.x - a.x, b.y - a.y);
}

double pointDistance(ofPoint &a, ofPoint&b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double getCutParameter(ofPoint &a, ofPoint &va, ofPoint &c, ofPoint &vc) {
    double divisor = va.x * vc.y - vc.x * va.y,
    parameter = 0;
    
    if (divisor > 0.1 || divisor < -0.1)
        parameter = (va.x * (a.y - c.y) + va.y * (c.x - a.x)) / divisor;
    
    return parameter;
}

ofPoint getCutPoint(ofPoint &a, ofPoint &b, ofPoint &c, ofPoint &d) {
    ofPoint va(b.x - a.x, b.y - a.y),
    vc(d.x - c.x, d.y - c.y);
    
    double p = getCutParameter(a, va, c, vc);
    return ofPoint(c.x + p * vc.x, c.y + p * vc.y);
}