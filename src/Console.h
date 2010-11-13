/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

#ifndef _CONSOLE_
#define _CONSOLE_

#include "ofMain.h"

#include <vector>

class Console {
private:
    struct Information {
        string name;
        int* value;
        Information(string n, int* v) : 
            name(n), value(v) {
        }
    };
    
    struct Regulation {
        string name;
        int* value;
        int minValue, maxValue;
        Regulation(string n, int* v, int m, int x) : 
            name(n), value(v), minValue(m), maxValue(x) {
        }
    };
    
    vector<Information> infos;
    vector<Regulation> values;
    vector<Regulation>::iterator it;
    
public:
    bool show;
    
    Console();
    void draw();
    void addInformation(string name, int* value);
    void addRegulation(string name, int* value, int minValue, int maxValue);
    void next(bool toNext);
    void change(bool increase);
};

#endif

