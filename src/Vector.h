/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

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

    bool operator==(Vector &v) {
        return x == v.x && y == v.y;
    }
    Vector operator+(Vector &v) {
        return Vector(x + v.x, y + v.y);
    }
    Vector operator-(Vector &v) {
        return Vector(x - v.x, y - v.y);
    }
    Vector operator*(double k) {
        return Vector(x * k, y * k);
    }
    Vector operator/(double k) {
        assert(k);
        return Vector(x / k, y / k);
    }
    double operator*(Vector &v) {
        return x * v.x + y * v.y;
    }
    static double norm(Vector u) {
        return sqrt(u * u);
    }

    static double angle(Vector u, Vector v) {
        return acos((u * v)/(norm(u)*norm(v)));
    }
    
    static Vector toLength(Vector u, double l) {
        return u / norm(u) * l;
    }
    
    static double distance(Vector a, Vector b) {
        return norm(a-b);
    }
    
    double distanceToLine(Vector a, Vector b) {
        assert(b.x || b.y);
        double t = (b.x * (x - a.x) + b.y * (y - a.y)) / (b.x * b.x + b.y * b.y);
        Vector c(a.x + b.x * t - x, a.y + b.y * t - y);
        return norm(c);
    }
    
    static Vector projectionVector(Vector a, Vector b) {
        return b * ((a * b) / (b * b));
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