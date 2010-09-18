#ifndef _VECTOR_
#define _VECTOR_

#include <iostream>
#include <cmath>
#include <cassert>

#ifndef PI
#define PI (2*acos(0))
#endif

class Vector {
public:
    double x, y;
    
    Vector() {
        x = y = 0;
    }
    
    Vector(double x, double y) {
        this->x = x;
        this->y = y;
    }

    void set(double x, double y) {
        this->x = x;
        this->y = y;
    }
    
    void print() {
         std::cout << "(" << x << ", " << y << ")\n";
    }

    static bool same(Vector u, Vector v) {
        return u.x == v.x && u.y == v.y;
    }
    static Vector add(Vector u, Vector v) {
        return Vector(u.x + v.x, u.y + v.y);
    }
    static Vector sub(Vector u, Vector v) {
        return Vector(u.x - v.x, u.y - v.y);
    }
    static Vector mul(Vector u, double k) {
        return Vector(u.x * k, u.y * k);
    }
    static Vector div(Vector u, double k) {
        assert(k);
        return Vector(u.x / k, u.y / k);
    }
    static double dot(Vector u, Vector v) {
        return u.x * v.x + u.y * v.y;
    }
    static double norm(Vector u) {
        return sqrt(dot(u, u));
    }
//    static Vector cross(Vector u, Vector v) {
//        return Vector(
//            u.y * v.z - u.z * v.y,
//            u.z * v.x - u.x * v.z,
//            u.x * v.y - u.y * v.x
//        );
//    }
    static double angle(Vector u, Vector v) {
        return acos(dot(u, v)/(norm(u)*norm(v)));
    }
    static Vector toLength(Vector u, double l) {
        return mul(div(u, norm(u)), l);
    }
    static double distance(Vector a, Vector b) {
        return norm(sub(a,b));
    }
//    double distanceToLine(Vector a, Vector b) {
//        assert(b.x || b.y || b.z);
//        double t = (b.x * (x - a.x) + b.y * (y - a.y) + b.z * (z - a.z)) / (b.x * b.x + b.y * b.y + b.z * b.z);
//        Vector c(a.x + b.x * t - x, a.y + b.y * t - y, a.z + b.z * t - z);
//        return norm(c);
//    }
    static Vector projectionVector(Vector a, Vector b) {
        return mul(b, dot(a, b) / dot(b, b));
    }
    
    static Vector getVector(Vector &a, Vector&b) {
        return Vector(b.x - a.x, b.y - a.y);
    }
    
    static double pointDistance(Vector &a, Vector&b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    
    static double getCutParameter(Vector &a, Vector &va, Vector &c, Vector &vc) {
        double divisor = va.x * vc.y - vc.x * va.y,
        parameter = 0.0;
        
        if (divisor > 0.1 || divisor < -0.1)
            parameter = (va.x * (a.y - c.y) + va.y * (c.x - a.x)) / divisor;
        
        return parameter;
    }
    
    static Vector getCutPoint(Vector &a, Vector &b, Vector &c, Vector &d) {
        Vector va(b.x - a.x, b.y - a.y),
        vc(d.x - c.x, d.y - c.y);
        
        double p = getCutParameter(a, va, c, vc);
        return Vector(c.x + p * vc.x, c.y + p * vc.y);
    }
};

#endif