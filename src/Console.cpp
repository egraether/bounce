/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

#include "Console.h"
#include "ofMain.h"

Console::Console() : 
    show(false),
    it(0) {
}

void Console::draw() {
    if (show) {
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(it->name + " " + ofToString(*it->value), 15, ofGetHeight() - 20);
        
        for (int i = 0; i < infos.size(); i++) {
            ofDrawBitmapString(infos[i].name + " " + ofToString(*infos[i].value), 15, 25 + i * 20);
        }
        
        ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), ofGetWidth() - 200, 40);
    }
} 

void Console::addInformation(string name, int* value) {
    infos.push_back(Information(name, value));
}

void Console::addRegulation(string name, int* value, int min, int max) {
    values.push_back(Regulation(name, value, min, max));
    it = values.begin();
    addInformation(name, value);
}

void Console::next(bool toNext) {
    if (show) {
        if (toNext) {
            if (it == --values.end())
                it = values.begin();
            else 
                it++;
        }
        else {
            if (it == values.begin())
                it = --values.end();
            else 
                it--;
        }
    }
}

void Console::change(bool increase) {
    if (show) {
        if (increase)
            *it->value = *it->value + 1 > it->max ? it->max : *it->value + 1;
        else
            *it->value = *it->value - 1 < it->min ? it->min : *it->value - 1;
    }
}